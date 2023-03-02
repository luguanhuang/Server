#include "pch.h"
#include "garden/rpcg2m_getgardeninfo.h"

// generate by ProtoGen at date: 2017/1/5 10:33:06

RPC_CLIENT_IMPLEMETION(RpcG2M_GetGardenInfo, GetGardenInfoArg, GetGardenInfoRes)

void RpcG2M_GetGardenInfo::OnReply(const GetGardenInfoArg &roArg, const GetGardenInfoRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_pUserPtr = (void*)(&roRes);
	CRpc::ReplyDelayRpc(roArg.rpc_id(), data);
}

void RpcG2M_GetGardenInfo::OnTimeout(const GetGardenInfoArg &roArg, const CUserData &roUserData)
{
}
