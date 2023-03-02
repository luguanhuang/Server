#include "pch.h"
#include "spactivity/rpcg2m_syncactivityend2ms.h"

// generate by ProtoGen at date: 2017/2/23 11:36:44

RPC_CLIENT_IMPLEMETION(RpcG2M_SyncActivityEnd2MS, SyncActivityEnd2MSArg, SyncActivityEnd2MSRes)

void RpcG2M_SyncActivityEnd2MS::OnReply(const SyncActivityEnd2MSArg &roArg, const SyncActivityEnd2MSRes &roRes, const CUserData &roUserData)
{
	if (roArg.actid())
	{
		if (roRes.error() == KKSG::ERR_SUCCESS)
		{
			LogInfo("sync spactivity id:%u success.", roArg.actid());
		}
		else
		{
			LogInfo("sync spactivity id:%u error.", roArg.actid());
		}
	}
}

void RpcG2M_SyncActivityEnd2MS::OnTimeout(const SyncActivityEnd2MSArg &roArg, const CUserData &roUserData)
{
	LogInfo("sync spactivity id:%u timeout.", roArg.actid());
}
