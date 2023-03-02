#include "pch.h"
#include "common/rpcc2g_synctime.h"
#include "timeutil.h"

// generate by ProtoGen at date: 2014/11/7 14:07:57

RPC_SERVER_IMPLEMETION(RpcC2G_SyncTime, SyncTimeArg, SyncTimeRes)

void RpcC2G_SyncTime::OnCall(const SyncTimeArg &roArg, SyncTimeRes &roRes)
{
	roRes.set_servertime((UINT64)TimeUtil::GetMilliSecond());
	//LogDebug("sync time");
}

void RpcC2G_SyncTime::OnDelayReplyRpc(const SyncTimeArg &roArg, SyncTimeRes &roRes, const CUserData &roUserData)
{
}
