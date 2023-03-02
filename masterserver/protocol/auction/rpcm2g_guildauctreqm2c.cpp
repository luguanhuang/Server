#include "pch.h"
#include "auction/rpcm2g_guildauctreqm2c.h"

// generate by ProtoGen at date: 2016/11/1 23:24:13

RPC_CLIENT_IMPLEMETION(RpcM2G_GuildAuctReqM2C, GuildAuctReqM2GArg, GuildAuctReqM2GRes)

void RpcM2G_GuildAuctReqM2C::OnReply(const GuildAuctReqM2GArg &roArg, const GuildAuctReqM2GRes &roRes, const CUserData &roUserData)
{
	UINT32 delayid = roArg.delayid();

	GuildAuctReqM2GRes tmpRes;
	tmpRes.CopyFrom(roRes);

	CUserData pUserData;
	pUserData.m_pUserPtr = (void*)(&tmpRes);

	ReplyDelayRpc(delayid, pUserData);
}

void RpcM2G_GuildAuctReqM2C::OnTimeout(const GuildAuctReqM2GArg &roArg, const CUserData &roUserData)
{
}
