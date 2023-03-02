#include "pch.h"
#include "wedding/rpcm2g_weddingoperm2g.h"

// generate by ProtoGen at date: 2017/7/31 15:27:03

RPC_CLIENT_IMPLEMETION(RpcM2G_WeddingOperM2G, WeddingOperM2GArg, WeddingOperM2GRes)

void RpcM2G_WeddingOperM2G::OnReply(const WeddingOperM2GArg &roArg, const WeddingOperM2GRes &roRes, const CUserData &roUserData)
{
	CUserData userData(roRes.result(), NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcM2G_WeddingOperM2G::OnTimeout(const WeddingOperM2GArg &roArg, const CUserData &roUserData)
{
	CUserData userData(KKSG::ERR_TIMEOUT, NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
