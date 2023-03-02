#include "pch.h"
#include "wedding/rpcm2g_holdweddingcheckm2g.h"

// generate by ProtoGen at date: 2017/7/29 16:57:07

RPC_CLIENT_IMPLEMETION(RpcM2G_Holdweddingcheckm2g, Holdweddingcheckm2gArg, Holdweddingcheckm2gRes)

void RpcM2G_Holdweddingcheckm2g::OnReply(const Holdweddingcheckm2gArg &roArg, const Holdweddingcheckm2gRes &roRes, const CUserData &roUserData)
{
	CUserData data(roRes.result(), (void*)&roArg);
	CRpc::ReplyDelayRpc(roArg.delayid(), data);
}

void RpcM2G_Holdweddingcheckm2g::OnTimeout(const Holdweddingcheckm2gArg &roArg, const CUserData &roUserData)
{
	CUserData data(KKSG::ERR_TIMEOUT, NULL);
	CRpc::ReplyDelayRpc(roArg.delayid(), data);
}
