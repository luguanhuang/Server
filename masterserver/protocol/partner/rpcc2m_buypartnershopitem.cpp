#include "pch.h"
#include "partner/rpcc2m_buypartnershopitem.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"
#include "tablemgr/partnertablemgr.h"
#include "partner/rpcm2g_buypartneritemm2g.h"

// generate by ProtoGen at date: 2016/12/15 11:38:29

RPC_SERVER_IMPLEMETION(RpcC2M_BuyPartnerShopItem, BuyPartnerShopItemArg, BuyPartnerShopItemRes)

void RpcC2M_BuyPartnerShopItem::OnCall(const BuyPartnerShopItemArg &roArg, BuyPartnerShopItemRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (partner == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	PartnerShop& shop = partner->GetPartnerShop();
	PartnerShopItem* item = shop.GetItem(roArg.id());
	if (item == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_ITEM_NOT_FOUND);
		return;
	}
	ShopTable::RowData* itemTable = PartnerTableMgr::Instance()->GetPartnerShopItem(roArg.id());
	if (itemTable == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_ITEM_NOT_FOUND);
		return;
	}

	if (item->IsBuying())
	{
		roRes.set_result(KKSG::ERR_PARTNER_OTHER_BUYING);
		return;
	}
	if (itemTable->DailyCountCondition && roArg.count() + item->buyCount > itemTable->DailyCountCondition)
	{
		roRes.set_result(KKSG::ERR_PARTNER_SHOP_NO_COUNT);
		return;
	}
	PartnerMember* member = partner->GetMember(pRole->GetID());
	if (member == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	if (member->refreshShopLevel < itemTable->LevelShow[0] || member->refreshShopLevel > itemTable->LevelShow[1])
	{
		roRes.set_result(KKSG::ERR_PARTNER_ITEM_NOT_FOUND);
		return;
	}

	item->SetBuying();

	RpcM2G_BuyPartnerItemM2G* rpc = RpcM2G_BuyPartnerItemM2G::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_id(roArg.id());
	rpc->m_oArg.set_count(roArg.count());
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_BuyPartnerShopItem::OnDelayReplyRpc(const BuyPartnerShopItemArg &roArg, BuyPartnerShopItemRes &roRes, const CUserData &roUserData)
{
	roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}
	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (partner == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	PartnerShop& shop = partner->GetPartnerShop();
	PartnerShopItem* item = shop.GetItem(roArg.id());
	if (item == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_ITEM_NOT_FOUND);
		return;
	}
	// 设置成非购买状态后才检查是否成功，然后return
	item->buying = false;
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		return;
	}

	ShopTable::RowData* itemTable = PartnerTableMgr::Instance()->GetPartnerShopItem(roArg.id());
	if (itemTable == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_ITEM_NOT_FOUND);
		return;
	}

	item->buyCount += roArg.count();
	shop.AddBuyRecord(pRole->GetID(), itemTable->ItemId, roArg.count());
}
