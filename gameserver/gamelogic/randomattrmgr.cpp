#include "pch.h"
#include "randomattrmgr.h"
#include "util/XRandom.h"
#include "itemsuffix.h"
#include "item.h"
#include "itemconfig.h"
#include "unit/role.h"
#include "bagtransition.h"
#include "tablemgr/XEntityInfoLibrary.h"

#define EPS 1e-6

std::pair<UINT32, UINT32> RandAttrAll::GetInitRandomAttr(RandAttrSingle** nowSingle)
{
	if (allprob == 0)
	{
		LogError("it should not happen, allprob = 0, already avoid it in check table data");
		return std::make_pair(0, 0);
	}
	UINT32 random = XRandom::randInt(1, allprob + 1), prob = 0;
	for (auto i = singles.begin(); i != singles.end(); ++i)
	{
		prob += i->second.prob;
		if (prob >= random)
		{
			if (nowSingle != NULL)
			{
				*nowSingle = &i->second;
			}
			UINT32 attr = XRandom::randInt(i->second.initAttr[0], i->second.initAttr[1] + 1);
			return std::make_pair(i->first, attr);
		}
	}
	LogError("it should not happen, already avoid it in check table data");
	return std::make_pair(0, 0);
}

std::pair<UINT32, UINT32> RandAttrAll::GetForgeRandomAttr(RandAttrSingle** nowSingle, std::unordered_set<UINT32>& haveAttrs)
{
	if (allprob == 0)
	{
		LogError("it should not happen, allprob = 0, already avoid it in check table data");
		return std::make_pair(0, 0);
	}
	UINT32 probSum = 0;
	std::vector<std::pair<const UINT32, RandAttrSingle>*> tmpSingleVec;
	for (auto i = singles.begin(); i != singles.end(); ++i)
	{
		UINT32 attr = i->first;
		if (i->first == BASIC_MagicAtk)
		{
			attr = BASIC_PhysicalAtk;
		}
		if (haveAttrs.find(attr) == haveAttrs.end())
		{
			probSum += i->second.prob;
			tmpSingleVec.push_back(&*i);
		}
	}
	if (probSum == 0)
	{
		haveAttrs.clear();
		return GetInitRandomAttr(nowSingle);
	}
	UINT32 random = XRandom::randInt(1, probSum + 1), prob = 0;
	for (auto i = tmpSingleVec.begin(); i != tmpSingleVec.end(); ++i)
	{
		prob += (*i)->second.prob;
		if (prob >= random)
		{
			if (nowSingle != NULL)
			{
				*nowSingle = &(*i)->second;
			}
			UINT32 attr = XRandom::randInt((*i)->second.initAttr[0], (*i)->second.initAttr[1] + 1);
			return std::make_pair((*i)->first, attr);
		}
	}
	LogError("it should not happen, already avoid it in check table data");
	return std::make_pair(0, 0);
}

template<typename T, typename M>
static bool HandlerTableData(const T& table, M& mapAttr, const std::string& tableName)
{
	for (auto i = table.Table.begin(); i != table.Table.end(); ++i)
	{
		RandAttrAll& all = mapAttr[(*i)->EquipID][(*i)->Slot - 1];
		if (all.singles.find((*i)->AttrID) != all.singles.end())
		{
			LogWarn("equip:%u, has more than 1 line, same slot:%u and attr:%u in %s", (*i)->EquipID, (*i)->Slot, (*i)->AttrID, tableName.c_str());
			return false;
		}
		all.allprob += (*i)->Prob;

		RandAttrSingle& single = all.singles[(*i)->AttrID];
		single.prob = (*i)->Prob;

		single.initAttr = (*i)->InitialRange;
		single.attrRange = (*i)->Range;
		single.smeltExpectation = (*i)->SmeltExpectation;

		if (single.initAttr[0] > single.initAttr[1]) std::swap(single.initAttr[0], single.initAttr[1]);
		if (single.attrRange[0] > single.attrRange[1]) std::swap(single.attrRange[0], single.attrRange[1]);

		if (single.initAttr[0] < single.attrRange[0] || single.initAttr[0] > single.attrRange[1]
		|| single.initAttr[1] < single.attrRange[0] || single.initAttr[1] > single.attrRange[1])
		{
			LogWarn("equip:%u, slot:%u, attr:%u, InitialRange not in Range in %s", (*i)->EquipID, (*i)->Slot, (*i)->AttrID, tableName.c_str());
			return false;
		}

		memset(single.smeltAllProb, 0, sizeof(single.smeltAllProb));
		for (UINT32 j = 0; j < 3; ++j)
		{
			std::vector<Sequence<int, 3>>* smeltAttr;
			if (j == 0) smeltAttr = &(*i)->SmeltAttr1;
			else if (j == 1) smeltAttr = &(*i)->SmeltAttr2;
			else smeltAttr = &(*i)->SmeltAttr3;

			for (auto k = smeltAttr->begin(); k != smeltAttr->end(); ++k)
			{
				if ((*k)[2] <= 0)
				{
					LogWarn("equip:%u, slot:%u, attr:%u, SmeltAttr%u wrong, one prob <= zero in %s", (*i)->EquipID, (*i)->Slot, (*i)->AttrID, j + 1, tableName.c_str());
					return false;
				}
				single.smeltAllProb[j] += (*k)[2];
				single.smeltAttr[j][single.smeltAllProb[j]] = (*k)[0] <= (*k)[1] ? std::make_pair((*k)[0], (*k)[1] - (*k)[0]) : std::make_pair((*k)[1], (*k)[0] - (*k)[1]);
			}
		}
	}

	for (auto i = mapAttr.begin(); i != mapAttr.end(); ++i)
	{
		UINT32 x = 0, size = i->second.size();
		for (auto j = i->second.begin(); j != i->second.end(); ++j, x++)
		{
			if (j->first != x)
			{
				LogWarn("equip:%u, slot error, not continuous in %s", i->first, tableName.c_str());
				return false;
			}
			if (x != size - 1)
			{
				if (j->second.singles.find(0) != j->second.singles.end())
				{
					LogWarn("equip:%u, slot:%u, not the final slot, but one attrid is 0 in %s", i->first, x + 1, tableName.c_str());
					return false;
				}
			}
			if (j->second.allprob <= 0)
			{
				LogWarn("equip:%u, slot:%u, all prob sum <= zero in %s", i->first, x + 1, tableName.c_str());
				return false;
			}
		}
	}

	return true;
}

RandomAttrMgr::RandomAttrMgr()
{

}

RandomAttrMgr::~RandomAttrMgr()
{

}

bool RandomAttrMgr::Init()
{
	return LoadFile();
}

void RandomAttrMgr::Uninit()
{

}

bool RandomAttrMgr::CheckFile()
{
	RandomAttributes tmpRandAttr;
	if (!tmpRandAttr.LoadFile("table/RandomAttributes.txt"))
	{
		LogWarn("Load RandomAttributes.txt failed");
		return false;
	}

	ForgeAttributes tmpForgeAttr;
	if (!tmpForgeAttr.LoadFile("table/ForgeAttributes.txt"))
	{
		LogWarn("Load ForgeAttributes.txt failed");
		return false;
	}

	std::unordered_map<UINT32, std::map<UINT32, RandAttrAll>> tmpMapRandAttr;
	if (!HandlerTableData(tmpRandAttr, tmpMapRandAttr, "RandomAttributes.txt"))
	{
		return false;
	}

	std::unordered_map<UINT32, std::map<UINT32, RandAttrAll>> tmpMapForgeAttr;
	if (!HandlerTableData(tmpForgeAttr, tmpMapForgeAttr, "ForgeAttributes.txt"))
	{
		return false;
	}

	ForgeTransAttr tmpForgeTransAttr;
	if (!tmpForgeTransAttr.LoadFile("table/ForgeTransAttr.txt"))
	{
		LogWarn("Load ForgeTransAttr.txt failed");
		return false;
	}
	// guarantee all AttrID size is equal
	int size = -1;
	for (auto i = tmpForgeTransAttr.Table.begin(); i != tmpForgeTransAttr.Table.end(); ++i)
	{
		if (size == -1)
		{
			size = (*i)->AttrID.size();
		}
		else if (size != (*i)->AttrID.size())
		{
			LogWarn("ForgeTransAttr.txt error, AttrID size not equal");
			return false;
		}
	}
	
	return true;
}

bool RandomAttrMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oRandTable.LoadFile("table/RandomAttributes.txt");
	m_oForgeTable.LoadFile("table/ForgeAttributes.txt");

	HandlerTableData(m_oRandTable, m_mapRandAttr, "RandomAttributes.txt");
	HandlerTableData(m_oForgeTable, m_mapForgeAttr, "ForgeAttributes.txt");

	m_oForgeTransAttr.LoadFile("table/ForgeTransAttr.txt");
		
	return true;
}

void RandomAttrMgr::ClearFile()
{
	m_oRandTable.Clear();
	m_oForgeTable.Clear();

	m_mapRandAttr.clear();
	m_mapForgeAttr.clear();

	m_oForgeTransAttr.Clear();
}

const RandAttrSingle* RandomAttrMgr::GetRandomAttrSingle(UINT32 itemID, UINT32 slot, UINT32 attrID)
{
	auto iter = m_mapRandAttr.find(itemID);
	if (iter == m_mapRandAttr.end())
	{
		return NULL;
	}
	auto iter2 = iter->second.find(slot);
	if (iter2 == iter->second.end())
	{
		return NULL;
	}
	auto iter3 = iter2->second.singles.find(attrID);
	if (iter3 == iter2->second.singles.end())
	{
		return NULL;
	}
	return &iter3->second;
}

const RandAttrSingle* RandomAttrMgr::GetForgeAttrSingle(UINT32 itemID, UINT32 slot, UINT32 attrID)
{
	auto iter = m_mapForgeAttr.find(itemID);
	if (iter == m_mapForgeAttr.end())
	{
		return NULL;
	}
	auto iter2 = iter->second.find(slot);
	if (iter2 == iter->second.end())
	{
		return NULL;
	}
	auto iter3 = iter2->second.singles.find(attrID);
	if (iter3 == iter2->second.singles.end())
	{
		return NULL;
	}
	return &iter3->second;
}

void RandomAttrMgr::InitRandomAttr(XItem* pItem)
{
	if (pItem == NULL)
	{
		return;
	}
	auto i = m_mapRandAttr.find(pItem->itemID);
	if (i == m_mapRandAttr.end())
	{
		return;
	}
	if (pItem->randAttr == NULL)
	{
		pItem->randAttr = new RandomAttr;
	}
	pItem->randAttr->attrs.clear();
	for (auto j = i->second.begin(); j != i->second.end(); ++j)
	{
		std::pair<UINT32, UINT32> attr = j->second.GetInitRandomAttr();
		pItem->randAttr->attrs.push_back(ItemChangeAttr(attr.first, attr.second));
	}
}

KKSG::ErrorCode RandomAttrMgr::SmeltAttr(XItem* pItem, UINT32 slot, bool isForge, Role* pRole, UINT32& newAttrValue, std::vector<ItemDesc>& vlist)
{
	if (pItem == NULL || pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	const ItemConf* pItemConf = ItemConfig::Instance()->GetItemConf(pItem->itemID);
	if (pItemConf == NULL)
	{
		LogWarn("itemid:%u not find in ItemList", pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}
	if (pItemConf->ReqLevel < GetGlobalConfig().GetInt("SmeltEquipMinLevel", 32))
	{
		return KKSG::ERR_SMELT_MINLEVEL;
	}
	const EquipConf* pEquipConf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (pEquipConf == NULL)
	{
		LogError("itemid [%u] not find in EquipList.txt, change the table?", pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}
	if (!pEquipConf->IsCanSmelt)
	{
		return KKSG::ERR_FAILED;
	}
	if ((int)slot < 0)	///> 防 + 1 溢出
	{
		return KKSG::ERR_UNKNOWN;
	}

	const RandAttrSingle* pSingle = NULL;
	ItemChangeAttr* pAttr;
	if (!isForge)
	{
		if (pItem->randAttr == NULL || pItem->randAttr->attrs.size() < slot + 1)
		{
			return KKSG::ERR_UNKNOWN;
		}
		pAttr = &pItem->randAttr->attrs[slot];
		pSingle = GetRandomAttrSingle(pItem->itemID, slot, pAttr->AttrID);
	}
	else
	{
		if (pItem->forge == NULL || pItem->forge->attrs.size() < slot + 1)
		{
			return KKSG::ERR_UNKNOWN;
		}
		pAttr = &pItem->forge->attrs[slot];
		pSingle = GetForgeAttrSingle(pItem->itemID, slot, pAttr->AttrID);
	}
	if (pSingle == NULL)
	{
		LogError("item [%llu], itemid [%u], slot [%u], isforge:%d, attrid [%u] not in table, change the table?", pItem->uid, pItem->itemID, slot, isForge, pAttr->AttrID);
		return KKSG::ERR_UNKNOWN;
	}

	ItemChangeAttr& attr = *pAttr;
	const RandAttrSingle& single = *pSingle;

	///> 纠正改表之后属性差
	if (attr.AttrValue < single.attrRange[0]) attr.AttrValue = single.attrRange[0];
	if (attr.AttrValue > single.attrRange[1]) attr.AttrValue = single.attrRange[1];

	if (attr.AttrValue >= single.attrRange[1])
	{
		return KKSG::ERR_SMELT_MAXVALUE;
	}

	UINT32 pos = _GetSmeltAttrPos(attr.AttrValue, single.attrRange[0], single.attrRange[1]);
	if (single.smeltAttr[pos].empty())
	{
		LogWarn("item [%llu], itemid[%u], slot [%u], isforge:%d, attrid[%u], SmeltAttr%u is empty", pItem->uid, pItem->itemID, slot, isForge, attr.AttrID, pos + 1);
		return KKSG::ERR_FAILED;
	}

	UINT32 random = XRandom::randInt(1, single.smeltAllProb[pos] + 1);
	auto smeltIter = single.smeltAttr[pos].lower_bound(random);
	if (smeltIter == single.smeltAttr[pos].end())
	{
		LogError("it should be guarantee in handler table data");
		return KKSG::ERR_UNKNOWN;
	}

	BagTakeItemTransition oTakeTransition(pRole);
	oTakeTransition.SetReason(ItemFlow_Smelt, ItemFlow_SmeltEquip);
	if(!oTakeTransition.TakeItem(GOLD, pEquipConf->SmeltNeedMoney))
	{
		oTakeTransition.RollBack();
		return KKSG::ERR_SMELTING_LACKMONEY;
	}
	for(auto i = pEquipConf->SmeltNeedItem.begin(); i != pEquipConf->SmeltNeedItem.end(); ++i)
	{
		UINT32 num = (*i)[1];
		std::vector<UINT32> replaceItems;
		_GetCanReplaceEquipSmeltItem((*i)[0], replaceItems);
		for (auto j = replaceItems.begin(); j != replaceItems.end() && num > 0; ++j)
		{
			UINT32 haveNum = pRole->Get<Bag>()->CountItemInBag(*j);
			if (haveNum > 0)
			{
				haveNum = std::min(haveNum, num);
				oTakeTransition.TakeItem(*j, haveNum);
				num -= haveNum;

				vlist.push_back(ItemDesc(*j, haveNum));
			}
		}
		if (num > 0)
		{
			oTakeTransition.RollBack();
			return KKSG::ERR_SMELTING_LACKMONEY;
		}
	}
	oTakeTransition.NotifyClient();

	UINT32 range = XRandom::randInt(0, smeltIter->second.second + 1);
	int delta = smeltIter->second.first + (int)range;
	if (delta > 0)
	{
		attr.AttrValue += (UINT32)delta;
		if (attr.AttrValue > single.attrRange[1]) attr.AttrValue = single.attrRange[1];	
		newAttrValue = attr.AttrValue;
	}
	else
	{
		newAttrValue = (UINT32)((int)attr.AttrValue + delta);
		if (newAttrValue < single.attrRange[0]) newAttrValue = single.attrRange[0];
	}

	///> 洗炼对于未替换锻造的锻造属性要提升
	if (isForge && pItem->forge->unReplacedAttr.AttrID != 0)
	{
		const RandAttrSingle* repSingle = GetForgeAttrSingle(pItem->itemID, slot, pItem->forge->unReplacedAttr.AttrID);
		if (repSingle != NULL)
		{
			double percent = _GetPercent(attr.AttrValue, single.attrRange[0], single.attrRange[1]);
			percent *= GetGlobalConfig().ForgeSmeltTransRate / 100.0;
			pItem->forge->unReplacedAttr.AttrValue = std::max(pItem->forge->unReplacedAttr.AttrValue,
				repSingle->attrRange[0] + (UINT32)(percent * (repSingle->attrRange[1] - repSingle->attrRange[0]) + EPS));
		}
	}

	///> 增加洗炼总次数
	++pItem->smeltCount;

	BagUpdateItemTransition oChangeTransition(pRole);
	oChangeTransition.AddChangedItem(pItem);
	oChangeTransition.NotifyClient();

	pRole->RecalcuateAttributeAndNotify(RecalAttr_RandomAttr);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode RandomAttrMgr::ForgeEquip(XItem* pItem, bool isUsedStone, Role* pRole, std::vector<ItemDesc>& vlist)
{
	if (pItem == NULL || pRole == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	EquipConf* conf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (conf == NULL)
	{
		LogError("itemid [%u] not find in EquipList.txt, change the table?", pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}
	auto iter = m_mapForgeAttr.find(pItem->itemID);
	if (iter == m_mapForgeAttr.end())
	{
		return KKSG::ERR_EQUIP_CANTFORGE;
	}

	std::vector<Sequence<UINT32, 2>> needItem;
	Sequence<UINT32, 2> specialItem;
	UINT32 lowRate;
	UINT32 highRate;
	if (pItem->forge != NULL && !pItem->forge->attrs.empty())
	{
		needItem = conf->ForgeNeedItemAfter;
		specialItem = conf->ForgeSpecialItemAfter;
		lowRate = conf->ForgeLowRateAfter;
		highRate = conf->ForgeHighRateAfter;
	}
	else
	{
		needItem = conf->ForgeNeedItem;
		specialItem = conf->ForgeSpecialItem;
		lowRate = conf->ForgeLowRate;
		highRate = conf->ForgeHighRate;
	}	

	BagTakeItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Forge, ItemFlow_Forge_Take);
	for (UINT32 i = 0; i < needItem.size(); ++i)
	{
		if (!transition.TakeItem(needItem[i][0], needItem[i][1]))
		{
			transition.RollBack();
			return KKSG::ERR_EQUIP_FORGE_LACK_ITEM;
		}
		vlist.push_back(ItemDesc(needItem[i][0], needItem[i][1]));
	}

	UINT32 random = XRandom::randInt(1, 100 + 1);
	bool isSucceed = false;
	if (isUsedStone)
	{
		if (!transition.TakeItem(specialItem[0], specialItem[1]))
		{
			transition.RollBack();
			return KKSG::ERR_EQUIP_FORGE_LACK_STONE;
		}
		if (random <= highRate)
		{
			isSucceed = true;
		}
		vlist.push_back(ItemDesc(specialItem[0], specialItem[1]));
	}
	else
	{
		if (random <= lowRate)
		{
			isSucceed = true;
		}
	}
	transition.NotifyClient();

	///> 锻造失败
	if (isSucceed == false)
	{
		return KKSG::ERR_EQUIP_FORGE_FAILED;
	}

	///> 目前只有一条锻造属性，所以就取位置1的。预处理数据保证了map一定非空
	if (iter->second.empty())
	{
		LogError("map is empty, it should be guarantee in handler table data");
		return KKSG::ERR_UNKNOWN;
	}
	
	if (pItem->forge == NULL)
	{
		pItem->forge = new ForgeAttr;
	}

	RandAttrAll& all = iter->second.begin()->second;
	RandAttrSingle* single = NULL;

	UINT32 preAttrID = pItem->forge->attrs.empty() ? 0 : pItem->forge->attrs[0].AttrID;
	const ProfessionTable::RowData* profConf = XEntityInfoLibrary::Instance()->TemplateID2Data_Role(pRole->GetProfession());
	if (profConf == NULL)
	{
		LogError("role [%llu], Profession [%d] not find in Profession.txt when ForgeEquip", pRole->GetID(), pRole->GetProfession());
	}
	std::pair<UINT32, UINT32> attr;
	attr = all.GetForgeRandomAttr(&single, pItem->forge->haveAttrs);
	if (profConf != NULL)
	{
		if (profConf->AttackType == 2 && attr.first == BASIC_PhysicalAtk)
		{
			attr.first = BASIC_MagicAtk;
		}
		if (profConf->AttackType == 1 && attr.first == BASIC_MagicAtk)
		{
			attr.first = BASIC_PhysicalAtk;
		}
	}
	///> 每一轮不能出重复的属性
	if (attr.first == BASIC_MagicAtk)
	{
		pItem->forge->haveAttrs.insert(BASIC_PhysicalAtk);
	}
	else
	{
		pItem->forge->haveAttrs.insert(attr.first);
	}

	bool canPushBack = true;
	if (single != NULL)
	{
		if (!pItem->forge->attrs.empty())
		{
			const RandAttrSingle* preSingle = GetForgeAttrSingle(pItem->itemID, 0, pItem->forge->attrs.begin()->AttrID);
			if (preSingle != NULL)
			{
				double percent = _GetPercent(pItem->forge->attrs.begin()->AttrValue, preSingle->attrRange[0], preSingle->attrRange[1]);
				percent *= GetGlobalConfig().ForgeSmeltTransRate / 100.0;
				pItem->forge->unReplacedAttr.AttrID = attr.first;
				pItem->forge->unReplacedAttr.AttrValue = std::max(attr.second,
										               single->attrRange[0] + (UINT32)(percent * (single->attrRange[1] - single->attrRange[0]) + EPS));
				canPushBack = false;
			}
		}
	}
	if (canPushBack)
	{
		pItem->forge->attrs.clear();
		pItem->forge->attrs.push_back(ItemChangeAttr(attr.first, attr.second));
	}

	BagUpdateItemTransition updateTran(pRole);
	updateTran.AddChangedItem(pItem);
	updateTran.NotifyClient();

	pRole->RecalcuateAttributeAndNotify(RecalAttr_RandomAttr);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode RandomAttrMgr::ForgeReplace(XItem* pItem, Role* pRole)
{
	if (pItem == NULL || pRole == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	if (pItem->forge == NULL || pItem->forge->unReplacedAttr.AttrID == 0)
	{
		return KKSG::ERR_FORGE_NOREPLACE;
	}
	ForgeAttr* forge = pItem->forge;
	forge->attrs.clear();
	forge->attrs.push_back(ItemChangeAttr(forge->unReplacedAttr.AttrID, forge->unReplacedAttr.AttrValue));
	forge->unReplacedAttr.AttrID = forge->unReplacedAttr.AttrValue = 0;

	BagUpdateItemTransition updateTran(pRole);
	updateTran.AddChangedItem(pItem);
	updateTran.NotifyClient();

	pRole->RecalcuateAttributeAndNotify(RecalAttr_RandomAttr);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode RandomAttrMgr::ForgeRetain(XItem* pItem, Role* pRole)
{
	if (pItem == NULL || pRole == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	if (pItem->forge != NULL)
	{
		pItem->forge->unReplacedAttr.AttrID = pItem->forge->unReplacedAttr.AttrValue = 0;

		BagUpdateItemTransition updateTran(pRole);
		updateTran.AddChangedItem(pItem);
		updateTran.NotifyClient();
	}

	return KKSG::ERR_SUCCESS;
}

void RandomAttrMgr::RandomAttrTransfer(XItem* pItem, UINT32 preItemID)
{
	if (pItem == NULL)
	{
		return;
	}
	auto preIter = m_mapRandAttr.find(preItemID);
	if (preIter == m_mapRandAttr.end() || pItem->randAttr == NULL)
	{
		InitRandomAttr(pItem);
		return;
	}
	std::vector<double> vecPercent;
	std::map<UINT32, RandAttrAll>::iterator randAllIter = preIter->second.begin();
	for (auto i = pItem->randAttr->attrs.begin(); i != pItem->randAttr->attrs.end(); ++i, randAllIter++)
	{
		if (randAllIter == preIter->second.end())
		{
			break;
		}
		RandAttrAll& all = randAllIter->second;
		if (all.singles.find(i->AttrID) == all.singles.end())
		{
			vecPercent.push_back(0);
			continue;
		}
		const RandAttrSingle& single = all.singles[i->AttrID];
		vecPercent.push_back(_GetPercent(i->AttrValue, single.attrRange[0], single.attrRange[1]));
	}

	pItem->randAttr->attrs.clear();

	RandomAttr& randAttr = *pItem->randAttr;

	auto nextIter = m_mapRandAttr.find(pItem->itemID);
	if (nextIter == m_mapRandAttr.end())
	{
		return;
	}

	std::vector<double>::iterator percentIter = vecPercent.begin();
	for (auto i = nextIter->second.begin(); i != nextIter->second.end(); ++i)
	{
		std::pair<UINT32, UINT32> attr = i->second.GetInitRandomAttr();
		if (percentIter != vecPercent.end())
		{
			if (i->second.singles.find(attr.first) != i->second.singles.end())
			{
				const RandAttrSingle& single = i->second.singles[attr.first];
				double percent = *percentIter;
				UINT32 value = single.attrRange[0] + (UINT32)(percent * (single.attrRange[1] - single.attrRange[0]) + EPS);
				attr.second = value;
			}
			++percentIter;
		}
		randAttr.attrs.push_back(ItemChangeAttr(attr.first, attr.second));
	}
}

void RandomAttrMgr::ForgeEquipTransfer(Role* pRole, XItem* pItem, UINT32 preItemID, UINT32 from, UINT32 to)
{
	if (pItem == NULL || pItem->forge == NULL || pItem->forge->attrs.empty())
	{
		return;
	}
	if (pItem->forge->attrs.size() != 1)
	{
		LogError("Strange, equip forge size not 1, uid [%llu]", pItem->uid);
		ItemChangeAttr tmpAttr = *pItem->forge->attrs.begin();
		pItem->forge->attrs.clear();
		pItem->forge->attrs.push_back(tmpAttr);
	}
	ItemChangeAttr& forgeAttr = *pItem->forge->attrs.begin();
	const RandAttrSingle* single = GetForgeAttrSingle(preItemID, 0, forgeAttr.AttrID);
	if (single == NULL)
	{
		pItem->forge->attrs.clear();
		LogError("role [%llu], uid [%llu], can't get preItemID [%u], AttrID [%u] forge data in table when change prof, clear forge data!!!",
					pRole->GetID(), pItem->uid, preItemID, forgeAttr.AttrID);
		return;
	}
	double percent = _GetPercent(forgeAttr.AttrValue, single->attrRange[0], single->attrRange[1]);

	if (from != to)
	{
		ForgeTransAttrConf* fromConf = m_oForgeTransAttr.GetByProfID(from);
		ForgeTransAttrConf* toConf = m_oForgeTransAttr.GetByProfID(to);
		if (fromConf != NULL && toConf != NULL && fromConf->AttrID.size() == toConf->AttrID.size())
		{
			for (int i = 0; i < fromConf->AttrID.size(); ++i)
			{
				if (forgeAttr.AttrID == fromConf->AttrID[i])
				{
					const RandAttrSingle* newSingle = GetForgeAttrSingle(pItem->itemID, 0, toConf->AttrID[i]);
					if (newSingle != NULL)
					{
						forgeAttr.AttrID = toConf->AttrID[i];
						forgeAttr.AttrValue = newSingle->attrRange[0] + (UINT32)(percent * (newSingle->attrRange[1] - newSingle->attrRange[0]) + EPS);
						return;
					}
				}
			}
		}
	}
	const RandAttrSingle* newSingle = GetForgeAttrSingle(pItem->itemID, 0, forgeAttr.AttrID);
	if (newSingle == NULL)
	{
		pItem->forge->attrs.clear();
		LogError("role [%llu], uid [%llu], can't get ItemID [%u], AttrID [%u] forge data in table when forge transfer, clear forge data!!!",
			pRole->GetID(), pItem->uid, pItem->itemID, forgeAttr.AttrID);
		return;
	}
	forgeAttr.AttrValue = newSingle->attrRange[0] + (UINT32)(percent * (newSingle->attrRange[1] - newSingle->attrRange[0]) + EPS);
}

void RandomAttrMgr::InitSmeltCountForOldItem(XItem* pItem)
{
	if (pItem == NULL)
	{
		return;
	}
	const EquipConf* pEquipConf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (pEquipConf == NULL)
	{
		LogError("itemid [%u] not find in EquipList.txt, change the table?", pItem->itemID);
		return;
	}
	if (!pEquipConf->IsCanSmelt)
	{
		return;
	}
	pItem->smeltCount = 0;
	if (pItem->randAttr != NULL)
	{
		for (UINT32 i = 0; i < pItem->randAttr->attrs.size(); ++i)
		{
			ItemChangeAttr& attr = pItem->randAttr->attrs[i];
			const RandAttrSingle* single = GetRandomAttrSingle(pItem->itemID, i, attr.AttrID);
			if (single == NULL)
			{
				continue;
			}
			double percent = _GetPercent(attr.AttrValue, single->initAttr[1], single->attrRange[1]);
			pItem->smeltCount += (UINT32)(percent * single->smeltExpectation + EPS);
		}
	}
	if (pItem->forge != NULL)
	{
		for (UINT32 i = 0; i < pItem->forge->attrs.size(); ++i)
		{
			ItemChangeAttr& attr = pItem->forge->attrs[i];
			const RandAttrSingle* single = GetForgeAttrSingle(pItem->itemID, i, attr.AttrID);
			if (single == NULL)
			{
				continue;
			}
			double percent = _GetPercent(attr.AttrValue, single->initAttr[1], single->attrRange[1]);
			pItem->smeltCount += (UINT32)(percent * single->smeltExpectation + EPS);
		}
	}
}

void RandomAttrMgr::ResetRandomAttrAndForgeAttr(XItem* pItem)
{
	if (pItem == NULL)
	{
		return;
	}
	if (pItem->randAttr != NULL)
	{
		for (UINT32 i = 0; i < pItem->randAttr->attrs.size(); ++i)
		{
			ItemChangeAttr& attr = pItem->randAttr->attrs[i];
			const RandAttrSingle* single = GetRandomAttrSingle(pItem->itemID, i, attr.AttrID);
			if (single == NULL)
			{
				continue;
			}
			attr.AttrValue = XRandom::randInt(single->initAttr[0] , single->initAttr[1] + 1);
		}
	}
	if (pItem->forge != NULL)
	{
		for (UINT32 i = 0; i < pItem->forge->attrs.size(); ++i)
		{
			ItemChangeAttr& attr = pItem->forge->attrs[i];
			const RandAttrSingle* single = GetForgeAttrSingle(pItem->itemID, i, attr.AttrID);
			if (single == NULL)
			{
				continue;
			}
			attr.AttrValue = XRandom::randInt(single->initAttr[0] , single->initAttr[1] + 1);
		}
	}
}

void RandomAttrMgr::FixSmeltCount(XItem* pItem)
{
	if (pItem == NULL)
	{
		return;
	}
	if (pItem->smeltCount == 0)
	{
		return;
	}
	bool isEmpty = true;
	if (pItem->randAttr != NULL)
	{
		for (UINT32 i = 0; i < pItem->randAttr->attrs.size(); ++i)
		{
			ItemChangeAttr& attr = pItem->randAttr->attrs[i];
			const RandAttrSingle* single = GetRandomAttrSingle(pItem->itemID, i, attr.AttrID);
			if (single == NULL)
			{
				continue;
			}
			if (attr.AttrValue > single->initAttr[1])
			{
				isEmpty = false;
				break;
			}
		}
	}
	if (pItem->forge != NULL)
	{
		for (UINT32 i = 0; i < pItem->forge->attrs.size(); ++i)
		{
			ItemChangeAttr& attr = pItem->forge->attrs[i];
			const RandAttrSingle* single = GetForgeAttrSingle(pItem->itemID, i, attr.AttrID);
			if (single == NULL)
			{
				continue;
			}
			if (attr.AttrValue > single->initAttr[1])
			{
				isEmpty = false;
				break;
			}
		}
	}
	if (isEmpty)
	{
		pItem->smeltCount = 0;
	}
}

double RandomAttrMgr::_GetPercent(UINT32 attrValue, UINT32 attrMin, UINT32 attrMax)
{
	if (attrMin >= attrMax) return 0.0;
	if (attrValue < attrMin) return 0.0;
	if (attrValue > attrMax) return 1.0;
	return 1.0 * (attrValue - attrMin) / (double)(attrMax - attrMin);
}

// attrvalue belong [attrMin, attrMax]
UINT32 RandomAttrMgr::_GetSmeltAttrPos(UINT32 attrValue, UINT32 attrMin, UINT32 attrMax)
{
	if (attrMin > attrMax || attrValue < attrMin || attrValue > attrMax)
	{
		LogError("Program error, should judge before this function");
		return 0;
	}
	if (attrMin == attrMax)
	{
		return 0;
	}
	const std::vector<UINT32>& smeltColorRange = GetGlobalConfig().SmeltCorlorRange;
	UINT32 size = smeltColorRange.size();
	if (smeltColorRange.size() < 3)
	{
		return 0;
	}
	UINT32 percent = (UINT32)(_GetPercent(attrValue, attrMin, attrMax) * 100);
	if (percent < smeltColorRange[size - 3]) return 0;
	if (percent < smeltColorRange[size - 2]) return 1;
	return 2;
}

void RandomAttrMgr::_GetCanReplaceEquipSmeltItem(UINT32 itemID, std::vector<UINT32>& items)
{
	bool isFind = false;
	for (auto i = GetGlobalConfig().EquipAttackSmeltExchanged.begin(); i != GetGlobalConfig().EquipAttackSmeltExchanged.end(); ++i)
	{
		if (*i == itemID)
		{
			isFind = true;
		}
		if (isFind)
		{
			items.push_back(*i);
		}
	}
	if (isFind)
	{
		return;
	}
	for (auto i = GetGlobalConfig().EquipDefenseSmeltExchanged.begin(); i != GetGlobalConfig().EquipDefenseSmeltExchanged.end(); ++i)
	{
		if (*i == itemID)
		{
			isFind = true;
		}
		if (isFind)
		{
			items.push_back(*i);
		}
	}
}
