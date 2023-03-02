#include "pch.h"
#include "partner/rpcm2g_buypartneritemm2g.h"

// generate by ProtoGen at date: 2016/12/15 16:11:52

RPC_CLIENT_IMPLEMETION(RpcM2G_BuyPartnerItemM2G, BuyPartnerShopItemM2GArg, BuyPartnerShopItemM2GRes)

void RpcM2G_BuyPartnerItemM2G::OnReply(const BuyPartnerShopItemM2GArg &roArg, const BuyPartnerShopItemM2GRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = roRes.result();
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcM2G_BuyPartnerItemM2G::OnTimeout(const BuyPartnerShopItemM2GArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
