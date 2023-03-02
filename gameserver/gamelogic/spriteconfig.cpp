#include "pch.h"
#include "spriteconfig.h"
#include "util/XRandom.h"
#include "buff/XBuffTemplateManager.h"
#include "globalconfig.h"
#include "unit/combatattribute.h"

INSTANCE_SINGLETON(SpriteConfig)

#define SkillBoundary 2000

SpriteTableData::SpriteTableData()
{

}

SpriteTableData::~SpriteTableData()
{

}

bool SpriteTableData::HandlerTableData()
{
	if (!m_oSpriteTable.LoadFile("table/SpriteTable.txt"))
	{
		LogWarn("Load SpriteTable.txt failed!");
		return false;
	}
	if (!m_oSpriteLevelTable.LoadFile("table/SpriteLevel.txt"))
	{
		LogWarn("Load SpriteLevel.txt failed!");
		return false;
	}
	if (!m_oSpriteEvolution.LoadFile("table/SpriteEvolution.txt"))
	{
		LogWarn("Load SpriteEvolution.txt failed!");
		return false;
	}
	if (!m_oSpriteSkillTable.LoadFile("table/SpriteSkill.txt"))
	{
		LogWarn("Load SpriteSkill.txt failed!");
		return false;
	}
	if (!m_oSpritePassiveSkillBank.LoadFile("table/SpritePassiveSkillBank.txt"))
	{
		LogWarn("Load SpritePassiveSkillBank.txt failed!");
		return false;
	}	

	int index = 0;
	for (auto i = m_oSpriteTable.Table.begin(); i != m_oSpriteTable.Table.end(); i++)
	{
		m_id2index[(*i)->SpriteID] = index;
		index++;

#define test(x) \
	UINT32 rangeMin##x = (*i)->Range##x[0], rangeMax##x = (*i)->Range##x[1]; \
	UINT32 initMin##x = (*i)->InitialRange##x[0], initMax##x = (*i)->InitialRange##x[1]; \
	if (rangeMin##x > rangeMax##x || initMin##x > initMax##x) \
		{ \
		LogWarn("spriteid [%u], InitialRange or Range is invalid", (*i)->SpriteID); \
		return false; \
		} \
		if (initMin##x < rangeMin##x || initMin##x > rangeMax##x) \
		{ \
		LogWarn("spriteid [%u], InitialRange%s is not in Range%s", (*i)->SpriteID, #x, #x); \
		return false; \
		} \
		if (initMax##x < rangeMin##x || initMax##x > rangeMax##x) \
		{ \
		LogWarn("spriteid [%u], InitialRange%s is not in Range%s", (*i)->SpriteID, #x, #x); \
		return false; \
		} \
		for (auto j = (*i)->SmeltAttr##x.begin(); j != (*i)->SmeltAttr##x.end(); j++) \
		{ \
		if ((*j)[2] <= 0) \
		{ \
		LogWarn("spriteid [%u], SmeltAttr%s is invalid, one prob <= 0", (*i)->SpriteID, #x); \
		return false; \
		} \
		if ((*j)[0] > (*j)[1]) \
		{ \
		LogWarn("spriteid [%u], SmeltAttr%s range is invalid", (*i)->SpriteID, #x); \
		return false; \
		} \
		}

		test(1); test(2); test(3); test(4); test(5);

#undef test
	}
	//SpriteLevel
	for (auto i = m_oSpriteLevelTable.Table.begin(); i != m_oSpriteLevelTable.Table.end(); i++)
	{
		m_level[(*i)->Quality].push_back(SpriteLevelInfo((*i)->Level, (*i)->Exp, (*i)->Ratio));
	}
	//sum and test
	for (auto i = m_level.begin(); i != m_level.end(); i++)
	{
		std::sort((i->second).begin(), (i->second).end());
		for (UINT32 j = 0; j < (i->second).size(); j++)
		{
			if (0 == (i->second)[j].exp)
			{
				LogWarn("exp error in SpriteLevel.txt");
				return false;
			}
			if ((i->second)[j].level != j + 1)
			{
				LogWarn("level error in SpriteLevel.txt");
				return false;
			}
			if (j > 0)
			{
				(i->second)[j].expSum = (i->second)[j].exp + (i->second)[j - 1].expSum;
				(i->second)[j].ratioSum += (i->second)[j - 1].ratioSum;
			}
			else
			{
				(i->second)[j].expSum = (i->second)[j].exp;
			}
		}
	}
	//SpriteEvolution
	for (auto i = m_oSpriteEvolution.Table.begin(); i != m_oSpriteEvolution.Table.end(); i++)
	{
		m_evolution[(*i)->SpriteID].push_back(SpriteEvolutionInfo((*i)->EvolutionLevel, (*i)->LevelLimit,
			(*i)->EvolutionCost, (*i)->TrainExp, (*i)->ResetTrainCost, (*i)->AddAttr, (*i)->TrainAttr));
		if ((*i)->TrainExp[0] > (*i)->TrainExp[1] || (*i)->TrainExp[0] <= 0)
		{
			LogWarn("spriteid [%u], evolutionLevel [%u], TrainExp error", (*i)->SpriteID, (*i)->EvolutionLevel);
			return false;
		}
	}
	//test
	for (auto i = m_evolution.begin(); i != m_evolution.end(); i++)
	{
		std::sort((i->second).begin(), (i->second).end());
		for (UINT32 j = 0; j < (i->second).size(); j++)
		{
			if ((i->second)[j].evolutionLevel != j + 1)
			{
				LogWarn("spriteid [%u], evolutionLevel error in SpriteEvolution.txt", i->first);
				return false;
			}
			if (i->second[j].trainAttr.size() != SpriteBasicAttrNum - 1)
			{
				LogWarn("spriteid [%u], evolutionLevel [%u] size != %u", i->first, j, SpriteBasicAttrNum - 1);
				return false;
			}
			bool isPhysicalAtk = false;
			for (UINT32 k = 0; k < SpriteBasicAttrNum - 1; k++)
			{
				if (i->second[j].trainAttr[k][0] == BASIC_PhysicalAtk)
				{
					isPhysicalAtk = true;
					break;
				}
			}
			if (!isPhysicalAtk)
			{
				LogWarn("spriteid [%u], evolutionLevel [%u], TrainAttr no PhysicalAtk attrid", i->first, j);
				return false;
			}
		}
	}
	//SpritePassiveSkillBank
	for (auto i = m_oSpritePassiveSkillBank.Table.begin(); i != m_oSpritePassiveSkillBank.Table.end(); i++)
	{
		m_passiveSkill[(*i)->PassiveSkillsID].push_back(SpriteRandomPassiveSkill((*i)->SkillEffectID, (*i)->SkillID, (*i)->Weight));
	}
	//SpriteSkillTable
	for (auto i = m_oSpriteSkillTable.Table.begin(); i != m_oSpriteSkillTable.Table.end(); ++i)
	{
		m_skill[(*i)->SkillID].push_back(SpriteSkillInfo((*i)->SkillID, (*i)->SkillQuality, (*i)->PassiveSkillType, (*i)->SkillName, (*i)->NoticeDetail, (*i)->ShowNotice, 
			(*i)->Attrs, (*i)->BaseAttrs, (*i)->Buffs));	
	}
	//test
	for (auto i = m_skill.begin(); i != m_skill.end(); i++)
	{
		if(i->first < SkillBoundary) //active
		{
			for (UINT32 j = 0; j < i->second.size(); j++)
			{
				if ((i->second)[j].skillQuality != j+1)
				{
					LogWarn("skillQuality(active) error in SpriteSkill.txt");
					return false;
				}
			}
		}
		else // passive
		{
			if (i->second.size() != 1)
			{
				LogWarn("skillQuality(passive) error in SpriteSkill.txt");
				return false;
			}
		}
	}

	return true;
}


SpriteConfig::SpriteConfig()
{

}

SpriteConfig::~SpriteConfig()
{

}

bool SpriteConfig::Init()
{
	return HandlerTableData();
}

void SpriteConfig::Uninit()
{

}

bool SpriteConfig::CheckFile()
{
	SpriteTableData tmpTableData;
	return tmpTableData.HandlerTableData();
}

bool SpriteConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandlerTableData();

	return true;
}

void SpriteConfig::ClearFile()
{
	m_oSpriteTable.Clear();
	m_oSpriteLevelTable.Clear();
	m_oSpriteEvolution.Clear();

	m_oSpriteSkillTable.Clear();
	m_oSpritePassiveSkillBank.Clear();

	m_id2index.clear();
	m_level.clear();
	m_evolution.clear();
	m_passiveSkill.clear();
	m_skill.clear();
}

UINT32 SpriteConfig::GetSpriteIndex(UINT32 spriteID)
{
	if (m_id2index.find(spriteID) == m_id2index.end())
	{
		return (UINT32)(-1);
	}
	return m_id2index[spriteID];
}

UINT32 SpriteConfig::GetSpriteFoodExp(UINT32 itemID)
{
	for (auto i =  GetGlobalConfig().SpriteFoodExp.begin(); i !=  GetGlobalConfig().SpriteFoodExp.end(); i++)
	{
		if ((*i)[0] == itemID)
		{
			return (*i)[1];
		}
	}
	return (UINT32)(-1);
}

const SpriteLevelInfo* SpriteConfig::GetSpriteLevelInfo(UINT32 quality, UINT32 level)
{
	if (m_level.find(quality) == m_level.end())
	{
		LogWarn("the quality:%u can't find in SpriteLevel.txt", quality);
		return NULL;
	}
	std::vector<SpriteLevelInfo>& vecInfos = m_level[quality];
	if (vecInfos.size() < level || 0 == level)
	{
		return NULL;
	}
	return &vecInfos[level - 1];
}

const SpriteEvolutionInfo* SpriteConfig::GetSpriteEvolutionInfo(UINT32 spriteID, UINT32 evolutionLevel)
{
	if (m_evolution.find(spriteID) == m_evolution.end())
	{
		LogWarn("the quality:%u can't find in SpriteEvolution.txt", spriteID);
		return NULL;
	}
	std::vector<SpriteEvolutionInfo>& vecInfos = m_evolution[spriteID];
	if (vecInfos.size() < evolutionLevel || 0 == evolutionLevel)
	{
		return NULL;
	}
	return &vecInfos[evolutionLevel - 1];
}

const SpriteSkillInfo* SpriteConfig::GetSpriteSkillInfo(UINT32 skillID, UINT32 evoLevel)
{
	if (m_skill.find(skillID) == m_skill.end())
	{
		LogWarn("the skillID:%u can't find in SpriteSKill.txt", skillID);
		return NULL;
	}
	if (skillID < SkillBoundary) //active
	{
		std::vector<SpriteSkillInfo>& vecInfos = m_skill[skillID];
		if (vecInfos.size() < evoLevel + 1)
		{
			LogWarn("the skillID:%u max skillQuality:%u < evoLevel:%u + 1", skillID, vecInfos.size(), evoLevel);
			return NULL;
		}
		return &vecInfos[evoLevel];
	}
	else //passive
	{
		return &m_skill[skillID][0];
	}
}

void SpriteConfig::GetRandomPassiveSkillID(UINT32 skillBankID, UINT32 num, std::vector<UINT32>& skills)
{
	if (num == 0 || m_passiveSkill.find(skillBankID) == m_passiveSkill.end())
	{
		return;
	}
	std::vector<SpriteRandomPassiveSkill>& skillBank = m_passiveSkill[skillBankID];
	std::set<UINT32> has;
	for (UINT32 i = 0; i < num; i++)
	{
		UINT32 sum = 0;
		for (auto j = skillBank.begin(); j != skillBank.end(); j++)
		{
			if (has.find((*j).skillEffectID) == has.end())
			{
				sum += (*j).weight;
			}
		}
		if(0 == sum)
		{
			return;
		}
		UINT32 random = XRandom::randInt(1, sum + 1);
		UINT32 tmpRand = 0;
		for (auto j = skillBank.begin(); j != skillBank.end(); j++)
		{
			if (has.find((*j).skillEffectID) == has.end())
			{
				tmpRand += (*j).weight;
				if (tmpRand >= random)
				{
					skills.push_back((*j).skillID);
					has.insert((*j).skillEffectID);
					break;
				}
			}
		}
	}
}

void SpriteConfig::GetAllEvolutionCost(UINT32 spriteID, UINT32 evolutionLevel, UINT32 trainExp, std::map<UINT32, UINT32>& allcost)
{
	if (m_evolution.find(spriteID) == m_evolution.end())
	{
		LogWarn("the quality:%u can't find in SpriteEvolution.txt", spriteID);
		return;
	}
	std::vector<SpriteEvolutionInfo>& vecInfos = m_evolution[spriteID];
	if (evolutionLevel > vecInfos.size())
	{
		LogError("spriteid [%u], evolutionLevel [%u] bigger than SpriteEvolution.txt", spriteID, evolutionLevel);
		return;
	}
	for (UINT32 i = 0; i < evolutionLevel; i++)
	{
		allcost[vecInfos[i].cost[0]] += vecInfos[i].trainExp[1] / vecInfos[i].trainExp[0] * vecInfos[i].cost[1];
	}
	if (evolutionLevel < vecInfos.size() && trainExp > 0)
	{
		if (trainExp > vecInfos[evolutionLevel].trainExp[1]) trainExp = vecInfos[evolutionLevel].trainExp[1];
		allcost[vecInfos[evolutionLevel].cost[0]] += trainExp / vecInfos[evolutionLevel].trainExp[0] * vecInfos[evolutionLevel].cost[1];
	}
}

void SpriteConfig::GetAllExpCost(UINT32 quality, UINT32 level, UINT32 exp, std::map<UINT32, UINT32>& allcost)
{
	if (m_level.find(quality) == m_level.end())
	{
		LogWarn("the quality:%u can't find in SpriteLevel.txt", quality);
		return;
	}
	if (m_level[quality].size() < level || 0 == level)
	{
		return;
	}
	if (level >= 2)
	{
		exp += m_level[quality][level - 2].expSum;
	}
	exp = (UINT32)(1.0 * exp * GetGlobalConfig().SpriteExpRatio);
	for (auto i = GetGlobalConfig().SpriteFoodExp.rbegin(); i != GetGlobalConfig().SpriteFoodExp.rend(); i++)
	{
		if (exp > (*i)[1])
		{
			UINT32 num = exp / (*i)[1];
			allcost[(*i)[0]] += num;

			exp -= num * (*i)[1];
		}
	}
}

UINT32 SpriteConfig::GetSpriteQuality(UINT32 spriteID)
{
	const SpriteConf* pConf = GetSpriteConf(spriteID);
	if (pConf)
	{
		return pConf->SpriteQuality;
	}
	return 0;
}
