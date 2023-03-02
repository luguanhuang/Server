#include "pch.h"
#include "misc/rpcm2g_checknotice.h"

// generate by ProtoGen at date: 2017/2/3 20:12:41

RPC_CLIENT_IMPLEMETION(RpcM2G_CheckNotice, CheckNoticeArg, CheckNoticeRes)

void RpcM2G_CheckNotice::OnReply(const CheckNoticeArg &roArg, const CheckNoticeRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_CheckNotice::OnTimeout(const CheckNoticeArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)(KKSG::ERR_TIMEOUT);
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}
