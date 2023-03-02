#include "pch.h"
#include "guildcamprank.h"
#include "pb/project.pb.h"
#include "role/rolesummarymgr.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "account/punishmgr.h"
#include "guild/guildcamp.h"
#include "guild/guilddef.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"

CGuildCampRankInfo::CGuildCampRankInfo(std::vector<UINT64>& roleids, bool isAscend)
{
	m_isAscend = isAscend;
	m_timestamp = 0;
	m_var = 0;
	for (size_t i = 0; i < roleids.size(); ++i)
	{
		m_roleIDs.insert(roleids[i]);
	}
}

CGuildCampRankInfo::~CGuildCampRankInfo()
{

}

bool CGuildCampRankInfo::Convert2KKSGGuildCampRankInfo(KKSG::GuildCampRankInfo& info) const
{
	info.set_rankvar(m_var);
	for (auto roleBegin = m_roleIDs.begin(); roleBegin != m_roleIDs.end(); ++roleBegin)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(*roleBegin);
		if (!summary)
		{
			continue;
		}
		KKSG::RoleBriefInfo* brief = info.add_roles();
		brief->set_roleid(*roleBegin);
		brief->set_type((KKSG::RoleType)summary->GetProfession());
		brief->set_name(summary->GetName());
	}
	if (!info.roles_size())
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
GuildCampRankList::GuildCampRankList(CGuildCamp* camp)
{	
	m_guildCamp = camp;
}

GuildCampRankList::~GuildCampRankList()
{
	Clear();
}

void GuildCampRankList::Clear()
{
	m_rankinfos.clear();
	SetChanged();
}


void GuildCampRankList::LoadFromDb(const KKSG::GuildCampSave& guildCampInfo)
{
	for (int i = 0; i < guildCampInfo.rankinfos_size(); ++i)
	{
		std::vector<UINT64> roleids;
		for (int j = 0; j < guildCampInfo.rankinfos(i).roleids_size(); ++j)
		{
			roleids.push_back(guildCampInfo.rankinfos(i).roleids(j));
		}
		CGuildCampRankInfo tempInfo(roleids, m_isAscend);
		tempInfo.m_var = guildCampInfo.rankinfos(i).var();
		tempInfo.m_timestamp = guildCampInfo.rankinfos(i).timestamp();

		m_rankinfos.insert(tempInfo);
	}

}

void GuildCampRankList::SaveDb(KKSG::GuildCampSave& guildCampInfo)
{
	for (auto iter = m_rankinfos.begin(); iter != m_rankinfos.end(); ++iter)
	{
		KKSG::GuildCampRankInfo2DB* temp = guildCampInfo.add_rankinfos();
		for (auto i = iter->m_roleIDs.begin(); i != iter->m_roleIDs.end(); ++i)
		{
			temp->add_roleids(*i);
		}
		temp->set_var(iter->m_var);
		temp->set_timestamp(iter->m_timestamp);
	}
}

void GuildCampRankList::OnChange(std::vector<UINT64>& roleids, int var, UINT32 timestamp)
{
	if (roleids.empty())
	{
		return;
	}
	bool isChange = false;
	CGuildCampRankInfo temp(roleids, m_isAscend);
	temp.m_timestamp = timestamp;
	temp.m_var = var;

	auto find = m_rankinfos.find(temp);
	if (find == m_rankinfos.end())
	{
		m_rankinfos.insert(temp);
		isChange = true;
	}
	else
	{
		if (m_isAscend)
		{
			if (temp.m_var < find->m_var)
			{
				m_rankinfos.erase(find);
				m_rankinfos.insert(temp);
				isChange = true;
			}
		}
		else
		{
			if (temp.m_var > find->m_var)
			{
				m_rankinfos.erase(find);
				m_rankinfos.insert(temp);
				isChange = true;
			}
		}
	}
	if (m_rankinfos.size() > CAMPRANKCOUNT)
	{
		m_rankinfos.erase(--m_rankinfos.end());
	}

	if(isChange)
		SetChanged();
}

void GuildCampRankList::FillRankInfo2Client(KKSG::GuildCampInfoRes& roRes)
{
	int rank = 1;
	for (std::set<CGuildCampRankInfo, GuildCampRankInfoCompare>::iterator iter = m_rankinfos.begin(); iter != m_rankinfos.end(); ++iter)
	{
		KKSG::GuildCampRankInfo info;
		if (iter->Convert2KKSGGuildCampRankInfo(info))
		{
			info.set_rank(rank++);
			roRes.add_rankinfos()->CopyFrom(info);
		}
	}
}

void GuildCampRankList::SetChanged()
{
	m_guildCamp->SetChanged();
}

void GuildCampRankList::SetRankType(UINT32 type)
{
	auto data = CGuildConfig::Instance()->GetGuildCampTableData(type);
	if(!data)
		return;
	switch(data->Type)
	{
	case GUILDCAMP_COSTTIME:
		m_isAscend = true;
		break;
	case GUILDCAMP_KILLNUM:
		m_isAscend = false;
		break;
	case GUILDCAMP_LIVETIME:
		m_isAscend = false;
		break;
	default:
		break;
	}
}

void GuildCampRankList::OnDayPass()
{
	UINT32 rank = 1;
	std::map<UINT32, UINT32> rewards;
	std::set<UINT64> rewardRoles;
	std::stringstream content;
	for (auto iter = m_rankinfos.begin(); iter != m_rankinfos.end(); ++iter)
	{
		rewards.clear();
		if(CGuildConfig::Instance()->GetGuildCampRankReward(rank++, rewards))
		{
			std::vector<ItemDesc> items;
			for (auto i = rewards.begin(); i != rewards.end(); ++i)
			{
				ItemDesc item(i->first, i->second);
				items.push_back(item);
			}
			if (items.size())
			{
				content.str("");
				content << GetGlobalConfig().GuildCampRankMailContent << (rank - 1) <<"!";
				MailData mailData( GetGlobalConfig().GuildCampRankMailTitle, content.str(), items, MAIL_EXPIRE_TIME);
				mailData.SetReason(ItemFlow_GuildCampRankAward);
				for (auto roleBegin = iter->m_roleIDs.begin(); roleBegin != iter->m_roleIDs.end(); ++roleBegin)
				{
					auto find = rewardRoles.find(*roleBegin);
					if (find == rewardRoles.end())
					{
						CMailMgr::Instance()->SendMail(*roleBegin, mailData);
						rewardRoles.insert(*roleBegin);
					}
				}
			}
		}
		else
			break;
	}
}
