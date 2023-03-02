#include "pch.h"
#include "guild/rpcg2m_guildcardsyncms.h"

// generate by ProtoGen at date: 2016/9/17 19:33:47

RPC_CLIENT_IMPLEMETION(RpcG2M_GuildCardSyncMs, GuildCardSyncMsArg, GuildCardSyncMsRes)

void RpcG2M_GuildCardSyncMs::OnReply(const GuildCardSyncMsArg &roArg, const GuildCardSyncMsRes &roRes, const CUserData &roUserData)
{
	if (roArg.has_delayrpc())
	{
		CUserData data;
		data.m_pUserPtr = (void*)&roRes;
		CRpc::ReplyDelayRpc(roArg.delayrpc(), data);		
	}
}

void RpcG2M_GuildCardSyncMs::OnTimeout(const GuildCardSyncMsArg &roArg, const CUserData &roUserData)
{
}
