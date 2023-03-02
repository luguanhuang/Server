#ifndef __SKILLTABLEMGR_H__
#define __SKILLTABLEMGR_H__

#include "SkillList.h"
#include "buff/XBuffTemplateManager.h"
#include "ProfessionSkill.h"
#include "unit/combatattribute.h"
#include "table/GuildSkillTable.h"
#include "gamelogic/itemsuffix.h"
#include "table/PvpProfessionBias.h"

class Unit;
class SkillFlags;
class XPieceWiseDataMgr;
class XEntity;

enum DamageElement
{
	DE_NONE,
	DE_FIRE = 1,
	DE_WATER = 2,
	DE_LIGHT = 3,
	DE_DARK = 4,
};

struct SkillEffect
{
	SkillEffect();
	~SkillEffect();
	DamageElement DamageElementType;
	double PhyRatio;
	double PhyFixed;
	double MagRatio;
	double MagFixed;
	double PercentDamage;
	float  DecSuperArmor;
	std::vector<BuffDesc> Buffs;
	UINT32 ExclusiveMask;
	XPieceWiseDataMgr* pDistanceScale;
};

struct SkillStartEffect
{
	int   IncSuperArmor;
	float MpCost;
	std::vector<BuffDesc> Buffs;
};

struct ProfessionSkillInfo
{
	UINT32 SkillHash;
	int    UnlockLevel;
};

class XSkillEffectMgr
{
	XSkillEffectMgr();
	~XSkillEffectMgr();
	DECLARE_SINGLETON(XSkillEffectMgr)

public:
	bool Init();
	void Uninit();
	
	bool CheckFile();
	bool LoadFile();
	void ClearFile();	

	int GetSkillHpMaxLimit(UINT32 skillHash, int level, UINT32 casterTypeID);
	int GetSkillHpMaxLimit(const std::string &skillName, int level, UINT32 casterTypeID);
	int GetSkillHpMinLimit(UINT32 skillHash, int level, UINT32 casterTypeID);
	int GetSkillHpMinLimit(const std::string &skillName, int level, UINT32 casterTypeID);
	int GetSkillSuperArmorMax(const std::string &skillName, int level, UINT32 casterTypeID);
	int GetSkillSuperArmorMin(const std::string &skillName, int level, UINT32 casterTypeID);
	double GetSkillCostMP(UINT32 skillHash, int level, UINT32 casterTypeID);
	float GetSkillCDStaticRatio(UINT32 skillHash, int level, UINT32 casterTypeID, bool IsPVP);
	float GetSkillCDDynamicRatio(Unit *pUnit, UINT32 skillHash);
	float GetSkillCDDynamicRatio(Unit *pUnit);
	float GetSkillInitCDRatio(UINT32 skillHash, int level, UINT32 casterTypeID, bool IsPVP, Unit* pUnit);
	float GetCDRatio(CombatAttribute *pCombatAttr);
	float GetAttackSpeedRatio(CombatAttribute *pCombatAttr);

	void GetSkillEffect(UINT32 skillHash, int hitpoint, int skillLevel, const SkillFlags* flags, SkillEffect &effect, UINT32 casterTypeID, bool IsPVP);
	void GetSkillStartEffect(UINT32 skillHash, int skillLevel, const SkillFlags* flags, SkillStartEffect &effect, UINT32 casterTypeID, bool IsPVP);

	SkillList::RowData* GetSkillConfig(UINT32 skillHash, int skillLevel);
	SkillList::RowData* GetSkillConfig(UINT32 skillHash, int skillLevel, UINT32 casterTypeID);
	const std::string& GetSkillName(UINT32 skillHash);

	// guild
	void GetGuildLearnCost(UINT32 guildSkillId, UINT32 guildSkillLvl, std::map<UINT32, UINT32>& costMap);
	void GetGuildSkillAttr(const std::map<UINT32, UINT32>& guildSkillMap, std::vector<ItemChangeAttr>& attrsVec);

	// ex skill
	UINT32 GetExSkill(UINT32 skillhash, UINT32 casterTypeID);
	UINT32 GetPreSkill(UINT32 skillhash, UINT32 casterTypeID);
	bool IsExSkill(UINT32 skillhash, UINT32 casterTypeID);

	// other
	float GetFixedEnmity(int prof);
	float GetEnmityCoefficient(int prof);
	void SetMobProperty(Unit* pMobUnit, Unit* pCaster, UINT32 skillID);
	bool CanChangeCD(UINT32 skillHash, int level, UINT32 casterTypeID);
	float GetSkillCDSemiDynamicRatio(Unit *pUnit, UINT32 skillHash);
	float CalcSkillCDDynamicRatio(float dynamicCDRatio, float semiDynamicCDRatio);
	bool AICantCast(UINT32 skillHash, int level, UINT32 casterTypeID);

	bool CanBindSkillInSlot(UINT32 skillhash, UINT32 slot);

	const std::vector<ProfessionSkillInfo>& GetProfessionSkill(int prof) { return m_ProfessionSkills[prof]; }
	float GetRemainingCDNotify(UINT32 skillHash, int level, UINT32 casterTypeID);
	int GetStrengthValue(UINT32 skillHash, int level, UINT32 casterTypeID);
	int GetUsageCount(UINT32 skillHash, int level, UINT32 casterTypeID);
	double GetXULIPower(XEntity* pEntity);
	float GetPvpBias(Unit* pCaster, Unit* pReceiver);
	float GetPvpBias(UINT32 casterProf, UINT32 receiverProf);
private:
	bool LoadFileSkill(const char *fileName);
	bool LoadFileProfSkill(const char *fileName);
	bool LoadGuildSkill(const char *fileName);
	bool LoadPvpBias(const char *fileName);

	bool CheckRowValid(const SkillList::RowData& row);
	void AppendRow(const std::string& skillname, UINT32 entityid, std::vector<SkillList::RowData*>& templist);

	SkillList::RowData* GetEnemySpecialSkillConfig(UINT32 skillHash, int skillLevel, UINT32 casterTypeID);
private:
	SkillList m_SkillTable;
	std::unordered_map<UINT32, UINT32> m_pre2ex;
	std::unordered_map<UINT32, UINT32> m_ex2pre;
	std::unordered_map<UINT64, UINT32> m_specialEnemyPre2ex;
	std::unordered_map<UINT64, UINT32> m_specialEnemyEx2pre;
	std::unordered_map<UINT32, std::vector<SkillList::RowData*>> m_SkillIndex;
	std::unordered_map<UINT64, std::vector<SkillList::RowData*>> m_SpecialEnemySkillIndex;

	ProfessionSkill m_ProfSkillTable;
	std::unordered_map<int, float> m_FixedEnmity;
	std::unordered_map<int, float> m_EnmityCoefficient;
	std::unordered_map<int, std::vector<ProfessionSkillInfo>> m_ProfessionSkills;

	GuildSkillTable m_GuildSkillTable;
	PvpProfessionBias m_PvpProfessionBiasTable;
	std::unordered_map<UINT32, float> m_PvpBiasProf2Damage;
};

#endif // __SKILLTABLEMGR_H__

