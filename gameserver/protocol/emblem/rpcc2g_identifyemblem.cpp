#include "pch.h"
#include "emblem/rpcc2g_identifyemblem.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/item.h"
#include "gamelogic/emblemmgr.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/itemconfig.h"

// generate by ProtoGen at date: 2016/7/9 12:01:17

RPC_SERVER_IMPLEMETION(RpcC2G_IdentifyEmblem, IdentifyEmblemArg, IdentifyEmblemRes)

void RpcC2G_IdentifyEmblem::OnCall(const IdentifyEmblemArg &roArg, IdentifyEmblemRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Bag *pBag = pRole->Get<Bag>();
	XItem* pItem = pBag->FindUniqueItemInBag(roArg.uid());
	if(pItem == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}
	EmblemBasicConf* pEmblemConf = ItemConfig::Instance()->GetEmblemBasicConf(pItem->itemID);
	if(pEmblemConf->EmblemType > EmblemBoundary)
	{
		roRes.set_errorcode(KKSG::ERR_EMBLEM_CANTIDENTIFY);
		return;
	}
	UINT32 dragonCoinCost = pEmblemConf->DragonCoinCost;
	BagTakeItemTransition stTakeTransition(pRole);
	stTakeTransition.SetReason(ItemFlow_Emblem, ItemFlow_Emblem_IdentifyCost);
	if(false == stTakeTransition.TakeItem(DRAGON_COIN, dragonCoinCost))
	{
		stTakeTransition.RollBack();
		roRes.set_errorcode(KKSG::ERR_ITEM_NEED_DRAGONCOIN);
		return;
	}
	stTakeTransition.NotifyClient();

	KKSG::ErrorCode errcode = CEmblemMgr::Instance()->IdentifyAttrEmblem(pItem, pRole);
	if(KKSG::ERR_SUCCESS == errcode)
	{
		BagUpdateItemTransition bagUpdate(pRole);
		bagUpdate.AddChangedItem(pItem);
		bagUpdate.NotifyClient();

		std::vector<ItemDesc> vlist;
		XItem::PrintEmblemTLog(pRole, pItem, TX_Emblem_Identify, -1, vlist);
	}
	roRes.set_errorcode(errcode);
}

void RpcC2G_IdentifyEmblem::OnDelayReplyRpc(const IdentifyEmblemArg &roArg, IdentifyEmblemRes &roRes, const CUserData &roUserData)
{
}
