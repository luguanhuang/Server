#include "pch.h"
#include "guild/rpcm2g_getguildladderrewardtime.h"

// generate by ProtoGen at date: 2016/10/13 21:04:11

RPC_CLIENT_IMPLEMETION(RpcM2G_GetGuildLadderRewardTime, GetGuildLadderRewardTimeArg, GetGuildLadderRewardTimeRes)

void RpcM2G_GetGuildLadderRewardTime::OnReply(const GetGuildLadderRewardTimeArg &roArg, const GetGuildLadderRewardTimeRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.time();
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_GetGuildLadderRewardTime::OnTimeout(const GetGuildLadderRewardTimeArg &roArg, const CUserData &roUserData)
{
}
