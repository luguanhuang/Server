#include "pch.h"
#include "pay/rpcc2m_giftibreqgoods.h"
#include "pay/rpcm2g_giftibreqgsgoods.h"
#include "pay/ibgiftmgr.h"
#include "role/rolemanager.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2017/5/5 13:34:18

RPC_SERVER_IMPLEMETION(RpcC2M_GiftIbReqGoods, GiftIbReqGoodsArg, GiftIbReqGoodsRes)

void RpcC2M_GiftIbReqGoods::OnCall(const GiftIbReqGoodsArg &roArg, GiftIbReqGoodsRes &roRes)
{
	//1查询订单是否完
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!pRole) 
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	PayGiftIbItem *pIbItem =  IBGiftMgr::Instance()->GetGoods(pRole);
	if (pIbItem == NULL)
	{
		roRes.set_error(KKSG::ERR_GIFTIB_NOTGOODS);
		return;
	}
	RpcM2G_GiftIbReqGSGoods *pRpc = RpcM2G_GiftIbReqGSGoods::CreateRpc();
	pRpc->m_oArg.set_rpcid(DelayRpc());
	pRpc->m_oArg.mutable_item()->CopyFrom(*pIbItem);
	GSLink::Instance()->SendToLine(pRole->GetGsLine(), *pRpc);

}

void RpcC2M_GiftIbReqGoods::OnDelayReplyRpc(const GiftIbReqGoodsArg &roArg, GiftIbReqGoodsRes &roRes, const CUserData &roUserData)
{

}
