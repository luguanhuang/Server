#include "pch.h"
#include "guild/rpcm2g_reqrecvfatigue.h"

// generate by ProtoGen at date: 2016/9/19 10:23:40

RPC_CLIENT_IMPLEMETION(RpcM2G_ReqRecvFatigue, ReqRecvFatigueArg, ReqRecvFatigueRes)

void RpcM2G_ReqRecvFatigue::OnReply(const ReqRecvFatigueArg &roArg, const ReqRecvFatigueRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.recvnum();
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_ReqRecvFatigue::OnTimeout(const ReqRecvFatigueArg &roArg, const CUserData &roUserData)
{
}
