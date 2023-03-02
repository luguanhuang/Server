#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild.h"
#include "guildcard.h"
#include "guildcardmatch.h"
#include "role/rolesummarymgr.h"
#include "guild/ptcm2c_guildcardmatchntf.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "guild/ptcm2c_guildbestcardsntfms.h"
#include "network/gslink.h"
#include "notice/noticemgr.h"
#include "define/systemiddef.h"
#include "pokertournament/cardmatchmgr.h"
#include "guild/ptcm2c_guildcardrankntf.h"
#include "role/role.h"
#include "util/gametime.h"
#include "common/rpcm2g_msgiveitem.h"

#define GUILDCARD_MATCH_INTERVAL 1000

GuildCardMatch::GuildCardMatch(Guild* guild)
	:m_guild(guild)
{
	m_state = KKSG::CardMatch_StateBegin;
	m_time = 0;
	m_timeround = 0;
	m_round = 0;
	m_timer = INVALID_HTIMER;

	GUILDCARD_MATCH_WAITING		 = GetGlobalConfig().CardMatchWaiting;
	GUILDCARD_MATCH_ROUNDWAITING = GetGlobalConfig().CardMatchRoundWaiting;
	GUILDCARD_MATCH_ROUNDING	 = GetGlobalConfig().CardMatchRounding;
	GUILDCARD_MATCH_ROUND		 = GetGlobalConfig().CardMatchRound;
	GUILDCARD_MATCH_CHANGE		 = GetGlobalConfig().CardMatchChange;
	GUILDCARD_MATCH_CHANGEADD	 = GetGlobalConfig().CardMatchChangeAdd;
}

GuildCardMatch::GuildCardMatch(const std::unordered_set<UINT64>& roles)
{
	m_state = KKSG::CardMatch_StateBegin;
	m_time = 0;
	m_timeround = 0;
	m_round = 0;
	m_timer = INVALID_HTIMER;

	auto it = roles.begin();
	auto it_end = roles.end();
	for (; it != it_end; ++it)
	{
		roles_.push_back(*it);
	}

	GUILDCARD_MATCH_WAITING		 = GetGlobalConfig().PokerTournamentWaiting;
	GUILDCARD_MATCH_ROUNDWAITING = GetGlobalConfig().PokerTournamentRoundWaiting;
	GUILDCARD_MATCH_ROUNDING	 = GetGlobalConfig().PokerTournamentRounding;
	GUILDCARD_MATCH_ROUND		 = GetGlobalConfig().PokerTournamentRound;
	GUILDCARD_MATCH_CHANGE		 = GetGlobalConfig().PokerTournamentChange;
	GUILDCARD_MATCH_CHANGEADD	 = GetGlobalConfig().PokerTournamentChangeAdd;
}

GuildCardMatch::~GuildCardMatch()
{
	Cancel();	
}

void GuildCardMatch::Run()
{
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, GUILDCARD_MATCH_INTERVAL, -1, __FILE__, __LINE__);
	m_time = time(NULL) + GUILDCARD_MATCH_WAITING;
}

void GuildCardMatch::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	time_t nowtime = GameTime::GetTime();
	switch(m_state)
	{
	case KKSG::CardMatch_StateBegin:
		{
			m_time = nowtime + GUILDCARD_MATCH_WAITING;
			m_state = KKSG::CardMatch_StateWaiting;

			BeginMatch();					
			break;
		}
	case KKSG::CardMatch_StateWaiting:
		{
			if (nowtime >= m_time)
			{
				m_timeround = 0;
				m_round = 0;
				m_state = KKSG::CardMatch_StateRoundWaiting;
				if (roles_.size() > 0)
				{
					NoticeMgr::Instance()->NoticePokerTournamentCardMatch(roles_, PokerTournamentRoundNotice, m_round + 1);
				}
				else
				{
					NoticeMgr::Instance()->NoticeGuildCardMatch(m_guild->GetID(), GuildCardRoundNotice, m_round + 1);
				}
			}
			break;
		}
	case KKSG::CardMatch_StateRoundWaiting:
		{
			if (nowtime >= m_timeround)
			{
				EmptyWait();
				m_state = KKSG::CardMatch_StateRoundBegin;
			}
			break;
		}
	case KKSG::CardMatch_StateRoundBegin:
		{
			m_timeround = nowtime + GUILDCARD_MATCH_ROUNDING;
			// dispatch cards
			BeginRound();

			m_state = KKSG::CardMatch_StateRounding;
			break;
		}
	case KKSG::CardMatch_StateRounding:
		{
			// check round end
			if (nowtime >= m_timeround)
			{
				m_state = KKSG::CardMatch_StateRoundEnd;	
			}
			break;
		}
	case KKSG::CardMatch_StateRoundEnd:
		{
			EndRound();
			// check match end
			++m_round;
			if (m_round >= GUILDCARD_MATCH_ROUND)
			{
				m_state = KKSG::CardMatch_StateEnd;
			}
			else
			{
				m_timeround = nowtime + GUILDCARD_MATCH_ROUNDWAITING;
				m_state = KKSG::CardMatch_StateRoundWaiting;
				WaitRound();
			}
			break;
		}
	case KKSG::CardMatch_StateEnd:
		{
			EndMatch();
			m_state = KKSG::CardMatch_StateDummy;
			DelRoleFromMatch();
			break;
		}
	case KKSG::CardMatch_StateDummy:
		{
			Cancel();
			break;
		}
	default:
		break;
	}
}

bool GuildCardMatch::AddRole(UINT64 roleid, UINT32& timeleft, UINT32& round)
{
	if (m_round + 1 >= GUILDCARD_MATCH_ROUND)
	{
		return false;
	}
	time_t nowtime = GameTime::GetTime();
	if (m_state == KKSG::CardMatch_StateWaiting || m_state == KKSG::CardMatch_StateBegin)
	{
		bool exist = false;
		auto it = m_waitingroles.begin();
		auto it_end = m_waitingroles.end();
		for (; it != it_end; ++it)
		{
			if (*it == roleid)
			{
				exist = true;
			}
		}
		if (!exist)
		{
			m_waitingroles.push_back(roleid);
		}
		timeleft = m_time >= nowtime ? (UINT32)(m_time - nowtime) : 0;
		round = (UINT32)(-1);

		UpdateRank();

		return true;
	}
	else if (m_state >= KKSG::CardMatch_StateRoundWaiting || m_state <= KKSG::CardMatch_StateRoundEnd)
	{
		bool exist = false;
		auto it = m_waitingroles.begin();
		auto it_end = m_waitingroles.end();
		for (; it != it_end; ++it)
		{
			if (*it == roleid)
			{
				exist = true;
			}
		}
		if (!exist)
		{
			m_waitingroles.push_back(roleid);
		}

		timeleft = m_timeround >= nowtime ? (UINT32)(m_timeround - nowtime) : 0;
		round = m_round;

		UpdateRank();

		return true;
	}
	else
	{
		return false;
	}
}

void GuildCardMatch::DelRole(UINT64 roleid)
{
	UINT32 count = 0;
	auto i = m_roleid2cards.find(roleid);
	if (i != m_roleid2cards.end())
	{
		count = i->second.changecount;
		m_roleid2cards.erase(i);
		m_roleid2changecount[roleid] = count;
	}
	else
	{
		for (auto i = m_waitingroles.begin(); i != m_waitingroles.end(); ++i)
		{
			if (roleid == *i)
			{
				m_waitingroles.erase(i);
				break;
			}
		}
	}
	return;
}

std::vector<UINT32>* GuildCardMatch::ChangeRoundRole(UINT64 roleid, UINT32 card, bool& canchange)
{
	GuildCardMatchRoleInfo* info = GetInfo(roleid);
	if (NULL == info)
	{
		return NULL;
	}
	if (info->changecount > 0)
	{
		canchange = true;
		--info->changecount;
		GuildCardMgr::Instance()->ChangeCard(card, info->cards, info->cardstore);
	}
	else
	{
		canchange = false;
	}
	return &info->cards;
}

UINT32 GuildCardMatch::EndRoundRole(UINT64 roleid)
{
	UINT32 result = CardDummy;
	GuildCardMatchRoleInfo* info = GetInfo(roleid);
	if (NULL == info)
	{
		return result;
	}
	if (!info->isend)
	{
		result = EndRoundRole(roleid, info);
		if (UpdateBestCards(roleid, info->result, info->cards))
		{
			NotifyBestCards();
		}
	}
	return result;
}

void GuildCardMatch::EmptyWait()
{
	while (!m_waitingroles.empty())
	{
		UINT64 roleid = m_waitingroles.front();
		m_roleid2cards[roleid] = GuildCardMatchRoleInfo(); 
		auto i = m_roleid2changecount.find(roleid);
		if (i != m_roleid2changecount.end())
		{
			m_roleid2cards[roleid].changecount = i->second;
			m_roleid2cards[roleid].isnew = false;
			m_roleid2changecount.erase(i);
		}
		m_waitingroles.pop_front();
	}
}

void GuildCardMatch::WaitRound()
{
	EmptyWait();
	for (auto i = m_roleid2cards.begin(); i != m_roleid2cards.end(); ++i)
	{
		NotifyClient(i->first, KKSG::CardMatch_StateRoundWaiting, &i->second);
	}
	if (roles_.size() > 0)
	{
		NoticeMgr::Instance()->NoticePokerTournamentCardMatch(roles_, PokerTournamentRoundNotice, m_round + 1);
	}
	else
	{
		NoticeMgr::Instance()->NoticeGuildCardMatch(m_guild->GetID(), GuildCardRoundNotice, m_round + 1);
	}
}

void GuildCardMatch::BeginRound()
{
	m_bestinfo.Reset();
	for (auto i = m_roleid2cards.begin(); i != m_roleid2cards.end(); ++i)
	{
		i->second.Reset();
		i->second.changecount += i->second.isnew ? GUILDCARD_MATCH_CHANGE : GUILDCARD_MATCH_CHANGEADD;
		i->second.isnew = false;
		i->second.store = GuildCardMgr::Instance()->RandomStore(i->second.cards, i->second.cardstore);
		i->second.result = GuildCardMgr::Instance()->GetResult(i->second.cards);
		i->second.isend = false;

		NotifyClient(i->first, KKSG::CardMatch_StateRoundBegin, &i->second);
	}
}

UINT32 GuildCardMatch::EndRoundRole(UINT64 roleid, GuildCardMatchRoleInfo* info)
{
	UINT32 result = GuildCardMgr::Instance()->GetResult(info->cards);
	info->isend = true;
	info->result = result;

	std::vector<ItemDesc> items;
	// update point
	if (roles_.size() > 0)
	{
		UpdateMatchPoints(roleid, GuildCardMgr::Instance()->GetPoint(result));
		GuildCardMgr::Instance()->GetPokerTournamentMatchReward(result, items);
	}
	else
	{
		m_guild->Get<CGuildCard>()->UpdateMatchPoints(roleid, GuildCardMgr::Instance()->GetPoint(result));
		GuildCardMgr::Instance()->GetMatchReward(result, items);
	}
	// reward	
	if (!items.empty())
	{
		std::unordered_map<UINT32,UINT32> award_items;
		auto it = role_totalawards_.find(roleid);
		if (it != role_totalawards_.end())
		{
			auto it_item = it->second.begin();
			auto it_item_end = it->second.end();
			for (; it_item != it_item_end; ++it_item)
			{
				award_items.insert(std::make_pair(it_item->first,it_item->second));
			}
		}

		CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
		if (NULL != role)
		{
			RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
			rpc->m_oArg.set_roleid(roleid);
			rpc->m_oArg.set_reason(ItemFlow_Guild);
			rpc->m_oArg.set_subreason(ItemFlow_GuildCardMatch);
			for (auto i = items.begin(); i != items.end(); ++i)
			{
				KKSG::ItemBrief* item = rpc->m_oArg.add_items();
				item->set_itemid(i->itemID);
				item->set_itemcount(i->itemCount);

				auto item_it = award_items.find(i->itemID);
				if (item_it != award_items.end())
				{
					item_it->second += i->itemCount;
				}
				else
				{
					award_items.insert(std::make_pair(i->itemID,i->itemCount));
				}
			}
			GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);

			role_totalawards_[role->GetID()] = award_items;
		}		
	}

	NotifyClient(roleid, KKSG::CardMatch_StateRoundEnd, info);
	return result;
}

void GuildCardMatch::EndRound()
{
	bool isnotify = false;
	for (auto i = m_roleid2cards.begin(); i != m_roleid2cards.end(); ++i)
	{
		if (!i->second.isend)
		{
			EndRoundRole(i->first, &i->second);
			isnotify |= UpdateBestCards(i->first, i->second.result, i->second.cards);
		}
	}
	if (isnotify)
	{
		NotifyBestCards();
	}
}

void GuildCardMatch::BeginMatch()
{
	if (roles_.size() > 0)
	{
		// 先要清理积分
		match_points_.clear();

		PtcM2C_HintNotifyMS ntf;
		ntf.m_Data.add_systemid(SYS_POKERTOURNAMENT);
		ntf.m_Data.set_isremove(false);
		auto it = roles_.begin();
		auto it_end = roles_.end();
		for ( ; it != it_end; ++it)
		{
			CRole* role = CRoleManager::Instance()->GetByRoleID(*it);
			if (NULL == role)
			{
				continue;
			}
			role->Send(ntf);
		}

		NoticeMgr::Instance()->NoticePokerTournamentCardMatch(roles_, PokerTournamentBeginNotice);
	}
	else
	{
		m_guild->Get<CGuildCard>()->ClearMatchPoints();

		PtcM2C_HintNotifyMS ntf;
		ntf.m_Data.add_systemid(GUILDCARD_MATCH_SYSID);
		ntf.m_Data.set_isremove(false);
		m_guild->BroadCast(ntf);

		NoticeMgr::Instance()->NoticeGuildCardMatch(m_guild->GetID(), GuildCardBeginNotice);
	}
}

void GuildCardMatch::EndMatch()
{
	for (auto i = m_roleid2cards.begin(); i != m_roleid2cards.end(); ++i)
	{
		NotifyClient(i->first, KKSG::CardMatch_RoundEnd, &i->second);
	}

	PtcM2C_HintNotifyMS ntf;
	ntf.m_Data.set_isremove(true);
	if (roles_.size() > 0)
	{
		ntf.m_Data.add_systemid(SYS_POKERTOURNAMENT);
		auto it = roles_.begin();
		auto it_end = roles_.end();
		for ( ; it != it_end; ++it)
		{
			CRole* role = CRoleManager::Instance()->GetByRoleID(*it);
			if (NULL == role)
			{
				continue;
			}
			role->Send(ntf);
		}
		// 发奖励
		CardMatchMgr::Instance()->DispatchPokerTournamentReward(match_points_);
		// 发通知				
		NoticeMgr::Instance()->NoticePokerTournamentCardMatch(roles_,PokerTournamentEndNotice);
		// 积分第一的玩家
		if (match_points_.size() > 0)
		{
			auto it = match_points_.begin();
			CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(it->roleid);
			if (NULL != role)
			{
				NoticeMgr::Instance()->NoticePokerTournamentFirst(roles_, role);
			}
		}
	}
	else
	{
		ntf.m_Data.add_systemid(GUILDCARD_MATCH_SYSID);
		m_guild->BroadCast(ntf);
		m_guild->Get<CGuildCard>()->DispatchMatchReward();
		NoticeMgr::Instance()->NoticeGuildCardMatch(m_guild->GetID(), GuildCardEndNotice);

		const std::set<CardPoint>* toppoints = m_guild->Get<CGuildCard>()->GetMatchPoints();
		if (NULL != toppoints && !toppoints->empty())
		{
			CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(toppoints->begin()->roleid);
			if (NULL != role)
			{
				NoticeMgr::Instance()->NoticeGuildCardFirst(m_guild->GetID(), role);
			}
		}
	}
}

void GuildCardMatch::NotifyClient(UINT64 roleid, UINT32 type, GuildCardMatchRoleInfo* info)
{
	if (NULL == info)
	{
		return;
	}
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == role)
	{
		return;
	}
	time_t nowtime = GameTime::GetTime();
	UINT32 timeleft = m_timeround >= nowtime ? (UINT32)(m_timeround - nowtime) : 0;
	// wait round
	PtcM2C_GuildCardMatchNtf ntf;
	if (roles_.size() > 0 )
	{
		ntf.m_Data.set_match_type(1);
	}
	auto it = role_totalawards_.find(roleid);
	if (it != role_totalawards_.end())
	{
		auto item_it = it->second.begin();
		auto item_it_end = it->second.end();
		for (; item_it != item_it_end; ++item_it)
		{
			KKSG::ItemBrief* brief = ntf.m_Data.add_items();
			brief->set_itemid(item_it->first);
			brief->set_itemcount(item_it->second);
		}
	}

	ntf.m_Data.set_state((KKSG::CardMatchState)(m_state));
	if (KKSG::CardMatch_StateRoundWaiting == type)
	{
		ntf.m_Data.set_timeleft(timeleft);	
		ntf.m_Data.set_op(KKSG::CardMatch_RoundWaiting);
		role->Send(ntf);
	}
	// cards
	else if (KKSG::CardMatch_StateRoundBegin == type)
	{
		ntf.m_Data.set_op(KKSG::CardMatch_RoundBegin   );
		ntf.m_Data.set_timeleft(timeleft);	
		ntf.m_Data.set_round(m_round);
		ntf.m_Data.set_changecount(info->changecount);
		ntf.m_Data.set_store(info->store);
		ntf.m_Data.set_result(info->result);
		for (auto i = info->cards.begin(); i != info->cards.end(); ++i)
		{
			ntf.m_Data.add_cards(*i);
		}
		role->Send(ntf);
	}
	// result
	else if (KKSG::CardMatch_StateRoundEnd == type)
	{
		ntf.m_Data.set_op(KKSG::CardMatch_RoundEnd);
		ntf.m_Data.set_timeleft(timeleft);	
		ntf.m_Data.set_result(info->result);
		role->Send(ntf);
	}
	// all end
	else if (KKSG::CardMatch_StateEnd == type)
	{
		ntf.m_Data.set_op(KKSG::CardMatch_End);
		role->Send(ntf);
	}
}

void GuildCardMatch::Cancel()
{
	if (INVALID_HTIMER != m_timer)
	{
		CTimerMgr::Instance()->KillTimer(m_timer);
		m_timer = INVALID_HTIMER;
	}	
}

void GuildCardMatch::DelRoleFromMatch()
{
	auto it = roles_.begin();
	auto it_end = roles_.end();
	for (; it != it_end; ++it)
	{
		CardMatchMgr::Instance()->DeleteFromCardMatch(*it);
	}
}

GuildCardMatchRoleInfo* GuildCardMatch::GetInfo(UINT64 roleid)
{
	auto i = m_roleid2cards.find(roleid);
	if (i == m_roleid2cards.end())
	{
		return NULL;
	}
	return &i->second;
}

void GuildCardMatch::NotifyBestCards()
{
	PtcM2C_GuildBestCardsNtfMs ptc;
	if (roles_.size() > 0 )
	{
		ptc.m_Data.set_match_type(1);
	}
	ptc.m_Data.set_type(1);
	ptc.m_Data.set_bestresult(m_bestinfo.result);
	ptc.m_Data.set_bestrole(CRoleSummaryMgr::Instance()->GetRoleName(m_bestinfo.roleid));
	for (auto i = m_bestinfo.cards.begin(); i != m_bestinfo.cards.end(); ++i)
	{
		ptc.m_Data.add_bestcards(*i);
	}
	for (auto i = m_roleid2cards.begin(); i != m_roleid2cards.end(); ++i)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(i->first);
		if (NULL != role)
		{
			role->Send(ptc);
		}
	}
}

bool GuildCardMatch::UpdateBestCards(UINT64 roleid, UINT32 result, std::vector<UINT32>& cards)
{
	if (result > m_bestinfo.result)
	{
		return false;
	}
	else if (result == m_bestinfo.result)
	{
		if (GuildCardMgr::Instance()->Compare(result, m_bestinfo.cards, cards))
		{
			return false;
		}
	}
	m_bestinfo.roleid = roleid;
	m_bestinfo.result = result;
	m_bestinfo.cards = cards;
	return true;
}

void GuildCardMatch::UpdateMatchPoints(UINT64 roleid, UINT32 point)
{	
	for (auto i = match_points_.begin(); i != match_points_.end(); ++i)
	{
		if (i->roleid == roleid)
		{
			point += i->point;	
			match_points_.erase(i);
			break;
		}
	}
	match_points_.insert(CardPoint(roleid, point, (UINT32)GameTime::GetTime()));
}

UINT32 GuildCardMatch::GetRolesNum()
{
	return roles_.size();
}


void GuildCardMatch::UpdateRank()
{
	auto role_it = m_waitingroles.begin();
	auto role_it_end = m_waitingroles.end();
	for (; role_it != role_it_end; ++role_it)
	{
		if (roles_.size() > 0)
		{
			UpdateMatchPoints(*role_it, 0);
		}
		else
		{
			m_guild->Get<CGuildCard>()->UpdateMatchPoints(*role_it, 0);
		}
	}

	const std::set<CardPoint>* points  = NULL;	
	PtcM2C_GuildCardRankNtf ntf;
	if (roles_.size() > 0)
	{
		ntf.m_Data.set_type(3);
		points = GetMatchPoints();	
	}
	else
	{
		ntf.m_Data.set_type(2);
		points = m_guild->Get<CGuildCard>()->GetMatchPoints();
	}
	for (auto i = points->begin(); i != points->end(); ++i)
	{
		ntf.m_Data.add_name(CRoleSummaryMgr::Instance()->GetRoleName(i->roleid));	
		ntf.m_Data.add_point(i->point);	
	}

	auto it		= m_waitingroles.begin();
	auto it_end = m_waitingroles.end();
	for (; it != it_end; ++it)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(*it);
		if (NULL == role)
		{
			SSWarn<<"role is NULL, role_id:"<<*it<<END;
			continue;
		}

		role->Send(ntf);
	}	
}