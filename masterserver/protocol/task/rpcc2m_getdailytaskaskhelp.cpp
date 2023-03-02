#include "pch.h"
#include "task/rpcc2m_getdailytaskaskhelp.h"
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
#include "define/taskdef.h"

// generate by ProtoGen at date: 2017/9/18 1:36:22

RPC_SERVER_IMPLEMETION(RpcC2M_GetDailyTaskAskHelp, GetDailyTaskAskHelpArg, GetDailyTaskAskHelpRes)

void RpcC2M_GetDailyTaskAskHelp::OnCall(const GetDailyTaskAskHelpArg &roArg, GetDailyTaskAskHelpRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	CRoleTask* pRTask = RoleTaskMgr::Instance()->GetRoleTask(pRole->GetID());
	if (pRTask == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	roRes.set_help_num(pRTask->GetRemainHelpNum());
	roRes.set_luck(pRTask->m_luck);
	if (pRTask->GetRemainHelpNum() == 0)
	{
		pRTask->m_beAskHelpInfo.clear();
		pRTask->SetDirty();
	}
	else
	{
		for (auto it = pRTask->m_beAskHelpInfo.begin(); it != pRTask->m_beAskHelpInfo.end(); )
		{
			CRoleTask* pOTask = RoleTaskMgr::Instance()->GetRoleTask(it->m_roleId);
			if (pOTask)
			{
				if (!pOTask->CanShowInOtherHelp())
				{
					it = pRTask->m_beAskHelpInfo.erase(it);
					pRTask->SetDirty();
					continue;
				}
			}
			it->FillInfo(*roRes.add_askinfos());
			it++;
		}
	}
}

void RpcC2M_GetDailyTaskAskHelp::OnDelayReplyRpc(const GetDailyTaskAskHelpArg &roArg, GetDailyTaskAskHelpRes &roRes, const CUserData &roUserData)
{
}
