#include "pch.h"
#include "partner/rpcm2g_buypartneritemm2g.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/rolepartner.h"
#include "tablemgr/partnertablemgr.h"

// generate by ProtoGen at date: 2016/12/15 16:11:52

RPC_SERVER_IMPLEMETION(RpcM2G_BuyPartnerItemM2G, BuyPartnerShopItemM2GArg, BuyPartnerShopItemM2GRes)

void RpcM2G_BuyPartnerItemM2G::OnCall(const BuyPartnerShopItemM2GArg &roArg, BuyPartnerShopItemM2GRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	ShopTable::RowData* itemTable = PartnerTableMgr::Instance()->GetPartnerShopItem(roArg.id());
	if (itemTable == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_ITEM_NOT_FOUND);
		return;
	}
	if (pRole->GetLevel() < itemTable->LevelCondition)
	{
		roRes.set_result(KKSG::ERR_SHOP_LEVELLIMIT);
		return;
	}

	BagTakeItemTransition trans(pRole);
	trans.SetReason(ItemFlow_Partner, ItemFlow_PartnerShop);
	UINT32 costNum = PartnerTableMgr::Instance()->GetCostNum(itemTable) * roArg.count();
	if (!trans.TakeItem(itemTable->ConsumeItem[0], costNum))
	{
		trans.RollBack();
		roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
		return;
	}
	trans.NotifyClient();

	BagGiveItemTransition giveTrans(pRole);
	giveTrans.SetReason(ItemFlow_Partner, ItemFlow_PartnerShop);
	giveTrans.GiveItem(itemTable->ItemId, roArg.count(), itemTable->IsNotBind != 0 ? false : true);
	giveTrans.NotifyClient();

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcM2G_BuyPartnerItemM2G::OnDelayReplyRpc(const BuyPartnerShopItemM2GArg &roArg, BuyPartnerShopItemM2GRes &roRes, const CUserData &roUserData)
{
}
