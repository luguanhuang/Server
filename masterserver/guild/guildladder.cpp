#include "pch.h"
#include "guildladder.h"
#include "guild.h"
#include "guildmgr.h"
#include "table/globalconfig.h"
#include "../share/util/XCommon.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include <time.h>
#include "notice/noticemgr.h"
#include "table/globalconfig.h"
#include "guild/ptcm2c_noticeguildladderstart.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "table/OpenSystemMgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "mail/mailconfig.h"

#include "foreach.h"




void GuildGroup::AddTime(UINT64 roleId)
{
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roleId);
	if (guildId == 0)
	{
		return ;
	}

	UINT32 guildNum = 0;
	foreach (i in m_guildRank)
	{
		if (i->second == guildId)
		{
			guildNum = i->first;
			m_guildRank.erase(i);
			m_guildRank.insert(make_pair(++guildNum, guildId));
			break;
		}
	}

	UINT32 roleNum = 0;
	foreach (j in m_roleRank)
	{
		if (j->second == roleId)
		{
			roleNum = j->first;
			m_roleRank.erase(j);
			break;
		}
	}
	m_roleRank.insert(make_pair(++roleNum, roleId));

	if ( GetGlobalConfig().GuildLadderWinNotice.find(roleNum) != GetGlobalConfig().GuildLadderWinNotice.end())
	{
		NoticeMgr::Instance()->NoticeGuildLadderWinTimes(guildId, roleId, roleNum);
	}
	else
	{
		NoticeMgr::Instance()->NoticeGuildLadderWin(guildId, roleId);
	}

}

void GuildGroup::DoEnd()
{
	if (m_isEnd)
	{
		return ;
	}

	m_isEnd = true;
	GiveRoleRankReward();
	GiveGuildRankReward();
}

void GuildGroup::GiveRoleRankReward()
{
	UINT32 rank = 0;

	for (std::multimap<UINT32, UINT64>::reverse_iterator iter = m_roleRank.rbegin(); iter != m_roleRank.rend(); ++iter)
	{
		std::vector<ItemDesc> vecItem;
		CGuildLadder::Instance()->GetRoleReward(++rank, vecItem);
		std::string content = MailConfig::Instance()->ReplaceString("0", ToString(rank), GetGlobalConfig().GuildLadderRoleCon);
		if (!vecItem.empty())
		{
			MailData mail( GetGlobalConfig().GuildLadderRoleTitle, content, vecItem, MAIL_EXPIRE_TIME);
			mail.SetReason(ItemFlow_GuildLadderAward);
			CMailMgr::Instance()->SendMail(iter->second, mail);
		}
	}
	/*
	foreach (i in m_roleRank)
	{
		std::vector<ItemDesc> vecItem;
		CGuildLadder::Instance()->GetRoleReward(++rank, vecItem);
		if (!vecItem.empty())
		{
			MailData mail( GetGlobalConfig().GuildLadderRoleTitle, GetGlobalConfig().GuildLadderRoleCon, vecItem, MAIL_EXPIRE_TIME);
			CMailMgr::Instance()->SendMail(i->second, mail);
		}
	}
	*/
}

void GuildGroup::GiveGuildRankReward()
{
	UINT32 rank = 0;
	for (std::multimap<UINT32, UINT64>::reverse_iterator iter = m_guildRank.rbegin(); iter != m_guildRank.rend(); ++iter)
	{
		std::map<UINT32, UINT32> guildReward;
		std::vector<ItemDesc> vecItem;
		CGuildLadder::Instance()->GetGuildReward(++rank, vecItem, guildReward);
		std::string content = MailConfig::Instance()->ReplaceString("0", ToString(rank), GetGlobalConfig().GuildLadderCon);

		Guild* pGuild = CGuildMgr::Instance()->GetGuild(iter->second);
		if (pGuild != NULL)
		{
			pGuild->AddGuildReward(guildReward);

			
			MailData mail( GetGlobalConfig().GuildLadderTitle, content, vecItem, MAIL_EXPIRE_TIME);
			mail.SetReason(ItemFlow_GuildLadderAward);
			const std::map<UINT64, GuildMember>& allMember = pGuild->GetGuildMember();
			foreach (j in allMember)
			{
				CMailMgr::Instance()->SendMail(j->first, mail);
			}
		}

	}
	/*
	foreach (i in m_guildRank)
	{
		std::map<UINT32, UINT32> guildReward;
		std::vector<ItemDesc> vecItem;
		CGuildLadder::Instance()->GetGuildReward(++rank, vecItem, guildReward);

		Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->second);
		if (pGuild != NULL)
		{
			pGuild->AddGuildReward(guildReward);

			
			MailData mail( GetGlobalConfig().GuildLadderTitle, GetGlobalConfig().GuildLadderCon, vecItem, MAIL_EXPIRE_TIME);
			const std::vector<GuildMember>& allMember = pGuild->GetGuildMember();
			foreach (j in allMember)
			{
				CMailMgr::Instance()->SendMail(j->qwRoleID, mail);
			}
		}
	}
	*/
}

bool GuildGroup::TryReward(UINT64 roleId)
{
	if (m_rewardTimes[roleId] >= GetGlobalConfig().GuildLadderRewardTimes)
	{
		return false;
	}

	m_rewardTimes[roleId]++;
	return true;
}

UINT32 GuildGroup::GetGuildRank(UINT64 guildId)
{
	UINT32 rank = 0;
	for (std::multimap<UINT32, UINT64>::reverse_iterator iter = m_guildRank.rbegin(); iter != m_guildRank.rend(); ++iter)
	{
		++rank;
		if (iter->second == guildId)
		{
			return rank;
		}
	}
	return rank;
}

void GuildGroup::GetThreeMember(UINT64 guildId, std::set<UINT64>& roleSet)
{
	UINT32 cnt = 3; // 取前3名
	for (std::multimap<UINT32, UINT64>::reverse_iterator iter = m_roleRank.rbegin(); iter != m_roleRank.rend() && cnt > 0; ++iter)
	{
		if (CGuildMgr::Instance()->GetGuildIdByRoleId(iter->second) == guildId)
		{
			roleSet.insert(iter->second);
			--cnt;
		}
	}
}

//////////////////////////////////////////////

INSTANCE_SINGLETON(CGuildLadder)

CGuildLadder::CGuildLadder()
{
	m_state = READY;
}

CGuildLadder::~CGuildLadder()
{

}

bool CGuildLadder::Init()
{
	if (!m_guildRankTable.LoadFile("table/GuildPkRankReward.txt"))
	{
		SSWarn<<"load file table/GuildPkRankReward.txt failed"<<END;
		return false;
	}

	if (!m_roleRankTable.LoadFile("table/GuildPkRoleRankReward.txt"))
	{
		SSWarn<<"load file table/GuildPkRoleRankReward.txt failed"<<END;
		return false;
	}

	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);
	return true;
}

void CGuildLadder::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timehandler);
}

void CGuildLadder::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (m_state == READY && _IsReadyTime())  // 预告公告
	{
		NoticeMgr::Instance()->NoticeGuildLadderReady();
	}

	if (m_state == GOING && _IsLastTen())  // 结束前10分钟公告
	{
		_NoticeTen();
	}

	if (m_state == GOING && _IsLastFive()) // 结束前5分钟公告
	{
		_NoticeFive();
	}

	if (m_state == READY && _GetTimeState() == GOING) // 开启活动
	{
		_DoStart();
		m_state = GOING;
	}

	if (m_state == GOING && _GetTimeState() != GOING) // 结束活动
	{
		_DoEnd();
		m_state = READY;
	}
}

void CGuildLadder::_ClearData()
{
	foreach (j in m_groupSet)
	{
		delete (*j);
	}

	m_groupSet.clear();
	m_groupMap.clear();
}

void CGuildLadder::_CreateGroup(const std::set<UINT64>& guildSet)
{
	if (guildSet.empty()) // 空则不创建
	{
		return;
	}

	GuildGroup* pGroup = new GuildGroup(guildSet);
	m_groupSet.insert(pGroup);

	foreach (i in guildSet)
	{
		m_groupMap[*i] = pGroup;
		NoticeMgr::Instance()->NoticeGuildLadderBegin(*i);
	}
}

void CGuildLadder::_DoStart()
{
	m_groupMap.clear();
	//const std::unordered_map<UINT64, Guild*>& guildMap = CGuildMgr::Instance()->GetGuilds(); 
	//UINT32 size = GetGlobalConfig().GuildLadderNum > guildMap.size() ? guildMap.size() : GetGlobalConfig().GuildLadderNum;

	std::vector<Guild *>& pGuildVec = CGuildMgr::Instance()->GetSorter().GetSortResult(KKSG::GuildSortByPrestige);
	UINT32 size = GetGlobalConfig().GuildLadderNum > pGuildVec.size() ? pGuildVec.size() : GetGlobalConfig().GuildLadderNum;

	std::set<UINT64> guildSet;
	std::set<UINT64> tempSet;
	auto iter = pGuildVec.begin();
	while (size > 0)
	{
		if (size == 7)
		{
			guildSet.clear();
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			_CreateGroup(guildSet);

			guildSet.clear();
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			_CreateGroup(guildSet);

			guildSet.clear();
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			_CreateGroup(guildSet);
			break;
		}
		if (size <= 3)
		{
			guildSet.clear();
			for (UINT32 cnt = 0; cnt < size; ++cnt)
			{
				guildSet.insert((*(iter++))->GetID());
			}
			_CreateGroup(guildSet);
			break;
		}
		if (size == 4)
		{
			guildSet.clear();
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			_CreateGroup(guildSet);

			guildSet.clear();
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			_CreateGroup(guildSet);
			break;
		}
		if (size == 5)
		{
			guildSet.clear();
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			_CreateGroup(guildSet);

			guildSet.clear();
			guildSet.insert((*(iter++))->GetID());
			guildSet.insert((*(iter++))->GetID());
			_CreateGroup(guildSet);
			break;
		}

		guildSet.clear();
		for (UINT32 cnt = 0; cnt < 6; ++cnt)
		{
			guildSet.insert((*(iter++))->GetID());
			--size;
		}

		tempSet.clear();
		for (UINT32 i = 0; i < 3; ++i)
		{
			auto iterSet = guildSet.begin();
			UINT32 rank = XRandom::randInt(0, guildSet.size());
			for (UINT32 j = 0; j < rank; ++j)
			{
				++iterSet;
			}
			tempSet.insert(*iterSet);
			guildSet.erase(iterSet);
		}
		_CreateGroup(guildSet);
		_CreateGroup(tempSet);
	}

	PtcM2C_NoticeGuildLadderStart msg;
	msg.m_Data.set_isstart(true);
	foreach (i in m_groupMap)
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->first);
		if (pGuild != NULL)
		{
			const std::map<UINT64, GuildMember>& members = pGuild->GetGuildMember();
			foreach (j in members)
			{
				CRole* pRole = CRoleManager::Instance()->GetByRoleID(j->first);
				if (pRole == NULL)
				{
					continue;
				}

				CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
				if (pRole != NULL && pRoleSum != NULL && pRoleSum->GetLevel() >= OpenSystemMgr::Instance()->GetPlayerLevelBySystemID(860))
				{
					pRole->Send(msg);
				}
			}
		}
	}

}

bool CGuildLadder::IsOpen()
{
	if (m_state == GOING)
	{
		return true;
	}

	return false;
}

CGuildLadder::STATE CGuildLadder::_GetTimeState()
{
	time_t newTime = time(NULL);
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	foreach (i in GetGlobalConfig().GuildLadderTime)
	{
		if (i->seq[0] == weekNum)  // 星期几
		{
			time_t beginTime = XCommon::MakeTime(newTime, i->seq[1] / 100, i->seq[1] % 100, 0);
			time_t endTime = XCommon::MakeTime(newTime, i->seq[2] / 100, i->seq[2] % 100, 0);

			if (newTime >= beginTime && newTime < endTime)
			{
				return GOING;
			}
			else
			{
				return READY;
			}
		}
	}
	return READY;
}

void CGuildLadder::_DoEnd()
{
	PtcM2C_NoticeGuildLadderStart msg;
	msg.m_Data.set_isstart(false);
	foreach (i in m_groupMap)
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->first);
		if (pGuild != NULL)
		{
			const std::map<UINT64, GuildMember>& members = pGuild->GetGuildMember();
			foreach (j in members)
			{
				CRole* pRole = CRoleManager::Instance()->GetByRoleID(j->first);
				if (pRole != NULL)
				{
					pRole->Send(msg);
				}
			}
		}
	}

	foreach (i in m_groupMap)
	{
		i->second->DoEnd();
	}
	_NoticeEnd();  // 发结束公告
	_ClearData();  // 清理这次的数据
}

void CGuildLadder::GetRoleReward(UINT32 rank, std::vector<ItemDesc>& vecItem)
{
	vecItem.clear();
	foreach (i in m_roleRankTable.Table)
	{
		if (rank >= (*i)->rank.seq[0] && rank <= (*i)->rank.seq[1])
		{
			foreach (j in (*i)->reward)
			{
				vecItem.push_back(ItemDesc(j->seq[0], j->seq[1]));
			}
			return ;
		}
	}
}

void CGuildLadder::GetGuildReward(UINT32 rank, std::vector<ItemDesc>& vecItem, std::map<UINT32, UINT32>& guildReward)
{
	vecItem.clear();
	foreach (i in m_guildRankTable.Table)
	{
		if (rank == (*i)->rank)
		{
			foreach (j in (*i)->reward)
			{
				vecItem.push_back(ItemDesc(j->seq[0], j->seq[1]));
			}
			foreach (k in (*i)->guildreward)
			{
				guildReward[k->seq[0]] = k->seq[1];
			}
			return ;
		}
	}
}

bool CGuildLadder::TryReward(UINT64 roleId)
{
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roleId);
	if (guildId == 0)
	{
		return false;
	}

	auto iter = m_groupMap.find(guildId);
	if (iter == m_groupMap.end())
	{
		return false;
	}

	return iter->second->TryReward(roleId);
}

/*
void CGuildLadder::_NoticeReady()
{
	NoticeMgr::Instance()->NoticeGuildLadderReady();
}
*/

bool CGuildLadder::_IsReadyTime()
{
	time_t newTime = time(NULL);
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	foreach (i in GetGlobalConfig().GuildLadderTime)
	{
		if (i->seq[0] == weekNum)  // 星期几
		{
			time_t beginTime = XCommon::MakeTime(newTime, 14, 0, 0);
			if (newTime <= beginTime && ((beginTime - newTime) == 0 || (beginTime - newTime) == 1))
			{
				return true;
			}
		}
	}
	return false;
}

bool CGuildLadder::_IsLastTen()
{
	time_t newTime = time(NULL);
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	foreach (i in GetGlobalConfig().GuildLadderTime)
	{
		if (i->seq[0] == weekNum)  // 星期几
		{
			time_t endTime = XCommon::MakeTime(newTime, i->seq[2] / 100, i->seq[2] % 100, 0);
			if (newTime < endTime && ((endTime - newTime) == 10 * 60 || (endTime - newTime) == 10 * 60 + 1))
			{
				return true;
			}
		}
	}
	return false;
}


bool CGuildLadder::_IsLastFive()
{
	time_t newTime = time(NULL);
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	foreach (i in GetGlobalConfig().GuildLadderTime)
	{
		if (i->seq[0] == weekNum)  // 星期几
		{
			time_t endTime = XCommon::MakeTime(newTime, i->seq[2] / 100, i->seq[2] % 100, 0);
			if (newTime < endTime && ((endTime - newTime) == 5 * 60 || (endTime - newTime) == 5 * 60 + 1))
			{
				return true;
			}
		}
	}
	return false;
}

UINT32 CGuildLadder::GetLastTime()
{
	time_t newTime = time(NULL);
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	foreach (i in GetGlobalConfig().GuildLadderTime)
	{
		if (i->seq[0] == weekNum)  // 星期几
		{
			time_t beginTime = XCommon::MakeTime(newTime, i->seq[1] / 100, i->seq[1] % 100, 0);
			time_t endTime = XCommon::MakeTime(newTime, i->seq[2] / 100, i->seq[2] % 100, 0);
			if (newTime >= beginTime && newTime < endTime)
			{
				return endTime - newTime;
			}
		}
	}

	return 0;
}

void CGuildLadder::_NoticeTen()
{
	foreach (i in m_groupMap)
	{
		UINT32 rank = i->second->GetGuildRank(i->first);
		NoticeMgr::Instance()->NoticeGuildLadderTen(i->first, rank);
	}
}

void CGuildLadder::_NoticeFive()
{
	foreach (i in m_groupMap)
	{
		UINT32 rank = i->second->GetGuildRank(i->first);
		NoticeMgr::Instance()->NoticeGuildLadderFive(i->first, rank);
	}
}

GuildGroup* CGuildLadder::GetGuildGroup(UINT64 guildId)
{
	auto iter = m_groupMap.find(guildId);
	if (iter != m_groupMap.end())
	{
		return iter->second;
	}
	return NULL;
}

void CGuildLadder::DoOnlineCheck(UINT64 roleId)
{
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roleId);
	GuildGroup* pGroup = GetGuildGroup(guildId);
	if (pGroup != NULL)
	{
		PtcM2C_NoticeGuildLadderStart msg;
		msg.m_Data.set_isstart(true);

		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
		if (pRole != NULL)
		{
			CRoleSummary* pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
			if (pSummary != NULL && pSummary->GetLevel() >= OpenSystemMgr::Instance()->GetPlayerLevelBySystemID(860))
			{
				pRole->Send(msg);
			}
		}
	}
}

void CGuildLadder::_NoticeEnd()
{
	foreach (i in m_groupMap)
	{
		UINT32 rank = i->second->GetGuildRank(i->first);
		std::set<UINT64> roleSet;
		i->second->GetThreeMember(i->first, roleSet);
		if (roleSet.size() >= 3) // 不足3个 不发公告
		{
			NoticeMgr::Instance()->NoticeGuildLadderEnd(i->first, rank, roleSet);
		}
	}
}
