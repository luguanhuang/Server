#include "pch.h"
#include "chat/rpcm2g_chatcansend.h"

// generate by ProtoGen at date: 2016/7/25 15:35:56

RPC_CLIENT_IMPLEMETION(RpcM2G_ChatCanSend, ChatCanSendReq, ChatCanSendRes)

void RpcM2G_ChatCanSend::OnReply(const ChatCanSendReq &roArg, const ChatCanSendRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_ChatCanSend::OnTimeout(const ChatCanSendReq &roArg, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = ERR_TIMEOUT;
	oUserData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}
