#include "pch.h"
#include "emblemmgr.h"
#include "itemconfig.h"
#include "bag.h"
#include "item.h"
#include "util/XRandom.h"
#include "unit/role.h"
#include "define/itemdef.h"
#include "table/XSkillEffectMgr.h"
#include "bagtransition.h"
#include "globalconfig.h"
#include "scene/scene.h"
#include "unit/rolemanager.h"
#include "tablemgr/XEntityInfoLibrary.h"

#define EPS 1e-6

INSTANCE_SINGLETON(CEmblemMgr)

CEmblemMgr::CEmblemMgr()
{
}

CEmblemMgr::~CEmblemMgr()
{
}

bool CEmblemMgr::Init()
{
	return LoadFile();
}

void CEmblemMgr::Uninit()
{
}

static bool HandlerTableData(AttributeEmblem& attrEmblem, std::unordered_map<UINT32, AttrEmblem>& mapAttrEmblem,
					SkillEmblem& skillEmblem, std::unordered_map<UINT32, SkEmblem>& m_mapSkill)
{
	if (!attrEmblem.LoadFile("table/AttributeEmblem.txt"))
	{
		LogWarn("load file table/AttributeEmblem.txt failed");
		return false;
	}

	for (auto i = attrEmblem.Table.begin(); i != attrEmblem.Table.end(); ++i)
	{
		AttrEmblem& seq = mapAttrEmblem[(*i)->EmblemID];
		if ((*i)->Position > EMBLEMATTRNUM || (*i)->Position <= 0)
		{
			LogWarn("position data wrong itemid:%u", (*i)->EmblemID);
			return false;
		}
		AttrEmblemSlot& slot = seq.slot[(*i)->Position - 1];
		if (slot.singles.find((*i)->AttrID) != slot.singles.end())
		{
			LogWarn("emblemid [%u], has more than 1 line, same slot [%u], attrid [%u]", (*i)->EmblemID, (*i)->Position, (*i)->AttrID);
			return false;
		}
		slot.allprob += (*i)->Probability;

		AttrEmblemSingle& single = slot.singles[(*i)->AttrID];
		single.prob = (*i)->Probability;

		single.initAttr = (*i)->InitialRange;
		single.attrRange = (*i)->Range;
		single.smeltExpectation = (*i)->SmeltExpectation;

		if (single.initAttr[0] > single.initAttr[1]) std::swap(single.initAttr[0], single.initAttr[1]);
		if (single.attrRange[0] > single.attrRange[1]) std::swap(single.attrRange[0], single.attrRange[1]);

		if (single.initAttr[0] < single.attrRange[0] || single.initAttr[0] > single.attrRange[1]
		|| single.initAttr[1] < single.attrRange[0] || single.initAttr[1] > single.attrRange[1])
		{
			LogWarn("emblemid:%u, slot:%u, attr:%u, InitialRange not in Range", (*i)->EmblemID, (*i)->Position, (*i)->AttrID);
			return false;
		}

		memset(single.smeltAllProb, 0, sizeof(single.smeltAllProb));
		for (UINT32 j = 0; j < 3; ++j)
		{
			std::vector<Sequence<int, 3>>* smeltAttr;
			if (j == 0) smeltAttr = &(*i)->SmeltAttr;
			else if (j == 1) smeltAttr = &(*i)->SmeltAttr2;
			else smeltAttr = &(*i)->SmeltAttr3;
			for (auto k = smeltAttr->begin(); k != smeltAttr->end(); ++k)
			{
				if ((*k)[2] <= 0)
				{
					LogWarn("emblemid:%u, slot:%u, attr:%u, SmeltAttr wrong, one prob <= zero", (*i)->EmblemID, (*i)->Position, (*i)->AttrID);
					return false;
				}
				single.smeltAllProb[j] += (*k)[2];
				single.smeltAttr[j][single.smeltAllProb[j]] = (*k)[0] <= (*k)[1] ? std::make_pair((*k)[0], (*k)[1] - (*k)[0]) : std::make_pair((*k)[1], (*k)[0] - (*k)[1]);
			}
		}
	}

	for (auto i = mapAttrEmblem.begin(); i != mapAttrEmblem.end(); ++i)
	{
		const AttrEmblem& emblem = i->second;
		for (UINT32 j = 0; j < EMBLEMATTRNUM - 1; ++j)
		{
			if (emblem.slot[j].allprob <= 0)
			{
				LogWarn("AttributeEmblem table error, emblemid:%u, slot:%u, prob sum <= zero", i->first, j);
				return false;
			}
		}
	}

	if (!skillEmblem.LoadFile("table/SkillEmblem.txt"))
	{
		LogWarn("load file table/SkillEmblem.txt failed");
		return false;
	}

	for (auto i = skillEmblem.Table.begin(); i != skillEmblem.Table.end(); ++i)
	{
		m_mapSkill[(*i)->EmblemID] = SkEmblem(*i);
	}

	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* pRole = *i;
		pRole->Get<Bag>()->ReCalSkillEmblem();
	}
	return true;
}

bool CEmblemMgr::CheckFile()
{
	AttributeEmblem attrEmblem;
	std::unordered_map<UINT32, AttrEmblem> mapAttrEmblem;
	SkillEmblem skillEmblem;
	std::unordered_map<UINT32, SkEmblem> mapSkill;
	return HandlerTableData(attrEmblem, mapAttrEmblem, skillEmblem, mapSkill);
}

bool CEmblemMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandlerTableData(m_oAttrEmblem, m_mapAttrEmblem, m_oSkillEmblem, m_mapSkill);

	return true;
}

void CEmblemMgr::ClearFile()
{
	m_oAttrEmblem.Clear();
	m_mapAttrEmblem.clear();
	m_oSkillEmblem.Clear();
	m_mapSkill.clear();
}

void CEmblemMgr::InitAttrEmblemAttr(XItem* pItem)
{
	if (pItem == NULL) return;
	auto iter = m_mapAttrEmblem.find(pItem->itemID);
	if (iter == m_mapAttrEmblem.end())
	{
		LogWarn("emblemid [%u] can't find in AttributeEmblem.txt", pItem->itemID);
		return;
	}
	pItem->changeAttr.clear();
	const AttrEmblem& emblem = iter->second;
	for (UINT32 i = 0; i < EMBLEMATTRNUM - 1; ++i)
	{
		const AttrEmblemSlot& slot = emblem.slot[i];
		UINT32 random = XRandom::randInt(1, slot.allprob + 1), prob = 0, attrid = -1, attrvalue;
		for (auto j = slot.singles.begin(); j != slot.singles.end(); ++j)
		{
			prob += j->second.prob;
			if (random <= prob)
			{
				attrid = j->first;
				attrvalue = XRandom::randInt(j->second.initAttr[0], j->second.initAttr[1] + 1);
				break;
			}
		}
		if (attrid == -1)
		{
			LogError("emblemid [%u], slot [%u], allprob [%u], attrid is -1, it should not happen!", pItem->itemID, i + 1, slot.allprob);
			return;
		}
		pItem->changeAttr.push_back(ItemChangeAttr(attrid, attrvalue));

		LogDebug("New AttrEmblem [%u], attr [%u], value [%u], slot [%u]", pItem->itemID, attrid, attrvalue, i + 1);
	}
	return;
}

KKSG::ErrorCode CEmblemMgr::IdentifyAttrEmblem(XItem* pItem, Role* pRole)
{
	if (pItem == NULL || pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	EmblemInfo* pEmblemInfo = pItem->emblemInfo;
	if (pEmblemInfo == NULL)
	{
		LogWarn("This emblemid [%u], uid [%llu] is not AttributeEmblem", pItem->itemID, pItem->uid);
		return KKSG::ERR_UNKNOWN;
	}
	if (pEmblemInfo->thirdSlot == 0)
	{
		LogWarn("No ThirdSlot in this emblemid [%u], uid [%llu]", pItem->itemID, pItem->uid);
		return KKSG::ERR_EMBLEM_CANTIDENTIFY;
	}
	else if (pEmblemInfo->thirdSlot == 2)
	{
		LogWarn("This emblemid [%u], uid [%llu] has been identified", pItem->itemID, pItem->uid);
		return KKSG::ERR_EMBLEM_CANTIDENTIFY;
	}

	auto iter = m_mapAttrEmblem.find(pItem->itemID);
	if (iter == m_mapAttrEmblem.end())
	{
		LogWarn("emblemid [%u] can't find in AttributeEmblem.txt", pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}

	const AttrEmblem& emblem = iter->second;
	if (emblem.slot[EMBLEMATTRNUM - 1].allprob <= 0)
	{
		LogWarn("emblemid [%u], uid [%llu], don't have third attribute in table", pItem->itemID, pItem->uid);
		return KKSG::ERR_EMBLEM_CANTIDENTIFY;
	}

	const AttrEmblemSlot& slot = emblem.slot[EMBLEMATTRNUM - 1];
	UINT32 random = XRandom::randInt(1, slot.allprob + 1), prob = 0, attrid = 0, attrvalue = 0;
	for (auto j = slot.singles.begin(); j != slot.singles.end(); ++j)
	{
		prob += j->second.prob;
		if (random <= prob)
		{
			attrid = j->first;
			attrvalue = XRandom::randInt(j->second.initAttr[0], j->second.initAttr[1] + 1);
			break;
		}
	}

	///> 根据职业改变攻击力属性
	const ProfessionTable::RowData* profConf = XEntityInfoLibrary::Instance()->TemplateID2Data_Role(pRole->GetProfession());
	if (profConf != NULL)
	{
		if (profConf->AttackType == 2 && attrid == BASIC_PhysicalAtk)
		{
			attrid = BASIC_MagicAtk;
		}
		if (profConf->AttackType == 1 && attrid == BASIC_MagicAtk)
		{
			attrid = BASIC_PhysicalAtk;
		}
	}
	else
	{
		LogError("role [%llu], Profession [%d] not find in Profession.txt when Identify Emblem", pRole->GetID(), pRole->GetProfession());
	}

	pItem->changeAttr.push_back(ItemChangeAttr(attrid, attrvalue));
	pItem->emblemInfo->thirdSlot = 2; // has been identified
	pItem->isbind = true;

	LogDebug("AttrEmblem's ThirdSlot: attrid [%u], value[%u]", attrid, attrvalue);
	return KKSG::ERR_SUCCESS;
}

const SkEmblem* CEmblemMgr::GetSkillEmblemByID(UINT32 itemid)
{
	if (m_mapSkill.find(itemid) == m_mapSkill.end())
	{
		LogWarn("%d can't find in SkillEmblem.txt", itemid);
		return NULL;
	}
	return &m_mapSkill[itemid];
}

float CEmblemMgr::GetSkillEmblemEffect(UINT32 skillHash, Role* pRole)
{
	if (pRole == NULL || (pRole->GetCurrScene() == NULL && pRole->GetEnterSceneType() == 0))
		return 0.0f;

	UINT32 sceneType = pRole->GetEnterSceneType();
	if (pRole->GetCurrScene() != NULL)
		sceneType = pRole->GetCurrScene()->GetSceneType();

	if(GlobalConfig::GetInstance().SkillEmblemDisableSceneTypes.count(sceneType) != 0)
		return 0.0f;

	Bag* pBag = pRole->Get<Bag>();
	for (UINT32 i = 0; i < SkillEmblemCount; ++i)
	{
		const SkEmblem* pSk = pBag->GetWearedSkillEmblemBySlot(i);
		if (pSk == NULL)
		{
			continue;
		}
		if (pSk->IsEqualHash(skillHash) && pSk->conf->SkillType == 1)
		{
			return (float)(1.0 * pSk->conf->SkillPercent / 100.0);
		}
	}

	return 0.0f;
}

float CEmblemMgr::GetSkillCDRatio(UINT32 skillHash, Role* pRole)
{
	if (pRole == NULL || (pRole->GetCurrScene() == NULL && pRole->GetEnterSceneType() == 0))
		return 0.0f;

	UINT32 sceneType = pRole->GetEnterSceneType();
	if (pRole->GetCurrScene() != NULL)
		sceneType = pRole->GetCurrScene()->GetSceneType();

	if(GlobalConfig::GetInstance().SkillEmblemDisableSceneTypes.count(sceneType) != 0)
		return 0.0f;

	float ratio = 0.0f;
	Bag* pBag = pRole->Get<Bag>();
	for (UINT32 i = 0; i < SkillEmblemCount; ++i)
	{
		const SkEmblem* pSk = pBag->GetWearedSkillEmblemBySlot(i);
		if (pSk == NULL)
		{
			continue;
		}
		if (pSk->IsEqualHash(skillHash) && pSk->conf->SkillType == 2)
		{
			ratio = -(float)(1.0 * pSk->conf->SkillPercent / 100.0);
			break;
		}
	}
	return ratio;
}

bool CEmblemMgr::IsActOnSameSkill(UINT32 type1, UINT32 type2)
{
	auto iter1 = m_mapSkill.find(type1);
	auto iter2 = m_mapSkill.find(type2);
	if (iter1 == m_mapSkill.end() || iter2 == m_mapSkill.end())
	{
		return false;
	}
	return (iter1->second.conf->SkillType == iter2->second.conf->SkillType) && iter1->second.IsHaveSameHash(iter2->second);
}

const AttrEmblemSingle* CEmblemMgr::GetAttrEmblemSingle(UINT32 itemID, UINT32 slot, UINT32 attrID)
{
	auto iter = m_mapAttrEmblem.find(itemID);
	if (iter == m_mapAttrEmblem.end())
	{
		return NULL;
	}
	if (slot >= EMBLEMATTRNUM)
	{
		return NULL;
	}
	if (iter->second.slot[slot].allprob == 0)
	{
		return NULL;
	}
	auto iter2 = iter->second.slot[slot].singles.find(attrID);
	return iter2 == iter->second.slot[slot].singles.end() ? NULL : &iter2->second;
}

KKSG::ErrorCode CEmblemMgr::SmeltAttrEmblem(XItem* pItem, UINT32 slot, UINT32& newAttrValue, Role* pRole, std::vector<ItemDesc>& vlist)
{
	const ItemConf* pItemConf = ItemConfig::Instance()->GetItemConf(pItem->itemID);
	if (pItemConf == NULL)
	{
		LogWarn("itemid:%u not find in ItemList", pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}
	if (pItemConf->ReqLevel < GetGlobalConfig().GetInt("SmeltEmblemMinLevel", 24))
	{
		return KKSG::ERR_SMELT_MINLEVEL;
	}
	if (pItem->changeAttr.empty())
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (pItem->changeAttr.size() - 1 < slot || EMBLEMATTRNUM - 1 < slot)
	{
		LogWarn("unknow wrong item attr size:%u, slot:%u", pItem->changeAttr.size(), slot);
		return KKSG::ERR_UNKNOWN;
	}
	const EmblemBasicConf* pEmblemConf = ItemConfig::Instance()->GetEmblemBasicConf(pItem->itemID);
	if (pEmblemConf == NULL)
	{
		LogWarn("emblemid [%u] not find in EmblemBasic.txt", pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}

	auto iter = m_mapAttrEmblem.find(pItem->itemID);
	if (iter == m_mapAttrEmblem.end())
	{
		LogWarn("emblemid [%u] can't find in AttributeEmblem.txt", pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}
	ItemChangeAttr& attr = pItem->changeAttr[slot];
	const AttrEmblemSlot& emblemSlot = m_mapAttrEmblem[pItem->itemID].slot[slot];

	auto singleIter = emblemSlot.singles.find(attr.AttrID);
	if (singleIter == emblemSlot.singles.end())
	{
		LogWarn("can't find emblemid [%u], slot [%u], attrid [%u] in AttributeEmblem.txt", pItem->itemID, slot, attr.AttrID);
		return KKSG::ERR_UNKNOWN;
	}
	const AttrEmblemSingle& single = singleIter->second;

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
		LogWarn("item [%llu], itemid[%u], slot [%u], attrid[%u], SmeltAttr%u is empty", pItem->uid, pItem->itemID, slot, attr.AttrID, pos + 1);
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
	oTakeTransition.SetReason(ItemFlow_Smelt, ItemFlow_SmeltEmblem);
	if (!oTakeTransition.TakeItem(GOLD, pEmblemConf->SmeltNeedMoney))
	{
		oTakeTransition.RollBack();
		return KKSG::ERR_SMELTING_LACKMONEY;
	}
	for (auto i = pEmblemConf->SmeltNeedItem.begin(); i != pEmblemConf->SmeltNeedItem.end(); ++i)
	{
		UINT32 num = (*i)[1];
		std::vector<UINT32> replaceItems;
		_GetCanReplaceEmblemSmeltItem((*i)[0], replaceItems);
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

	//> 增加洗炼总次数
	pItem->smeltCount++;

	BagUpdateItemTransition oChangeTransition(pRole);
	oChangeTransition.AddChangedItem(pItem);
	oChangeTransition.NotifyClient();

	pRole->RecalcuateAttributeAndNotify(RecalAttr_Emblem);

	return KKSG::ERR_SUCCESS;
}

void CEmblemMgr::InitSmeltCountForOldItem(XItem* pItem)
{
	if (pItem == NULL)
	{
		return;
	}
	pItem->smeltCount = 0;
	for (UINT32 i = 0; i < pItem->changeAttr.size(); ++i)
	{
		ItemChangeAttr& attr = pItem->changeAttr[i];
		const AttrEmblemSingle* single = GetAttrEmblemSingle(pItem->itemID, i, attr.AttrID);
		if (single == NULL)
		{
			continue;
		}
		double percent = _GetPercent(attr.AttrValue, single->initAttr[1], single->attrRange[1]);
		pItem->smeltCount += (UINT32)(percent * single->smeltExpectation + EPS);
	}
}

void CEmblemMgr::ResetAttr(XItem* pItem)
{
	if (pItem == NULL)
	{
		return;
	}
	for (UINT32 i = 0; i < pItem->changeAttr.size(); ++i)
	{
		ItemChangeAttr& attr = pItem->changeAttr[i];
		const AttrEmblemSingle* single = GetAttrEmblemSingle(pItem->itemID, i, attr.AttrID);
		if (single == NULL)
		{
			continue;
		}
		attr.AttrValue = XRandom::randInt(single->initAttr[0], single->initAttr[1] + 1);
	}
}

void CEmblemMgr::FixSmeltCount(XItem* pItem)
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
	for (UINT32 i = 0; i < pItem->changeAttr.size(); ++i)
	{
		ItemChangeAttr& attr = pItem->changeAttr[i];
		const AttrEmblemSingle* single = GetAttrEmblemSingle(pItem->itemID, i, attr.AttrID);
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
	if (isEmpty)
	{
		pItem->smeltCount = 0;
	}
}

UINT32 CEmblemMgr::GetAttributeEmblemSlotLevelLimit(int level)
{
	UINT32 i = 0;
	for (i = 0; i < GetGlobalConfig().AttributeEmblemSlotLevelLimit.size(); ++i)
	{
		if (GetGlobalConfig().AttributeEmblemSlotLevelLimit[i] <= level)
		{
			continue;
		}
		return i;
	}
	return i;
}

UINT32 CEmblemMgr::GetSkillEmblemSlotLevelLimit(int level)
{
	UINT32 i = 0;
	for (i = 0; i < GetGlobalConfig().SkillEmblemSlotLevelLimit.size(); ++i)
	{
		if (GetGlobalConfig().SkillEmblemSlotLevelLimit[i] <= level)
		{
			continue;
		}
		return i;
	}
	return i;
}

KKSG::EmblemSlotType CEmblemMgr::GetSlotTypeByIndex(UINT32 index)
{
	if (index < AttributeEmblemCount)
	{
		return KKSG::EmblemSlotType_Attri;
	}
	if (index < ExtraSkillEmblemStartIndex)
	{
		return KKSG::EmblemSlotType_Skill;
	}
	if (index < EmblemCount)
	{
		return KKSG::EmblemSlotType_ExtraSkill;
	}
	return KKSG::EmblemSlotType_None;
}

UINT32 CEmblemMgr::_GetSmeltAttrPos(UINT32 attrValue, UINT32 attrMin, UINT32 attrMax)
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
	UINT32 percent = (UINT32)(1.0 * (attrValue - attrMin) * 100.0 / (double)(attrMax - attrMin));
	if (percent < smeltColorRange[size - 3]) return 0;
	if (percent < smeltColorRange[size - 2]) return 1;
	return 2;
}

double CEmblemMgr::_GetPercent(UINT32 attrValue, UINT32 attrMin, UINT32 attrMax)
{
	if (attrMin >= attrMax) return 0.0;
	if (attrValue < attrMin) return 0.0;
	if (attrValue > attrMax) return 1.0;
	return 1.0 * (attrValue - attrMin) / (double)(attrMax - attrMin);
}

void CEmblemMgr::_GetCanReplaceEmblemSmeltItem(UINT32 itemID, std::vector<UINT32>& items)
{
	bool isFind = false;
	for (auto i = GetGlobalConfig().EmblemSmeltExchanged.begin(); i != GetGlobalConfig().EmblemSmeltExchanged.end(); ++i)
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