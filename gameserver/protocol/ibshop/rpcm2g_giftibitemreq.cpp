#include "pch.h"
#include "ibshop/rpcm2g_giftibitemreq.h"
#include "pay/ptcg2c_allgiftibitemntf.h"
#include "utility/tlogger.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/ibshoprecord.h"
#include "gamelogic/ibshopmgr.h"
// generate by ProtoGen at date: 2017/3/15 20:37:28

RPC_SERVER_IMPLEMETION(RpcM2G_GiftIbItemReq, GiftIbItemArg, GiftIbItemRes)

void RpcM2G_GiftIbItemReq::OnCall(const GiftIbItemArg &roArg, GiftIbItemRes &roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole==NULL)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	PtcG2C_AllGiftIBItemNtf ntf;
	for (int i = 0; i < roArg.allitem().size(); i++)
	{
		const KKSG::PayGiftIbItem* pItem = &(roArg.allitem(i));
		if (!pRecord->HasOrder(pItem->billno()))
		{
			KKSG::GiftIbItem* pClientItem = ntf.m_Data.add_gift();
			pClientItem->set_name(pItem->name());
			pClientItem->set_text(pItem->text());
			pClientItem->set_orderid(pItem->billno());;
			pClientItem->mutable_item()->set_itemid(pItem->itemid());
			pClientItem->mutable_item()->set_itemcount(pItem->count());
		}
	}
	pRole->Send(ntf);
}

void RpcM2G_GiftIbItemReq::OnDelayReplyRpc(const GiftIbItemArg &roArg, GiftIbItemRes &roRes, const CUserData &roUserData)
{

}
 