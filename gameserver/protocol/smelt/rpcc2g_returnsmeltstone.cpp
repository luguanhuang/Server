#include "pch.h"
#include "smelt/rpcc2g_returnsmeltstone.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/randomattrmgr.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/emblemmgr.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2017/5/13 19:38:04

RPC_SERVER_IMPLEMETION(RpcC2G_ReturnSmeltStone, ReturnSmeltStoneArg, ReturnSmeltStoneRes)

void RpcC2G_ReturnSmeltStone::OnCall(const ReturnSmeltStoneArg &roArg, ReturnSmeltStoneRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	XItem* pItem = pRole->Get<Bag>()->FindUniqueItemInBag(roArg.uid());
	if (pItem == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}
	if (pItem->smeltCount == 0)
	{
		roRes.set_errorcode(KKSG::ERR_SMELTCOUNT_ZERO);
		return;
	}
	std::vector<std::pair<UINT32,UINT32> > vList;
	switch (pItem->type)
	{
	case EQUIP:
		{
			EquipConf* conf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
			if (conf == NULL)
			{
				LogWarn("itemid [%u] not find in EquipList.txt when role [%llu] want to return smelt stone", pItem->itemID, pRole->GetID());
				roRes.set_errorcode(KKSG::ERR_UNKNOWN);
				return;
			}

			RandomAttrMgr::Instance()->ResetRandomAttrAndForgeAttr(pItem);

			UINT32 count = pItem->smeltCount * conf->ReturnSmeltStoneRate;
			pItem->smeltCount = 0;

			BagGiveItemTransition give(pRole);
			give.SetReason(ItemFlow_ReturnSmeltStone, ItemFlow_ReturnSmeltStone_Equip);
			for (auto i = conf->SmeltNeedItem.begin(); i != conf->SmeltNeedItem.end(); i++)
			{
				give.GiveItem((*i)[0], (*i)[1] * count / 100);
				vList.push_back(std::make_pair((*i)[0], (*i)[1] * count / 100));
			}
			give.NotifyClient();
			
			roRes.set_errorcode(KKSG::ERR_SUCCESS);
			break;
		}
	case EMBLEM:
		{
			EmblemBasicConf* conf = ItemConfig::Instance()->GetEmblemBasicConf(pItem->itemID);
			if (conf == NULL)
			{
				LogWarn("itemid [%u] not find in EmblemBasic.txt when role [%llu] want to return smelt stone", pItem->itemID, pRole->GetID());
				roRes.set_errorcode(KKSG::ERR_UNKNOWN);
				return;
			}

			CEmblemMgr::Instance()->ResetAttr(pItem);
			
			UINT32 count = pItem->smeltCount * conf->ReturnSmeltStoneRate;
			pItem->smeltCount = 0;

			BagGiveItemTransition give(pRole);
			give.SetReason(ItemFlow_ReturnSmeltStone, ItemFlow_ReturnSmeltStone_Emblem);
			for (auto i = conf->SmeltNeedItem.begin(); i != conf->SmeltNeedItem.end(); i++)
			{
				give.GiveItem((*i)[0], (*i)[1] * count / 100);
				vList.push_back(std::make_pair((*i)[0], (*i)[1] * count / 100));
			}
			give.NotifyClient();

			roRes.set_errorcode(KKSG::ERR_SUCCESS);
			break;
		}
	default:
		{
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
	}

	BagUpdateItemTransition updateItem(pRole);
	updateItem.AddChangedItem(pItem);
	updateItem.NotifyClient();

	TItemReturnFlow oLog(pRole);
	oLog.m_ItemId = pItem->itemID;
	oLog.m_ItemUid = pItem->uid;
	for (UINT32 i = 0; i < vList.size(); i ++)
	{
		oLog.m_BackItemId = vList[i].first;
		oLog.m_BackItemNum = vList[i].second;
		oLog.Do();
	}
}

void RpcC2G_ReturnSmeltStone::OnDelayReplyRpc(const ReturnSmeltStoneArg &roArg, ReturnSmeltStoneRes &roRes, const CUserData &roUserData)
{
}
