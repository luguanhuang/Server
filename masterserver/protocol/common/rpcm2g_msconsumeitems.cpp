#include "pch.h"
#include "common/rpcm2g_msconsumeitems.h"

// generate by ProtoGen at date: 2016/10/4 16:03:56

RPC_CLIENT_IMPLEMETION(RpcM2G_MSConsumeItems, ConsumeItemsArg, ConsumeItemsRes)

void RpcM2G_MSConsumeItems::OnReply(const ConsumeItemsArg &roArg, const ConsumeItemsRes &roRes, const CUserData &roUserData)
{
	if (roArg.rpc_id())
	{
		CUserData oUser;
		oUser.m_dwUserData = (UINT32)roRes.result();	
		CRpc::ReplyDelayRpc(roArg.rpc_id(), oUser);
	}
}

void RpcM2G_MSConsumeItems::OnTimeout(const ConsumeItemsArg &roArg, const CUserData &roUserData)
{
}
