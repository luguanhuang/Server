#include "pch.h"
#include "sprite.h"
#include "spriteconfig.h"
#include "pb/project.pb.h"
#include "unit/calcattribute.h"
#include "unit/combatattribute.h"
#include "table/PowerPointMgr.h"
#include "util/XRandom.h"
#include "unit/role.h"
#include "spritetransition.h"
#include "spriterecord.h"
#include "bagtransition.h"
#include "utility/tlogger.h"
#include "payv2Record.h"

#define RunAttrFunc(x) x(1); x(2); x(3); x(4); x(5);

const double SpriteAttrRatio = 100.0;

Sprite::Sprite(Role* pRole):m_pRole(pRole)
{
	m_uid = 0;
	m_spriteID = 0;
	m_spriteID = 0;
	m_level = 1;
	m_evolutionLevel = 0;
	m_exp = 0;
	m_powerpoint = 0;
	m_trainExp = 0;
}

Sprite::Sprite(UINT64 id, UINT32 spriteid, Role* pRole):m_pRole(pRole)
{
	NewSprite(id, spriteid);
}

Sprite::~Sprite()
{

}

void Sprite::KKSGToSprite(const KKSG::SpriteInfo& infoData)
{
	m_uid = infoData.uid();
	m_spriteID = infoData.spriteid();
	m_skillID = infoData.skillid();
	m_level = infoData.level();
	m_evolutionLevel = infoData.evolutionlevel();
	m_exp = infoData.exp();
	m_powerpoint = infoData.powerpoint();
	m_trainExp = infoData.trainexp();

	m_passiveSkillID.clear();
	for (int i = 0; i < infoData.passiveskillid_size(); ++i)
	{
		m_passiveSkillID.push_back(infoData.passiveskillid(i));
	}

	if (infoData.attrid_size() !=  SpriteBasicAttrNum || infoData.attrid_size() != infoData.attrvalue_size()
		|| infoData.addvalue_size() != infoData.attrid_size())
	{
		LogError("uid:%lld spriteID:%u, SpriteInfo attr data is error", m_uid, m_spriteID);
		NewSprite(m_uid, m_spriteID);
		return;
	}
	if (infoData.evoattrid_size() != infoData.evoattrvalue_size())
	{
		LogError("uid:%lld spriteID:%u, SpriteInfo evoattr data is error", m_uid, m_spriteID);
		NewSprite(m_uid, m_spriteID);
		return;
	}
	if (infoData.thislevelevoattrid_size() != infoData.thislevelevoattrvalue_size())
	{
		LogError("uid:%lld spriteID:%u, SpriteInfo evoattr data is error", m_uid, m_spriteID);
		NewSprite(m_uid, m_spriteID);
		return;
	}

	m_addAttr.clear();
	m_attr.clear();
	m_evoAttr.clear();
	m_thisLevelEvoAttr.clear();
	
	for (int i = 0; i < SpriteBasicAttrNum; ++i)
	{
		m_attr[infoData.attrid(i)] = infoData.attrvalue(i);
		m_addAttr[infoData.attrid(i)] = infoData.addvalue(i);
	}
	for (int i = 0; i < infoData.evoattrid_size(); ++i)
	{
		m_evoAttr[infoData.evoattrid(i)] = infoData.evoattrvalue(i);
	}
	for (int i = 0; i < infoData.thislevelevoattrid_size(); ++i)
	{
		m_thisLevelEvoAttr[infoData.thislevelevoattrid(i)] = infoData.thislevelevoattrvalue(i);
	}
}

void Sprite::SpriteToKKSG(KKSG::SpriteInfo* infoData)
{
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pSpriteConf == NULL)
	{
		LogError("The sprite's spriteid:%u is not exist in SpriteTable.txt", m_spriteID);
		return;
	}
	infoData->Clear();

	infoData->set_uid(m_uid);
	infoData->set_spriteid(m_spriteID);
	infoData->set_skillid(m_skillID);
	infoData->set_level(m_level);
	infoData->set_evolutionlevel(m_evolutionLevel);
	infoData->set_exp(m_exp);
	infoData->set_powerpoint(m_powerpoint);
	infoData->set_trainexp(m_trainExp);

	for (auto i = m_passiveSkillID.begin(); i != m_passiveSkillID.end(); ++i)
	{
		infoData->add_passiveskillid(*i);
	}

#define AddBasicAttr(i) \
		infoData->add_attrid(pSpriteConf->AttrID##i);\
		infoData->add_attrvalue(m_attr[pSpriteConf->AttrID##i]);\
		infoData->add_addvalue(m_addAttr[pSpriteConf->AttrID##i]);

	RunAttrFunc(AddBasicAttr);

#undef AddBasicAttr

	for (auto i = m_evoAttr.begin(); i != m_evoAttr.end(); ++i)
	{
		infoData->add_evoattrid(i->first);
		infoData->add_evoattrvalue(i->second);
	}
	for (auto i = m_thisLevelEvoAttr.begin(); i != m_thisLevelEvoAttr.end(); ++i)
	{
		infoData->add_thislevelevoattrid(i->first);
		infoData->add_thislevelevoattrvalue(i->second);
	}
}

void Sprite::ApplyEffect(CombatAttribute* combatAttr)
{
	CombatAttribute tmpCombat;
	for (auto i = m_attr.begin(); i != m_attr.end(); ++i)
	{
		tmpCombat.AddAttr((CombatAttrDef)(i->first), i->second);
	}

	for (auto i = m_evoAttr.begin(); i != m_evoAttr.end(); ++i)
	{
		tmpCombat.AddAttr((CombatAttrDef)(i->first), i->second);
	}

	for (auto i = m_passiveSkillID.begin(); i != m_passiveSkillID.end(); ++i)
	{
		const SpriteSkillInfo* pSpriteSkillInfo = SpriteConfig::Instance()->GetSpriteSkillInfo(*i, 0);
		if (pSpriteSkillInfo == NULL)
		{
			LogWarn("can't find passiveskill id [%u] in SpriteSkill.txt", *i);
			continue;
		}
		if (1 == pSpriteSkillInfo->passiveSkillType) 
		{
			for (auto j = pSpriteSkillInfo->attr.begin(); j != pSpriteSkillInfo->attr.end(); ++j) ///> 等级固定成长
			{
				tmpCombat.AddAttr((CombatAttrDef)((*j)[0]), m_level * (*j)[1]);
			}
			for (auto j = pSpriteSkillInfo->baseAttr.begin(); j != pSpriteSkillInfo->baseAttr.end(); ++j) ///> 固定属性
			{
				tmpCombat.AddAttr((CombatAttrDef)((*j)[0]), (*j)[1]);
			}
		}
		else //百分比
		{
			for (auto j = pSpriteSkillInfo->attr.begin(); j != pSpriteSkillInfo->attr.end(); ++j)
			{
				tmpCombat.AddAttr((CombatAttrDef)((*j)[0]), (*j)[1]);
			}
		}
	}
	CalcRoleAttribute spriteAttribute;
	spriteAttribute.CalcAttributeConvert(&tmpCombat, m_pRole->GetProfession()); ///> 一级属性转化为二级属性

	///> 只加二级属性
	for (auto i = PowerPointMgr::Instance()->m_oCoeff.begin(); i != PowerPointMgr::Instance()->m_oCoeff.end(); ++i)
	{
		CombatAttrDef attrType = (CombatAttrDef)(i->first);
		if (attrType != BASIC_Strength && attrType != BASIC_Agility && attrType != BASIC_Intelligence && attrType != BASIC_Vitality) ///> 非一级属性
		{
			double val = tmpCombat.GetAttr((CombatAttrDef)(CA_TOTAL_START + attrType));
			combatAttr->AddAttr((CombatAttrDef)attrType, val);
		}
	}
}

void Sprite::RecalculateAttr()
{
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pSpriteConf == NULL)
	{
		LogWarn("spriteid:%u is not exist in SpriteTable.txt", m_spriteID);
		return;
	}
	const SpriteLevelInfo* pLevelInfo = SpriteConfig::Instance()->GetSpriteLevelInfo(pSpriteConf->SpriteQuality, m_level);	

	if (pLevelInfo == NULL)
	{
		LogWarn("spriteid [%u], quality [%u], level [%u], can't find in SpriteLevel.txt", pSpriteConf->SpriteID, pSpriteConf->SpriteQuality, m_level);
		return;
	}

	m_attr.clear();

#define calc(i) m_attr[pSpriteConf->AttrID##i] = 1.0 * pSpriteConf->BaseAttr##i / SpriteAttrRatio + m_addAttr[pSpriteConf->AttrID##i] * pLevelInfo->ratioSum;

	RunAttrFunc(calc);

#undef calc

	CombatAttribute combatAttr;
	ApplyEffect(&combatAttr);
	CalcRoleAttribute roleAttribute;
	roleAttribute.CalcAttributeConvert(&combatAttr, m_pRole->GetProfession());
	m_powerpoint = (UINT32)PowerPointMgr::Instance()->CalcPowerPoint(&combatAttr, m_pRole->GetProfession());
}

void Sprite::NewSprite(UINT64 id, UINT32 spriteid)
{
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(spriteid);
	if (pSpriteConf == NULL)
	{
		LogWarn("spriteid [%u] is not exist in SpriteTable.txt", spriteid);
		return;
	}
	m_uid = id;
	m_spriteID = spriteid;
	m_skillID = pSpriteConf->SpriteSkillID;
	m_level = 1;
	m_evolutionLevel = 0;
	m_exp = 0;
	m_trainExp = 0;

	m_passiveSkillID.clear();
	m_addAttr.clear();
	m_attr.clear();
	m_evoAttr.clear();
	m_thisLevelEvoAttr.clear();

	UINT32 skillNum = XRandom::randInt(0, pSpriteConf->PassiveSkillMax + 1);
	SpriteConfig::Instance()->GetRandomPassiveSkillID(pSpriteConf->PassiveSkillsID, skillNum, m_passiveSkillID);
	
#define randomAddAttr(x) \
	UINT32 initMin##x = pSpriteConf->InitialRange##x[0], initMax##x = pSpriteConf->InitialRange##x[1]; \
	m_addAttr[pSpriteConf->AttrID##x] = 1.0 * XRandom::randInt(initMin##x, initMax##x + 1) / SpriteAttrRatio;

	RunAttrFunc(randomAddAttr);

#undef randomAddAttr

	RecalculateAttr();
}

KKSG::ErrorCode Sprite::CheckAwake()
{
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pSpriteConf == NULL)
	{
		LogWarn("spriteid [%u] is not exist in SpriteTable.txt", m_spriteID);
		return KKSG::ERR_UNKNOWN;
	}

#define check(x) \
	double& attr##x = m_addAttr[pSpriteConf->AttrID##x]; \
	if (attr##x * SpriteAttrRatio < 1.0 * pSpriteConf->Range##x[0]) attr##x = 1.0 * pSpriteConf->Range##x[0] / SpriteAttrRatio; \
	if (attr##x * SpriteAttrRatio > 1.0 * pSpriteConf->Range##x[1]) attr##x = 1.0 * pSpriteConf->Range##x[1] / SpriteAttrRatio; \
	if (attr##x * SpriteAttrRatio < 1.0 * pSpriteConf->Range##x[1]) \
	{ \
		return KKSG::ERR_SUCCESS; \
	}

	RunAttrFunc(check);

#undef check

	return KKSG::ERR_SPRITE_AWAKE_MAX;
}

void Sprite::AwakeRandom()
{
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pSpriteConf == NULL)
	{
		LogWarn("spriteid [%u] is not exist in SpriteTable.txt", m_spriteID);
		return;
	}
	m_passiveSkillID.clear();

	UINT32 skillNum = XRandom::randInt(0, pSpriteConf->PassiveSkillMax + 1);
	SpriteConfig::Instance()->GetRandomPassiveSkillID(pSpriteConf->PassiveSkillsID, skillNum, m_passiveSkillID);

	UINT32 allprob, prob, random;

#define awake(x) \
	allprob = 0; \
	double& attr##x = m_addAttr[pSpriteConf->AttrID##x]; \
	if (attr##x * SpriteAttrRatio < 1.0 * pSpriteConf->Range##x[0]) attr##x = 1.0 * pSpriteConf->Range##x[0] / SpriteAttrRatio; \
	if (attr##x * SpriteAttrRatio > 1.0 * pSpriteConf->Range##x[1]) attr##x = 1.0 * pSpriteConf->Range##x[1] / SpriteAttrRatio; \
	for (auto i = pSpriteConf->SmeltAttr##x.begin(); i != pSpriteConf->SmeltAttr##x.end(); ++i) \
	{ \
		allprob += (*i)[2]; \
	} \
	random = XRandom::randInt(1, allprob + 1); \
	prob = 0; \
	for (auto i = pSpriteConf->SmeltAttr##x.begin(); i != pSpriteConf->SmeltAttr##x.end(); ++i) \
	{ \
		prob += (*i)[2]; \
		if (prob >= random) \
		{ \
			int deltaMin = (*i)[0], deltaMax = (*i)[1]; \
			int delta = (int)XRandom::randInt(0, deltaMax - deltaMin + 1) + deltaMin; \
			attr##x += 1.0 * delta / SpriteAttrRatio; \
			if (attr##x * SpriteAttrRatio < 1.0 * pSpriteConf->Range##x[0]) attr##x = 1.0 * pSpriteConf->Range##x[0] / SpriteAttrRatio; \
			if (attr##x * SpriteAttrRatio > 1.0 * pSpriteConf->Range##x[1]) attr##x = 1.0 * pSpriteConf->Range##x[1] / SpriteAttrRatio; \
			break; \
		} \
	}

	RunAttrFunc(awake);

#undef awake

	RecalculateAttr();
}

KKSG::ErrorCode Sprite::AddExp(UINT32 itemID)
{
	if (m_pRole->GetLevel() <= (int)m_level)
	{
		return KKSG::ERR_SPRITE_LEVELMAX;
	}

	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pSpriteConf == NULL)
	{
		LogWarn("spriteid [%u] is not exist in SpriteTable.txt", m_spriteID);
		return KKSG::ERR_UNKNOWN;
	}
	const SpriteLevelInfo* pSpriteLevelInfo = SpriteConfig::Instance()->GetSpriteLevelInfo(pSpriteConf->SpriteQuality, m_level);
	if (pSpriteLevelInfo == NULL)
	{
		LogWarn("pSpriteLevelInfo is NULL, quality:%u, level:%u", pSpriteConf->SpriteQuality, m_level);
		return KKSG::ERR_UNKNOWN;
	}

	UINT32 addExp = SpriteConfig::Instance()->GetSpriteFoodExp(itemID);
	if ((UINT32)(-1) == addExp)
	{
		LogWarn("itemID:%u is not in Sprite food", itemID);
		return KKSG::ERR_UNKNOWN;
	}

	if (m_exp >= pSpriteLevelInfo->exp && SpriteConfig::Instance()->GetSpriteLevelInfo(pSpriteConf->SpriteQuality, m_level + 1) == NULL)
	{
		LogWarn("pSpriteLevelInfo is NULL, quality:%u, level:%u when add exp", pSpriteConf->SpriteQuality, m_level + 1);
		return KKSG::ERR_SPRITE_LEVELMAX;
	}

	BagTakeItemTransition oTakeItemTransition(m_pRole);
	oTakeItemTransition.SetReason(ItemFlow_Sprite, ItemFlow_SpriteLevelUp);
	std::string strbill = TTag::Instance()->GetTagString();
	oTakeItemTransition.SetTransTag(strbill);
	if (!oTakeItemTransition.TakeItem(itemID, 1))
	{
		oTakeItemTransition.RollBack();
		return KKSG::ERR_SPRITE_LEVELUP_LACKOFCOST;
	}
	oTakeItemTransition.NotifyClient();

	m_exp += addExp;
	UINT32 tmpLevel = m_level;

	while (m_exp >= pSpriteLevelInfo->exp)
	{
		m_exp -= pSpriteLevelInfo->exp;	
		UINT32 tmpExp = pSpriteLevelInfo->exp;
		pSpriteLevelInfo = SpriteConfig::Instance()->GetSpriteLevelInfo(pSpriteConf->SpriteQuality, m_level + 1);
		if (pSpriteLevelInfo == NULL)
		{
			LogWarn("pSpriteLevelInfo is NULL, quality:%u, level:%u", pSpriteConf->SpriteQuality, m_level + 1);
			m_exp = tmpExp;
			break;
		}
		m_level++;

		if ((int)m_level >= m_pRole->GetLevel()) //不能超过人物等级
		{
			if (m_exp >= pSpriteLevelInfo->exp)
			{
				m_exp = pSpriteLevelInfo->exp - 1;
			}
			break;
		}
	}
	if (m_level > tmpLevel)
	{
		RecalculateAttr();
		NotifyChangeSprite();
		if (m_pRole->Get<CSpriteRecord>()->IsInfight(m_uid))
		{
			m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
		}
	}
	else
	{
		m_pRole->Get<CSpriteRecord>()->SetModify(); //just add exp, no need to recalculate attribute
	}

	//TLog
	SpriteTLogVar var;
	var.m_iOperType = TX_Sprite_AddExp;
	var.m_iChangeValue = addExp;
	PrintTLog(var, strbill);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Sprite::Train(const std::vector<UINT32>& notToChoose, std::vector<std::pair<UINT32, UINT32>>& addAttr)
{
	if (notToChoose.size() > GetGlobalConfig().SpriteTrainNoToChooseMaxNum)
	{
		return KKSG::ERR_SPRITE_TRAIN_CHOOSEMAX;
	}
	const SpriteConf* pConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pConf == NULL)
	{
		LogWarn("spriteid:%u is not exist in SpriteTable.txt", m_spriteID);
		return KKSG::ERR_UNKNOWN;
	}
	const SpriteEvolutionInfo* pEvolutionInfo = SpriteConfig::Instance()->GetSpriteEvolutionInfo(m_spriteID, m_evolutionLevel + 1);
	if (pEvolutionInfo == NULL)
	{
		return KKSG::ERR_SPRITE_EVOLUTION_LEVELMAX;
	}

	if (m_trainExp >= pEvolutionInfo->trainExp[1])
	{
		m_trainExp = pEvolutionInfo->trainExp[1];
		return KKSG::ERR_SPRITE_TRAIN_MAX;
	}
	if (GetGlobalConfig().SpriteTrainCost.empty())
	{
		LogWarn("SpriteTrainCost is not in GlobalConfig!");
		return KKSG::ERR_UNKNOWN;
	}
	UINT32 pos = pConf->SpriteQuality;
	if (pos > GetGlobalConfig().SpriteTrainCost.size())
	{
		pos = GetGlobalConfig().SpriteTrainCost.size();
	}
	if (pos <= 0) pos = 1;
	Sequence<UINT32, 4> dragonCost = GetGlobalConfig().SpriteTrainCost[pos - 1];

	pos = notToChoose.size();
	if (pos >= 3) pos = 2;
	BagTakeItemTransition takeTrans(m_pRole);
	takeTrans.SetReason(ItemFlow_Sprite, ItemFlow_SpriteTrain);
	std::string strbill = TTag::Instance()->GetTagString();
	takeTrans.SetTransTag(strbill);
	if (!takeTrans.TakeItem(dragonCost[0], dragonCost[pos + 1]))
	{
		takeTrans.RollBack();
		return KKSG::ERR_SPRITE_TRAIN_LACKITEM;
	}
	if (!takeTrans.TakeItem(pEvolutionInfo->cost[0], pEvolutionInfo->cost[1]))
	{
		takeTrans.RollBack();
		return KKSG::ERR_SPRITE_TRAIN_LACKITEM;
	}
	takeTrans.NotifyClient();

	UINT32 preTrainExp = m_trainExp;
	m_trainExp += pEvolutionInfo->trainExp[0];
	if (m_trainExp >= pEvolutionInfo->trainExp[1])
	{
		m_trainExp = pEvolutionInfo->trainExp[1];
	}

	UINT32 allprob = 0;
	std::vector<Sequence<UINT32, 3>> canAddAttr;
	for (UINT32 i = 0; i < pEvolutionInfo->trainAttr.size(); ++i)
	{
		bool isHave = false;
		for (UINT32 j = 0; j < notToChoose.size(); ++j)
		{
			if (pEvolutionInfo->trainAttr[i][0] == notToChoose[j])
			{
				isHave = true;
				break;
			}
		}
		if (!isHave)
		{
			allprob += pEvolutionInfo->trainAttr[i][2];
			canAddAttr.push_back(pEvolutionInfo->trainAttr[i]);
		}
	}
	UINT32 random = XRandom::randInt(1, allprob + 1), prob = 0;
	for (UINT32 i = 0; i < canAddAttr.size(); ++i)
	{
		prob += canAddAttr[i][2];
		if (prob >= random)
		{
			m_evoAttr[canAddAttr[i][0]] += canAddAttr[i][1];
			m_thisLevelEvoAttr[canAddAttr[i][0]] += canAddAttr[i][1];
			addAttr.push_back(std::make_pair(canAddAttr[i][0], canAddAttr[i][1]));

			if (canAddAttr[i][0] == BASIC_PhysicalAtk)
			{
				m_evoAttr[BASIC_MagicAtk] += canAddAttr[i][1];	
				m_thisLevelEvoAttr[BASIC_MagicAtk] += canAddAttr[i][1];
			}
			break;
		}
	}

	RecalculateAttr();
	NotifyChangeSprite();
	if (m_pRole->Get<CSpriteRecord>()->IsInfight(m_uid))
	{
		m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
	}

	//TLog
	SpriteTLogVar var;
	var.m_iOperType = TX_Sprite_Train;
	var.m_iChangeValue = preTrainExp - m_trainExp;
	PrintTLog(var, strbill);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Sprite::ResetTrain(UINT32 chooseid, bool needCost)
{
	if (m_trainExp == 0)
	{
		return KKSG::ERR_SPRITE_NOTTRAIN;
	}
	const SpriteEvolutionInfo* pEvolutionInfo = SpriteConfig::Instance()->GetSpriteEvolutionInfo(m_spriteID, m_evolutionLevel + 1);
	if (pEvolutionInfo == NULL)
	{
		return KKSG::ERR_SPRITE_EVOLUTION_LEVELMAX;
	}
	std::string strbill = TTag::Instance()->GetTagString();
	if (needCost)
	{
		if (chooseid >= pEvolutionInfo->resetTrainCost.size())
		{
			return KKSG::ERR_FAILED;
		}
		BagTakeItemTransition takeTrans(m_pRole);
		takeTrans.SetReason(ItemFlow_Sprite, ItemFlow_SpriteResetTrain);
		takeTrans.SetTransTag(strbill);
		if (!takeTrans.TakeItem(pEvolutionInfo->resetTrainCost[chooseid][0], pEvolutionInfo->resetTrainCost[chooseid][1]))
		{
			takeTrans.RollBack();
			return KKSG::ERR_SPRITE_RESETTRAIN_LACKITEM;
		}
		takeTrans.NotifyClient();
	}

	BagGiveItemTransition giveTrans(m_pRole);
	giveTrans.SetReason(ItemFlow_Sprite, ItemFlow_SpriteResetTrain);
	giveTrans.SetTransTag(strbill);
	giveTrans.GiveItem(pEvolutionInfo->cost[0], m_trainExp / pEvolutionInfo->trainExp[0] * pEvolutionInfo->cost[1]);
	giveTrans.NotifyClient();

	m_trainExp = 0;
	for (auto i = m_thisLevelEvoAttr.begin(); i != m_thisLevelEvoAttr.end(); ++i)
	{
		m_evoAttr[i->first] -= i->second;
		if (m_evoAttr[i->first] < 0)
		{
			LogError("spriteid:%u, uis:%llu, evoAttr < 0 when reset train", m_spriteID, m_uid);
			m_evoAttr[i->first] = 0;
		}
	}
	m_thisLevelEvoAttr.clear();

	RecalculateAttr();
	NotifyChangeSprite();
	if (m_pRole->Get<CSpriteRecord>()->IsInfight(m_uid))
	{
		m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
	}

	//TLog
	SpriteTLogVar var;
	var.m_iOperType = TX_Sprite_ResetTrain;
	var.m_iChangeValue = 0;
	PrintTLog(var, strbill);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Sprite::Rebirth(bool needCost)
{
	if (m_trainExp != 0 || m_evolutionLevel == 0)
	{
		return KKSG::ERR_SPRITE_NOTREBIRTH;
	}
	std::string strbill = TTag::Instance()->GetTagString();
	if (needCost)
	{
		BagTakeItemTransition takeTrans(m_pRole);
		takeTrans.SetReason(ItemFlow_Sprite, ItemFlow_SpriteRebirth);
		takeTrans.SetTransTag(strbill);
		for (auto i = GetGlobalConfig().SpriteZero.begin(); i != GetGlobalConfig().SpriteZero.end(); ++i)
		{
			if (!takeTrans.TakeItem((*i)[0], (*i)[1]))
			{
				takeTrans.RollBack();
					return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
		}
		takeTrans.NotifyClient();
	}

	BagGiveItemTransition giveTrans(m_pRole);
	giveTrans.SetReason(ItemFlow_Sprite, ItemFlow_SpriteRebirth);
	giveTrans.SetTransTag(strbill);
	for (UINT32 i = 1; i <= m_evolutionLevel; ++i)
	{
		const SpriteEvolutionInfo* pEvolutionInfo = SpriteConfig::Instance()->GetSpriteEvolutionInfo(m_spriteID, i);
		if (pEvolutionInfo == NULL)
		{
			continue;
		}
		giveTrans.GiveItem(pEvolutionInfo->cost[0], pEvolutionInfo->trainExp[1] / pEvolutionInfo->trainExp[0] * pEvolutionInfo->cost[1]);
	}
	giveTrans.NotifyClient();

	m_trainExp = 0;
	m_evolutionLevel = 0;

	m_evoAttr.clear();
	m_thisLevelEvoAttr.clear();

	RecalculateAttr();
	NotifyChangeSprite();
	if (m_pRole->Get<CSpriteRecord>()->IsInfight(m_uid))
	{
		m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
	}

	//TLog
	SpriteTLogVar var;
	var.m_iOperType = TX_Sprite_Rebirth;
	var.m_iChangeValue = 0;
	PrintTLog(var, strbill);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Sprite::EvolutionLevelUp()
{
	const SpriteConf* pConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pConf == NULL)
	{
		LogWarn("spriteid:%u is not exist in SpriteTable.txt", m_spriteID);
		return KKSG::ERR_UNKNOWN;
	}
	const SpriteEvolutionInfo* pEvolutionInfo = SpriteConfig::Instance()->GetSpriteEvolutionInfo(m_spriteID, m_evolutionLevel + 1);
	if (pEvolutionInfo == NULL)
	{
		return KKSG::ERR_SPRITE_EVOLUTION_LEVELMAX;
	}

	if (m_trainExp < pEvolutionInfo->trainExp[1])
	{
		return KKSG::ERR_SPRITE_TRAIN_NOT_ENOUGH;
	}

	if (pEvolutionInfo->levelLimit > m_level)
	{
		return KKSG::ERR_SPRITE_EVOLUTION_LEVELLIMIT;
	}

	if (m_pRole->GetLevel() < (int)GetGlobalConfig().SpriteEvolutionMinLevel)
	{
		return KKSG::ERR_SPRITE_EVOLUTION_ROLE_LEVELLIMIT; //人物等级不满足
	}	
	
	m_trainExp = 0;
	m_thisLevelEvoAttr.clear();

	m_evolutionLevel++;
	for (auto i = pEvolutionInfo->attr.begin(); i != pEvolutionInfo->attr.end(); ++i)
	{
		m_evoAttr[(*i)[0]] += (*i)[1];
	}

	RecalculateAttr();
	NotifyChangeSprite();
	if (m_pRole->Get<CSpriteRecord>()->IsInfight(m_uid))
	{
		m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
	}

	//TLog
	SpriteTLogVar var;
	var.m_iOperType = TX_Sprite_EvolutionLevelUp;
	var.m_iChangeValue = 1;
	PrintTLog(var);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Sprite::GetEvolutionLevelUpPPT(UINT32& ppt)
{
	const SpriteConf* pConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pConf == NULL)
	{
		LogWarn("spriteid [%u] is not exist in SpriteTable.txt", m_spriteID);
		return KKSG::ERR_UNKNOWN;
	}
	const SpriteEvolutionInfo* pEvolutionInfo = SpriteConfig::Instance()->GetSpriteEvolutionInfo(m_spriteID, m_evolutionLevel + 1);
	if (pEvolutionInfo == NULL)
	{
		return KKSG::ERR_SPRITE_EVOLUTION_LEVELMAX;
	}

	if(m_pRole->GetLevel() < (int)GetGlobalConfig().SpriteEvolutionMinLevel)
	{
		return KKSG::ERR_SPRITE_EVOLUTION_ROLE_LEVELLIMIT; //人物等级不满足
	}
	/*if(pSpriteEvolutionInfo->levelLimit > level)
	{
		return KKSG::ERR_SPRITE_EVOLUTION_LEVELLIMIT;
	}*/

	m_evolutionLevel++;
	for (auto i = pEvolutionInfo->attr.begin(); i != pEvolutionInfo->attr.end(); ++i)
	{
		m_evoAttr[(*i)[0]] += (*i)[1];
	}
	RecalculateAttr();

	ppt = m_powerpoint;

	m_evolutionLevel--;
	for (auto i = pEvolutionInfo->attr.begin(); i != pEvolutionInfo->attr.end(); ++i)
	{
		m_evoAttr[(*i)[0]] -= (*i)[1];
	}
	RecalculateAttr();

	return KKSG::ERR_SUCCESS;
}

void Sprite::NotifyChangeSprite()
{
	SpriteTransition pSpriteTransition(m_pRole);
	pSpriteTransition.ChangeSprite(this);
	pSpriteTransition.NotifyClient();
}

void Sprite::PrintTLog(const SpriteTLogVar& var, const std::string& trans)
{
	TSpriteFlow oLog(m_pRole);
	oLog.m_uUid = m_uid;
	oLog.m_uSpriteID = m_spriteID;
	oLog.m_uSkillID = m_skillID;
	oLog.m_uLevel = m_level;
	oLog.m_uEvolutionLevel = m_evolutionLevel;
	oLog.m_uExp = m_exp;
	oLog.m_uPowerPoint = m_powerpoint;
	for(auto i = m_passiveSkillID.begin(); i != m_passiveSkillID.end(); ++i)
	{
		if(i != m_passiveSkillID.begin())
		{
			oLog.m_strPassiveSkillID += string(",");
		}
		oLog.m_strPassiveSkillID += string("ID") + ToString(*i);
	}
	oLog.m_iChangeValue = var.m_iChangeValue;
	oLog.m_iOperType = var.m_iOperType;
	oLog.m_iReason = var.m_iReason;
	oLog.m_iSubReason = var.m_iSubReason;
	oLog.SetTransTag(trans);
	oLog.Do();
}

void Sprite::PrintTLogOut(const std::string& trans)
{
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(m_spriteID);
	if (pSpriteConf == NULL)
	{
		return;
	}
	TSpriteLogoutFlow oLog(m_pRole);
	oLog.m_uid = m_uid;
	oLog.m_uExp = m_exp;
	oLog.m_spriteID = m_spriteID;
	oLog.m_skillID = m_skillID;
	oLog.m_spritelevel = m_level;
	oLog.m_evolutionLevel = m_evolutionLevel;
	oLog.m_spritepowerpoint = m_powerpoint;
	oLog.m_Quality = SpriteConfig::Instance()->GetSpriteQuality(m_spriteID);
	oLog.m_IsCaptain =  m_pRole->Get<CSpriteRecord>()->IsLeader(m_uid);
	TBaseAttr attr;
	double ratio = 0.0;
#define SetAddAttrLog(x) \
	attr.m_AttrID = pSpriteConf->AttrID##x;\
	ratio = PowerPointMgr::Instance()->_GetPowerPointAttributeWeight(attr.m_AttrID, 0);\
	attr.m_AttrValue = (UINT32)(m_addAttr[pSpriteConf->AttrID##x]*SpriteAttrRatio*ratio);\
	attr.m_AttrFirst = UINT32(pSpriteConf->Range##x[0]*ratio);\
	attr.m_AttrSecond = UINT32(pSpriteConf->Range##x[1]*ratio);\
	oLog.m_vAddAttrList.push_back(attr);
	RunAttrFunc(SetAddAttrLog)
#undef SetAddAttrLog
	oLog.m_attr = m_attr;
	oLog.m_evoAttr = m_evoAttr;
	oLog.SetTransTag(trans);
	oLog.Do();
}
