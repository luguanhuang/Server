#include "pch.h"
#include "guildinherit.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "table/globalconfig.h"
#include "../share/util/gametime.h"
#include "guild/ptcm2g_noticeguildinheritnuminfo.h"
#include "levelseal/levelsealMgr.h"
#include "guild/ptcm2g_tryteacherinherit.h"
#include <time.h>

#include "foreach.h"


INSTANCE_SINGLETON(CGuildInherit);
CGuildInherit::CGuildInherit()
{
	m_inheritDataMap.clear();
}

CGuildInherit::~CGuildInherit()
{

}

bool CGuildInherit::Init()
{
	/*
	if (!m_table.LoadFile("table/GuildInheritExp.txt"))
	{
		SSWarn<<"load file table/GuildInheritExp.txt failed"<<END;
		return false;
	}
	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	*/
	return true;
}

void CGuildInherit::Uninit()
{
	//CTimerMgr::Instance()->KillTimer(m_timehandler);
}

/*
void CGuildInherit::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 nowTime = time(NULL);
	for (std::vector<InheritData>::iterator iter = m_doingData.begin(); iter != m_doingData.end();)
	{
		if ((nowTime - iter->m_startTime) == 0 || (nowTime - iter->m_startTime) % GetGlobalConfig().GuildInheritTime != 0)
		{
			++iter;
			continue;
		}

		UINT32 turn = (nowTime - iter->m_startTime) / GetGlobalConfig().GuildInheritTime; 
		if (turn <= GetGlobalConfig().GuildInheritTimes)
		{
			//_GiveExp(*iter);
		}

		if (turn >= GetGlobalConfig().GuildInheritTimes)
		{
			iter = m_doingData.erase(iter);
		}
		else
		{
			++iter;
		}
	}

}
*/

void CGuildInherit::AddInherit(UINT64 roleId, UINT64 reqRoleId)
{
	std::map<UINT64, UINT32>& roleMap = m_inheritDataMap[roleId];
	roleMap[reqRoleId] = time(NULL);
	NoticeReqNum(roleId);
}

void CGuildInherit::ClearInherit()
{
	m_inheritDataMap.clear();
}

UINT32 CGuildInherit::AcceptInherit(UINT64 roleId, UINT64 reqRoleId)
{/*
	auto iter = m_inheritDataMap.find(roleId);
	if (iter == m_inheritDataMap.end() )
	{
		return ERR_UNKNOWN;
	}

	auto it = iter->second.find(reqRoleId);
	if (it == iter->second.end())
	{
		return ERR_GUILD_INHERIT_NOT_EXIT;
	}
	else
	{
		iter->second.erase(it);  // 删除掉
	}

	
	UINT32 errorCode = CanInherit(roleId, reqRoleId);
	if (errorCode != ERR_SUCCESS)
	{
		return errorCode;
	}*/

	NoticeReqNum(roleId);
	NoticeReqNum(reqRoleId);
	return ERR_SUCCESS;

	/*
	std::set<UINT64>& roleSet = m_inheritDataMap[roleId];
	roleSet.insert(reqRoleId);

	InheritData data;
	data.m_roleOne = roleId; 
	data.m_roleTwo = reqRoleId; 
	data.m_startTime = time(NULL);

	m_doingData.push_back(data);
	*/
}

UINT32 CGuildInherit::CanInherit(UINT64 roleId, UINT64 reqRoleId)
{
	CRoleSummary* pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	CRoleSummary* pReqRole = CRoleSummaryMgr::Instance()->GetRoleSummary(reqRoleId);
	if (pRole == NULL || pReqRole == NULL)
	{
		return ERR_UNKNOWN;
	}

	UINT32 lvlLow = 0; 
	UINT32 lvlHig = 0; 
	if (pRole->GetLevel() < pReqRole->GetLevel())
	{
		lvlLow = pRole->GetLevel();
		lvlHig = pReqRole->GetLevel(); 
	}
	else
	{
		lvlLow = pReqRole->GetLevel();
		lvlHig = pRole->GetLevel(); 
	}

	UINT32 lvlGap = 0;
	foreach (i in GetGlobalConfig().GuildInheritRoleLvlGap)
	{
		if (CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType == i->seq[0])
		{
			lvlGap = i->seq[1];
		}
	}
	if (lvlGap == 0 && !GetGlobalConfig().GuildInheritRoleLvlGap.empty())
	{
		lvlGap = GetGlobalConfig().GuildInheritRoleLvlGap.back().seq[1]; 
	}

	if ((lvlHig - lvlLow) < lvlGap) // 等级差检查
	{
		return ERR_GUILD_INHERIT_GAP;
	}

	if (lvlLow < GetGlobalConfig().GuildInheritRoleLvlLow || lvlHig < GetGlobalConfig().GuildInheritRoleLvlHig) // 等级检查
	{
		return ERR_GUILD_INHERIT_LVL;
	}

	Guild* pGuildOne = CGuildMgr::Instance()->GetGuildByRoleId(roleId); 
	Guild* pGuildTwo = CGuildMgr::Instance()->GetGuildByRoleId(reqRoleId); 
	if (pGuildOne == NULL || pGuildTwo == NULL || pGuildOne != pGuildTwo) // 是否在同一个公会
	{
		return ERR_GUILD_MEMBER_NOT_EXIST;
	}

	return ERR_SUCCESS;
}

bool CGuildInherit::IsInherit(UINT64 roleId,  UINT64 reqRoleId)
{
	auto iter = m_inheritDataMap.find(roleId);
	if (iter != m_inheritDataMap.end())
	{
		if (iter->second.find(reqRoleId) != iter->second.end())
		{
			return true;
		}
	}
	return false;
}

void CGuildInherit::DelInherit(UINT64 roleId, UINT64 reqRoleId)
{
	auto iter = m_inheritDataMap.find(roleId);
	if (iter != m_inheritDataMap.end())
	{
		auto it = iter->second.find(reqRoleId); 
		if (it != iter->second.end())
		{
			iter->second.erase(it);
		}
	}
}

void CGuildInherit::GetReqRole(UINT64 roleId, std::map<UINT64, UINT32>& dataMap)
{
	auto iter = m_inheritDataMap.find(roleId);
	if (iter != m_inheritDataMap.end())
	{
		dataMap = iter->second;
	}
}

void CGuildInherit::DelAll(UINT64 roleId)
{
	auto iter = m_inheritDataMap.find(roleId);
	if (iter != m_inheritDataMap.end())
	{
		m_inheritDataMap.erase(iter);
	}
}

bool CGuildInherit::IsMapRight(UINT64 roleOne, UINT64 roleTwo)
{
	CRole* pRoleOne = CRoleManager::Instance()->GetByRoleID(roleOne);
	CRole* pRoleTwo = CRoleManager::Instance()->GetByRoleID(roleTwo);

	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		return false;
	}

	if ((pRoleOne->GetMapID() == HALL_MAP_ID || pRoleOne->GetMapID() == GUILD_MAP_ID)
		&& (pRoleTwo->GetMapID() == HALL_MAP_ID || pRoleTwo->GetMapID() == GUILD_MAP_ID))
	{
		return true;
	}

	return false;
}

void CGuildInherit::NoticeReqNum(UINT64 roleId)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return ;
	}

	PtcM2G_NoticeGuildInheritNumInfo msg;
	msg.m_Data.set_roleid(roleId);
	auto iter = m_inheritDataMap.find(roleId);
	if (iter != m_inheritDataMap.end() && !iter->second.empty())
	{
		msg.m_Data.set_reqnum(iter->second.size());
	}
	else
	{
		msg.m_Data.set_reqnum(0);
	}
	pRole->SendMsgToGS(msg);
}

void CGuildInherit::DoRoleOnline(UINT64 roleId)
{
	NoticeReqNum(roleId);
}

KKSG::ErrorCode CGuildInherit::StartTeacherInherit(UINT64 roleOne, UINT64 roleTwo)
{

	CRole* pRoleOne = CRoleManager::Instance()->GetByRoleID(roleOne);
	CRole* pRoleTwo = CRoleManager::Instance()->GetByRoleID(roleTwo);

	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		return KKSG::ERR_ROLE_NOT_ONLINE;
	}

	if (!IsMapRight(roleOne, roleTwo))
	{
		return KKSG::ERR_GUILD_INHERIT_MAP_WRONG;
	}

	/*PtcM2G_TryTeacherInherit msg;
	msg.m_Data.set_guildidone(roleOne);
	msg.m_Data.set_guildidtwo(roleTwo);
	pRoleOne->SendMsgToGS(msg);*/

	return KKSG::ERR_SUCCESS;
}
