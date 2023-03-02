#include "pch.h"
#include "common/rpcm2k_queryrouter.h"
#include "network/rslink.h"

// generate by ProtoGen at date: 2017/3/3 16:53:43

RPC_CLIENT_IMPLEMETION(RpcM2K_QueryRouter, QueryRouterArg, QueryRouterRes)

void RpcM2K_QueryRouter::OnReply(const QueryRouterArg &roArg, const QueryRouterRes &roRes, const CUserData &roUserData)
{
	if (roRes.has_info())
	{
		RSLink::Instance()->OnQueryRouterReply(roRes.info());
		LogInfo("query router success, line %u", roRes.line());
	}
	else
	{
		LogWarn("query router failed");
	}
}

void RpcM2K_QueryRouter::OnTimeout(const QueryRouterArg &roArg, const CUserData &roUserData)
{
}
