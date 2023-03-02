#ifndef __XBUFF_H__
#define __XBUFF_H__

#include "XBuffEnum.h"
#include "table/BuffTable.h"
#include "unit/combatattrdef.h"

class  XEntity;
struct HurtInfo;
struct ProjectDamageResult;
class XBuff;
class XRealDeadActionArgs;
class XOnQTEOnEventArgs;
struct BuffDesc;
class CombatEffectHelper;

class BuffEffectData
{
public:
	BuffEffectData()
		: MobID(0)
		, bReduceCD(false)
		, TransformID(0)
	{

	}
public:
	UINT64 MobID;
	bool bReduceCD;
	INT32 TransformID;
};

class BuffExclusive
{
	//std::unordered_set<int> _ExclusiveTypes;
	std::unordered_set<int> _ClearTypes;
	std::unordered_set<int> _SingleEffect;
public:
	//enum SingleEffectType
	//{
	//	SET_Transform,
	//	SET_Scale,
	//};

public:
	void Set(const BuffTable::RowData* pRowData);

	bool CanAdd(int clearType) const;
	//bool IsExcluded(const BuffTable::RowData* pRowData) const;

	//bool IsExcluded(const std::vector<int>& other) const;

	bool IsSingleEffectConflict(const BuffExclusive& other) const;

	static bool Overlaped(const std::unordered_set<int>& left, const std::unordered_set<int>& right);

	bool ShouldClear(int clearType) const;
};

class BuffEffect
{
public:
	virtual ~BuffEffect(){}
	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper){}
	virtual void OnRemove(XBuffRemoveReason reason){}
	virtual void OnAppend(XEntity*pEntity) {}
	virtual bool OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result){ return true; }
	virtual bool OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result){ return true; }
	virtual bool OnCastSkill(const HurtInfo &hurtInfo){ return true; }
	virtual void OnComboChange(UINT32 comboCount) { }
	virtual void OnAttributeChanged(CombatAttrDef attr) {}
	virtual void OnQTEStateChanged(const XOnQTEOnEventArgs* arg) {}
	virtual void OnRealDead(const XRealDeadActionArgs* arg) {}
	virtual bool CanBuffAdd(BuffTable::RowData * pBuffData){ return true; }
	virtual void OnUpdate() { }
	virtual void OnBattleEnd() { }
	virtual void OnResetTime() { }
	virtual XBuffEffectPrioriy GetPriority() { return XBuffEffectPrioriy_Default; }

	inline bool GetValid() { return m_bValid; }
	inline void SetValid(bool valid) { m_bValid = valid; }
private:
	bool m_bValid;
};

struct SkillCostModifyEffect
{
	double CostModify;
	//double CoolDownModify;
	double CastDamageModify;
	double IncReceivedDamageModify;
	double DecReceivedDamageModify;

	SkillCostModifyEffect()
	{
		CostModify = 0;
		//CoolDownModify = 1;
		CastDamageModify = 0;
		IncReceivedDamageModify = 0;
		DecReceivedDamageModify = 0;
	}

	bool TryCreate(CombatEffectHelper* pEffectHeler, XBuff *pBuff);

	void SetChangeSkillDamage(CombatEffectHelper* pEffectHeler);
	double GetChangeSkillDamage(uint skillID);

	void OnRemove(XBuff* pBuff);
	void OnBattleEnd(XBuff* pBuff);
private:
	std::map<UINT32, double> ChangeSkillDamage;

	void _RefreshSecurity(XBuff* pBuff);
};

#define MAX_BUFFEFFECT_COUNT 10

typedef void(*BuffEffectCB)(BuffEffect* pBuffEffect, const HurtInfo& rHurtInfo, ProjectDamageResult& rResult);

class XBuff
{
public:
	XBuff();
	virtual ~XBuff();

	void Init(const BuffDesc& buffDesc, CombatEffectHelper* pEffectHeler);

	void AddBuffTime(UINT32 extendTime);
	UINT32 GetLeftTime() const;
	void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHeler);
	bool Append(CombatEffectHelper* pEffectHeler);
	void OnAppend();
	bool ResetBuffTime();
	void OnRemove(XBuffRemoveReason reason);
	void OnAttributeChanged(CombatAttrDef attr);
	void OnQTEStateChanged(const XOnQTEOnEventArgs* arg);
	void OnRealDead(const XRealDeadActionArgs *Args);
	void OnBattleEnd();
	bool OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result, BuffEffectCB func);
	bool OnCastSkill(const HurtInfo &hurtInfo);
	bool OnComboChange(UINT32 comboCount);
	bool CanBuffAdd(BuffTable::RowData* pBuffData);
	static bool CanBuffAdd(XEntity* pEntity, const BuffTable::RowData* pRowData);
	bool OnBuffTimeEnd(IArgs *, void *);
	void OnUpdate();
	void OnResetTime();

	inline bool IsValid() { return m_valid; }
	inline XEntity *GetEntity() { return m_Entity; }
	inline int GetID() const { return m_BuffInfo->BuffID; }
	inline int GetLevel() const { return m_BuffInfo->BuffLevel; }
	inline UINT32 GetDuration() { return m_TimeDuration; }
	inline UINT32 GetOriginalDuration() { return m_OriginalDuration; }
	inline int GetTargetType() { return m_BuffInfo->TargetType; }
	inline XBuffMergeType GetMergeType() { return (XBuffMergeType)m_BuffInfo->BuffMergeType; }
	inline BuffTable::RowData *GetBuffInfo() {return m_BuffInfo;}
	inline int GetClearType() { return m_BuffInfo->BuffClearType; }
	inline const BuffEffectData& GetEffectData() const { return m_EffectData; }
	inline BuffEffectData* GetEffectData() { return &m_EffectData; }
	inline double GetHP() const { return m_HP; }
	inline double GetMaxHP() const { return m_MaxHP; }
	inline UINT32 GetStackCount() const { return m_StackCount; }
	inline UINT32 GetSkillID() const { return m_SkillID; }
	inline UINT32 GetType() const { return m_Type; }
	inline UINT64 GetCaster() const { return m_Caster; }
	inline bool IsMustNotify() const { return m_bMustNotify; }
	inline const std::unordered_set<UINT32>* GetRelevantSkills() const { return m_RelevantSkills; }
	inline BuffExclusive& GetExclusive() { return m_Exclusive; }
	void SetCaster(UINT64 caster);

	//double ModifySkillCoolDown();
	double ModifySkillCost();
	double ModifySkillCastDamage();
	double ModifyIncReceivedDamage();
	double ModifyDecReceivedDamage();
	double ChangeSkillDamage(UINT32 skillID);

	double ChangeBuffHP(double deltaHP);
	static XBuffEffectPrioriy EffectEnumeratorPriorityCur;
	static XBuffEffectPrioriy EffectEnumeratorPriorityNext;
	static void OnHurt(BuffEffect* pBuffEffect, const HurtInfo& rHurtInfo, ProjectDamageResult& rResult);
	static void OnCastDamage(BuffEffect* pBuffEffect, const HurtInfo& rHurtInfo, ProjectDamageResult& rResult);
	bool HasTag(XBuffTag tag);
	void ReApplyChangeAttributeEffect(CombatEffectHelper* pEffectHeler);

	UINT32 GetActualDuration();


	void AddBuffEffect(BuffEffect *eff);

protected:
	void _AppendHP(const Sequence<float, 2>& seq);
	void _InitHP(CombatEffectHelper* pEffectHeler);
	void _AddBuffEffect(CombatEffectHelper* pEffectHeler, bool bIncludeTrigger = true);
	void _SortEffect();
	bool     m_valid;
	UINT64   m_Caster;
	UINT32   m_TimeDuration;
	UINT32	 m_OriginalDuration;	///> 原始的持续时间，MergeType为ExtendTime时，使用此时间增加
	UINT64	 m_StartTime;
	HTIMER   m_TimerToken;
	XEntity *m_Entity;
	BuffTable::RowData *m_BuffInfo;
	BuffEffect *m_BuffEffect[MAX_BUFFEFFECT_COUNT];
	SkillCostModifyEffect m_SkillModify;
	BuffEffectData m_EffectData;
	double	 m_HP;
	double	 m_MaxHP;
	UINT32	 m_StackCount;
	int m_EffectEnumeratorIndex;
	UINT32	 m_SkillID;
	UINT32	 m_Type;	///> Buff来源类型，0：普通；1：Doodad
	std::unordered_set<UINT32>* m_RelevantSkills;
	bool m_bMustNotify;	///> 客户端需要特效或者逻辑，必须同步此Buff
	BuffExclusive m_Exclusive;
};

#endif // __XBUFF_H__