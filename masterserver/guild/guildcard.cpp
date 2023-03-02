#include "pch.h"
#include "guildcard.h"
#include "util/XCommon.h"
#include "pb/project.pb.h"
#include "role/rolesummarymgr.h"
#include "guild/guildcardmgr.h"
#include "guild/guild.h"
#include "guild/guildcardmatch.h"
#include "role/role.h"
#include "mail/mailmgr.h"
#include "mail/mailconfig.h"
#include "global/GlobalConfigDBBuild.h"
CGuildCard::CGuildCard(Guild* guild)
{
	m_guild = guild;
	m_updateday = 0;
	m_allcount = 0;
	m_bestroleid = 0;
	m_result = CardDummy;
	m_recordindex = 0;
	m_match = NULL;
}

CGuildCard::~CGuildCard()
{
	if (NULL != m_match)
	{
		delete m_match;
		m_match = NULL;
	}
}

bool CGuildCard::IsChanged()
{
	if (m_ischanged)
	{
		m_ischanged = false;
		return true;
	}
	return false;
}

bool CGuildCard::Load(const KKSG::GuildAllDBData& guilddata)
{
	if (guilddata.has_card() && !guilddata.card().empty())
	{
		KKSG::GuildCard card;
		card.ParseFromString(guilddata.card());

		m_updateday = card.updateday();
		m_allcount = card.allcount();
		m_result = card.result();
		m_bestroleid = card.roleid();
		for (int i = 0; i < card.cards_size(); ++i)
		{
			m_bestcards.push_back(card.cards(i));
		}
		for (int j = 0; j < card.matchrecord_size(); ++j)
		{
			const KKSG::CardPointRecord& record = card.matchrecord(j);
			m_matchpoints.insert(CardPoint(record.roleid(), record.point(), record.timestamp()));
		}
		for (int k = 0; k < card.teamrecord_size(); ++k)
		{
			const KKSG::CardPointRecord& record = card.teamrecord(k);
			m_id2teamrecords[record.index()].insert(CardPoint(record.roleid(), record.point(), record.timestamp()));
			UpdateTopRecord(record.roleid(), record.point());

			m_roleid2team[record.roleid()] = record.index();

			if (m_recordindex < record.index())
			{
				m_recordindex = record.index();
			}
		}
		if (0 != card.teamrecord_size())
		{
			++m_recordindex;
		}

	}

	return true;
}

void CGuildCard::Save(KKSG::GuildAllDBData& guilddata)
{
	guilddata.mutable_card()->assign(ToBinary());
}

const std::string CGuildCard::ToBinary()
{
	KKSG::GuildCard card;
	card.set_updateday(m_updateday);
	card.set_allcount(m_allcount);
	card.set_result(m_result);
	card.set_roleid(m_bestroleid);
	for (auto i = m_bestcards.begin(); i != m_bestcards.end(); ++i)
	{
		card.add_cards(*i);
	}
	for (auto j = m_matchpoints.begin(); j != m_matchpoints.end(); ++j)
	{
		KKSG::CardPointRecord* record = card.add_matchrecord();
		record->set_roleid(j->roleid);
		record->set_point(j->point);
		record->set_timestamp(j->timestamp);
	}
	for (auto k = m_id2teamrecords.begin(); k != m_id2teamrecords.end(); ++k)
	{
		for (auto l = k->second.begin(); l != k->second.end(); ++l)
		{
			KKSG::CardPointRecord* record = card.add_teamrecord();
			record->set_index(k->first);
			record->set_roleid(l->roleid);
			record->set_point(l->point);
			record->set_timestamp(l->timestamp);
		}
	}

	return card.SerializeAsString();
}

bool CGuildCard::Update()
{
	UINT32 day = XCommon::GetTodayUnique();
	if (m_updateday != day)
	{
		m_allcount = 0;
		m_result = CardDummy;
		m_bestcards.clear();
		m_bestroleid = 0;
		m_updateday = day;

		UpdateRecordDay();
		ClearMatchPoints();

		m_ischanged = true;

		return true;
	}
	return false;
}

bool CGuildCard::IsCountLimit(UINT32 count)
{
	return m_allcount >= count;
}

void CGuildCard::AddCount()
{
	++m_allcount;
	m_ischanged = true;
}

bool CGuildCard::PushCards(UINT64 roleid, UINT32 result, std::vector<UINT32>& cards)
{
	// point
	UpdateRecord(roleid, GuildCardMgr::Instance()->GetPoint(result));

	if (result > m_result)
	{
		return false;
	}
	else if (result == m_result)
	{
		if (GuildCardMgr::Instance()->Compare(result, m_bestcards, cards))
		{
			return false;
		}
	}
	m_result = result;
	m_bestcards = cards;
	m_bestroleid = roleid;

	m_ischanged = true;

	return true;	
}

const std::vector<UINT32>& CGuildCard::GetCards()
{
	return m_bestcards;
}

std::string CGuildCard::GetBestRoleName()
{
	if (0 == m_bestroleid)
	{
		return "";
	}
	return CRoleSummaryMgr::Instance()->GetRoleName(m_bestroleid);
}

void CGuildCard::OnAddMember(Guild *pGuild, GuildMember *pMember)
{
	AddRecord(pMember->qwRoleID);
}

void CGuildCard::OnRemoveMember(Guild *pGuild, UINT64 qwRoleID)
{
	DelRecord(qwRoleID);
}

void CGuildCard::UpdateRecordDay()
{
	int level_seal = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eLevelSealType));
	// reward
	for (auto i = m_id2teamrecords.begin(); i != m_id2teamrecords.end(); ++i)
	{
		UINT32 count = 0;
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			if (0 != j->point)
			{
				// mail
				const std::vector<ItemDesc>* items = GuildCardMgr::Instance()->GetRankReward(0, count,level_seal);
				if (NULL != items)
				{
					const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_CardTeam);
					if (NULL != conf)
					{
						std::string content = MailConfig::Instance()->ReplaceString("0", ToString(count + 1), conf->m_content);
						MailData mail(conf->m_title, content, *items, MAIL_EXPIRE_TIME);
						mail.SetReason(ItemFlow_GuildCardAward);
						CMailMgr::Instance()->SendMail(j->roleid, mail);
					}
				}
			}
			else
			{
				break;
			}
			++count;
		}
	}
	// clear and arrange
	ArrangeRecord();

	m_ischanged = true;
}

std::set<CardPoint>* CGuildCard::GetRecord(UINT64 roleid)
{
	auto i = m_roleid2team.find(roleid);
	if (i == m_roleid2team.end())
	{
		SSError<<"roleid:"<<roleid<<" find no record:"<<END;
		return NULL;
	}
	auto j = m_id2teamrecords.find(i->second);
	if (j == m_id2teamrecords.end()) 
	{
		SSError<<"roleid:"<<roleid<<" find no record:"<<i->second<<END;
		return NULL;
	}
	return &j->second;
}

void CGuildCard::UpdateRecord(UINT64 roleid, int point)
{
	if (0 == point)
	{
		return;
	}
	std::set<CardPoint>* teamrecord = GetRecord(roleid);
	if (NULL != teamrecord)
	{
		for (auto i = teamrecord->begin(); i != teamrecord->end(); ++i)
		{
			if (i->roleid == roleid)
			{
				point += i->point;
				teamrecord->erase(i);
				break;
			}
		}
		teamrecord->insert(CardPoint(roleid, point, (UINT32)time(NULL)));
		UpdateTopRecord(roleid, point);
		m_ischanged = true;
	}
}

void CGuildCard::UpdateTopRecord(UINT64 roleid, int point)
{
	if (0 == point)
	{
		return;
	}
	bool isin = false;
	for (auto i = m_toprecords.begin(); i != m_toprecords.end(); ++i)
	{
		if (i->roleid == roleid)
		{
			m_toprecords.erase(i);
			isin = true;
			break;
		}
	}
	if (isin)
	{
		m_toprecords.insert(CardPoint(roleid, point, (UINT32)time(NULL)));
		return;
	}

	if (m_toprecords.size() < CARDPOINT_TOPCOUNT)
	{
		m_toprecords.insert(CardPoint(roleid, point, (UINT32)time(NULL)));
		return;
	}
	auto i = m_toprecords.begin();
	std::advance(i, m_toprecords.size() - 1);
	if (i->point < point)
	{
		m_toprecords.erase(i);
		m_toprecords.insert(CardPoint(roleid, point, (UINT32)time(NULL)));
	}
	m_ischanged = true;
}

void CGuildCard::AddRecord(UINT64 roleid)
{
	auto i = m_roleid2team.find(roleid);
	if (i != m_roleid2team.end())
	{
		return;
	}
	CardPoint pointrecord(roleid);
	bool hasempty = false;
	for (auto i = m_id2teamrecords.begin(); i != m_id2teamrecords.end(); ++i)
	{
		if (i->second.size() < CARDTEAM_COUNT)
		{
			i->second.insert(pointrecord);
			hasempty = true;

			m_roleid2team[roleid] = i->first;
			SSInfo<<"role:"<<roleid<<" index:"<<i->first<<END;
			break;
		}
	}
	if (!hasempty)
	{
		std::set<CardPoint> cardpoints;
		cardpoints.insert(pointrecord);
		m_id2teamrecords[m_recordindex] = cardpoints;

		m_roleid2team[roleid] = m_recordindex;

		SSInfo<<"role:"<<roleid<<" index:"<<m_recordindex<<END;

		++m_recordindex;
	}
	m_ischanged = true;
}

void CGuildCard::DelRecord(UINT64 roleid)
{
	auto i = m_roleid2team.find(roleid);
	if (i == m_roleid2team.end())
	{
		return;
	}
	auto j = m_id2teamrecords.find(i->second);
	if (j == m_id2teamrecords.end())
	{
		return;
	}
	auto k = j->second.find(roleid);
	if (k != j->second.end())
	{
		j->second.erase(k);
	}
	m_roleid2team.erase(i);

	m_ischanged = true;
}

void CGuildCard::ArrangeRecord()
{
	m_toprecords.clear();
	m_recordindex = 0;
	m_roleid2team.clear();
	m_id2teamrecords.clear();

	const std::map<UINT64, GuildMember>& members = m_guild->GetGuildMember();
	UINT32 count = 0;
	UINT32 allcount = members.size();
	std::set<CardPoint> cardpoint;
	std::set<CardPoint>* lastcardpoint = NULL;
	for (auto i = members.begin(); i != members.end(); ++i)
	{
		++count;
		m_roleid2team[i->first] = m_recordindex;
		cardpoint.insert(CardPoint(i->first));
		///> 没4人一组，剩下最后的一组
		if ((0 == count % CARDTEAM_COUNT) || (count == allcount))
		{
			m_id2teamrecords[m_recordindex] = cardpoint;
			// mark last
			lastcardpoint = &m_id2teamrecords[m_recordindex];

			cardpoint.clear();
			++m_recordindex;	
		}
	}
	if (m_id2teamrecords.size() > 1 && 1 == lastcardpoint->size())
	{
		auto i = m_id2teamrecords[0].begin();
		UINT64 roleid = i->roleid;
		m_id2teamrecords[0].erase(i);
		lastcardpoint->insert(CardPoint(roleid));
		m_roleid2team[roleid] = (m_recordindex - 1);
	}

	m_ischanged = true;

	SSInfo<<"arrange record size:"<<m_id2teamrecords.size()<<" index:"<<m_recordindex<<END;
}

bool CGuildCard::BeginMatch()
{
	if (!m_matchpoints.empty())
	{
		SSError<<"match has opened today"<<END;
		return false;
	}
	if (NULL != m_match && KKSG::CardMatch_StateDummy != m_match->GetState())
	{
		SSError<<"match is running, can not begin a new match";
		return false;					
	}
	else
	{
		m_match = new GuildCardMatch(m_guild);
	}
	m_match->Run();
	return true;
}

void CGuildCard::EndMatch()
{
	if (NULL != m_match)
	{
		delete m_match;
		m_match = NULL;
	}
}

bool CGuildCard::AddMatch(UINT64 roleid)
{
	if (NULL == m_match)
	{
		return false;
	}
	UINT32 timeleft = 0;
	UINT32 round = (UINT32)(-1);
	return m_match->AddRole(roleid, timeleft, round);
}

bool CGuildCard::DelMatch(UINT64 roleid)
{
	if (NULL == m_match)
	{
		return false;
	}
	m_match->DelRole(roleid);
	return true;
}

void CGuildCard::OnLogin(CRole* role)
{
	if (NULL != m_match && m_match->IsBegin())
	{
		role->HintNotify(GUILDCARD_MATCH_SYSID, false);
		m_match->DelRole(role->GetID());
		return;
	}
}

void CGuildCard::UpdateMatchPoints(UINT64 roleid, UINT32 point)
{
	/*if (0 == point)
	{
	return;
	}*/
	for (auto i = m_matchpoints.begin(); i != m_matchpoints.end(); ++i)
	{
		if (i->roleid == roleid)
		{
			point += i->point;	
			m_matchpoints.erase(i);
			break;
		}
	}
	m_matchpoints.insert(CardPoint(roleid, point, (UINT32)time(NULL)));
}

void CGuildCard::DispatchMatchReward()
{
	int level_seal = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eLevelSealType));
	UINT32 count = 0;
	for (auto i = m_matchpoints.begin(); i != m_matchpoints.end(); ++i)
	{
		// mail
		const std::vector<ItemDesc>* items = GuildCardMgr::Instance()->GetRankReward(1, count,level_seal);
		if (NULL != items)
		{
			const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_CardMatch);
			if (NULL != conf)
			{
				std::string content = MailConfig::Instance()->ReplaceString("0", ToString(count + 1), conf->m_content);
				MailData mail(conf->m_title, content, *items, MAIL_EXPIRE_TIME);
				mail.SetReason(ItemFlow_GuildCardAward);
				CMailMgr::Instance()->SendMail(i->roleid, mail);
			}
		}
		++count;
	}
}

// 由于客户端与服务器已经把状态弄混了，现在只要按照已有逻辑状态处理
KKSG::MulActivityTimeState  CGuildCard::GetMatchStatus()
{
	if (NULL != m_match)
	{
		if (m_match->IsBegin())
		{
			return KKSG::MULACTIVITY_RUNNING;
		}
		return KKSG::MULACTIVITY_END; 
	}

	if (m_matchpoints.empty())
	{
		return KKSG::MULACTIVITY_BEfOREOPEN;
	}

	return KKSG::MULACTIVITY_END;
}

void CGuildCard::Test()
{
	for (UINT64 i = 0; i < 1000; ++i)
	{
		m_guild->AddMember(i, GPOS_MEMBER);
	}
	ArrangeRecord();
}
