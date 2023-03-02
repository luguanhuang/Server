#include "pch.h"
#include "task/rpcm2g_dailytaskrefreshm2g.h"

// generate by ProtoGen at date: 2017/9/19 15:53:20

RPC_CLIENT_IMPLEMETION(RpcM2G_DailyTaskRefreshM2G, DailyTaskRefreshM2GArg, DailyTaskRefreshM2GRes)

void RpcM2G_DailyTaskRefreshM2G::OnReply(const DailyTaskRefreshM2GArg &roArg, const DailyTaskRefreshM2GRes &roRes, const CUserData &roUserData)
{
	if (roArg.rpcid())
	{
		CUserData oUserData(roRes.result(), NULL);
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
	}
}

void RpcM2G_DailyTaskRefreshM2G::OnTimeout(const DailyTaskRefreshM2GArg &roArg, const CUserData &roUserData)
{
	if (roArg.rpcid())
	{
		CUserData oUserData(KKSG::ERR_TIMEOUT, NULL);
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
	}
}
