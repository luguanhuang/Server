#include "pch.h"
#include "login/rpcm2d_readaccountdata.h"
#include "db/mysqlmgr.h"
#include "db/task/accountreadtask.h"

// generate by ProtoGen at date: 2016/7/9 14:20:59

RPC_SERVER_IMPLEMETION(RpcM2D_ReadAccountData, ReadAccountDataArg, ReadAccountDataRes)

void RpcM2D_ReadAccountData::OnCall(const ReadAccountDataArg &roArg, ReadAccountDataRes &roRes)
{
	CAccountReadTask* poTask = new CAccountReadTask();
	poTask->m_strAccount = roArg.account();
	poTask->m_dwRpcDelayed = DelayRpc();
	CMysqlMgr::Instance()->PushTask(poTask, GetAccountThreadIndex());
}

void RpcM2D_ReadAccountData::OnDelayReplyRpc(const ReadAccountDataArg &roArg, ReadAccountDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	KKSG::LoadAccountData* poAccount = (KKSG::LoadAccountData*)roUserData.m_pUserPtr;
	roRes.set_result(nResult);
	if(nResult == KKSG::ERR_SUCCESS && poAccount != NULL)
	{
		*roRes.mutable_accountdata() = *poAccount;
	}
}
