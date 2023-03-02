#include "pch.h"
#include "guild/rpcm2g_tryaddguildinherit.h"

// generate by ProtoGen at date: 2016/11/17 16:07:00

RPC_CLIENT_IMPLEMETION(RpcM2G_tryAddGuildInherit, tryAddGuildInheritArg, tryAddGuildInheritRes)

void RpcM2G_tryAddGuildInherit::OnReply(const tryAddGuildInheritArg &roArg, const tryAddGuildInheritRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.errorcode();
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_tryAddGuildInherit::OnTimeout(const tryAddGuildInheritArg &roArg, const CUserData &roUserData)
{
}
