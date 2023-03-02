#include "pch.h"
#include "mobabattle/rpcm2d_querymobarolerecord.h"
#include "db/task/mobareadtask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/6/17 19:25:04

RPC_SERVER_IMPLEMETION(RpcM2D_QueryMobaRoleRecord, QueryMobaRoleRecordArg, QueryMobaRoleRecordRes)

void RpcM2D_QueryMobaRoleRecord::OnCall(const QueryMobaRoleRecordArg &roArg, QueryMobaRoleRecordRes &roRes)
{
	MobaReadTask* poTask = new MobaReadTask;
	poTask->m_qwRoleID = roArg.roleid();
	poTask->m_dwRpcDelayed = DelayRpc();
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
}

void RpcM2D_QueryMobaRoleRecord::OnDelayReplyRpc(const QueryMobaRoleRecordArg &roArg, QueryMobaRoleRecordRes &roRes, const CUserData &roUserData)
{
	QueryMobaRoleRecordRes& res = *(QueryMobaRoleRecordRes*)roUserData.m_pUserPtr;
	roRes.CopyFrom(res);
}
