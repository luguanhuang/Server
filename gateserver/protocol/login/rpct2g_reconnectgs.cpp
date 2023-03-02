#include "pch.h"
#include "login/rpct2g_reconnectgs.h"

// generate by ProtoGen at date: 2016/10/15 20:05:37

RPC_CLIENT_IMPLEMETION(RpcT2G_ReconnectGs, ReconnectGsArg, ReconnectGsRes)

void RpcT2G_ReconnectGs::OnReply(const ReconnectGsArg &roArg, const ReconnectGsRes &roRes, const CUserData &roUserData)
{
	if (roArg.delayrpc() == 0)
	{
		return;
	}
	CUserData data;
	data.m_dwUserData = roRes.errorcode();
	CRpc::ReplyDelayRpc(roArg.delayrpc(), data);
}

void RpcT2G_ReconnectGs::OnTimeout(const ReconnectGsArg &roArg, const CUserData &roUserData)
{
}
