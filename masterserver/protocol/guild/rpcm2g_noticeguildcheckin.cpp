#include "pch.h"
#include "guild/rpcm2g_noticeguildcheckin.h"

// generate by ProtoGen at date: 2016/9/18 11:17:49

RPC_CLIENT_IMPLEMETION(RpcM2G_NoticeGuildCheckin, NoticeGuildCheckinArg, NoticeGuildCheckinRes)

void RpcM2G_NoticeGuildCheckin::OnReply(const NoticeGuildCheckinArg &roArg, const NoticeGuildCheckinRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.errorcode();
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}


void RpcM2G_NoticeGuildCheckin::OnTimeout(const NoticeGuildCheckinArg &roArg, const CUserData &roUserData)
{
}
