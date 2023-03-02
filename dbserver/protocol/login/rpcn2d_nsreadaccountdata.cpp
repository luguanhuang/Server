#include "pch.h"
#include "login/rpcn2d_nsreadaccountdata.h"
#include "db/mysqlmgr.h"
#include "db/task/accountreadtask.h"

// generate by ProtoGen at date: 2016/11/3 1:33:34

RPC_SERVER_IMPLEMETION(RpcN2D_NsReadAccountData, ReadAccountDataArg, ReadAccountDataRes)

void RpcN2D_NsReadAccountData::OnCall(const ReadAccountDataArg &roArg, ReadAccountDataRes &roRes)
{
	CAccountReadTask* poTask = new CAccountReadTask();
	poTask->m_strAccount = roArg.account();
	poTask->m_serverId = roArg.serverid();
	poTask->m_dwRpcDelayed = DelayRpc();
	CMysqlMgr::Instance()->PushTask(poTask, GetAccountThreadIndex());
}

void RpcN2D_NsReadAccountData::OnDelayReplyRpc(const ReadAccountDataArg &roArg, ReadAccountDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	KKSG::LoadAccountData* poAccount = (KKSG::LoadAccountData*)roUserData.m_pUserPtr;
	roRes.set_result(nResult);
	if(nResult == KKSG::ERR_SUCCESS && poAccount != NULL)
	{
		*roRes.mutable_accountdata() = *poAccount;
	}
}
