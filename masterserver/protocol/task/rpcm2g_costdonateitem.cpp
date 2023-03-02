#include "pch.h"
#include "task/rpcm2g_costdonateitem.h"

// generate by ProtoGen at date: 2016/11/16 15:21:25

RPC_CLIENT_IMPLEMETION(RpcM2G_CostDonateItem, CostDonateItemArg, CostDonateItemRes)

void RpcM2G_CostDonateItem::OnReply(const CostDonateItemArg &roArg, const CostDonateItemRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = roRes.result();
	userData.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcM2G_CostDonateItem::OnTimeout(const CostDonateItemArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
