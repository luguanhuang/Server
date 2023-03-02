#include "pch.h"
#include "shop/rpcc2g_queryshopitem.h"
#include "gamelogic/shop.h"
#include "unit/rolemanager.h"
#include "gamelogic/shoprecord.h"
#include "gamelogic/bagtransition.h"
#include "garden/rpcg2m_getgardeninfo.h"
#include "network/mslink.h"
// generate by ProtoGen at date: 2015/7/13 17:19:11

RPC_SERVER_IMPLEMETION(RpcC2G_QueryShopItem, QueryShopItemArg, QueryShopItemRes)

void RpcC2G_QueryShopItem::OnCall(const QueryShopItemArg &roArg, QueryShopItemRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT32 type = roArg.type();
	if (!ShopMgr::Instance()->IsOpen(role, type))
	{
		roRes.set_errorcode(ERR_SYS_NOTOPEN);
		return;
	}

	CShopRecord* record = role->Get<CShopRecord>();
	record->Update();

	if (roArg.isrefresh())
	{
		UINT32 count = record->GetRefreshCount(type);
		ItemDesc cost = ShopMgr::Instance()->GetRefreshCost(type, count);
		BagTakeItemTransition take(role);
		take.SetReason(ItemFlow_Shop, ItemFlow_Shop_Refresh);
		if (!take.TakeItem(cost.itemID,cost.itemCount))
		{
			take.RollBack();
			roRes.set_errorcode(ERR_ITEM_NOT_ENOUGH);
			return;
		}
		take.NotifyClient();
		record->Refresh(type, role->GetLevel(), role->GetProfession(), true, false, true);	
	}
	else
	{
		record->RefreshOne(type);
	}
	CShopRecordOne* shop = record->GetShop(type);
	if (shop->ishint)
	{
		shop->ishint = false;
		record->SetModify();
		ShopMgr::Instance()->Hint(role, true, type);
	}
	std::map<UINT32, XItem>* items = shop->GetItems();
	for (auto i = items->begin(); i != items->end(); ++i)
	{
		KKSG::ShopItem* shopitem = roRes.add_shopitem();
		shopitem->set_slot(i->first);
		KKSG::Item* item = shopitem->mutable_item();
		XItem::Item2KKSGItem(&i->second, item);
		shopitem->set_buycount(record->GetBuyCount(type, (UINT32)item->uid()));
		shopitem->set_dailybuycount(record->GetDailyBuyCount(type, (UINT32)item->uid()));
		shopitem->set_weekbuycount(record->GetWeekBuyCount(type, (UINT32)item->uid()));
	}
	roRes.set_refreshcount(record->GetRefreshCount(type));
	roRes.set_errorcode(ERR_SUCCESS);

	if (Shop_Garden == type)
	{
		RpcG2M_GetGardenInfo * msg = RpcG2M_GetGardenInfo::CreateRpc();
		msg->m_oArg.set_rpc_id(DelayRpc());
		msg->m_oArg.set_role_id(role->GetID());
		role->SendToMS(*msg);
	}

	return;
}

void RpcC2G_QueryShopItem::OnDelayReplyRpc(const QueryShopItemArg &roArg, QueryShopItemRes &roRes, const CUserData &roUserData)
{
	GetGardenInfoRes* res = (GetGardenInfoRes*)roUserData.m_pUserPtr;
	if (NULL == res)
	{
		return;
	}
	roRes.set_cooklevel(res->cook_level());
}
