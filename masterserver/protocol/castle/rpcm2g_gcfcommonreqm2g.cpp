#include "pch.h"
#include "castle/rpcm2g_gcfcommonreqm2g.h"

// generate by ProtoGen at date: 2016/12/31 22:32:09

RPC_CLIENT_IMPLEMETION(RpcM2G_GCFCommonReqM2G, GCFCommonArgM2G, GCFCommonResM2G)

void RpcM2G_GCFCommonReqM2G::OnReply(const GCFCommonArgM2G &roArg, const GCFCommonResM2G &roRes, const CUserData &roUserData)
{
	GCFCommonResM2G  tmpRes;
	tmpRes.CopyFrom(roRes);

	CUserData pUserData;
	pUserData.m_pUserPtr = (void*)(&tmpRes);

	ReplyDelayRpc(roArg.delayid(), pUserData);
}

void RpcM2G_GCFCommonReqM2G::OnTimeout(const GCFCommonArgM2G &roArg, const CUserData &roUserData)
{
}
