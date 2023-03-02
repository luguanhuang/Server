#include "pch.h"
#include "shop/rpcc2g_buyshopitem.h"
#include "unit/rolemanager.h"
#include "gamelogic/shop.h"
#include "gamelogic/shoprecord.h"
#include "gamelogic/bagtransition.h"
#include "utility/uidhelper.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2015/7/13 17:39:40

RPC_SERVER_IMPLEMETION(RpcC2G_BuyShopItem, BuyShopItemArg, BuyShopItemRes)

#define BUYCOUNT_LIMIT 9999

void RpcC2G_BuyShopItem::OnCall(const BuyShopItemArg &roArg, BuyShopItemRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	UINT32 uid = (UINT32)roArg.itemuniqueid();
	int count = roArg.count();
	if (count <= 0 || count > BUYCOUNT_LIMIT)
	{
		roRes.set_errorcode(ERR_SHOP_INVALID);
		return;
	}

	UINT32 type = ShopMgr::Instance()->GetType(uid);
	if (UINT32(-1) == type)
	{
		roRes.set_errorcode(ERR_SHOP_INVALID);
		return;
	}
	if (!ShopMgr::Instance()->IsOpen(role, type))
	{
		roRes.set_errorcode(ERR_SHOP_INVALID);
		return;
	}

	CShopRecord* record = role->Get<CShopRecord>();
	record->Update();

	XItem* item = record->GetItem(type, uid);
	if (NULL == item)
	{
		roRes.set_errorcode(ERR_SHOP_INVALID);
		return;
	}
	const ShopItemBreif* brief = ShopMgr::Instance()->GetBreif(type, uid, role->GetProfession());
	if (NULL == brief)
	{
		roRes.set_errorcode(ERR_SHOP_INVALID);
		return;
	}
	// limit
	ShopRoleParam param;
	param.count = count;
	UINT32 error = ShopMgr::Instance()->IsLimit(role, type, uid, brief, param);
	if (ERR_SUCCESS != error)
	{
		roRes.set_errorcode(ErrorCode(error));
		return;
	}
	// cost 
	const ItemDesc* cost = ShopMgr::Instance()->GetCost(brief);
	UINT32 costcount = ShopMgr::Instance()->GetCostCount(brief, count);
	BagTakeItemTransition take(role);
	take.SetReason(ItemFlow_Shop, ItemFlow_Shop_Buy);
	if (!take.TakeItem(cost->itemID, costcount))
	{
		take.RollBack();
		roRes.set_errorcode(ERR_ITEM_NOT_ENOUGH);
		return;
	}
	take.NotifyClient();
	// give
	XItem tempitem = *item;
	int overlap = (int)ItemConfig::Instance()->GetOverlap(tempitem.itemID);
	BagGiveItemTransition give(role);
	give.SetReason(ItemFlow_Shop, ItemFlow_Shop_Buy);
	if (0 == overlap)
	{
		tempitem.uid = UIDHelper::CreateUID(UID_Item);
		tempitem.itemCount = count;
		give.GiveItem(&tempitem);
	}
	else
	{
		while (count > 0)
		{
			tempitem.uid = UIDHelper::CreateUID(UID_Item);
			if (count >= overlap)
			{
				tempitem.itemCount = overlap;
				count -= overlap;
			}
			else
			{
				tempitem.itemCount = count;
				count = 0;
			}
			give.GiveItem(&tempitem);
		}
	}
	give.NotifyClient();

	record->AddBuyCount(type, uid, param.count);
	record->AddDailyBuyCount(type, uid, param.count);
	record->AddWeekBuyCount(type, uid, param.count);

	role->OnShopBuy();
	roRes.set_errorcode(ERR_SUCCESS);

	//tlog日志记录;
	TShopFlow oLog(role);
	oLog.m_iShopType = type;
	oLog.m_iMoneyType = cost->itemID;

	oLog.m_iMoneyValue = costcount;
	oLog.m_iItemID = tempitem.itemID;
	oLog.m_iItemCount = tempitem.itemCount;
	oLog.Do();
	return;
}

void RpcC2G_BuyShopItem::OnDelayReplyRpc(const BuyShopItemArg &roArg, BuyShopItemRes &roRes, const CUserData &roUserData)
{
}
