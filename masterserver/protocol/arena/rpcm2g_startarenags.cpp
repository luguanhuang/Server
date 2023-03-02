#include "pch.h"
#include "arena/rpcm2g_startarenags.h"

// generate by ProtoGen at date: 2016/9/11 19:07:13

RPC_CLIENT_IMPLEMETION(RpcM2G_StartArenaGs, StartArenaGsArg, StartArenaGsRes)

void RpcM2G_StartArenaGs::OnReply(const StartArenaGsArg &roArg, const StartArenaGsRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	StartArenaGsRes res = m_oRes;
	data.m_pUserPtr = (void*)&res;
	ReplyDelayRpc(roArg.delayrpcid(), data);
}

void RpcM2G_StartArenaGs::OnTimeout(const StartArenaGsArg &roArg, const CUserData &roUserData)
{
}
