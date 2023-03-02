#include "pch.h"
#include "enhanceequipmgr.h"
#include "util/XRandom.h"
#include "bagtransition.h"
#include "item.h"
#include "table/globalconfig.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "foreach.h"
#include "event/eventmgr.h"
#include "table/EnhanceAttribute.h"
#include "rolesummaryhandler.h"
#include "utility/tlogger.h"
#include "table/PlayerLevelManager.h"

INSTANCE_SINGLETON(EnhanceEquipMgr)

bool EnhanceEquipMgr::Init()
{
	if (!m_oTable.LoadFile("table/Enhance.txt"))
	{
		SSWarn << "Load table table/Enhance.txt failed!" << END;
		return false;
	}

	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); i++)
	{
		EnhanceConf conf;
		conf.needItem = (*i)->NeedItem;
		conf.successRate = (*i)->SuccessRate;
		conf.upRate = (*i)->UpRate;
		conf.isNeedBreak = (*i)->IsNeedBreak;
		UINT32 enhanceKey = _GetEnhanceKey((*i)->EquipPos, (*i)->EnhanceLevel);
		if(m_enhance.find(enhanceKey) != m_enhance.end())
		{
			LogWarn("key is not unique, equipPos:%u, enhanceLevel:%u", (*i)->EquipPos, (*i)->EnhanceLevel);
			return false;
		}
		m_enhance.insert(std::make_pair(enhanceKey, conf));
	}
	std::vector<UINT32> prof;
	ProfessionMgr::Instance()->GetAllProfession(prof);
	EnhanceAttribute attrTable;
	for (auto i = prof.begin(); i != prof.end(); i++)
	{
		std::string name = "table/EnhanceAttribute/EnhanceAttribute_" + ToString(*i) + ".txt";
		attrTable.Clear();
		if (!attrTable.LoadFile(name.c_str()))
		{
			SSWarn << "Load table " << name << " failed!" << END;
			return false;
		}
		if (attrTable.Table.size() != m_enhance.size())
		{
			SSWarn << name << " size != Enhance.txt size" << END;
			return false;
		}
		for (auto j = attrTable.Table.begin(); j != attrTable.Table.end(); j++)
		{
			UINT32 enhanceKey = _GetEnhanceKey((*j)->EquipPos, (*j)->EnhanceLevel);
			if (m_enhance.find(enhanceKey) == m_enhance.end())
			{
				LogWarn("enhanceKey not in Enhance.txt, table:%s equipPos:%u, enhanceLevel:%u", name.c_str(), (*j)->EquipPos, (*j)->EnhanceLevel);
				return false;
			}
			if (!m_enhance[enhanceKey].addAttr(*i, (*j)->EnhanceAttr))
			{
				LogWarn("table:%s has same enhanceKey, equipPos:%u, enhanceLevel:%u", name.c_str(), (*j)->EquipPos, (*j)->EnhanceLevel);
				return false;
			}
		}
	}

	if (!m_enhanceMaster.LoadFile("table/EnhanceMaster.txt"))
	{
		SSWarn<<"Load table table/EnhanceMaster.txt failed!"<<END;
		return false;
	}
	for (auto i = m_enhanceMaster.Table.begin(); i != m_enhanceMaster.Table.end(); i++)
	{
		m_enhanceMs[_GetEnhanceMsKey((*i)->ProfessionId,(*i)->TotalEnhanceLevel)] = *i;
	}

	return true;
}

void EnhanceEquipMgr::Uninit()
{
}

void EnhanceEquipMgr::ChangeAttr(Role* role, UINT32 destlevel, XItem* item)
{
	EquipConf* pEquipConf = ItemConfig::Instance()->GetEquipConf(item->itemID);
	if (pEquipConf == NULL)
	{
		return;
	}

	auto iter = m_enhance.find(_GetEnhanceKey(pEquipConf->EquipPos, destlevel));
	if(iter == m_enhance.end())
	{
		std::vector<ItemChangeAttr> attrs;
		BagChangeItemAttrTransition changeattr(role);
		changeattr.ChangeItemEnhanceAttr(item, attrs);
		changeattr.NotifyClient();
		return;
	}

	EnhanceConf& conf = iter->second;
	if(conf.enhanceAttr.find(role->GetProfession()) == conf.enhanceAttr.end())
	{
		LogWarn("Can't find in Enhance.txt, equippos:%u, destlevel:%u, role professtion:%d", pEquipConf->EquipPos, destlevel, role->GetProfession());
		return;
	}
	std::vector<Sequence<UINT32,2>>& preAttr = conf.enhanceAttr[role->GetProfession()];

	BagChangeItemAttrTransition changeattr(role);
	std::vector<ItemChangeAttr> attrs;
	for(auto i = preAttr.begin(); i != preAttr.end(); i++)
	{
		attrs.push_back(ItemChangeAttr((*i)[0], (*i)[1]));
	}
	
	changeattr.ChangeItemEnhanceAttr(item, attrs);
	changeattr.NotifyClient();
}

void EnhanceEquipMgr::DebugLevel(Role* role, XItem* item, UINT32 level)
{
	EquipConf* pEquipConf = ItemConfig::Instance()->GetEquipConf(item->itemID);
	if (pEquipConf == NULL)
	{
		return ;
	}

	auto iter = m_enhance.find(_GetEnhanceKey(pEquipConf->EquipPos, level));
	if (iter == m_enhance.end())
	{
		return;
	}
	EnhanceConf& conf = iter->second;
	if (conf.enhanceAttr.find(role->GetProfession()) == conf.enhanceAttr.end())
	{
		LogWarn("Can't find in Enhance.txt, equippos:%u, destlevel:%u, role professtion:%d", pEquipConf->EquipPos, level, role->GetProfession());
		return;
	}
	std::vector<Sequence<UINT32,2>>& preAttr = conf.enhanceAttr[role->GetProfession()];

	if (item->enhanceInfo == NULL)
	{
		item->enhanceInfo = new EnhanceInfo();
	}
	item->enhanceInfo->count = 0;
	item->enhanceInfo->level = level;
	item->enhanceInfo->attrs.clear();
	for(auto i = preAttr.begin(); i != preAttr.end(); i++)
	{
		item->enhanceInfo->attrs.push_back(ItemChangeAttr((*i)[0], (*i)[1]));
	}
}

const EnhanceConf* EnhanceEquipMgr::GetEnhanceConf(UINT32 equipPos, UINT32 enhanceLevel)
{
	auto iter = m_enhance.find(_GetEnhanceKey(equipPos, enhanceLevel));
	if (iter == m_enhance.end())
	{
		return NULL;
	}
	return &iter->second;
}

const std::vector<Sequence<UINT32, 2>>* EnhanceEquipMgr::GetEnhanceAttr(UINT32 equipPos, UINT32 enhanceLevel, UINT32 profession)
{
	const EnhanceConf* conf = GetEnhanceConf(equipPos, enhanceLevel);
	if (conf == NULL)
	{
		return NULL;
	}
	auto iter = conf->enhanceAttr.find(profession);
	if (iter == conf->enhanceAttr.end())
	{
		return NULL;
	}
	return &iter->second;
}

UINT32 EnhanceEquipMgr::DoTransfer(Role* role, XItem* origin, XItem* dest)
{
	UINT32 destLvl = NULL == dest->enhanceInfo ? 0 : dest->enhanceInfo->level;
	UINT32 originLvl = NULL == origin->enhanceInfo ? 0 : origin->enhanceInfo->level;
	if (origin->enhanceInfo == NULL)
	{
		origin->enhanceInfo = new EnhanceInfo;	
	}
	origin->enhanceInfo->count = 0;
	origin->enhanceInfo->level = destLvl;
	origin->isbind = true;
	ChangeAttr(role, destLvl, origin);

	if (dest->enhanceInfo == NULL)
	{
		dest->enhanceInfo = new EnhanceInfo;
	}
	dest->enhanceInfo->count = 0;
	dest->enhanceInfo->level = originLvl;
	dest->isbind = true;
	ChangeAttr(role, originLvl, dest);

	TItemEnHanceLog oLog(role);
	oLog.m_nOperType = TX_ITEM_TRANSTER;
	oLog.m_nChgLevel = originLvl;
	oLog.m_nAfterLevel = destLvl;
	oLog.m_iItemID = origin->itemID;
	oLog.m_lUID = origin->uid;
	oLog.m_iItemID2 = dest->itemID;
	oLog.m_iLevel2 = originLvl;
	oLog.m_lUID2 = dest->uid;
	oLog.Do();
	return KKSG::ERR_SUCCESS;
}

bool EnhanceEquipMgr::UseEnhanceItem(Role* role, UINT32 equipPos, UINT32 enhanceLvl, std::vector<std::pair<UINT32, UINT32>>& combinVec, std::vector<ItemDesc>& vlist)
{
	auto iter = m_enhance.find(_GetEnhanceKey(equipPos, enhanceLvl));
	if (iter == m_enhance.end())
	{
		return false;
	}
	Bag* pBag = role->Get<Bag>();

	BagTakeItemTransition take(role);
	take.SetReason(ItemFlow_Equip, ItemFlow_Equip_Enhance);
	for (auto i = iter->second.needItem.begin(); i != iter->second.needItem.end(); i++)
	{
		UINT32 tempId = (*i).seq[0];
		UINT32 tempNum = (*i).seq[1];
		if (take.TakeItem(tempId, tempNum))
		{
			if (tempId != GOLD)vlist.push_back(ItemDesc(tempId, tempNum));//用来记录tlog，金币不需要记录;
			continue;
		}
		take.RollBack();
		while (CanExchange(tempId))
		{
			UINT32 cnt = pBag->CountItemInBag(tempId);
			if (cnt >= tempNum)
			{
				take.TakeItem(tempId, tempNum);
				combinVec.push_back(std::make_pair(tempId, tempNum));
				tempNum = 0;
				break;
			}
			tempNum -= cnt;
			take.TakeItem(tempId, cnt);
			if (cnt)vlist.push_back(ItemDesc(tempId, cnt));
			if (tempId != (*i).seq[0] && cnt != 0)
			{
				combinVec.push_back(std::make_pair(tempId, cnt));
			}

			UINT32 rate = 0;
			GetExItemRate(tempId, tempId, rate);
			tempNum *= rate;
		}
		if (tempNum > 0)
		{
			if (!take.TakeItem(tempId, tempNum)) //最后一个并不可以exchange所以还需要判一下
			{
				take.RollBack();
				return false;
			}
			combinVec.push_back(std::make_pair(tempId, tempNum));
		}
	}
	take.NotifyClient();
	return true;
}

KKSG::ErrorCode EnhanceEquipMgr::EnhanceEquip(Role* pRole, XItem* pItem, UINT32 slot, std::vector<ItemDesc>& vlist)
{
	EquipConf* pEquipConf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (pEquipConf == NULL)
	{
		return KKSG::ERR_ENHANCE_FAILED;
	}

	if (pItem->enhanceInfo == NULL)
	{
		pItem->enhanceInfo = new EnhanceInfo;
	}

	TItemEnHanceLog oLog(pRole);
	oLog.m_iItemID = pItem->itemID;
	oLog.m_lUID = pItem->uid;
	oLog.m_ItemList.m_vList = vlist;
	///> 强化是否成功
	UINT32 currentlevel = pItem->enhanceInfo->level;
	UINT32 destlevel = currentlevel + 1;
	if (!GetResult(pEquipConf->EquipPos, destlevel, pItem->enhanceInfo->count))
	{
		pItem->enhanceInfo->count++;
		BagUpdateItemTransition transition(pRole);
		transition.AddChangedItem(pItem);
		transition.NotifyClient();

		///> 抛出玩家强化装备事件
		pRole->OnEnhanceEquip(slot);

		//强化失败也需要记录;
		oLog.m_nAfterLevel = currentlevel;
		oLog.m_nOperType = TX_ITEM_DEFAULT;
		oLog.m_nChgLevel = 0;
		oLog.Do();
		return KKSG::ERR_ENHANCE_FAILED;
	}

	pItem->enhanceInfo->count = 0;
	pItem->enhanceInfo->level = destlevel;

	///> 修改装备强化属性
	ChangeAttr(pRole, destlevel, pItem);
	///> 强化套装检查
	CheckEnhanceMaster(pRole, destlevel);
	///> 抛出玩家强化装备事件
	pRole->OnEnhanceEquip(slot);
	///> 强化身上装备需要重置角色属性和战斗力
	pRole->RecalcuateAttributeAndNotify(RecalAttr_Enhance);

	//强化成功;
	oLog.m_nAfterLevel = destlevel;
	oLog.m_nOperType = TX_ITEM_UP;
	oLog.m_nChgLevel = destlevel - currentlevel;
	oLog.Do();

	return KKSG::ERR_ENHANCE_SUCCEED;
}

bool EnhanceEquipMgr::CanExchange(UINT32 itemId)
{
	foreach (i in GetGlobalConfig().ExchangeRate)
	{
		if (i->seq[0] == itemId)
		{
			return true;
		}
	}
	return false;
}

void EnhanceEquipMgr::GetExItemRate(UINT32 itemId, UINT32& exhId, UINT32& rate)
{
	foreach (i in GetGlobalConfig().ExchangeRate)
	{
		if (i->seq[0] == itemId)
		{
			exhId = i->seq[1];
			rate = i->seq[2];
			return ;
		}
	}
	return ;
}

bool EnhanceEquipMgr::GetResult(UINT32 equipPos, UINT32 enhanceLvl, UINT32 losTime)
{
	auto iter = m_enhance.find(_GetEnhanceKey(equipPos, enhanceLvl));
	if(iter == m_enhance.end())
	{
		return false;
	}
	UINT32 perNum = iter->second.successRate + iter->second.upRate * losTime;

	UINT32 randNum = XRandom::randInt(1, 100 + 1);
	if (randNum <= perNum)
	{
		return true;
	}

	return false;
}

UINT32 EnhanceEquipMgr::_GetEnhanceKey(UINT32 equipPos, UINT32 enhanceLevel)
{
	return equipPos * 10000 + enhanceLevel;
}

UINT32 EnhanceEquipMgr::_GetEnhanceMsKey(UINT32 prof, UINT32 enhanceLevel)
{
	return prof * 10000 + enhanceLevel;
}

void EnhanceEquipMgr::CheckEnhanceMaster(Role* pRole, UINT32 enhanceLvl)
{
	auto iter = m_enhanceMs.find(_GetEnhanceMsKey(pRole->GetProfession(), enhanceLvl));
	if(iter == m_enhanceMs.end())
	{
		return;
	}
	Bag* pBag = pRole->Get<Bag>();

	for (UINT32 cnt = 0; cnt < EquipCount; cnt++)
	{
		XItem* pItem = pBag->GetItemOnBody(EQUIP, cnt);
		if (pItem == NULL)
		{
			return;
		}
		UINT32 currentlevel = pItem->enhanceInfo == NULL ? 0 : pItem->enhanceInfo->level;
		if (currentlevel < enhanceLvl)
		{
			return;
		}
	}
	pBag->SetEnhanceMaster(enhanceLvl);

	pRole->OnEnhanceMaster(enhanceLvl);
}

void EnhanceEquipMgr::ApplySuitEffect(CombatAttribute* combatAttr, Role* pRole)
{
	if (pRole == NULL || combatAttr == NULL)
	{
		return;
	}

	auto iter = m_enhanceMs.find(_GetEnhanceMsKey(pRole->GetProfession(), pRole->Get<Bag>()->GetEnhanceMaster()));
	if(iter == m_enhanceMs.end())
	{
		return;
	}
	foreach (i in iter->second->Attribute)
	{
		combatAttr->AddAttr((CombatAttrDef)(*i)[0], (*i)[1]);	
	}
}

void ReCalEnhanceAttr(XItem* item, Role* pRole)
{
	if (item == NULL) return;
	if (item->enhanceInfo == NULL || (item->enhanceInfo != NULL && item->enhanceInfo->level == 0))
	{
		return; ///> 没有强化等级
	}
	UINT32 level = item->enhanceInfo->level;
	EnhanceEquipMgr::Instance()->ChangeAttr(pRole, level, item);
}

void EnhanceEquipMgr::OnRoleChangeProfession(Role* pRole, bool isNotify)
{
	if (pRole == NULL)
	{
		return;
	}
	ItemCB callback = std::bind(ReCalEnhanceAttr, std::placeholders::_1, pRole);
	pRole->Get<Bag>()->ForeachEquip(callback);
	pRole->Get<Bag>()->ForeachEquipOnBody(callback);
	
	RoleSummaryHandler::GlobalHandler.OnEnhanceAllEquip(pRole, isNotify);
}

void EnhanceEquipMgr::GMEnhanceAllEquipOnBody(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	Bag* pBag = pRole->Get<Bag>();
	UINT32 destlevel = PlayerLevelManager::Instance()->GetEnhanceMaxLvl(pRole->GetLevel());
	for (UINT32 cnt = 0; cnt < EquipCount; cnt++)
	{
		XItem* pItem = pBag->GetItemOnBody(EQUIP, cnt);
		if (pItem == NULL)
		{
			continue;
		}
		UINT32 currentlevel = pItem->enhanceInfo == NULL ? 0 : pItem->enhanceInfo->level;	
		if (currentlevel >= destlevel)
		{
			continue;
		}
		if (pItem->enhanceInfo == NULL)
		{
			pItem->enhanceInfo = new EnhanceInfo;
		}
		pItem->enhanceInfo->count = 0;
		pItem->enhanceInfo->level = destlevel;
		///> 修改装备强化属性
		ChangeAttr(pRole, destlevel, pItem);
		///> 强化套装检查
		CheckEnhanceMaster(pRole, destlevel);
		///> 抛出玩家强化装备事件
		pRole->OnEnhanceEquip(cnt);
	}
	///> 强化身上装备需要重置角色属性和战斗力
	pRole->RecalcuateAttributeAndNotify(RecalAttr_Enhance);
}