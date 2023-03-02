#include "pch.h"
#include "role/rpcm2d_readroledata.h"
#include "db/mysqlmgr.h"
#include "db/task/rolereadtask.h"

// generate by ProtoGen at date: 2016/7/10 14:06:11

RPC_SERVER_IMPLEMETION(RpcM2D_ReadRoleData, ReadRoleDataArg, ReadRoleDataRes)

void RpcM2D_ReadRoleData::OnCall(const ReadRoleDataArg &roArg, ReadRoleDataRes &roRes)
{
	CRoleReadTask* poTask = new CRoleReadTask();
	poTask->m_qwRoleID = roArg.roleid();
	poTask->m_nSlot = roArg.index();
	poTask->m_dwRpcDelayed = DelayRpc();
	poTask->m_nReadType = roArg.readtype();
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
}

void RpcM2D_ReadRoleData::OnDelayReplyRpc(const ReadRoleDataArg &roArg, ReadRoleDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	KKSG::RoleAllInfo* poRoleInfo = (KKSG::RoleAllInfo*)roUserData.m_pUserPtr;
	roRes.set_result(nResult);
	if(nResult == KKSG::ERR_SUCCESS && poRoleInfo != NULL)
	{
		roRes.mutable_roledata()->CopyFrom(*poRoleInfo);
	}
}
