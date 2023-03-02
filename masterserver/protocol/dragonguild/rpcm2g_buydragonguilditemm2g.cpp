#include "pch.h"
#include "dragonguild/rpcm2g_buydragonguilditemm2g.h"

// generate by ProtoGen at date: 2017/9/12 18:33:58

RPC_CLIENT_IMPLEMETION(RpcM2G_BuyDragonGuildItemM2G, BuyDragonGuildShopItemM2GArg, BuyDragonGuildShopItemM2GRes)

void RpcM2G_BuyDragonGuildItemM2G::OnReply(const BuyDragonGuildShopItemM2GArg &roArg, const BuyDragonGuildShopItemM2GRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = roRes.result();
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcM2G_BuyDragonGuildItemM2G::OnTimeout(const BuyDragonGuildShopItemM2GArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
