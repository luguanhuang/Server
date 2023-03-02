#include "pch.h"
#include "XSkillEffectMgr.h"
#include "util/XCommon.h"
#include <assert.h>
#include "foreach.h"
#include "table/globalconfig.h"
#include "unit/unit.h"
#include "unit/role.h"
#include "gamelogic/emblemmgr.h"
#include "math/XMath.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "unit/attributewatcher.h"
#include "XEntityStatistics.h"
#include "gamelogic/skilllevel.h"
#include "gamelogic/skilldefine.h"
#include "entity/XActionDefine.h"
#include "entity/XObject.h"
#include "gamelogic/combateffect.h"
#include "buff/XBuffReduceDamage.h"
#include "entity/XEntity.h"

INSTANCE_SINGLETON(XSkillEffectMgr)


SkillEffect::SkillEffect()
{
	DamageElementType = DE_NONE;
	PhyRatio = 1;
	PhyFixed = 0;
	MagRatio = 1;
	MagFixed = 0;
	PercentDamage = 0;
	DecSuperArmor = 0;
	ExclusiveMask = 0;
	pDistanceScale = NULL;
}

SkillEffect::~SkillEffect()
{
	if (pDistanceScale != NULL)
	{
		delete pDistanceScale;
		pDistanceScale = NULL;
	}
}

XSkillEffectMgr::XSkillEffectMgr()
{

}

XSkillEffectMgr::~XSkillEffectMgr()
{

}

bool XSkillEffectMgr::Init()
{
	return LoadFile();
}

void XSkillEffectMgr::Uninit()
{
	ClearFile();
}

bool XSkillEffectMgr::CheckFile()
{
	SkillList tmpSkillList;
	if(!tmpSkillList.LoadFile("table/SkillList.txt"))
	{
		SSWarn<<"load file table/SkillList.txt failed"<<END;
		return false;
	}
	ProfessionSkill tmpProfessionSkill;
	if(!tmpProfessionSkill.LoadFile("table/ProfessionSkill.txt"))
	{
		SSWarn<<"load file table/ProfessionSkill.txt failed"<<END;
		return false;
	}
	GuildSkillTable tmpGuildSkillTable;
	if(!tmpGuildSkillTable.LoadFile("table/GuildSkill.txt"))
	{
		SSWarn<<"load file table/GuildSkill.txt failed"<<END;
		return false;
	}

	PvpProfessionBias tmpPvpProfessionBias;
	if(!tmpPvpProfessionBias.LoadFile("table/PvpProfessionBias.txt"))
	{
		SSWarn<<"load file table/PvpProfessionBias.txt failed"<<END;
		return false;
	}

	return true;
}

bool XSkillEffectMgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	m_ProfSkillTable.Clear();
	m_FixedEnmity.clear();
	m_EnmityCoefficient.clear();
	LoadFileProfSkill("table/ProfessionSkill.txt");

	m_GuildSkillTable.Clear();
	LoadGuildSkill("table/GuildSkill.txt");

	m_ex2pre.clear();
	m_pre2ex.clear();
	m_specialEnemyEx2pre.clear();
	m_specialEnemyPre2ex.clear();
	m_ProfessionSkills.clear();
	LoadFileSkill("table/SkillList.txt");

	m_PvpProfessionBiasTable.Clear();
	LoadPvpBias("table/PvpProfessionBias.txt");
	return true;
}

void XSkillEffectMgr::ClearFile()
{
	m_SkillTable.Clear();
	m_ex2pre.clear();
	m_pre2ex.clear();
	m_specialEnemyEx2pre.clear();
	m_specialEnemyPre2ex.clear();
	m_SkillIndex.clear();
	m_SpecialEnemySkillIndex.clear();

	m_ProfSkillTable.Clear();
	m_FixedEnmity.clear();
	m_EnmityCoefficient.clear();
	m_ProfessionSkills.clear();

	m_GuildSkillTable.Clear();
}

bool XSkillEffectMgr::CheckRowValid(const SkillList::RowData& row)
{
	unsigned a = row.PhysicalRatio.size();
	unsigned b = row.PvPRatio.size();
	//if(row.Profession != 0)
	{
		if(a != b)
		{
			LogError("SkillList.txt PhysicalRatio != PvPRatio in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
			assert(false);
			return false;
		}
	}
	if (a != row.PhysicalFixed.size())
	{
		LogError("SkillList.txt PhysicalRatio != PhysicalFixed in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	if (a != row.MagicRatio.size())
	{
		LogError("SkillList.txt PhysicalRatio != MagicRatio in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	if (a != row.MagicFixed.size())
	{
		LogError("SkillList.txt PhysicalRatio != MagicFixed in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	if (a != row.PercentDamage.size())
	{
		LogError("SkillList.txt PercentDamage != MagicFixed in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	if (/*row.Profession != 0 &&*/ a != row.DecreaseSuperArmor.size())
	{
		LogError("SkillList.txt PhysicalRatio != DecreaseSuperArmor in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	if (b != row.PvPFixed.size())
	{
		LogError("SkillList.txt PvPRatio != PvPFixed in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	if (b != row.PvPMagicRatio.size())
	{
		LogError("SkillList.txt PvPRatio != PvPMagicRatio in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	if (b != row.PvPMagicFixed.size())
	{
		LogError("SkillList.txt PvPRatio != PvPMagicFixed in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	if (/*row.Profession != 0 &&*/ b != row.PvPDecreaseSuperArmor.size())
	{
		LogError("SkillList.txt PvPRatio != PvPDecreaseSuperArmor in row [%s] [%s]", row.SkillScript.c_str(), row.ScriptName.c_str());
		assert(false);
		return false;
	}
	return true;
}

void XSkillEffectMgr::AppendRow(const std::string& skillname, UINT32 entityid, std::vector<SkillList::RowData*>& templist)
{
	if (templist.size() == 0)
		return;

	UINT32 hash = XCommon::XHash(skillname.c_str());
	if (templist[0]->Profession != 0)
	{
		ProfessionSkillInfo pInfo;
		pInfo.SkillHash = hash;
		pInfo.UnlockLevel = templist[0]->UnlockLevel;
		m_ProfessionSkills[templist[0]->Profession].push_back(pInfo);
	}

	SkillList::RowData* row = templist[0];
	// pre && ex
	if (!row->ExSkillScript.empty())
	{
		UINT32 hash = XCommon::XHash(skillname.c_str());
		const std::string& script = row->ExSkillScript; 
		UINT32 exhash = XCommon::XHash(script.c_str());

		if(entityid != 0)
		{
			UINT64 spcialhash = ((UINT64)hash) << 32 | entityid;
			UINT64 speicalexhash = ((UINT64)exhash) << 32 | entityid;
			m_specialEnemyPre2ex[spcialhash] = exhash;
			m_specialEnemyEx2pre[speicalexhash] = hash;
		}
		else
		{
			m_pre2ex[hash] = exhash;
			m_ex2pre[exhash] = hash;
		}
	}

	if (entityid != 0)
	{
		UINT64 key = hash;
		key = ((key << 32) | entityid);

		auto j = m_SpecialEnemySkillIndex.find(key);
		if (j == m_SpecialEnemySkillIndex.end())
		{
			std::vector<SkillList::RowData*> newlist;
			for (auto k = templist.begin(); k != templist.end(); ++k)
			{
				SkillList::RowData* newrow = new SkillList::RowData;
				newrow->CopyFrom(*k);
				newlist.push_back(newrow);
				m_SkillTable.Table.push_back(newrow);
			}
			m_SpecialEnemySkillIndex[key].swap(newlist);
		}
		else
		{
			for (UINT32 k = 0; k < templist.size(); ++k)
			{
				if (k < j->second.size())
				{
					j->second[k]->CopyFrom(templist[k]);
				}
				else
				{
					SkillList::RowData* newrow = new SkillList::RowData;
					newrow->CopyFrom(templist[k]);
					j->second.push_back(newrow);
					m_SkillTable.Table.push_back(newrow);
				}
			}
		}
	}
	else
	{
		auto j = m_SkillIndex.find(hash);
		if (j == m_SkillIndex.end())
		{
			std::vector<SkillList::RowData*> newlist;
			for (auto k = templist.begin(); k != templist.end(); ++k)
			{
				SkillList::RowData* newrow = new SkillList::RowData;
				newrow->CopyFrom(*k);
				newlist.push_back(newrow);
				m_SkillTable.Table.push_back(newrow);
			}
			m_SkillIndex[hash].swap(newlist);
		}
		else
		{
			for (UINT32 k = 0; k < templist.size(); ++k)
			{
				if (k < j->second.size())
				{
					j->second[k]->CopyFrom(templist[k]);
				}
				else
				{
					SkillList::RowData* newrow = new SkillList::RowData;
					newrow->CopyFrom(templist[k]);
					j->second.push_back(newrow);
					m_SkillTable.Table.push_back(newrow);
				}
			}
		}
	}
	templist.clear();
}

bool XSkillEffectMgr::LoadFileSkill(const char *fileName)
{
	SkillList tmpSkillList;
	if (!tmpSkillList.LoadFile(fileName))
	{
		SSWarn << "Load file " << fileName << " failed!" << END;
		return false;
	}

	UINT32 entityTempID = 0;
	std::string skillName;
	std::vector<SkillList::RowData*> list;
	for (UINT32 i = 0; i < tmpSkillList.Table.size(); ++i)
	{
		auto &row = *(tmpSkillList.Table[i]);
		if (row.SkillScript == "")
			continue;

		if (!CheckRowValid(row))
		{
			return false;
		}

		// first
		if (skillName.empty())
		{
			skillName = row.SkillScript;
			entityTempID = row.XEntityStatisticsID;
			list.push_back(&row);
			continue;
		}
		// same skill
		if (skillName == row.SkillScript &&
			entityTempID == row.XEntityStatisticsID)
		{
			list.push_back(&row);
			continue;
		}

		// mark
		AppendRow(skillName, entityTempID, list);

		skillName = row.SkillScript;
		entityTempID = row.XEntityStatisticsID;
		list.push_back(&row);
	}

	AppendRow(skillName, entityTempID, list);

	return true;
}

bool XSkillEffectMgr::LoadFileProfSkill(const char *fileName)
{
	if (!m_ProfSkillTable.LoadFile(fileName))
	{
		SSWarn << "Load file " << fileName << " failed!" << END;
		return false;
	}

	for (unsigned i = 0; i < m_ProfSkillTable.Table.size(); ++i)
	{
		auto &row = *m_ProfSkillTable.Table[i];

		m_FixedEnmity[row.ProfID] = row.FixedEnmity;
		m_EnmityCoefficient[row.ProfID] = row.EnmityCoefficient;
	}

	return true;
}

bool XSkillEffectMgr::LoadGuildSkill(const char *fileName)
{
	if (!m_GuildSkillTable.LoadFile(fileName))
	{
		SSWarn << "Load file " << fileName << " failed!" << END;
		return false;
	}
	return true;
}

UINT32 _MakeBiasKey(UINT32 profA, UINT32 profB)
{
	return (profA % 100 << 16) | (profB % 100);
}

bool XSkillEffectMgr::LoadPvpBias(const char *fileName)
{
	m_PvpBiasProf2Damage.clear();
	if (!m_PvpProfessionBiasTable.LoadFile(fileName))
	{
		SSWarn << "Load file " << fileName << " failed!" << END;
		return false;
	}

	for (auto it = m_PvpProfessionBiasTable.Table.begin(); it != m_PvpProfessionBiasTable.Table.end(); ++it)
	{
		auto data = (*it);
		CHECK_COND_WITH_LOG_RETURN(
			m_PvpBiasProf2Damage.insert(std::pair<UINT32, float>(_MakeBiasKey(data->ProfA, data->ProfB), data->DamageScale[0])).second,
			LogError("Duplicate PvpProfessionBias %d -> %d = %f", data->ProfA, data->ProfB, data->DamageScale[0]),
			false);

		CHECK_COND_WITH_LOG_RETURN(
			m_PvpBiasProf2Damage.insert(std::pair<UINT32, float>(_MakeBiasKey(data->ProfB, data->ProfA), data->DamageScale[1])).second,
			LogError("Duplicate PvpProfessionBias %d -> %d = %f", data->ProfB, data->ProfA, data->DamageScale[1]),
			false);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int XSkillEffectMgr::GetSkillHpMaxLimit(UINT32 skillHash, int level, UINT32 casterTypeID)
{
	auto row = GetSkillConfig(skillHash, level, casterTypeID);
	if (row == NULL)
	{
		return 0;
	}

	return row->HpMaxLimit;
}

int XSkillEffectMgr::GetSkillHpMaxLimit(const std::string &skillName, int level, UINT32 casterTypeID)
{
	return GetSkillHpMaxLimit(XCommon::XHash(skillName.c_str()), level, casterTypeID);
}

int XSkillEffectMgr::GetSkillHpMinLimit(UINT32 skillHash, int level, UINT32 casterTypeID)
{
	auto row = GetSkillConfig(skillHash, level, casterTypeID);
	if (row == NULL)
	{
		return 0;
	}

	return row->HpMinLimit;
}

int XSkillEffectMgr::GetSkillHpMinLimit(const std::string &skillName, int level, UINT32 casterTypeID)
{
	return GetSkillHpMinLimit(XCommon::XHash(skillName.c_str()), level, casterTypeID);
}

int XSkillEffectMgr::GetSkillSuperArmorMax(const std::string &skillName, int level, UINT32 casterTypeID)
{
	auto row = GetSkillConfig(XCommon::XHash(skillName.c_str()), level, casterTypeID);
	if (row == NULL)
	{
		return 0;
	}

	return row->SuperArmorMax;
}

int XSkillEffectMgr::GetSkillSuperArmorMin(const std::string &skillName, int level, UINT32 casterTypeID)
{
	auto row = GetSkillConfig(XCommon::XHash(skillName.c_str()), level, casterTypeID);
	if (row == NULL)
	{
		return 0;
	}

	return row->SuperArmorMin;
}

void XSkillEffectMgr::GetSkillStartEffect(UINT32 skillHash, int skillLevel, const SkillFlags* flags, SkillStartEffect &effect, UINT32 casterTypeID, bool IsPVP)
{
	effect.IncSuperArmor = 0;
	effect.MpCost = 0;

	auto row = GetSkillConfig(skillHash, skillLevel, casterTypeID);
	if (row == NULL)
	{
		return ;
	}

	if (IsPVP)
	{
		effect.IncSuperArmor = row->PvPIncreaseSuperArmor;
	}
	else
	{
		effect.IncSuperArmor = row->IncreaseSuperArmor;
	}
	
	effect.MpCost = row->CostMP[0] + row->CostMP[1]*skillLevel;

	for(auto it = row->StartBuffID.begin(); it != row->StartBuffID.end(); ++it)
	{
		Sequence<float, 4>& data = *it;
		if (data[0] != 0 && (flags == NULL || flags->IsFlagSet((UINT32)data[3])))
		{
			BuffDesc bd;
			bd.buffID = (int)data[0];
			bd.BuffLevel = (int)data[1] == 0 ? skillLevel : (int)data[1];
			bd.delayTime = data[2];
			bd.skillID = skillHash;
			effect.Buffs.push_back(bd);
		}
	}
}

void XSkillEffectMgr::GetSkillEffect(UINT32 skillHash, int hitpoint, int skillLevel, const SkillFlags* flags, SkillEffect &effect, UINT32 casterTypeID, bool IsPVP)
{
	auto row = GetSkillConfig(skillHash, skillLevel, casterTypeID);
	if (row == NULL)
	{
		return ;
	}

	effect.DamageElementType = (DamageElement)row->Element;

	if(!IsPVP)
	{
		CHECK_COND_WITH_LOG(
			hitpoint < (int)row->PhysicalRatio.size(),
			LogError("skill %s hitpoint %d out of range PhysicalRatio", row->SkillScript.c_str(), hitpoint);
		);

		/* 读表时比较过这些数组的长度，理论上只判第一个就行了
		 
		CHECK_COND_WITH_LOG(
			hitpoint < (int)row->PhysicalFixed.size(),
			LogError("skill %s hitpoint %d out of range PhysicalFixed", row->SkillScript.c_str(), hitpoint);
		);

		CHECK_COND_WITH_LOG(
			hitpoint < (int)row->MagicRatio.size(),
			LogError("skill %s hitpoint %d out of range MagicRatio", row->SkillScript.c_str(), hitpoint);
		);

		CHECK_COND_WITH_LOG(
			hitpoint < (int)row->MagicFixed.size(),
			LogError("skill %s hitpoint %d out of range MagicFixed", row->SkillScript.c_str(), hitpoint);
		);
		*/
	}
	else
	{
		CHECK_COND_WITH_LOG(
			hitpoint < (int)row->PvPRatio.size(),
			LogError("skill %s hitpoint %d out of range PvPRatio", row->SkillScript.c_str(), hitpoint);
		);

		/* 读表时比较过这些数组的长度，理论上只判第一个就行了
		
		CHECK_COND_WITH_LOG(
			hitpoint < (int)row->PvPFixed.size(),
			LogError("skill %s hitpoint %d out of range PvPFixed", row->SkillScript.c_str(), hitpoint);
		);

		CHECK_COND_WITH_LOG(
			hitpoint < (int)row->PvPMagicRatio.size(),
			LogError("skill %s hitpoint %d out of range PvPMagicRatio", row->SkillScript.c_str(), hitpoint);
		);

		CHECK_COND_WITH_LOG(
			hitpoint < (int)row->PvPMagicFixed.size(),
			LogError("skill %s hitpoint %d out of range PvPMagicFixed", row->SkillScript.c_str(), hitpoint);
		);
		*/
	}

	if (IsPVP)
	{
		const Sequence<float, 2>& Seq1 = row->PvPRatio[hitpoint];
		effect.PhyRatio = Seq1.seq[0] + Seq1.seq[1] * skillLevel;
		const Sequence<float, 2>& Seq2 = row->PvPFixed[hitpoint];
		effect.PhyFixed = Seq2.seq[0] + Seq2.seq[1] * skillLevel;
		const Sequence<float, 2>& Seq3 = row->PvPMagicRatio[hitpoint];
		effect.MagRatio = Seq3.seq[0] + Seq3.seq[1] * skillLevel;
		const Sequence<float, 2>& Seq4 = row->PvPMagicFixed[hitpoint];
		effect.MagFixed = Seq4.seq[0] + Seq4.seq[1] * skillLevel;
		const Sequence<float, 2>& Seq5 = row->PercentDamage[hitpoint];
		effect.PercentDamage = Seq5.seq[0] + Seq5.seq[1] * skillLevel;

		if (hitpoint < (int)row->PvPDecreaseSuperArmor.size())
		{
			effect.DecSuperArmor = row->PvPDecreaseSuperArmor[hitpoint];
		}
	}
	else
	{
		const Sequence<float, 2>& Seq1 = row->PhysicalRatio[hitpoint];
		effect.PhyRatio = Seq1.seq[0] + Seq1.seq[1] * skillLevel;
		const Sequence<float, 2>& Seq2 = row->PhysicalFixed[hitpoint];
		effect.PhyFixed = Seq2.seq[0] + Seq2.seq[1] * skillLevel;
		const Sequence<float, 2>& Seq3 = row->MagicRatio[hitpoint];
		effect.MagRatio = Seq3.seq[0] + Seq3.seq[1] * skillLevel;
		const Sequence<float, 2>& Seq4 = row->MagicFixed[hitpoint];
		effect.MagFixed = Seq4.seq[0] + Seq4.seq[1] * skillLevel;
		const Sequence<float, 2>& Seq5 = row->PercentDamage[hitpoint];
		effect.PercentDamage = Seq5.seq[0] + Seq5.seq[1] * skillLevel;

		if (hitpoint < (int)row->DecreaseSuperArmor.size())
		{
			effect.DecSuperArmor = row->DecreaseSuperArmor[hitpoint];
		}
	}

	

	for (UINT32 i = 0; i < row->AddBuffPoint.size(); ++i)
	{
		if (row->AddBuffPoint[i] == hitpoint && (flags == NULL || flags->IsFlagSet((UINT32)row->BuffID[i].seq[2])))
		{
			BuffDesc bd;
			bd.buffID = row->BuffID[i].seq[0];
			bd.BuffLevel = row->BuffID[i].seq[1] == 0 ? skillLevel : row->BuffID[i].seq[1];
			bd.skillID = skillHash;
			effect.Buffs.push_back(bd);
		}
	}

	for (UINT32 i = 0; i < row->ExclusiveMask.size(); ++i)
	{
		effect.ExclusiveMask |= ( 1 << row->ExclusiveMask[i]);
	}

	if (row->DistanceScale.size() != 0)
	{
		effect.pDistanceScale = new XPieceWiseDataMgr();
		effect.pDistanceScale->Init(row->DistanceScale);
	}
}

SkillList::RowData* XSkillEffectMgr::GetEnemySpecialSkillConfig(UINT32 skillHash, int skillLevel, UINT32 casterTypeID)
{
	UINT64 key = skillHash;
	key = ((key << 32)|casterTypeID);

	auto i = m_SpecialEnemySkillIndex.find(key);
	if (i == m_SpecialEnemySkillIndex.end())
	{
		return GetSkillConfig(skillHash, skillLevel);
	}

	auto &skillList = i->second;
	for (UINT32 j = 0; j < skillList.size(); ++j)
	{
		if (skillLevel <= skillList[j]->SkillLevel)
		{
			return skillList[j];
		}
	}

	return GetSkillConfig(skillHash, skillLevel);
}

SkillList::RowData* XSkillEffectMgr::GetSkillConfig(UINT32 skillHash, int skillLevel)
{
	auto i = m_SkillIndex.find(skillHash);
	if (i == m_SkillIndex.end())
	{
		return NULL;
	}

	auto &skillList = i->second;
	for (UINT32 j = 0; j < skillList.size(); ++j)
	{
		if (skillLevel <= skillList[j]->SkillLevel)
		{
			return skillList[j];
		}
	}
	return NULL;
}

SkillList::RowData* XSkillEffectMgr::GetSkillConfig(UINT32 skillHash, int skillLevel, UINT32 casterTypeID)
{
	if (casterTypeID == 0)
	{
		return GetSkillConfig(skillHash, skillLevel);
	}
	else
	{
		return GetEnemySpecialSkillConfig(skillHash, skillLevel, casterTypeID);
	}
}

const std::string& XSkillEffectMgr::GetSkillName(UINT32 skillHash)
{
	static std::string unknowSkillName = "unknow_skill";
	auto it = m_SkillIndex.find(skillHash);
	if (it != m_SkillIndex.end() && !it->second.empty())
	{
		return it->second[0]->SkillScript;
	}

	return unknowSkillName;
}

double XSkillEffectMgr::GetSkillCostMP(UINT32 skillHash, int level, UINT32 casterTypeID)
{
	auto pRow = GetSkillConfig(skillHash, level, casterTypeID);
	if (pRow != NULL)
	{
		return pRow->CostMP[0] + pRow->CostMP[1]*level;
	}

	return 0;
}

float XSkillEffectMgr::GetAttackSpeedRatio(CombatAttribute *pCombatAttr) 
{ 
	return (float)XClamp(
		(pCombatAttr->GetAttr(TOTAL_ATTACK_SPEED) /  GetGlobalConfig().GeneralCombatParam), 
		 GetGlobalConfig().AttackSpeedLowerBound, 
		 GetGlobalConfig().AttackSpeedUpperBound); 
}

float XSkillEffectMgr::GetCDRatio(CombatAttribute *pCombatAttr)
{
	double cdRatio = pCombatAttr->GetAttr(TOTAL_SKILL_CD) /  GetGlobalConfig().GeneralCombatParam;
	return cdRatio < 0 ? 0 : (float)cdRatio;
}

float XSkillEffectMgr::GetSkillInitCDRatio(UINT32 skillHash, int level, UINT32 casterTypeID, bool IsPVP, Unit* pUnit)
{
	auto pRow = GetSkillConfig(skillHash, level, casterTypeID);

	float CDReduceRatio = 1.0f;

	float res = 0;
	CombatEffectHelper effectHelper(pUnit, skillHash);
	effectHelper.GetSkillCD(res);
	///> 技能纹章影响
	if(pUnit->IsRole())
	{

		Role* pRole = static_cast<Role*>(pUnit);
		if(pRole != NULL)
		{
			res += CEmblemMgr::Instance()->GetSkillCDRatio(skillHash, pRole);
		}
	}

	if (res != 0.0f)
	{
		CDReduceRatio *= CalcSkillCDDynamicRatio(1.0f, res);
	}

	if (pRow != NULL)
	{
		if (IsPVP)
		{
			return pRow->PvPInitCD * CDReduceRatio;
		}
		else
		{
			return pRow->InitCD * CDReduceRatio;
		}
	}

	return 0;
}

float XSkillEffectMgr::GetSkillCDDynamicRatio(Unit *pUnit, UINT32 skillHash)
{
	return CalcSkillCDDynamicRatio(GetSkillCDDynamicRatio(pUnit), GetSkillCDSemiDynamicRatio(pUnit, skillHash));
}

float XSkillEffectMgr::GetSkillCDDynamicRatio(Unit *pUnit)
{
	CombatAttribute* pCombatAttr = pUnit->GetAttrPtr();
	if(!pCombatAttr)
		return 1;

	return GetCDRatio(pCombatAttr);
}

float XSkillEffectMgr::GetSkillCDSemiDynamicRatio(Unit *pUnit, UINT32 skillHash)
{
	float res = 0;
	CombatEffectHelper effectHelper(pUnit, skillHash);
	effectHelper.GetSkillCD(res);
	///> 技能纹章影响
	if(pUnit->IsRole())
	{
		Role* pRole = static_cast<Role*>(pUnit);
		if(pRole != NULL)
		{
			res += CEmblemMgr::Instance()->GetSkillCDRatio(skillHash, pRole);
		}
	}

	return res;
}

float XSkillEffectMgr::CalcSkillCDDynamicRatio(float dynamicCDRatio, float semiDynamicCDRatio)
{
	float ratio = dynamicCDRatio + semiDynamicCDRatio;
	ratio = XClamp(ratio,  GetGlobalConfig().CDChangeLowerBound,  GetGlobalConfig().CDChangeUpperBound);

	return ratio;
}

float XSkillEffectMgr::GetSkillCDStaticRatio(UINT32 skillHash, int level, UINT32 casterTypeID, bool IsPVP)
{	
	auto pRow = GetSkillConfig(skillHash, level, casterTypeID);

	if (pRow != NULL)
	{
		if (IsPVP)
		{
			if (pRow->PvPCDRatio[0] == 0 && pRow->PvPCDRatio[1] == 0)
			{
				return 1;
			}

			return (pRow->PvPCDRatio[0] + pRow->PvPCDRatio[1]*level);
		}
		else
		{
			if (pRow->CDRatio[0] == 0 && pRow->CDRatio[1] == 0)
			{
				return 1;
			}

			return (pRow->CDRatio[0] + pRow->CDRatio[1]*level);
		}
	}

	return 1;
}

double XSkillEffectMgr::GetXULIPower(XEntity* pEntity)
{
	if (pEntity == NULL || pEntity->GetUnit() == NULL)
		return 1.0;
	return pEntity->GetUnit()->GetAttr(TOTAL_XULI) / GetGlobalConfig().GeneralCombatParam;
}

float XSkillEffectMgr::GetFixedEnmity(int prof)
{
	auto value = m_FixedEnmity.find(prof);

	if (value == m_FixedEnmity.end())
		return 0.0f;

	return value->second;
}

float XSkillEffectMgr::GetEnmityCoefficient(int prof)
{
	auto value = m_EnmityCoefficient.find(prof);

	if (value == m_EnmityCoefficient.end())
		return 1.0f;

	return value->second;
}

void XSkillEffectMgr::SetMobProperty(Unit* pMobUnit, Unit* pCaster, UINT32 skillID)
{
	if(pMobUnit == NULL || pCaster == NULL)
		return;

	pMobUnit->SetCombatUseLevel(pCaster->GetCombatUseLevel());

	CombatAttribute* pMobAttr = pMobUnit->GetAttrPtr();
	CombatAttribute* pCasterAttr = pCaster->GetAttrPtr();

	AttributeWatcher watcher(pMobUnit);
	watcher.SetAttr(BASIC_MaxHP, pCasterAttr->GetAttr(BASIC_MaxHP));
	watcher.SetAttr(BASIC_CurrentHP, pCasterAttr->GetAttr(BASIC_MaxHP));

	watcher.SetAttr(BASIC_PhysicalAtk, pCasterAttr->GetAttr(BASIC_PhysicalAtk));
	watcher.SetAttr(BASIC_MagicAtk, pCasterAttr->GetAttr(BASIC_MagicAtk));
	watcher.SetAttr(BASIC_PhysicalAtkMod, pCasterAttr->GetAttr(BASIC_PhysicalAtkMod));
	watcher.SetAttr(BASIC_MagicAtkMod, pCasterAttr->GetAttr(BASIC_MagicAtkMod));
	watcher.SetAttr(BASIC_PhysicalDefMod, pCasterAttr->GetAttr(BASIC_PhysicalDefMod));
	watcher.SetAttr(BASIC_MagicDefMod, pCasterAttr->GetAttr(BASIC_MagicDefMod));

	watcher.SetAttr(BASIC_Critical, pCasterAttr->GetAttr(BASIC_Critical));
	watcher.SetAttr(BASIC_CritDamage, pCasterAttr->GetAttr(BASIC_CritDamage));
	watcher.SetAttr(BASIC_CritResist, pCasterAttr->GetAttr(BASIC_CritResist));

	watcher.SetAttr(BASIC_FireAtk, pCasterAttr->GetAttr(BASIC_FireAtk));
	watcher.SetAttr(BASIC_FireDef, pCasterAttr->GetAttr(BASIC_FireDef));
	watcher.SetAttr(BASIC_WaterAtk, pCasterAttr->GetAttr(BASIC_WaterAtk));
	watcher.SetAttr(BASIC_WaterDef, pCasterAttr->GetAttr(BASIC_WaterDef));
	watcher.SetAttr(BASIC_LightAtk, pCasterAttr->GetAttr(BASIC_LightAtk));
	watcher.SetAttr(BASIC_LightDef, pCasterAttr->GetAttr(BASIC_LightDef));
	watcher.SetAttr(BASIC_DarkAtk, pCasterAttr->GetAttr(BASIC_DarkAtk));
	watcher.SetAttr(BASIC_DarkDef, pCasterAttr->GetAttr(BASIC_DarkDef));

	watcher.SetAttr(BASIC_Strength, pCasterAttr->GetAttr(BASIC_Strength));
	watcher.SetAttr(BASIC_Agility, pCasterAttr->GetAttr(BASIC_Agility));
	watcher.SetAttr(BASIC_Intelligence, pCasterAttr->GetAttr(BASIC_Intelligence));
	watcher.SetAttr(BASIC_Vitality, pCasterAttr->GetAttr(BASIC_Vitality));

	watcher.CheckChangedAttributeAndNotify(true);

	///> 根据技能等级加buff
	uint skillLevel = pCaster->GetSkillLevelMgr()->GetSkillLevel(skillID);
	SkillList::RowData *rowData = GetSkillConfig(skillID, skillLevel, pCaster->GetSkillCasterTypeID());
	if (rowData == NULL || (!pCaster->IsRole() && rowData->MobBuffs.size() == 0))
		return;

	XAddBuffActionArgs arg;
	//if(pCaster->IsRole())
	{
		CombatEffectHelper effectHelper(pCaster, skillID);
		effectHelper.GetSkillAddMobBuff(skillLevel, arg.vecBuffs);
	}

	{
		for (auto i = rowData->MobBuffs.begin(); i != rowData->MobBuffs.end(); ++i)
		{
			BuffDesc desc;
			desc.buffID = *i;
			desc.BuffLevel = skillLevel;
			arg.vecBuffs.push_back(desc);
		}
	}

	if(arg.vecBuffs.size() > 0)
	{
		arg.Caster = pMobUnit->GetID();
		pMobUnit->GetXObject()->DispatchAction(&arg);
	}
}

void XSkillEffectMgr::GetGuildLearnCost(UINT32 guildSkillId, UINT32 guildSkillLvl, std::map<UINT32, UINT32>& costMap)
{
	foreach(i in m_GuildSkillTable.Table)
	{
		if ((*i)->skillid == guildSkillId && (*i)->level == guildSkillLvl)
		{
			foreach (j in (*i)->need)
			{
				costMap[j->seq[0]] += j->seq[1];
			}
			return ;
		}
	}
}

void XSkillEffectMgr::GetGuildSkillAttr(const std::map<UINT32, UINT32>& guildSkillMap, std::vector<ItemChangeAttr>& attrsVec)
{
	std::map<UINT32, UINT32> attrMap;
	foreach (i in guildSkillMap)
	{
		foreach (j in m_GuildSkillTable.Table)
		{
			if ((*j)->skillid == i->first && (*j)->level == i->second)
			{
				foreach (k in (*j)->attribute)
				{
					attrMap[k->seq[0]] += k->seq[1];
				}
				break;
			}
		}
	}

	foreach (t in attrMap)
	{
		ItemChangeAttr attr(t->first, t->second);
		attrsVec.push_back(attr);
	}
}

UINT32 XSkillEffectMgr::GetExSkill(UINT32 skillhash, UINT32 casterTypeID)
{
	if(casterTypeID != 0)
	{
		auto i = m_specialEnemyPre2ex.find(((UINT64)skillhash) << 32 | casterTypeID);
		if (i != m_specialEnemyPre2ex.end())
		{
			return i->second;
		}
	}

	auto i = m_pre2ex.find(skillhash);
	if (i == m_pre2ex.end())
	{
		return 0;
	}
	return i->second;
}

UINT32 XSkillEffectMgr::GetPreSkill(UINT32 skillhash, UINT32 casterTypeID)
{
	if(casterTypeID != 0)
	{
		auto i = m_specialEnemyEx2pre.find(((UINT64)skillhash) << 32 | casterTypeID);
		if (i != m_specialEnemyEx2pre.end())
		{
			return i->second;
		}
	}

	auto i = m_ex2pre.find(skillhash);
	if (i == m_ex2pre.end())
	{
		return 0;
	}
	return i->second;
}

bool XSkillEffectMgr::IsExSkill(UINT32 skillhash, UINT32 casterTypeID)
{
	if(casterTypeID != 0)
	{
		auto i = m_specialEnemyEx2pre.find(((UINT64)skillhash) << 32 | casterTypeID);
		if (i != m_specialEnemyEx2pre.end())
		{
			return true;
		}
	}

	auto i = m_ex2pre.find(skillhash);
	if (i == m_ex2pre.end())
	{
		return false;
	}
	return true;
}

bool XSkillEffectMgr::CanChangeCD(UINT32 skillHash, int level, UINT32 casterTypeID)
{	
	auto pRow = GetSkillConfig(skillHash, level, casterTypeID);
	if (pRow == NULL)
		return false;

	return pRow->UnchangableCD == 0;
}

bool XSkillEffectMgr::AICantCast(UINT32 skillHash, int level, UINT32 casterTypeID)
{
	auto pRow = GetSkillConfig(skillHash, level, casterTypeID);
	if (pRow == NULL)
		return false;

	return pRow->LinkType == 1;
}

bool XSkillEffectMgr::CanBindSkillInSlot(UINT32 skillhash, UINT32 slot)
{
	SkillList::RowData* pRow = GetSkillConfig(skillhash, 1);
	if (pRow != NULL)
	{
		switch(pRow->SkillType)
		{
			///> 小招
		case KKSG::Skill_Normal:
			{
				if(slot >= KKSG::Skill_1_Attack && slot <= KKSG::Skill_5_Attack)
					return true;
			}break;
			///> 大招
		case KKSG::Skill_Big:
			{
				if(slot == KKSG::Ultra_Attack)
					return true;
			}break;
			///> BUFF skill
		case KKSG::Skill_Buff:
			{
				if(slot >= KKSG::Skill_1_Buff && slot <= KKSG::Skill_2_Buff)
					return true;
			}
		}
	}
	return false;
}

float XSkillEffectMgr::GetRemainingCDNotify(UINT32 skillHash, int level, UINT32 casterTypeID)
{
	SkillList::RowData* pRow = GetSkillConfig(skillHash, level, casterTypeID);
	if (pRow != NULL)
	{
		return pRow->RemainingCDNotify;
	}

	return 0;
}

int XSkillEffectMgr::GetStrengthValue(UINT32 skillHash, int level, UINT32 casterTypeID)
{
	SkillList::RowData* pRow = GetSkillConfig(skillHash, level, casterTypeID);
	if (pRow != NULL)
	{
		return pRow->StrengthValue;
	}

	return 0;
}

int XSkillEffectMgr::GetUsageCount(UINT32 skillHash, int level, UINT32 casterTypeID)
{
	SkillList::RowData* pRow = GetSkillConfig(skillHash, level, casterTypeID);
	if (pRow != NULL)
	{
		return pRow->UsageCount;
	}

	return 0;
}

float XSkillEffectMgr::GetPvpBias(Unit* pCaster, Unit* pReceiver)
{
	if (pCaster == NULL || pReceiver == NULL || !pCaster->IsRole() || !pReceiver->IsRole())
		return 1.0f;

	return GetPvpBias((UINT32)((Role*)pCaster)->GetProfession(), (UINT32)((Role*)pReceiver)->GetProfession());
}

float XSkillEffectMgr::GetPvpBias(UINT32 casterProf, UINT32 receiverProf)
{
	auto it = m_PvpBiasProf2Damage.find(_MakeBiasKey(casterProf, receiverProf));
	if (it != m_PvpBiasProf2Damage.end())
		return it->second;

	return 1.0f;
}
