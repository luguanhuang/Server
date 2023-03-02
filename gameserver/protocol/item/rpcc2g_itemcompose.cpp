#include "pch.h"
#include "item/rpcc2g_itemcompose.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/itemconfig.h"
#include "define/tlogenum.h"
#include "gamelogic/bagtransition.h"
#include "unit/systeminfo.h"

// generate by ProtoGen at date: 2017/3/8 21:14:03

RPC_SERVER_IMPLEMETION(RpcC2G_ItemCompose, ItemComposeArg, ItemComposeRes)

void RpcC2G_ItemCompose::OnCall(const ItemComposeArg &roArg, ItemComposeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_ITEM_COMPOSE))
	{
		SSInfo<<"item compose system close"<<END;
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}
	Bag *pBag = pRole->Get<Bag>();
	UINT64 uid = convert<UINT64>(roArg.uid());
	XItem* pItem = pBag->FindUniqueItemInBag(uid);
	if (pItem == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}
	ItemComposeTableConf* conf = ItemConfig::Instance()->GetSpComposeConf(pItem->itemID);
	if (conf == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	if (pRole->GetLevel() < conf->Level)
	{
		roRes.set_errorcode(KKSG::ERR_FASHIONCOMPOSE_LEVEL_REQ);
		return;
	}
	if (conf->SrcItem1[1] == 0)
	{
		LogError("ItemCompose error, type 4, scritem1 [%u], count = 0", pItem->itemID);
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	UINT32 canGive = pItem->itemCount / conf->SrcItem1[1];
	if (conf->Coin != 0)
	{
		UINT64 count = pBag->CountVirtualItem(GOLD);
		if (canGive > count / conf->Coin)
		{
			canGive = (UINT32)(count / conf->Coin);
		}
	}
	if (canGive == 0)
	{
		roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
		return;
	}
	BagTakeItemTransition oTake(pRole);
	oTake.SetReason(ItemFlow_Compose, ItemFlow_Compose_Common);
	oTake.TakeUniqueItem(uid, canGive * conf->SrcItem1[1]);
	oTake.TakeItem(GOLD, canGive * conf->Coin);
	oTake.NotifyClient();

	BagGiveItemTransition oGive(pRole);
	oGive.SetReason(ItemFlow_Compose, ItemFlow_Compose_Common);
	oGive.GiveItem(conf->ItemID, canGive);
	oGive.NotifyClient();

	pBag->RearrangeBag();

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_ItemCompose::OnDelayReplyRpc(const ItemComposeArg &roArg, ItemComposeRes &roRes, const CUserData &roUserData)
{
}
