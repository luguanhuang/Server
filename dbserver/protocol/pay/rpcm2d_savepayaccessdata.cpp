#include "pch.h"
#include "pay/rpcm2d_savepayaccessdata.h"
#include "db/task/payaccessinfotask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2016/12/21 17:24:44

RPC_SERVER_IMPLEMETION(RpcM2D_SavePayAccessData, SavePayAccessArg, SavePayAccessDataRes)

void RpcM2D_SavePayAccessData::OnCall(const SavePayAccessArg &roArg, SavePayAccessDataRes &roRes)
{
	CPayAccessInfoTask* poTask = new CPayAccessInfoTask();
	poTask->m_Data.CopyFrom(roArg);
	if (roArg.op() == KKSG::PayAccess_Consume)
	{
		poTask->m_dwRpcDelayed = DelayRpc();
	}
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
}

void RpcM2D_SavePayAccessData::OnDelayReplyRpc(const SavePayAccessArg &roArg, SavePayAccessDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
