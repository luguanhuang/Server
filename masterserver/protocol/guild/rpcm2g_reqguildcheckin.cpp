#include "pch.h"
#include "guild/rpcm2g_reqguildcheckin.h"

// generate by ProtoGen at date: 2016/9/18 10:29:18

RPC_CLIENT_IMPLEMETION(RpcM2G_ReqGuildCheckin, ReqGuildCheckinArg, ReqGuildCheckinRes)

void RpcM2G_ReqGuildCheckin::OnReply(const ReqGuildCheckinArg &roArg, const ReqGuildCheckinRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = roRes.checkin();
	UINT32 boxmask = roRes.boxmask(); 
	oUser.m_pUserPtr = (void*)&boxmask;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_ReqGuildCheckin::OnTimeout(const ReqGuildCheckinArg &roArg, const CUserData &roUserData)
{
}
