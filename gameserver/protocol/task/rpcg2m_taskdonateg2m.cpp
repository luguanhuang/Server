#include "pch.h"
#include "task/rpcg2m_taskdonateg2m.h"

// generate by ProtoGen at date: 2017/9/8 16:05:38

RPC_CLIENT_IMPLEMETION(RpcG2M_TaskDonateG2M, TaskDonateG2MArg, TaskDonateG2MRes)

void RpcG2M_TaskDonateG2M::OnReply(const TaskDonateG2MArg &roArg, const TaskDonateG2MRes &roRes, const CUserData &roUserData)
{
	if (roArg.rpcid())
	{
		CUserData userData;
		userData.m_dwUserData = roRes.result();
		userData.m_pUserPtr = (void*)&roRes;
		CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
	}
}

void RpcG2M_TaskDonateG2M::OnTimeout(const TaskDonateG2MArg &roArg, const CUserData &roUserData)
{
	if (roArg.rpcid())
	{
		CUserData userData;
		userData.m_dwUserData = KKSG::ERR_TIMEOUT;
		userData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
	}
}
