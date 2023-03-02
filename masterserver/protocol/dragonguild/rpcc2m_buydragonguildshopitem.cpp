#include "pch.h"
#include "dragonguild/rpcc2m_buydragonguildshopitem.h"
#include "role/rolemanager.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/rpcm2g_buydragonguilditemm2g.h"

// generate by ProtoGen at date: 2017/9/12 18:30:17

RPC_SERVER_IMPLEMETION(RpcC2M_BuyDragonGuildShopItem, BuyDragonGuildShopItemArg, BuyDragonGuildShopItemRes)

void RpcC2M_BuyDragonGuildShopItem::OnCall(const BuyDragonGuildShopItemArg &roArg, BuyDragonGuildShopItemRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (pDGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		return ;
	}

	DragonGuildShop& shop = pDGuild->GetDragonGuildShop();
	DragonGuildShopItem* item = shop.GetItem(roArg.id());
	if (item == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_ITEM_NOT_FOUND);
		return ;
	}
	if (item->IsBuying())
	{
		roRes.set_result(KKSG::ERR_DG_OTHER_BUYING);
		return ;
	}

	ShopTable::RowData* itemTable = DragonGuildTableMgr::Instance()->GetDragonGuildShopItem(roArg.id());
	if (!itemTable)
	{
		LogError("Cannot find DragonGuildShopItem, id: %d", roArg.id());
		return ;
	}
	if (itemTable->DailyCountCondition && roArg.count() + item->buyCount > itemTable->DailyCountCondition)
	{
		roRes.set_result(KKSG::ERR_DG_SHOP_NO_COUNT);
		return ;
	}

	DragonGuildMember* member = pDGuild->GetMember(pRole->GetID());
	if (member == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		return ;
	}
	if (member->refreshShopLevel < itemTable->LevelShow[0] || member->refreshShopLevel > itemTable->LevelShow[1])
	{
		roRes.set_result(KKSG::ERR_DG_ITEM_NOT_FOUND);
		return ;
	}

	item->SetBuying();

	RpcM2G_BuyDragonGuildItemM2G* rpc = RpcM2G_BuyDragonGuildItemM2G::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_id(roArg.id());
	rpc->m_oArg.set_count(roArg.count());
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_BuyDragonGuildShopItem::OnDelayReplyRpc(const BuyDragonGuildShopItemArg &roArg, BuyDragonGuildShopItemRes &roRes, const CUserData &roUserData)
{
	roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (pDGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_DGUILD_NOT_EXIST);
		return ;
	}

	DragonGuildShop& shop = pDGuild->GetDragonGuildShop();
	DragonGuildShopItem* item = shop.GetItem(roArg.id());
	if (item == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_ITEM_NOT_FOUND);
		return ;
	}

	item->buying = false;
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		return ;
	}

	ShopTable::RowData* itemTable = DragonGuildTableMgr::Instance()->GetDragonGuildShopItem(roArg.id());
	if (itemTable == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_ITEM_NOT_FOUND);
		return;
	}

	item->buyCount += roArg.count();
	shop.AddBuyRecord(pRole->GetID(), itemTable->ItemId, roArg.count());
}
