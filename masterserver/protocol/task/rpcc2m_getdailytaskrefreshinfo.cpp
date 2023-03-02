#include "pch.h"
#include "task/rpcc2m_getdailytaskrefreshinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "task/roletaskmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "foreach.h"
#include "util/gametime.h"
#include "friend/friend.h"
#include "friend/friendmgr.h"

// generate by ProtoGen at date: 2017/9/18 1:24:06

RPC_SERVER_IMPLEMETION(RpcC2M_GetDailyTaskRefreshInfo, GetDailyTaskRefreshInfoArg, GetDailyTaskRefreshInfoRes)

static void FillInfo(UINT64 roleId, std::set<UINT64>& roleSet, GetDailyTaskRefreshInfoRes &roRes, bool is_asked)
{
	if (roleSet.find(roleId) != roleSet.end())
	{
		return;
	}
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pSu == NULL)
	{
		return;
	}
	UINT32 now = GameTime::GetTime();
	if (!is_asked)
	{
		if (pSu->GetLastLogout() > SECONDS_ONE_DAY)
		{
			return;
		}
	}
	CRoleTask* pOtherTask = RoleTaskMgr::Instance()->GetRoleTask(roleId);
	if (pOtherTask == NULL)
	{
		return;
	}

	KKSG::DailyTaskRefreshRoleInfo* pInfo = roRes.add_friendinfo();
	pSu->FillDailyTaskRefreshRoleInfo(*pInfo);
	pInfo->set_already_ask(is_asked);
	pInfo->set_luck(pOtherTask->m_luck);
	pInfo->set_refresh_num(pOtherTask->GetRemainHelpNum());
	pInfo->set_score(pOtherTask->m_currScore);

	roleSet.insert(roleId);
}

void RpcC2M_GetDailyTaskRefreshInfo::OnCall(const GetDailyTaskRefreshInfoArg &roArg, GetDailyTaskRefreshInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	CRoleTask* pRTask = RoleTaskMgr::Instance()->GetRoleTask(pRole->GetID());
	roRes.set_luck(pRTask->m_luck);
	roRes.set_refresh_num(pRTask->GetCanRefreshNum());
	roRes.set_extra_refresh_num(pRTask->m_extraDailyRefreshCount);
	roRes.set_today_buy_num(pRTask->m_dailyBuyCount);

	std::set<UINT64> roleSet;
	/*
	for (auto it = pRTask->m_askedRoleId.begin(); it != pRTask->m_askedRoleId.end(); ++it)
	{
		UINT64 roleId = *it;
		FillInfo(roleId, roleSet, roRes, false);
	}
	*/

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild != NULL)
	{
		const std::map<UINT64, GuildMember>& members = pGuild->GetGuildMember();
		for (auto it = members.begin(); it != members.end(); ++it)
		{
			UINT64 roleId = it->second.qwRoleID;
			if (roleId == pRole->GetID()) continue;
			FillInfo(roleId, roleSet, roRes, false);
		}
	}

	/*
	Friend* pFriend = FriendMgr::Instance()->GetFriendInfo(pRole->GetID());
	if (pFriend)
	{
		for (auto it = pFriend->m_friends.begin(); it != pFriend->m_friends.end(); ++it)
		{
			FillInfo(it->second.m_roleid, roleSet, roRes, false);
		}
	}
	*/
}

void RpcC2M_GetDailyTaskRefreshInfo::OnDelayReplyRpc(const GetDailyTaskRefreshInfoArg &roArg, GetDailyTaskRefreshInfoRes &roRes, const CUserData &roUserData)
{
}
