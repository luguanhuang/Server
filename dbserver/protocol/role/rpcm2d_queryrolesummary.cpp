#include "pch.h"
#include "db/mysqlmgr.h"
#include "db/task/rolesummaryquerytask.h"
#include "role/rpcm2d_queryrolesummary.h"

// generate by ProtoGen at date: 2017/4/26 19:27:56

RPC_SERVER_IMPLEMETION(RpcM2D_QueryRolesummary, QueryRolesummaryArg, QueryRolesummaryRes)

void RpcM2D_QueryRolesummary::OnCall(const QueryRolesummaryArg &roArg, QueryRolesummaryRes &roRes)
{
	CRolesummaryQueryTask* poTask = new CRolesummaryQueryTask();
	poTask->m_qwRoleID = roArg.roleid();
	poTask->m_dwRpcDelayed = DelayRpc();
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
	SSInfo<<"query rolesummary, roleid:"<<roArg.roleid()<<END;
}


void RpcM2D_QueryRolesummary::OnDelayReplyRpc(const QueryRolesummaryArg &roArg, QueryRolesummaryRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	KKSG::RoleSummaryStored* data = (KKSG::RoleSummaryStored*)roUserData.m_pUserPtr;
	roRes.set_errorcode(nResult);
	if(nResult == KKSG::ERR_SUCCESS && data != NULL)
	{
		roRes.mutable_data()->CopyFrom(*data);
	}
}
