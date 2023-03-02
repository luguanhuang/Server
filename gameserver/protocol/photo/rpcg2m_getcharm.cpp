#include "pch.h"
#include "photo/rpcg2m_getcharm.h"

// generate by ProtoGen at date: 2016/12/15 9:00:10

RPC_CLIENT_IMPLEMETION(RpcG2M_GetCharm, GetCharmArg, GetCharmRes)

void RpcG2M_GetCharm::OnReply(const GetCharmArg &roArg, const GetCharmRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_pUserPtr = (void*)(&roRes);
	CRpc::ReplyDelayRpc(roArg.rpc_id(), data);
}

void RpcG2M_GetCharm::OnTimeout(const GetCharmArg &roArg, const CUserData &roUserData)
{
}
