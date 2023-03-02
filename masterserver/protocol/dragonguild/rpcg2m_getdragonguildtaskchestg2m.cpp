#include "pch.h"
#include "dragonguild/rpcg2m_getdragonguildtaskchestg2m.h"
#include "dragonguild/dragonguildmgr.h"
#include "role/rolemanager.h"
#include "dragonguild/dragonguildtask.h"
// generate by ProtoGen at date: 2017/9/11 14:37:17

RPC_SERVER_IMPLEMETION(RpcG2M_GetDragonGuildTaskChestG2M, GetDragonGuildTaskChestG2MArg, GetDragonGuildTaskChestG2MRes)

void RpcG2M_GetDragonGuildTaskChestG2M::OnCall(const GetDragonGuildTaskChestG2MArg &roArg, GetDragonGuildTaskChestG2MRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;	
	}
	DragonGuild* p = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (p == NULL)
	{
		roRes.set_dragonguildid(0);
		return;
	}

	DragonGuildTask& taskRecord = p->GetDragonGuildTask();
	KKSG::ErrorCode errorCode = taskRecord.GetTaskStatus(roArg.type(), roArg.taskid());
	roRes.set_errorcode(errorCode);
	roRes.set_dragonguildid(p->GetID());
	roRes.set_exp(p->GetExp());
}

void RpcG2M_GetDragonGuildTaskChestG2M::OnDelayReplyRpc(const GetDragonGuildTaskChestG2MArg &roArg, GetDragonGuildTaskChestG2MRes &roRes, const CUserData &roUserData)
{
}
