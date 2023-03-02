#include "pch.h"
#include "guild/rpcm2g_reqguildinherittimes.h"
#include "guild/guildrecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/levelsealMgr.h"
#include <time.h>

#include "foreach.h"


// generate by ProtoGen at date: 2016/12/5 16:59:12

RPC_SERVER_IMPLEMETION(RpcM2G_ReqGuildInheritTimes, ReqGuildInheritTimesArg, ReqGuildInheritTimesRes)

void RpcM2G_ReqGuildInheritTimes::OnCall(const ReqGuildInheritTimesArg &roArg, ReqGuildInheritTimesRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		return ;
	}

	for (UINT32 cnt = 0; cnt < roArg.members_size(); ++cnt)
	{
		Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(roArg.members(cnt));
		if (pRoleTwo == NULL)
		{
			continue;
		}

		Role* pTea = NULL;
		Role* pStu = NULL;

		if (pRole->GetLevel() > pRoleTwo->GetLevel())
		{
			pTea = pRole;
			pStu = pRoleTwo;
		}
		else
		{
			pTea = pRoleTwo;
			pStu = pRole;
		}
		
		UINT32 nowTime = time(NULL);
		UINT32 cdTime = pTea->Get<CGuildRecord>()->GetGuildInheritCDTime(); 
		if (pStu->Get<CGuildRecord>()->GetInheritStuTime() < GetGlobalConfig().GuildInheritStuLimit
			&& pTea->Get<CGuildRecord>()->GetInheritTeaTime() < GetGlobalConfig().GuildInheritTeaLimit
			&& pStu->GetLevel() >= GetGlobalConfig().GuildInheritRoleLvlLow
			&& pTea->GetLevel() >= GetGlobalConfig().GuildInheritRoleLvlHig
//			&& (pTea->GetLevel() - pStu->GetLevel()) > GetGlobalConfig().GuildInheritRoleLvlGap
			&& (nowTime - cdTime) > (GetGlobalConfig().GuildInheritCDTime * 60))
		{
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

			if ((pTea->GetLevel() - pStu->GetLevel()) >= lvlGap)
			{
				roRes.add_roles(roArg.members(cnt));
			}
		}
	}
}

void RpcM2G_ReqGuildInheritTimes::OnDelayReplyRpc(const ReqGuildInheritTimesArg &roArg, ReqGuildInheritTimesRes &roRes, const CUserData &roUserData)
{
}
