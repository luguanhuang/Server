#include "pch.h"
#include "db/task/readhiddenrolestask.h"
#include "db/mysqlmgr.h"
#include "role/rpcg2d_readhiddenroles.h"

// generate by ProtoGen at date: 2017/1/12 17:26:14

RPC_SERVER_IMPLEMETION(RpcG2D_ReadHiddenRoles, ReadHiddenRolesArg, ReadHiddenRolesRes)

void RpcG2D_ReadHiddenRoles::OnCall(const ReadHiddenRolesArg &roArg, ReadHiddenRolesRes &roRes)
{
	ReadHiddenRolesTask* poTask = new ReadHiddenRolesTask();
	poTask->m_slot = roArg.slot();
	poTask->m_strAccount = roArg.account();
	poTask->m_serverid = roArg.serverid();
	poTask->m_dwRpcDelayed = DelayRpc();
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
}

void RpcG2D_ReadHiddenRoles::OnDelayReplyRpc(const ReadHiddenRolesArg &roArg, ReadHiddenRolesRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
	if(nResult == KKSG::ERR_SUCCESS)
	{
		roRes = *(KKSG::ReadHiddenRolesRes*)roUserData.m_pUserPtr;

		roRes.set_slot(roArg.slot());
		roRes.set_roleid(roArg.roleid());
	}
}
