#include "pch.h"
#include "match/rpcm2g_kmatchcommonreqm2c.h"
#include "match/matchrequest.h"

// generate by ProtoGen at date: 2017/5/14 19:48:10

RPC_CLIENT_IMPLEMETION(RpcM2G_KMatchCommonReqM2C, KMatchCommonArgM2G, KMatchCommonResM2G)

void RpcM2G_KMatchCommonReqM2C::OnReply(const KMatchCommonArgM2G &roArg, const KMatchCommonResM2G &roRes, const CUserData &roUserData)
{
	MatchRequest::HandleM2GRes(roArg, roRes);
}

void RpcM2G_KMatchCommonReqM2C::OnTimeout(const KMatchCommonArgM2G &roArg, const CUserData &roUserData)
{
}
