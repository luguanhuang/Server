#include "pch.h"
#include "jade/rpcc2g_buyjadeslot.h"
#include "unit/rolemanager.h"
#include "gamelogic/item.h"
#include "gamelogic/bag.h"
#include "gamelogic/takecostmgr.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/jademgr.h"
#include "gamelogic/bagtransition.h"
#include "common/ptcg2c_itemchangedntf.h"

// generate by ProtoGen at date: 2015/6/30 10:44:12

RPC_SERVER_IMPLEMETION(RpcC2G_BuyJadeSlot, BuyJadeSlotArg, BuyJadeSlotRes)

void RpcC2G_BuyJadeSlot::OnCall(const BuyJadeSlotArg &roArg, BuyJadeSlotRes &roRes)
{
	/*Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	Bag* pBag = pRole->Get<Bag>();
	if (NULL == pBag)
	{
		SSWarn<<"pBag is NULL, role id:"<<pRole->GetID()<<END;
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	XItem* pItem = pBag->FindUniqueEquipOnBody(roArg.equipuid());
	if (NULL == pItem)
	{
		SSWarn<<"pItem is NULL, item unique id:"<<roArg.equipuid()<<END;
		roRes.set_errorcode(ERR_ITEM_NOTEXIST);
		return;
	}
	if(pItem->type != EQUIP)
	{
		LogWarn("item is not equip, itemid:%u, uid:%llu", pItem->itemID, pItem->uid);
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	if (NULL == pItem->jadeInfo)
	{
		SSWarn<<"pJadeInfo is NULL, item unique id:"<<roArg.equipuid()<<END;
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	EquipConf* pEquipConf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (NULL == pEquipConf)
	{
		SSWarn<<"pItem is NULL, item unique id:"<<roArg.equipuid()<<END;
		roRes.set_errorcode(ERR_ITEM_NOTEXIST);
		return;		
	}
	UINT32 dwEmtpySlot = CJadeMgr::Instance()->GetNextClosedSlot(pItem->jadeInfo->slotInfo);
	if ((UINT32)(-1) == dwEmtpySlot)
	{
		roRes.set_errorcode(ERR_JADE_NOEMPTYSLOT);
		return;				
	}
	if (!TakeCostManager::Instance()->TakeCost("JadeOpenGate", pRole, dwEmtpySlot, ItemFlow_Jade, ItemFlow_Jade_OpenSlot))
	{
		roRes.set_errorcode(ERR_JADE_OPENJADELACKCOIN);
		return;
	}
	
	CJadeMgr::Instance()->OpenJadeSlot(pItem->jadeInfo->slotInfo, pEquipConf->EquipPos, dwEmtpySlot);
	
	BagUpdateItemTransition bagUpdate(pRole);
	bagUpdate.AddChangedItem(pItem);
	bagUpdate.NotifyClient();

	//PtcG2C_ItemChangedNtf stNtf;
	//KKSG::Item *pDBItem = stNtf.m_Data.add_attrchangeitems();
	//XItem::Item2KKSGItem(pItem, pDBItem);
	//pRole->Send(stNtf);

	roRes.set_errorcode(ERR_SUCCESS);
	return;*/
}

void RpcC2G_BuyJadeSlot::OnDelayReplyRpc(const BuyJadeSlotArg &roArg, BuyJadeSlotRes &roRes, const CUserData &roUserData)
{
}
