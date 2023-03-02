#ifndef __CombatEffect_H__
#define __CombatEffect_H__

#include "table/BuffTable.h"
#include "buff/XBuffTemplateManager.h"
#include "effectdef.h"

struct EffectDataParams;
class Scene;
class Bag;
struct SkillEffect;
class SkillFlags;
struct SkillStartEffect;
class Unit;

class CombatEffectHelper
{
public:
	CombatEffectHelper();
	CombatEffectHelper(Unit* pDefaultUnit, UINT64 casterID, BuffTable::RowData* pBuffInfo);
	CombatEffectHelper(Unit* pUnit, UINT32 skillHash);

	void SetFromBuff(Bag* pBag, BuffTable::RowData* pBuffInfo, Scene* pScene);
	void Set(Unit* pUnit, UINT32 skillHash);
	void Set(Bag* pBag, const string& skillName, UINT32 casterTypeID);
	void Set(Bag* pBag, UINT32 skillHash, UINT32 casterTypeID);

	bool HasEffect(CombatEffectType type);
	///> 最朴素的对param求和
	int GetSum(CombatEffectType type);

	bool GetBuffHP(std::vector<Sequence<float, 2>>& vecOut);
	bool GetBuffChangeAttribute(std::vector<Sequence<float, 3>>& vecOut);
	float GetBuffAuraRadius();
	float GetBuffTriggerRate();
	bool GetBuffReduceSkillCD(std::vector<Sequence<UINT32, 3>>& vecOut);
	bool GetBuffChangeSkillDamage(std::vector<Sequence<UINT32, 2>>& vecOut);
	bool GetBuffChangeDamage(double& castDamage, double&receiveDamage);
	bool GetBuffRegenerate(std::vector<Sequence<float, 3>>& vecDotOut, std::vector<Sequence<int, 2>>& vecFromCasterOut);

	bool GetSkillDamage(float& ratio);
	bool GetSkillCD(float& lfOut);
	bool GetSkillAddBuff(std::vector<BuffDesc>& vecBuffs, const SkillFlags* flags);
	bool GetSkillAddMobBuff(uint skillLevel, std::vector<BuffDesc>& vecBuffs);


	inline const EffectDataParams* GetEffectData() const { return m_pEffectData; }
	inline const BuffTable::RowData* GetBuffData() const { return m_pBuffInfo; }
	inline CombatEffectType GetCacheQueryType() const { return m_CacheType; }
	inline const void* GetCacheQueryTypeList() const { return m_CacheList; }
	inline void SetCacheQuery(CombatEffectType type, const void* list)
	{
		m_CacheType = type;
		m_CacheList = list;
	}
	inline void ClearCache()
	{
		m_CacheList = NULL;
		m_CacheType = CET_INVALID;
	}

	const BuffTable::RowData* GetTemplateBuffTable(UINT32 templateBuffID);

	template <typename T, unsigned N>
	void Merge( std::vector<Sequence<T, N> >& datas, int exclusiveIndex);

private:
	BuffTable::RowData* m_pBuffInfo;
	const EffectDataParams* m_pEffectData;
	Scene* m_pScene;
	UINT32 m_SkillHash;
	UINT32 m_ActualSkillHash;

	CombatEffectType m_CacheType;
	const void* m_CacheList;

	std::unordered_map<UINT32, BuffTable::RowData*> m_Template2BuffTable;
};

#endif