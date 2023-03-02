#include "pch.h"
#include "gamelogic/item.h"
#include "gamelogic/itemconfig.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/rolefashion.h"
#include "gamelogic/bagtransition.h"
#include "item/rpcc2g_itemsell.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2017/1/13 17:13:46

RPC_SERVER_IMPLEMETION(RpcC2G_ItemSell, ItemSellArg, ItemSellRes)

	void RpcC2G_ItemSell::OnCall(const ItemSellArg &roArg, ItemSellRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	bool istake = false;
	UINT32 itemid = 0;
	UINT32 itemcount = 1;
	XItem sell;
	bool isfashion = false;
	// item
	XItem* item = role->Get<Bag>()->FindUniqueItemInBag(roArg.uid());
	if (item != NULL)
	{
		itemid = item->itemID;
		itemcount = item->itemCount;
	}
	else
	{		
		FashionInfo*  fashion = role->Get<RoleFashion>()->GetFashionItemByUID(roArg.uid());
		if (fashion != NULL)
		{
			// 时装不堆叠，所以count = 1
			itemid = fashion->itemID;
			isfashion = true;
		}
	}

	if (itemid > 0)
	{			
		sell = ItemConfig::Instance()->GetSell(itemid);
		if (0 == sell.itemID)
		{
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}		
		if (isfashion)
		{
			role->Get<RoleFashion>()->DeleteFashionByUID(roArg.uid());
		}
		else
		{
			BagTakeItemTransition trans(role);
			trans.SetReason(ItemFlow_ItemSell, ItemFlow_ItemSell_Out);
			if (!trans.TakeUniqueItem(item->uid, itemcount))
			{
				trans.RollBack();
				roRes.set_errorcode(KKSG::ERR_FAILED);
				return;
			}
			trans.NotifyClient();
		}

		istake = true;
	}
	else
	{
		SSWarn<<"not find uid:"<<roArg.uid()<<END;
	}
	// fashion
	//if (!istake)
	//{
	//	FashionInfo* fashion = role->Get<RoleFashion>()->FindFashionByUId(roArg.uid());
	//	itemid = fashion->itemID;

	//	sell = ItemConfig::Instance()->GetSell(itemid);
	//	if (0 == sell.itemID)
	//	{
	//		roRes.set_errorcode(KKSG::ERR_FAILED);
	//		return;
	//	}

	//	if (NULL != fashion)
	//	{
	//		istake = role->Get<RoleFashion>()->DelFashion(roArg.uid());

	//		// item consume reason
	//		TItemFlowLog oLog1(role);
	//		oLog1.m_nItemID = itemid;
	//		oLog1.m_nType = FASHION;
	//		oLog1.m_nCount = (INT32)1;
	//		oLog1.m_nAfterCount = role->Get<RoleFashion>()->GetFashionNumByItemId(itemid);
	//		oLog1.m_nReason = ItemFlow_ItemSell;
	//		oLog1.m_nSubReason = ItemFlow_ItemSell_Out;
	//		oLog1.m_nMoneyCost = 0;
	//		oLog1.m_nMoneyType = 0;
	//		oLog1.m_nAddOrReduce = TX_REDUCE;
	//		oLog1.Do();
	//	}
	//}

	// sell
	if (istake)
	{
		BagGiveItemTransition transgive(role);
		transgive.SetReason(ItemFlow_ItemSell, ItemFlow_ItemSell_In);
		transgive.GiveItem(sell.itemID, sell.itemCount * itemcount);
		transgive.NotifyClient();
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
		return;
	}
	roRes.set_errorcode(KKSG::ERR_FAILED);
}

void RpcC2G_ItemSell::OnDelayReplyRpc(const ItemSellArg &roArg, ItemSellRes &roRes, const CUserData &roUserData)
{
}
