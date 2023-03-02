#include "pch.h"
#include "jade/rpcc2g_jadecompose.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/item.h"
#include "gamelogic/jademgr.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/noticemgr.h"

// generate by ProtoGen at date: 2015/6/17 16:35:01

RPC_SERVER_IMPLEMETION(RpcC2G_JadeCompose, JadeComposeArg, JadeComposeRes)

void RpcC2G_JadeCompose::OnCall(const JadeComposeArg &roArg, JadeComposeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Bag *pBag = pRole->Get<Bag>();
	if (0 == roArg.composetype())		///> 合成一个
	{
		XItem* pItem = pBag->FindUniqueItemInBag(roArg.jadeuniqueid());
		if (NULL == pItem)
		{
			roRes.set_errorcode(KKSG::ERR_JADECOMPOSE_NOTFIND);
			return;
		}

		if (!CJadeMgr::Instance()->JadeCanBeComposed(pItem->itemID))
		{
			roRes.set_errorcode(KKSG::ERR_JADE_MAXLEVEL);
			return;
		}

		UINT32 destid;
		std::map<UINT32, UINT32> costItems;
		KKSG::ErrorCode errorCode = CJadeMgr::Instance()->GetNeedItemsComposeJade(pRole, pItem->itemID, roArg.addlevel(), false, destid, costItems);

		if (errorCode != KKSG::ERR_SUCCESS)
		{
			roRes.set_errorcode(errorCode);
			return;
		}

		BagTakeItemTransition take(pRole);
		take.SetReason(ItemFlow_Jade, ItemFlow_Jade_Compose);
		for (auto i = costItems.begin(); i != costItems.end(); ++i)
		{
			if (!take.TakeItem(i->first, i->second))
			{
				take.RollBack();
				if (i->first == GOLD)
				{
					roRes.set_errorcode(KKSG::ERR_JADE_GOLDNOTENOUGH);
				}
				else if (i->first == DRAGON_COIN)
				{
					roRes.set_errorcode(KKSG::ERR_JADE_DRAGONNOTENOUGH);
				}
				else
				{
					roRes.set_errorcode(KKSG::ERR_JADE_COUNTNOTENOUGH);
				}
				return;
			}
		}
		take.NotifyClient();

		BagGiveItemTransition stGiveTransition(pRole);
		stGiveTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_Compose);

		stGiveTransition.GiveItem(destid, 1);
		stGiveTransition.NotifyClient();

		roRes.set_errorcode(KKSG::ERR_SUCCESS);

		XItem item(0, destid, 1, JADE);
		NoticeMgr::Instance()->ItemNotice(pRole, &item, CJadeMgr::Instance()->GetLevel(destid));

		pRole->OnComposeJade(CJadeMgr::Instance()->GetLevel(destid), 1);

		return;
	}
	else if (1 == roArg.composetype()) //装备上的龙玉升级
	{
		int index;
		XItem* pItem = pBag->FindUniqueEquipOnBody(roArg.equipuniqueid(), &index);
		if (NULL == pItem)
		{
			roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
			return;
		}
		if (pItem->type != EQUIP)
		{
			LogWarn("item is not equip, itemid:%u, uid:%llu", pItem->itemID, pItem->uid);
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			return;
		}
		JadeInfo* pJadeInfo = pItem->jadeInfo;
		if (NULL == pJadeInfo)
		{
			roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
			return;
		}
		JadeSingle* pJadeSingle = pJadeInfo->GetJadeSingle(roArg.slotpos());
		if (NULL == pJadeSingle)
		{
			roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
			return;
		}
		UINT32 destid;
		std::map<UINT32, UINT32> costItems;
		KKSG::ErrorCode errorCode = CJadeMgr::Instance()->GetNeedItemsComposeJade(pRole, pJadeSingle->jadeid, roArg.addlevel(), true, destid, costItems);	

		if (false == CJadeMgr::Instance()->CanWearJade(destid, pRole->GetLevel()))
		{
			roRes.set_errorcode(KKSG::ERR_JADE_MINEQUIPLEVEL);
			return;
		}

		if (errorCode != KKSG::ERR_SUCCESS)
		{
			roRes.set_errorcode(errorCode);
			return;
		}

		BagTakeItemTransition take(pRole);
		take.SetReason(ItemFlow_Jade, ItemFlow_Jade_BodyCompose);
		for (auto i = costItems.begin(); i != costItems.end(); ++i)
		{
			if (!take.TakeItem(i->first, i->second))
			{
				take.RollBack();
				if (i->first == GOLD)
				{
					roRes.set_errorcode(KKSG::ERR_JADE_GOLDNOTENOUGH);
				}
				else if (i->first == DRAGON_COIN)
				{
					roRes.set_errorcode(KKSG::ERR_JADE_DRAGONNOTENOUGH);
				}
				else
				{
					roRes.set_errorcode(KKSG::ERR_JADE_COUNTNOTENOUGH);
				}
				return;
			}
		}	

		///> 新龙玉		
		JadeConf* pNextJadeConf = ItemConfig::Instance()->GetJadeConf(destid);
		if (pNextJadeConf == NULL)
		{
			LogError("Jade compose, Jadeid:%d can't find in Jadelist", destid);
			take.RollBack();
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			return;
		}

		take.NotifyClient();

		std::vector<ItemDesc> vlist;
		XItem::PrintEquipTLog(pRole, pItem, TX_Item_JadeUp, vlist, destid, pJadeSingle->jadeid);
	
		JadeSingle stJadeSingle;
		stJadeSingle.slotPos = pJadeSingle->slotPos;
		stJadeSingle.jadeid = destid;
		for (auto i = pNextJadeConf->JadeAttributes.begin(); i != pNextJadeConf->JadeAttributes.end(); ++i)
		{
			stJadeSingle.attrs.push_back(ItemChangeAttr(i->seq[0], i->seq[1]));
		}
		BagChangeItemAttrTransition stChangeAttr(pRole);
		stChangeAttr.ChangeItemJade(pItem, stJadeSingle);
		stChangeAttr.NotifyClient();

		pRole->RecalcuateAttributeAndNotify(RecalAttr_Jade);

		roRes.set_errorcode(ERR_SUCCESS);

		XItem item(0, stJadeSingle.jadeid, 1, JADE);
		NoticeMgr::Instance()->ItemNotice(pRole, &item, CJadeMgr::Instance()->GetLevel(stJadeSingle.jadeid));

		pRole->OnComposeJade(CJadeMgr::Instance()->GetLevel(stJadeSingle.jadeid), 1);
		pRole->OnChangeJadeOnEquip((UINT32)index);

		return;
	}

	roRes.set_errorcode(ERR_UNKNOWN);
}

void RpcC2G_JadeCompose::OnDelayReplyRpc(const JadeComposeArg &roArg, JadeComposeRes &roRes, const CUserData &roUserData)
{
}
