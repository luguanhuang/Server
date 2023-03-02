#include "pch.h"
#include "marriage/rpcm2g_givemarriagelvrerewardm2g.h"

// generate by ProtoGen at date: 2017/7/28 23:41:27

RPC_CLIENT_IMPLEMETION(RpcM2G_GiveMarriageLvRerewardM2G, GiveMarriageLvRewardM2GArg, GiveMarriageLvRewardRes)

void RpcM2G_GiveMarriageLvRerewardM2G::OnReply(const GiveMarriageLvRewardM2GArg &roArg, const GiveMarriageLvRewardRes &roRes, const CUserData &roUserData)
{
	CUserData oUser(roRes.result(), (void*)&roRes);
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_GiveMarriageLvRerewardM2G::OnTimeout(const GiveMarriageLvRewardM2GArg &roArg, const CUserData &roUserData)
{
	CUserData oUser(KKSG::ERR_TIMEOUT, NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}
