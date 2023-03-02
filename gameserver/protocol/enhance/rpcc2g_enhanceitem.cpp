#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/item.h"
#include "gamelogic/enhanceequipmgr.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/bagtransition.h"
#include "unit/combatattribute.h"
#include "unit/calcattribute.h"
#include "enhance/rpcc2g_enhanceitem.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/payv2Record.h"
#include "table/PlayerLevelManager.h"

// generate by ProtoGen at date: 2015/5/21 20:43:48

RPC_SERVER_IMPLEMETION(RpcC2G_EnhanceItem, EnhanceItemArg, EnhanceItemRes)

void RpcC2G_EnhanceItem::OnCall(const EnhanceItemArg &roArg, EnhanceItemRes &roRes)
{
	///> 获取制定装备
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL"<<END;
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	int slot = -1;
	XItem* pItem = pRole->Get<Bag>()->FindUniqueEquipOnBody(roArg.uniqueitemid(), &slot);
	if (NULL == pItem)
	{
		SSWarn<<"pItem is NULL, item unique id:"<<roArg.uniqueitemid()<<END;
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}
	if(pItem->type != EQUIP)
	{
		LogWarn("item is not equip, itemid:%u, uid:%llu", pItem->itemID, pItem->uid);
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	EquipConf* pEquipConf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (NULL == pEquipConf)
	{
		SSWarn<<"pEquipConf is NULL, item id:"<<pItem->itemID<<END;
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}

	///> 是否可以强化
	UINT32 currentlevel = pItem->enhanceInfo == NULL ? 0 : pItem->enhanceInfo->level; 
	if (currentlevel >= PlayerLevelManager::Instance()->GetEnhanceMaxLvl(pRole->GetLevel()))
	{
		roRes.set_errorcode(KKSG::ERR_ENHANCE_MAX);
		return;
	}

	///> 消耗材料
	std::vector<ItemDesc> vlist;
	std::vector<std::pair<UINT32, UINT32>> vec;
	if (!EnhanceEquipMgr::Instance()->UseEnhanceItem(pRole, pEquipConf->EquipPos, currentlevel + 1, vec, vlist))
	{
		roRes.set_errorcode(KKSG::ERR_ENHANCE_LACKITEM);
		return;
	}
	for(auto i = vec.begin(); i != vec.end(); i++)
	{
		KKSG::ComAgate* pAgate =  roRes.add_comagates();
		pAgate->set_itemid((*i).first);
		pAgate->set_comnum((*i).second);
		vlist.push_back(ItemDesc((*i).first, (*i).second));
	}

	KKSG::ErrorCode code = EnhanceEquipMgr::Instance()->EnhanceEquip(pRole, pItem, slot, vlist);
	roRes.set_errorcode(code);

	if (code == KKSG::ERR_ENHANCE_SUCCEED)
	{
		///> 下一次强化属性
		const std::vector<Sequence<UINT32, 2>>* pAttrs = EnhanceEquipMgr::Instance()->GetEnhanceAttr(slot, pItem->enhanceInfo->level, pRole->GetProfession());	
		if (pAttrs == NULL)
		{
			roRes.set_isend(true);
			return;
		}
		roRes.set_isend(false);
		for (auto i = pAttrs->begin(); i != pAttrs->end(); ++i)
		{
			KKSG::AttributeInfo* attr = roRes.add_nextattrs();
			attr->set_id((*i)[0]);
			attr->set_value((*i)[1]);
		}
	}

	SSInfo << "finish enhance item, begin to handle desigantion and achieve, roleid=" << pRole->GetID() << ", itemid=" << pItem->itemID << ", errorcode=" << roRes.errorcode() << END;
	if (code == KKSG::ERR_ENHANCE_SUCCEED)
	{
		NoticeMgr::Instance()->ItemNotice(pRole, pItem, pItem->enhanceInfo->level);
		DesignationMgr::Instance()->OnAnyEquipLevel(pRole, pItem->enhanceInfo->level);
		AchieveV2Mgr::Instance()->OnEquipLevel(pRole, pItem->enhanceInfo->level);

		SSInfo << "begin to handle suit level achieve, roleid=" << pRole->GetID() << ", itemid=" << pItem->itemID << END;
		AchieveV2Mgr::Instance()->OnSuitLevel(pRole, roArg.uniqueitemid(), pItem->enhanceInfo->level);
	}	
}

void RpcC2G_EnhanceItem::OnDelayReplyRpc(const EnhanceItemArg &roArg, EnhanceItemRes &roRes, const CUserData &roUserData)
{

}
