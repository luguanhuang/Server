#include "pch.h"
#include "task/rpcg2m_getdailytaskg2m.h"

// generate by ProtoGen at date: 2017/10/12 20:31:37

RPC_CLIENT_IMPLEMETION(RpcG2M_GetDailyTaskG2M, GetDailyTaskG2MArg, GetDailyTaskG2MRes)

void RpcG2M_GetDailyTaskG2M::OnReply(const GetDailyTaskG2MArg &roArg, const GetDailyTaskG2MRes &roRes, const CUserData &roUserData)
{
	CUserData userData(KKSG::ERR_SUCCESS, (void*)&roRes);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcG2M_GetDailyTaskG2M::OnTimeout(const GetDailyTaskG2MArg &roArg, const CUserData &roUserData)
{
	CUserData userData(KKSG::ERR_TIMEOUT, NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
