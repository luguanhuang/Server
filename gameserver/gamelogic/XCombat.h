#ifndef __XCOMBAT_H__
#define __XCOMBAT_H__

#include "buff/XBuffEnum.h"
#include "table/XSkillEffectMgr.h"
#include "entity/XActionDefine.h"
#include "combateffect.h"

enum DamageType
{
	DMG_SKILL,
	DMG_INVALID
};

enum DamageFlag
{
	DMGFLAG_NONE = 0,
	DMGFLAG_CRITICAL = 1, //暴击
	DMGFLAG_STUN = 2,
	DMGFLAG_REFLECTION = 4, // 伤害反弹
};

enum HurtDirection
{
	HURTDIR_NONE,
	HURTDIR_FRONT,
	HURTDIR_BACK,
};

enum ProjectResultType
{
	PJRES_IMMORTAL = 1,
	PJRES_BATI     = 2,
	PJRES_STUN     = 3,
	PJRES_BEHIT    = 4,

	PJRES_MAX
};

struct XResultData;
class XSkillCore;
class XEntity;
class XObject;
enum CombatAttrDef;
struct KillerWatchParam;

struct HurtInfo
{
	XEntity *m_caster; // 施放伤害的单位
	XEntity *m_target; // 被伤害的单位
	UINT32 m_skillId;
	UINT32 m_skillLevel;
	UINT64 m_skilltoken; // 技能唯一ID
	int  m_hitpoint;
	XDelegate m_callback;	//ExternalCallback
	HurtInfo();
};

struct ProjectDamageResult
{
	bool   m_Accept;
	bool   m_IsTargetDead;
	int    m_Result;
	int    m_flag;
	UINT64 m_CasterID;
	DamageElement m_elementType;
	DamageType m_type;
	double m_DefOriginalRatio;  // 除去防御的原始伤害比例，用于伤害反弹时恢复之前的伤害值

	CombatEffectHelper m_EffectHelper;

	int    m_ComboCount;

	bool IsCritical();
	void SetResult(int r) { if (r < m_Result) m_Result = r; }

	ProjectDamageResult();

	inline double GetValue() const { return m_value; }
	inline double GetTrueDamage() const { return m_TrueDamage; }
	inline double GetAbsorbDamage() const { return m_AbsorbDamage; }
	inline double GetBasicDamage() const { return m_value - m_TrueDamage; }

	void SetValue(double v);
	void SetTrueDamage(double v);
	void SetAbsorbDamage(double v);
	void ScaleValue(double ratio){ SetValue(GetValue() * std::max(ratio, 0.0)); }
private:
	double m_AbsorbDamage;
	double m_TrueDamage;
	double m_value;
};


class XCombat
{
public:
	static ProjectDamageResult ProjectDamage(const HurtInfo &hurtInfo);
	static void ProjectStart(const HurtInfo &hurtInfo);
	static void ProjectEnd(const HurtInfo &hurtInfo);
	static bool ApplyBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff);
	static bool ChangeHP(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff);
	static void Setup();
	static void Cleanup();
	static void FireEvent(double damage, Unit* pCaster, Unit* pTarget, bool bHasEnmity, UINT32 skillid);
	static double CheckChangeHPLimit(double damage, XObject* pEntity, bool bIgnoreImmortal, bool bForceCantDie);
	static double CheckChangeHPLimit(CombatAttrDef attr, double value, XObject* pEntity, bool bIgnoreImmortal, bool bForceCantDie);
	static void ProjectExternalDamage(double damage, UINT64 CasterID, XEntity* pTarget, int flag = 0);
	static bool CheckState(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff);
};

#endif // __XCOMBAT_H__