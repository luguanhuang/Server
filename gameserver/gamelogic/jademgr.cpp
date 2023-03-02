#include "pch.h"
#include "jademgr.h"
#include "util/XRandom.h"
#include "bagtransition.h"
#include "item.h"
#include "pb/project.pb.h"
#include "itemconfig.h"
#include "unit/role.h"
#include "common/ptcg2c_itemchangedntf.h"
#include "ibshopmgr.h"

INSTANCE_SINGLETON(CJadeMgr);

CJadeMgr::CJadeMgr()
{
	
}

CJadeMgr::~CJadeMgr()
{

}

bool CJadeMgr::Init()
{
	return LoadFile();
}

void CJadeMgr::Uninit()
{

}

bool CJadeMgr::CheckFile()
{
	JadeTransferTable tmpTransTable;
	if (!tmpTransTable.LoadFile("table/JadeTransfer.txt"))
	{
		SSWarn<<"Load table/JadeTransfer.txt failed!"<<END;
		return false;
	}

	JadeSlotTable tmpSlotTable;
	if (!tmpSlotTable.LoadFile("table/JadeSlot.txt"))
	{
		SSWarn<<"Load table/JadeSlot.txt failed!"<<END;
		return false;
	}
	std::set<UINT32> checkSlot;
	for (auto i = tmpSlotTable.Table.begin(); i != tmpSlotTable.Table.end(); ++i)
	{
		checkSlot.insert((*i)->EquipSlot);
		auto vec = (*i)->JadeSlotAndLevel;
		if (vec.size() > 32 / 4)
		{
			LogWarn("EquipSlot [%u], JadeSlotAndLevel size > 8 in JadeSlotTable, it's too large to encode, should change the code", (*i)->EquipSlot);
			return false;
		}
		for (UINT32 j = 0; j < vec.size(); ++j)
		{
			if (j == 0) continue;
			if (vec[j][0] != 1 && vec[j][0] != 2)
			{
				LogWarn("EquipSlot [%u], JadeSlotAndLevel slot != 1 or 2 invalid in JadeSlotTable", (*i)->EquipSlot);
				return false;
			}
			if (vec[j][1] < vec[j-1][1])
			{
				LogWarn("EquipSlot [%u], JadeSlotAndLevel level invalid in JadeSlotTable", (*i)->EquipSlot);
				return false;
			}
		}
	}
	if (checkSlot.size() != 10 || *checkSlot.begin() != 0 || *(--checkSlot.end()) != 9)
	{
		LogWarn("EquipSlot should have 0-9 in JadeSlotTable");
		return false;
	}
	return true;
}

bool CJadeMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_transTable.LoadFile("table/JadeTransfer.txt");
	m_slotTable.LoadFile("table/JadeSlot.txt");

	return true;
}

void CJadeMgr::ClearFile()
{
	m_transTable.Clear();
	m_slotTable.Clear();
}

UINT32 CJadeMgr::GetSlotInfo(UINT32 level, UINT32 pos)
{
	JadeSlotTable::RowData* data = m_slotTable.GetByEquipSlot(pos);
	if (data == NULL)
	{
		return 0;
	}
	UINT32 slotInfo = 0;
	for (UINT32 i = 0; i < data->JadeSlotAndLevel.size(); ++i)
	{
		if (data->JadeSlotAndLevel[i][1] > level)
		{
			break;
		}
		slotInfo |= data->JadeSlotAndLevel[i][0] << (JADESLOTMASKCOUNT * i);
	}
	return slotInfo;
}

UINT32 CJadeMgr::GetEmptySlot(JadeInfo* info, UINT32 level, UINT32 pos)
{
	if (info == NULL)
	{
		return JADEINVALIDSLOT;
	}
	UINT32 slotInfo = GetSlotInfo(level, pos);
	bool isHaveJade[JADESLOTCOUNT] = {0};
	for(auto i = info->vecJadeSingle.begin(); i != info->vecJadeSingle.end(); ++i)
	{
		if ((*i).slotPos >= JADESLOTCOUNT)
		{
			LogError("Jade slot position:%u error >= %d", (*i).slotPos, JADESLOTCOUNT);
			continue;
		}
		isHaveJade[(*i).slotPos] = true;
	}
	UINT32 ret = JADEINVALIDSLOT;
	for(UINT32 i = 0; i < JADESLOTCOUNT; ++i)
	{
		int slotType = JADESLOTMASK & (slotInfo >> JADESLOTMASKCOUNT*i);
		if (slotType == JADESLOTNEVER)
		{
			continue;
		}
		if (slotType != JADESLOTCLOSE && !isHaveJade[i])
		{
			ret = i;
			break;
		}
	}
	return ret;
}

void CJadeMgr::UpdateJadeOnEquip(Role* pRole, XItem* equip)
{
	if (pRole == NULL || equip == NULL || equip->jadeInfo == NULL)
	{
		return;
	}

	UINT32 slotInfo = GetSlotInfo(pRole->GetLevel(), XItem::GetPos(equip->itemID));
	
	std::vector<UINT32> jadeids;
	for (auto i = equip->jadeInfo->vecJadeSingle.begin(); i != equip->jadeInfo->vecJadeSingle.end();)
	{
		JadeSingle& single = *i;
		JadeConf* pJadeConf = ItemConfig::Instance()->GetJadeConf(single.jadeid);
		if (pJadeConf == NULL)
		{
			SSWarn<<"pJadeConf is NULL, item id:"<<single.jadeid<<END;
			return;
		}
		UINT32 slotType = JADESLOTMASK & (slotInfo >> JADESLOTMASKCOUNT * single.slotPos);
		if (JADESLOTNEVER == slotType || JADESLOTCLOSE == slotType || pJadeConf->JadeEquip != slotType) //没有孔 or 孔关闭 or 类型不符合
		{
			jadeids.push_back(single.jadeid);
			i = equip->jadeInfo->vecJadeSingle.erase(i);
		}
		else
		{
			++i;
		}
	}

	if (!jadeids.empty())
	{
		BagUpdateItemTransition oUpdateTransition(pRole);
		oUpdateTransition.AddChangedItem(equip);
		oUpdateTransition.NotifyClient();

		BagGiveItemTransition stGiveTransition(pRole);
		stGiveTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_TakeOff);
		for (auto i = jadeids.begin(); i != jadeids.end(); ++i)
		{
			stGiveTransition.GiveItem(*i, 1);
		}
		stGiveTransition.NotifyClient();
	}
}

bool CJadeMgr::GetComposeInfo(UINT32 dwItemId, JadeComposeInfo& stComposeInfo)
{
	JadeConf* pJadeConf = ItemConfig::Instance()->GetJadeConf(dwItemId);
	if (NULL == pJadeConf)
	{
		return false;
	}
	stComposeInfo.Init(pJadeConf->JadeCompose[0], pJadeConf->JadeCompose[1]);
	return true;
}

bool CJadeMgr::JadeCanBeComposed(UINT32 dwItemId)//是否可以合成
{
	JadeComposeInfo stComposeInfo;
	if (!GetComposeInfo(dwItemId, stComposeInfo))
	{
		return false;
	}
	if (0 == stComposeInfo.dwCount)
	{
		return false;
	}
	return true;
}

bool CJadeMgr::ComposeJade(UINT32 dwItemId, UINT32 dwItemCount, bool bIsOnce, ItemDesc& stConsumeItem, ItemDesc& stGiveItem)
{
	JadeComposeInfo stComposeInfo;
	if (!GetComposeInfo(dwItemId, stComposeInfo))
	{
		return false;
	}
	if (0 == stComposeInfo.dwCount || dwItemCount < stComposeInfo.dwCount)
	{
		return false;
	}
	
	stConsumeItem.itemID = dwItemId;
	stGiveItem.itemID = stComposeInfo.dwNextJade;
	
	stGiveItem.itemCount = bIsOnce ? 1 : (UINT32)(dwItemCount/stComposeInfo.dwCount);
	stConsumeItem.itemCount += stGiveItem.itemCount * stComposeInfo.dwCount;

	return true;
}

KKSG::ErrorCode CJadeMgr::AttachJade(Role* pRole, XItem* pEquipItem, UINT32 dwJadePos, XItem* pJadeItem)
{
	if (NULL == pRole || NULL == pEquipItem || NULL == pJadeItem)
	{
		SSWarn<<"pointer is NULL"<<END;
		return KKSG::ERR_UNKNOWN;
	}

	if (NULL == pEquipItem->jadeInfo)
	{
		pEquipItem->jadeInfo = new JadeInfo;
	}

	UpdateJadeOnEquip(pRole, pEquipItem);

	if (dwJadePos >= JADESLOTCOUNT) //位置不合法
	{
		LogWarn("jadepos:%u is invalid, roleid:%llu", dwJadePos, pRole->GetID());
		return KKSG::ERR_UNKNOWN;
	}

	JadeConf* pJadeConf = ItemConfig::Instance()->GetJadeConf(pJadeItem->itemID);
	if (NULL == pJadeConf)
	{
		SSWarn<<"pJadeConf is NULL, item id:"<<pJadeItem->itemID<<END;
		return KKSG::ERR_UNKNOWN;
	}	

	UINT32 slotInfo = GetSlotInfo(pRole->GetLevel(), XItem::GetPos(pEquipItem->itemID));

	UINT32 dwJadeType = pJadeConf->JadeEquip;
	UINT32 slotType = JADESLOTMASK & (slotInfo >> JADESLOTMASKCOUNT * dwJadePos);
	if (JADESLOTNEVER == slotType || JADESLOTCLOSE == slotType) //没有孔 or 孔关闭
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (dwJadeType != slotType)
	{
		return KKSG::ERR_JADE_WRONGTYPE;
	}

	if (false == CanWearJade(pJadeItem->itemID, pRole->GetLevel()))
	{
		return KKSG::ERR_JADE_MINEQUIPLEVEL; //人物等级不足 无法装备龙玉
	}

	std::vector<UINT32> vecSlotCanAttach; //其他相同孔形状的位置
	for (UINT32 i = 0; i < JADESLOTCOUNT; ++i)
	{
		UINT32 slotType = JADESLOTMASK & (slotInfo >> JADESLOTMASKCOUNT*i);
		if (JADESLOTNEVER == slotType)
		{
			continue;
		}
		if (dwJadeType == slotType)
		{
			vecSlotCanAttach.push_back(i);
		}
	}

	UINT32 dwFirstAttrId = (0 == pJadeItem->changeAttr.size()) ? 0 : pJadeItem->changeAttr[0].AttrID;

	UINT32 sameTypePos = (UINT32)(-1);
	for (auto i = vecSlotCanAttach.begin(); i != vecSlotCanAttach.end(); ++i)
	{
		auto k = std::find_if(pEquipItem->jadeInfo->vecJadeSingle.begin(), pEquipItem->jadeInfo->vecJadeSingle.end(), EqualJadeSlot(*i));
		if (k != pEquipItem->jadeInfo->vecJadeSingle.end())
		{
			///> 孔上有龙玉
			UINT32 dwAttrId = 0;
			if (0 != k->attrs.size())
			{
				dwAttrId = k->attrs[0].AttrID;
			}
			if (dwFirstAttrId == dwAttrId)
			{
				sameTypePos = *i;
				break;
			}
		}
	}

	if ((UINT32)(-1) != sameTypePos && sameTypePos != dwJadePos)
	{
		return KKSG::ERR_JADE_SAME_TYPE; //相同类型的龙玉不在这个pos
	}

	auto theOne = std::find_if(pEquipItem->jadeInfo->vecJadeSingle.begin(), pEquipItem->jadeInfo->vecJadeSingle.end(), EqualJadeSlot(dwJadePos));
	UINT32 preItemid = 0;
	if (theOne != pEquipItem->jadeInfo->vecJadeSingle.end())
	{
		preItemid = theOne->jadeid;
	}

	//打包龙玉信息
	JadeSingle stJadeSingle;
	stJadeSingle.jadeid = pJadeItem->itemID;
	stJadeSingle.attrs = pJadeItem->changeAttr;
	stJadeSingle.slotPos = dwJadePos;

	//扣掉龙玉
	BagTakeItemTransition stTakeTransition(pRole);
	stTakeTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_Equip);
	stTakeTransition.TakeItem(pJadeItem->itemID, 1);
	stTakeTransition.NotifyClient();	

	//镶嵌龙玉
	BagChangeItemAttrTransition stChangeAttrTransiton(pRole);
	stChangeAttrTransiton.ChangeItemJade(pEquipItem, stJadeSingle);
	stChangeAttrTransiton.NotifyClient();
	
	std::vector<ItemDesc> vlist;
	XItem::PrintEquipTLog(pRole, pEquipItem, TX_Item_JadeEquip, vlist, pJadeItem->itemID);
	if (preItemid)
	{
		BagGiveItemTransition stGiveTransition(pRole);
		stGiveTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_Equip_Replace);
		stGiveTransition.GiveItem(preItemid, 1);
		stGiveTransition.NotifyClient();

		std::vector<ItemDesc> vlist;
		XItem::PrintEquipTLog(pRole, pEquipItem, TX_Item_JadeOff, vlist, preItemid);
		return KKSG::ERR_JADE_REPLACE;
	}
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CJadeMgr::DetachJade(Role* pRole, XItem* pEquipItem, UINT32 dwJadePos)
{
	if (NULL == pRole || NULL == pEquipItem || NULL == pEquipItem->jadeInfo)
	{
		SSWarn<<"pointer is NULL"<<END;
		return KKSG::ERR_UNKNOWN;
	}

	UpdateJadeOnEquip(pRole, pEquipItem);

	auto i = std::find_if(pEquipItem->jadeInfo->vecJadeSingle.begin(), pEquipItem->jadeInfo->vecJadeSingle.end(), EqualJadeSlot(dwJadePos));
	if (i == pEquipItem->jadeInfo->vecJadeSingle.end())
	{
		return KKSG::ERR_UNKNOWN;
	}

	UINT32 dwJadeItemId = (*i).jadeid;
	std::vector<ItemDesc> desc;
	desc.push_back(ItemDesc(dwJadeItemId, 1));
	if(!pRole->Get<Bag>()->CanAddItem(&desc))
	{
		std::vector<XItem*> items;
		pRole->Get<Bag>()->FindItemsInBag(dwJadeItemId, items, Bind_Is);
		UINT32 overlap = ItemConfig::Instance()->GetOverlap(dwJadeItemId);
		if(0 != overlap)
		{
			bool canAdd = false;
			for(UINT32 i = 0; i < items.size(); i++)
			{
				if(items[i]->itemCount < overlap)
				{
					canAdd = true;
					break;
				}
			}
			if(!canAdd)
			{
				return KKSG::ERR_BAG_FULL_TAKEOFF_JADE;
			}
		}	
	}

	BagChangeItemAttrTransition stChangeAttrTransiton(pRole);
	stChangeAttrTransiton.ChangeItemJade(pEquipItem, *i, true);
	stChangeAttrTransiton.NotifyClient();

	///> 构造新的龙玉给玩家
	BagGiveItemTransition stGiveTransition(pRole);
	stGiveTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_TakeOff);
	stGiveTransition.GiveItem(dwJadeItemId, 1);
	stGiveTransition.NotifyClient();

	std::vector<ItemDesc> vlist;
	XItem::PrintEquipTLog(pRole, pEquipItem, TX_Item_JadeOff, vlist, dwJadeItemId);
	return KKSG::ERR_SUCCESS;
}

void CJadeMgr::GetAllJade(XItem* item, std::map<UINT32, UINT32>& items)
{
	if (NULL == item || NULL == item->jadeInfo)
	{
		return;
	}
	for (auto i = item->jadeInfo->vecJadeSingle.begin(); i != item->jadeInfo->vecJadeSingle.end(); ++i)
	{
		auto j = items.find(i->jadeid);
		if (j == items.end())
		{
			items[i->jadeid] = 1;
		}
		else
		{
			j->second += 1;
		}
	}	
	return;
}

UINT32 CJadeMgr::GetLevel(UINT32 itemid)
{
	JadeConf* conf = ItemConfig::Instance()->GetJadeConf(itemid);
	if (NULL == conf)
	{
		return 0;
	}
	return conf->JadeLevel;
}

void CJadeMgr::SwapJadeOnEquip(Role* pRole, XItem* origin, XItem* dest, bool isNotify)
{
	if (origin->jadeInfo == NULL)
	{
		return;
	}

	if (dest->jadeInfo == NULL)
	{
		dest->jadeInfo = new JadeInfo;
	}

	std::vector<JadeSingle>& jadeDest = dest->jadeInfo->vecJadeSingle;
	std::vector<JadeSingle>& jadeOrigin = origin->jadeInfo->vecJadeSingle;

	BagGiveItemTransition oGiveTransition(pRole);
	oGiveTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_TakeOff);
	for (auto i = jadeDest.begin(); i != jadeDest.end(); ++i) //dest上原来的龙玉放入背包
	{
		oGiveTransition.GiveItem((*i).jadeid, 1);
	}
	oGiveTransition.NotifyClient();

	jadeDest = jadeOrigin;
	jadeOrigin.clear();

	/*bool isUsedPos[JADESLOTCOUNT] = {0};
	for (auto i = jadeOrigin.begin(); i != jadeOrigin.end();)
	{
		JadeConf* pJadeConf = ItemConfig::Instance()->GetJadeConf((*i).jadeid);
		if (pJadeConf == NULL)
		{
			LogWarn("jade [%u] is not find in Jade.txt", (*i).jadeid);
			++i;
			continue;
		}
		if (!CanWearJade((*i).jadeid, pRole->GetLevel()))
		{
			++i;
			continue;
		}
		UINT32 slot = -1;
		for (UINT32 j = 0; j < JADESLOTCOUNT; ++j)
		{
			UINT32 slotType = JADESLOTMASK & ((dest->jadeInfo->slotInfo) >> JADESLOTMASKCOUNT * j);
			if (JADESLOTNEVER == slotType || JADESLOTCLOSE == slotType)
			{
				continue;
			}
			if (slotType == pJadeConf->JadeEquip && !isUsedPos[j])
			{
				slot = j;	
				break;
			}
		}
		if (slot != -1)
		{
			jadeDest.push_back(*i);
			jadeDest.back().slotPos = slot;
			isUsedPos[slot] = true;
			i = jadeOrigin.erase(i);
		}
		else
		{
			++i;
		}
	}*/

	if (isNotify)
	{
		BagUpdateItemTransition oUpdateTransition(pRole);
		oUpdateTransition.AddChangedItem(origin);
		oUpdateTransition.AddChangedItem(dest);
		oUpdateTransition.NotifyClient();
	}
}

void CJadeMgr::UpgradeJadeOnEquip(Role* pRole, XItem* equip, EquipConf* pEquipConf)
{
	/*if (pRole == NULL || equip == NULL || pEquipConf == NULL)
	{
		return;
	}
	if (equip->jadeInfo == NULL)
	{
		return;
	}
	std::vector<JadeSingle> jadeOrigin = equip->jadeInfo->vecJadeSingle;	

	equip->jadeInfo->vecJadeSingle.clear();

	std::vector<JadeSingle>& jadeDest = equip->jadeInfo->vecJadeSingle;

	bool isUsedPos[JADESLOTCOUNT] = {0};
	for (auto i = jadeOrigin.begin(); i != jadeOrigin.end();)
	{
		JadeConf* pJadeConf = ItemConfig::Instance()->GetJadeConf((*i).jadeid);
		if (pJadeConf == NULL)
		{
			LogWarn("jade [%u] is not find in Jade.txt", (*i).jadeid);
			++i;
			continue;
		}
		if (!CanWearJade((*i).jadeid, pRole->GetLevel()))
		{
			++i;
			continue;
		}
		UINT32 slot = -1;
		for (UINT32 j = 0; j < JADESLOTCOUNT; ++j)
		{
			UINT32 slotType = JADESLOTMASK & ((equip->jadeInfo->slotInfo) >> JADESLOTMASKCOUNT * j);
			if (JADESLOTNEVER == slotType || JADESLOTCLOSE == slotType)
			{
				continue;
			}
			if (slotType == pJadeConf->JadeEquip && !isUsedPos[j])
			{
				slot = j;
				break;
			}
		}
		if (slot != -1)
		{
			jadeDest.push_back(*i);
			jadeDest.back().slotPos = slot;
			isUsedPos[slot] = true;
			i = jadeOrigin.erase(i);
		}
		else
		{
			++i;
		}
	}

	if (!jadeOrigin.empty())
	{
		BagGiveItemTransition oGiveTransition(pRole);
		oGiveTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_TakeOff);
		for (auto i = jadeOrigin.begin(); i != jadeOrigin.end(); ++i) //不能放到新装备上的放入背包
		{
			oGiveTransition.GiveItem((*i).jadeid, 1);
		}
		oGiveTransition.NotifyClient();
	}*/
}

bool CJadeMgr::CanWearJade(UINT32 dwJadeID, UINT32 level)
{
	JadeConf* pJadeConf = ItemConfig::Instance()->GetJadeConf(dwJadeID);
	if (pJadeConf == NULL)
	{
		LogWarn("Can't find this JadeID:%u in JadeList", dwJadeID);
		return false;
	}
	if (GetGlobalConfig().JadeMosaicLevel.size() == 0)
	{
		LogWarn("JadeMosaicLevel in GlobalConfig size is 0");
		return false;
	}

	UINT32 JadeLevel = pJadeConf->JadeLevel;
	if (JadeLevel > GetGlobalConfig().JadeMosaicLevel.size())
	{
		JadeLevel = GetGlobalConfig().JadeMosaicLevel.size();
	}
	if (JadeLevel == 0) JadeLevel = 1;

	UINT32 minLevel = GetGlobalConfig().JadeMosaicLevel[JadeLevel - 1];

	return minLevel <= level; //龙玉镶嵌装备最小等级要 <= 人物才能装备上
}

UINT32 CJadeMgr::GetTransId(UINT32 itemid, UINT32 from, UINT32 to)
{
	JadeTransferTable::RowData* rowdata = m_transTable.GetByJadeID(itemid);
	if (NULL == rowdata)
	{
		return 0;
	}
	UINT32 size = rowdata->TransferID.size();
	if (UINT32(from -1) >= size || (UINT32)(to - 1) >= size)
	{
		return 0;
	}
	if (0 == rowdata->TransferID[from - 1] && rowdata->TransferID[to - 1] > 0)
	{
		return rowdata->TransferID[to - 1];
	}
	return 0;
}

KKSG::ErrorCode CJadeMgr::GetNeedItemsComposeJade(Role* pRole, UINT32 itemid, UINT32 addLevel, bool isBody, UINT32& destid, std::map<UINT32, UINT32>& costItems)
{
	if (pRole == NULL || addLevel == 0)
	{
		return KKSG::ERR_UNKNOWN;
	}
	UINT32 preItemid = itemid;
	for (UINT32 i = 0; i < addLevel; ++i)
	{
		JadeConf* conf = ItemConfig::Instance()->GetJadeConf(preItemid);
		if (conf == NULL)
		{
			LogWarn("preItemid [%u] not find in Jade.txt", preItemid);
			return KKSG::ERR_JADE_MAXLEVEL;
		}
		if (i != addLevel - 1)
		{
			preItemid = conf->JadeCompose[1];
		}
		else
		{
			destid = conf->JadeCompose[1];
		}
	}
	UINT32 lackCount = 1;
	std::pair<UINT32, UINT32> needBuyJade;
	JadeConf* conf = ItemConfig::Instance()->GetJadeConfByParentItemID(destid);
	while (conf != NULL)
	{
		UINT32 haveCount = pRole->Get<Bag>()->CountItemInBag(conf->JadeID);
		if (conf->JadeLevel == 0)
		{
			LogWarn("jade level is 0, bullshit!");
			return KKSG::ERR_FAILED;
		}
		UINT32 pos = conf->JadeLevel < GetGlobalConfig().JadeLevelUpCost.size() ? conf->JadeLevel : GetGlobalConfig().JadeLevelUpCost.size() - 1;
		costItems[GOLD] += GetGlobalConfig().JadeLevelUpCost[pos] * lackCount;
		lackCount *= conf->JadeCompose[0];
		if (isBody && itemid == conf->JadeID && lackCount > 0) --lackCount;
		if (haveCount >= lackCount)
		{
			costItems[conf->JadeID] += lackCount;
			return KKSG::ERR_SUCCESS;
		}
		if (haveCount != 0)
		{
			costItems[conf->JadeID] += haveCount;
			lackCount -= haveCount;
		}

		needBuyJade = std::make_pair(conf->JadeID, lackCount);

		conf = ItemConfig::Instance()->GetJadeConfByParentItemID(conf->JadeID);
	}

	const std::vector<IBShop::RowData*>* vecIBConf = IBShopMgr::Instance()->GetConfsByItemID(needBuyJade.first);
	if (vecIBConf == NULL)
	{
		return KKSG::ERR_JADE_COUNTNOTENOUGH;
	}
	for (auto i = vecIBConf->begin(); i != vecIBConf->end(); ++i)
	{
		if ((*i)->buycount == 0 && (*i)->currencytype == DRAGON_COIN)
		{
			costItems[DRAGON_COIN] += (*i)->currencycount * needBuyJade.second;
			return KKSG::ERR_SUCCESS;
		}
	}
	return KKSG::ERR_JADE_COUNTNOTENOUGH;
}