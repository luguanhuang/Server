#include "pch.h"
#include "dragonguild/rpcm2g_buydragonguilditemm2g.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "tablemgr/dragonguildtablemgr.h"

// generate by ProtoGen at date: 2017/9/12 18:33:58

RPC_SERVER_IMPLEMETION(RpcM2G_BuyDragonGuildItemM2G, BuyDragonGuildShopItemM2GArg, BuyDragonGuildShopItemM2GRes)

void RpcM2G_BuyDragonGuildItemM2G::OnCall(const BuyDragonGuildShopItemM2GArg &roArg, BuyDragonGuildShopItemM2GRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	ShopTable::RowData* itemTable = DragonGuildTableMgr::Instance()->GetDragonGuildShopItem(roArg.id());
	if (itemTable == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_ITEM_NOT_FOUND);
		return ;
	}

	if (pRole->GetLevel() < itemTable->LevelCondition)
	{
		roRes.set_result(KKSG::ERR_SHOP_LEVELLIMIT);
		return;
	}

	BagTakeItemTransition trans(pRole);
	trans.SetReason(ItemFlow_DragonGuild, ItemFlow_DragonGuildShop);
	UINT32 costNum = DragonGuildTableMgr::Instance()->GetCostNum(itemTable) * roArg.count();
	if (!trans.TakeItem(itemTable->ConsumeItem[0], costNum))
	{
		trans.RollBack();
		roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
		return;
	}
	trans.NotifyClient();

	BagGiveItemTransition giveTrans(pRole);
	giveTrans.SetReason(ItemFlow_DragonGuild, ItemFlow_DragonGuildShop);
	giveTrans.GiveItem(itemTable->ItemId, roArg.count(), itemTable->IsNotBind != 0 ? false : true);
	giveTrans.NotifyClient();

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcM2G_BuyDragonGuildItemM2G::OnDelayReplyRpc(const BuyDragonGuildShopItemM2GArg &roArg, BuyDragonGuildShopItemM2GRes &roRes, const CUserData &roUserData)
{
}
