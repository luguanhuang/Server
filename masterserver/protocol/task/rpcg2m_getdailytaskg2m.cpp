#include "pch.h"
#include "task/rpcg2m_getdailytaskg2m.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "task/roletask.h"
#include "task/roletaskmgr.h"

// generate by ProtoGen at date: 2017/10/12 20:31:37

RPC_SERVER_IMPLEMETION(RpcG2M_GetDailyTaskG2M, GetDailyTaskG2MArg, GetDailyTaskG2MRes)

void RpcG2M_GetDailyTaskG2M::OnCall(const GetDailyTaskG2MArg &roArg, GetDailyTaskG2MRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	CRoleTask* pTask = RoleTaskMgr::Instance()->GetRoleTask(pRole->GetID());
	roRes.set_remain_refresh_count(pTask->GetCanRefreshNum());
	roRes.set_luck(pTask->m_luck);
}

void RpcG2M_GetDailyTaskG2M::OnDelayReplyRpc(const GetDailyTaskG2MArg &roArg, GetDailyTaskG2MRes &roRes, const CUserData &roUserData)
{
}
