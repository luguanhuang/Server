#include "pch.h"
#include "role/rpcn2d_nsreadroledata.h"
#include "db/mysqlmgr.h"
#include "db/task/rolereadtask.h"

// generate by ProtoGen at date: 2016/11/3 1:34:54

RPC_SERVER_IMPLEMETION(RpcN2D_NsReadRoleData, ReadRoleDataArg, ReadRoleDataRes)

void RpcN2D_NsReadRoleData::OnCall(const ReadRoleDataArg &roArg, ReadRoleDataRes &roRes)
{
	CRoleReadTask* poTask = new CRoleReadTask();
	poTask->m_qwRoleID = roArg.roleid();
	poTask->m_nSlot = roArg.index();
	poTask->m_dwRpcDelayed = DelayRpc();
	poTask->m_nReadType = roArg.readtype();
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
}

void RpcN2D_NsReadRoleData::OnDelayReplyRpc(const ReadRoleDataArg &roArg, ReadRoleDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	KKSG::RoleAllInfo* poRoleInfo = (KKSG::RoleAllInfo*)roUserData.m_pUserPtr;
	roRes.set_result(nResult);
	if(nResult == KKSG::ERR_SUCCESS && poRoleInfo != NULL)
	{
		roRes.mutable_roledata()->CopyFrom(*poRoleInfo);
	}
}
