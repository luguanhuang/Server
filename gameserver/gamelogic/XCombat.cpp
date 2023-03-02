#include "pch.h"
#include <time.h>
#include "XCombat.h"
#include "entity/XEntity.h"
#include "component/XBuffComponent.h"
#include "unit/unit.h"
#include "unit/role.h"
#include "unit/combatattrdef.h"
#include "util/XRandom.h"
#include "table/XSkillEffectMgr.h"
#include "skill/XSkillCore.h"
#include "buff/XBuff.h"
#include "table/CombatCoff.h"
#include "globalconfig.h"
#include "table/CombatParamMgr.h"
#include "unit/combatattribute.h"
#include "skilllevel.h"
#include "unit/attributewatcher.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "scene/sceneconfig.h"
#include "scene/scenesecurityzone.h"
#include "scene/scenemanager.h"
#include "buff/XBuffSpecialState.h"
#include "emblemmgr.h"
#include "killerwatcher.h"
#include "entity/XActionDefine.h"
#include "scene/securitystatistics.h"
#include "component/XWoozyComponent.h"
#include "component/XSkillComponent.h"
#include "buff/XBuffReduceDamage.h"

struct DamageTime
{
	UINT32 flag;
	time_t time;
};

//static std::unordered_map<UINT64, DamageTime> SkillDamageFlag;

class SkillDamageFlagClearTimer : public ITimer
{
public:

	SkillDamageFlagClearTimer()
	{
		handler = INVALID_HTIMER;
	}

	void Start()
	{
		//handler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);
	}

	void Stop()
	{
		//if (handler != INVALID_HTIMER)
		//{
		//	CTimerMgr::Instance()->KillTimer(handler);
		//}
	}

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		//time_t now = time(NULL);
		//for (auto i = SkillDamageFlag.begin(); i != SkillDamageFlag.end();)
		//{
		//	if (now - i->second.time > 10)
		//	{
		//		SkillDamageFlag.erase(i++);
		//	}
		//	else
		//	{
		//		++i;
		//	}
		//}
	}

    HTIMER handler;
};

static SkillDamageFlagClearTimer *pClearTimer = NULL;


HurtInfo::HurtInfo()
{
	m_caster = NULL; // 施放伤害的单位
	m_target = NULL; // 被伤害的单位
	m_skillId = 0;
	m_skilltoken = 0;
}

ProjectDamageResult::ProjectDamageResult()
{
	m_Accept = true;
	m_Result = PJRES_BEHIT;
	m_flag = DMGFLAG_NONE;
	m_type = DMG_INVALID;
	m_elementType = DE_NONE;
	m_value = 0;
	m_TrueDamage = 0;
	m_AbsorbDamage = 0;
	m_IsTargetDead = false;
	m_CasterID = 0;
	m_ComboCount = 0;
}

bool ProjectDamageResult::IsCritical()
{
	return (m_flag & DMGFLAG_CRITICAL) != 0;
}

void ProjectDamageResult::SetValue(double v)
{
	if (v < m_value && m_TrueDamage > 0)
	{
		SetTrueDamage(GetTrueDamage() - (m_value - v));
	}
	m_value = v;
}

void ProjectDamageResult::SetTrueDamage(double v)
{
	m_value -= m_TrueDamage;
	m_TrueDamage = v; 
	if (m_TrueDamage < 0) 
		m_TrueDamage = 0;
	m_value += m_TrueDamage;
}

void ProjectDamageResult::SetAbsorbDamage(double v)
{
	m_value += m_AbsorbDamage;
	m_AbsorbDamage = v;
	if (GetValue() > m_AbsorbDamage)
		SetValue(GetValue() - m_AbsorbDamage);
	else
		SetValue(0);
}

typedef bool (*PrjDamageFun)(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff);

// boss放特殊技能时，玩家是否在特殊区域里面
static bool CheckBossSpecialSkill(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff)
{
	Unit *pCaster = hurtInfo.m_caster->GetUnit();
	if (pCaster->IsEnemy() && (((Enemy *)pCaster))->IsBoss())
	{
		Scene *pScene = pCaster->GetCurrScene();
		if (pScene)
		{
			ZoneEffect zeff;;
			const Vector3 &pos = hurtInfo.m_target->GetPosition_p();
			if (SceneManager::Instance()->GetSecurityZone()->IsInZone(pScene->GetSceneTemplateID(), hurtInfo.m_skillId, pos, zeff))
			{
				result.SetResult(zeff.ChangeCombatResult);

				if (zeff.ChangeCombatResult == PJRES_IMMORTAL)
				{
					result.m_Accept = false;
					return false;
				}
			}
		}
	}

	return true;
}
void SecurityStatistic(const HurtInfo& hurtInfo, const ProjectDamageResult& result);

bool XCombat::CheckState(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff)
{
	if (hurtInfo.m_target->GetUnit()->GetDeprecated())
	{
		result.m_Accept = false;
		return false;
	}

	XBuffComponent *pComponent = hurtInfo.m_target->BuffComponent();
	if (pComponent && pComponent->IsBuffStateOn(XBuffType_Immortal))
	{
		result.m_Accept = true;
		result.SetResult(PJRES_IMMORTAL);
		ApplyBuffEffect(hurtInfo, result, eff);
		return false;
	}

	if (hurtInfo.m_target->GetUnit()->GetTag() & eff->ExclusiveMask)
	{
		result.m_Accept = false;
		return false;
	}

	// 要检查无敌状态
	return true;
}

static CombatAttrDef MapElementToAttrAttack(DamageElement de)
{
	switch (de)
	{
	case DE_NONE: return TOTAL_VoidAtk;
	case DE_DARK: return TOTAL_DarkAtk;
	case DE_FIRE: return TOTAL_FireAtk;
	case DE_WATER: return TOTAL_WaterAtk;
	case DE_LIGHT: return TOTAL_LightAtk;
	default: return TOTAL_VoidAtk;
	}
}

static CombatAttrDef MapElementToAttrDefense(DamageElement de)
{
	switch (de)
	{
	case DE_NONE: return TOTAL_VoidDef;
	case DE_DARK: return TOTAL_DarkDef;
	case DE_FIRE: return TOTAL_FireDef;
	case DE_WATER: return TOTAL_WaterDef;
	case DE_LIGHT: return TOTAL_LightDef;
	default: return TOTAL_VoidDef;
	}
}

static void GetBaseDamage(Unit *pCaster, Unit *pTarget, SkillEffect &eff, ProjectDamageResult& result)
{
	auto defRow = CombatParamMgr::Instance()->GetCombatParamConf(pTarget);
	//double PhysicalBaseDamamge = pCaster->GetAttr(TOTAL_PhysicalAtkMod) - pTarget->GetAttr(TOTAL_PhysicalDefMod);
	//double MagicBaseDamage = pCaster->GetAttr(TOTAL_MagicAtkMod) - pTarget->GetAttr(TOTAL_MagicDefMod);
	double PhycialAvoidence = CombatParamMgr::Instance()->GetCombatValue(defRow->PhysicalDef, pTarget->GetAttr(TOTAL_PhysicalDefMod));
	if (PhycialAvoidence >  GetGlobalConfig().PhycialAvoidenceLimit)
	{
		PhycialAvoidence =  GetGlobalConfig().PhycialAvoidenceLimit;
	}
	double PhysicalBaseDamamge = pCaster->GetAttr(TOTAL_PhysicalAtkMod) * (1.0 - PhycialAvoidence);

	double MagicAvoidence = CombatParamMgr::Instance()->GetCombatValue(defRow->MagicDef, pTarget->GetAttr(TOTAL_MagicDefMod));
	if (MagicAvoidence >  GetGlobalConfig().MagicAvoidenceLimit)
	{
		MagicAvoidence =  GetGlobalConfig().MagicAvoidenceLimit;
	}
	double MagicBaseDamage = pCaster->GetAttr(TOTAL_MagicAtkMod) * (1.0 - MagicAvoidence);

	double DamageValue = 0;
	if (PhysicalBaseDamamge < 0)
	{
		PhysicalBaseDamamge = 0;
	}

	if (MagicBaseDamage < 0)
	{
		MagicBaseDamage = 0;
	}

	DamageValue += eff.PhyRatio * PhysicalBaseDamamge + eff.PhyFixed;
	DamageValue += eff.MagRatio * MagicBaseDamage + eff.MagFixed;

	if (DamageValue > 0)
	{
		double ValueWithoutDef = 0;
		ValueWithoutDef += eff.PhyRatio * PhysicalBaseDamamge / (1 - PhycialAvoidence) + eff.PhyFixed;
		ValueWithoutDef += eff.MagRatio * MagicBaseDamage / (1 - MagicAvoidence) + eff.MagFixed;
		result.m_DefOriginalRatio = ValueWithoutDef / DamageValue;

		auto atkRow = CombatParamMgr::Instance()->GetCombatParamConf(pCaster);

		double ElementAtk = CombatParamMgr::Instance()->GetCombatValue(atkRow->ElementAtk, pCaster->GetAttr(MapElementToAttrAttack(eff.DamageElementType)));
		double ElementDef = CombatParamMgr::Instance()->GetCombatValue(defRow->ElementDef, pTarget->GetAttr(MapElementToAttrDefense(eff.DamageElementType)));
		if (ElementAtk >  GetGlobalConfig().ElemAtkLimit)
		{
			ElementAtk =  GetGlobalConfig().ElemAtkLimit;
		}

		if (ElementDef >  GetGlobalConfig().ElemDefLimit)
		{
			ElementDef =  GetGlobalConfig().ElemDefLimit;
		}

		double FinalDamageRatio = CombatParamMgr::Instance()->GetCombatValue(atkRow->FinalDamageBase, pCaster->GetAttr(TOTAL_FinalDamage));
		if (FinalDamageRatio >  GetGlobalConfig().FinalDamageLimit)
		{
			FinalDamageRatio =  GetGlobalConfig().FinalDamageLimit;
		}

		double hurtInc = pTarget->GetAttr(TOTAL_HurtInc) /  GetGlobalConfig().GeneralCombatParam + 1.0l;
		if(hurtInc >  GetGlobalConfig().AttrChangeDamageLimit)
		{
			hurtInc =  GetGlobalConfig().AttrChangeDamageLimit;
		}

		result.SetValue(hurtInc * DamageValue * (1 + FinalDamageRatio) * (1 + ElementAtk) * (1 - ElementDef));
	}
	else
	{
		result.m_DefOriginalRatio = 1.0;
		result.SetValue(0);
	}
}

static bool IsAttackStun(Unit *pCaster)
{
	double x = pCaster->GetAttr(TOTAL_Stun);
	auto Row = CombatParamMgr::Instance()->GetCombatParamConf(pCaster);
	double dProb = x / (x + Row->StunBase);
	if (dProb >  GetGlobalConfig().StunLimit)
	{
		dProb =  GetGlobalConfig().StunLimit;
	}
	return XRandom::randDouble(0, 1) < dProb;
}

// 攻击方是否发生爆击
static bool IsAttackCritical(Unit *pCaster)
{
	double x = pCaster->GetAttr(TOTAL_Critical);
	auto Row = CombatParamMgr::Instance()->GetCombatParamConf(pCaster);
	double criticalProb = x / (x + Row->CriticalBase);
	if (criticalProb >  GetGlobalConfig().CriticalLimit)
	{
		criticalProb =  GetGlobalConfig().CriticalLimit;
	}
	return XRandom::randDouble(0, 1) < criticalProb;
}

// 被攻击方是否抵抗了爆击
static bool IsCriticalDefensed(Unit *pTarget)
{
	double x = pTarget->GetAttr(TOTAL_CritResist);
	auto Row = CombatParamMgr::Instance()->GetCombatParamConf(pTarget);
	double criticalProb = x / (x + Row->CritResistBase);
	if (criticalProb >  GetGlobalConfig().CritResistLimit)
	{
		criticalProb =  GetGlobalConfig().CritResistLimit;
	}
	return XRandom::randDouble(0, 1) < criticalProb;
}

static bool IsStunDefensed(Unit *pTarget)
{
	double x = pTarget->GetAttr(TOTAL_StunResist);
	auto Row = CombatParamMgr::Instance()->GetCombatParamConf(pTarget);
	double dProb = x / (x + Row->StunResistBase);
	if (dProb >  GetGlobalConfig().StunResistLimit)
	{
		dProb =  GetGlobalConfig().StunResistLimit;
	}
	return XRandom::randDouble(0, 1) < dProb;
}


static double GetCritialRatio(Unit *pCaster)
{
	double x = pCaster->GetAttr(TOTAL_CritDamage);
	auto Row = CombatParamMgr::Instance()->GetCombatParamConf(pCaster);
	double Ratio = x / (x + Row->CritDamageBase) + GetGlobalConfig().CritDamageBase;
	if (Ratio >  GetGlobalConfig().CritDamageUpperBound)
	{
		return  GetGlobalConfig().CritDamageUpperBound;
	}

	if (Ratio <  GetGlobalConfig().CritDamageLowerBound)
	{
		return  GetGlobalConfig().CritDamageLowerBound;
	}

	return Ratio;
}

static bool OnAddBuff(const XSkillExternalArgs* arg)
{
	XSkillExternalBuffArgs *extArgs = (XSkillExternalBuffArgs*)arg;

	XEntity* pEntity = XEntity::ValidEntity(extArgs->xTargetID);
	if(pEntity == NULL)
	{
		return true;
	}

	XAddBuffActionArgs args;
	args.singleBuff = extArgs->xBuffDesc;
	args.EffectImm = true;
	pEntity->DispatchAction(&args);
	return true;
}

static bool AddBuff(const HurtInfo &hurtInfo, const std::vector<BuffDesc> &Buffs)
{
	// 技能带的Buff效果
	if (hurtInfo.m_caster == NULL)
		return true;

	UINT32 n = Buffs.size();
	for (UINT32 i = 0; i < n; ++i)
	{
		const BuffDesc &buffDesc = Buffs[i];

		auto *buffInfo = XBuffTemplateManager::Instance()->GetBuffInfo(buffDesc.buffID, buffDesc.BuffLevel, hurtInfo.m_caster->GetUnit()->GetCurrScene());
		if (buffInfo == NULL)
		{
			continue;
		}

		XSkillExternalBuffArgs *extArgs = new XSkillExternalBuffArgs();
		extArgs->xBuffDesc.buffID = buffDesc.buffID;
		extArgs->xBuffDesc.BuffLevel = buffDesc.BuffLevel;
		extArgs->xBuffDesc.casterID = hurtInfo.m_caster->GetID();
		extArgs->xBuffDesc.skillID = hurtInfo.m_skillId;

		extArgs->delay = buffDesc.delayTime;
		extArgs->callback = OnAddBuff;
		// 作用于自己的Buff
		if (buffInfo->TargetType == 0)
		{
			extArgs->xTargetID = hurtInfo.m_caster->GetID();
		}
		// 作用于目标的Buff
		else if (buffInfo->TargetType == 1 && hurtInfo.m_target != NULL)
		{
			extArgs->xTargetID = hurtInfo.m_target->GetID();
		}

		if (extArgs->xTargetID != 0 && extArgs->delay > 0.0f)
		{
			//only can run on 64-bit machine
			if(!hurtInfo.m_callback(extArgs, (void*)(hurtInfo.m_skilltoken)))
			{
				///> 加buff期间，技能被打断。不走之后的流程了
				delete extArgs;
				return false;
			}
		}
		else
		{
			if(extArgs->xTargetID != 0)
			{
				OnAddBuff(extArgs);
			}
			delete extArgs;
		}
	}

	return true;
}

static bool CaculateDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff)
{
	XEntity *pCastEntity = hurtInfo.m_caster;
	XEntity *pTargetEntity = hurtInfo.m_target;
	Unit *pCaster = pCastEntity->GetUnit();
	Unit *pTarget = pTargetEntity->GetUnit();
	
	UINT32 flag = 0;
	SkillEffect &effect = *eff;
	//auto it = SkillDamageFlag.find(hurtInfo.m_skilltoken);
	//if (it != SkillDamageFlag.end())
	//{
	//	flag = it->second.flag;
	//}

	// 计算伤害
	GetBaseDamage(pCaster, pTarget, effect, result);
	//if (flag & DMGFLAG_CRITICAL)
	{
		if (!IsAttackCritical(pCaster) || IsCriticalDefensed(pTarget))
		{
			//result.m_value = DamageValue;
		}
		else
		{
			result.ScaleValue(GetCritialRatio(pCaster));
			result.m_flag |= DMGFLAG_CRITICAL;
		}
	}
	//else
	{
		//result.m_value = DamageValue;
	}

	//if (flag & DMGFLAG_STUN)
	{
		if (IsAttackStun(pCaster) && !IsStunDefensed(pTarget))
		{
			result.m_flag |= DMGFLAG_STUN;
		}
	}

	AddBuff(hurtInfo, effect.Buffs);
	
	result.m_elementType = eff->DamageElementType;

	double dBuffInc = 1.0l;
	double dBuffDec = 1.0l;
	double dSkillSpecified = 1.0l;
	if (hurtInfo.m_caster->BuffComponent() != NULL)
	{
		dBuffInc += hurtInfo.m_caster->BuffComponent()->ModifySkillCastDamage();
		dSkillSpecified += hurtInfo.m_caster->BuffComponent()->ChangeSkillDamage(hurtInfo.m_skillId);
	}
	if (hurtInfo.m_target->BuffComponent() != NULL)
	{
		dBuffInc += hurtInfo.m_target->BuffComponent()->ModifyIncReceivedDamage();
		dBuffDec += hurtInfo.m_target->BuffComponent()->ModifyDecReceivedDamage();
	}

	result.ScaleValue(dBuffInc * dBuffDec * dSkillSpecified);

	///> 根据距离缩放
	if (effect.pDistanceScale != NULL)
	{
		double dis = (double)Vector3::Distance(pCastEntity->GetPosition_p(), pTargetEntity->GetPosition_p());
		double s = effect.pDistanceScale->GetData(dis);
		result.ScaleValue(s);
	}

	///> 职业克制
	if (pCaster->GetCurrScene())
		result.ScaleValue(pCaster->GetCurrScene()->GetPvpProfessionBias(pCaster, pTarget));

	// 随机性
	double v = XRandom::randDouble(GetGlobalConfig().DamageRandomLowerBound, GetGlobalConfig().DamageRandomUpperBound);
	result.ScaleValue(v);

	if (NULL != pTarget->GetCurrScene())
	{
		result.ScaleValue(pTarget->GetCurrScene()->GetSceneInfo()->GetHurtCoef());
	}

	double trueDamage = pCaster->GetAttr(TOTAL_TrueDamage);

	// 百分比伤害
	if(eff->PercentDamage > 0.00001)
	{
		trueDamage += (pTarget->GetAttr(TOTAL_MaxHP) * eff->PercentDamage);
	}

	result.SetTrueDamage(trueDamage);

	if (result.GetValue() < 1)
	{
		result.SetValue(1);
	}

	return true;
}

bool XCombat::ApplyBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff)
{
	XBuffComponent *pCasterComp = NULL;
	XBuffComponent *pTargetComp = NULL;
	if(hurtInfo.m_caster != NULL)
		pCasterComp = hurtInfo.m_caster->BuffComponent();
	if(hurtInfo.m_target != NULL)
		pTargetComp = hurtInfo.m_target->BuffComponent();

	if (pCasterComp == NULL && pTargetComp == NULL)
		return true;


	XBuff::EffectEnumeratorPriorityCur = XBuffEffectPrioriy_START;
	XBuff::EffectEnumeratorPriorityNext = XBuffEffectPrioriy_END;

	while (XBuff::EffectEnumeratorPriorityCur != XBuffEffectPrioriy_END)
	{
		if(pTargetComp != NULL)
			pTargetComp->OnHurt(hurtInfo, result);
		if(pCasterComp != NULL)
			pCasterComp->OnCastDamage(hurtInfo, result);

		XBuff::EffectEnumeratorPriorityCur = XBuff::EffectEnumeratorPriorityNext;
		XBuff::EffectEnumeratorPriorityNext = XBuffEffectPrioriy_END;
	}

	return true;
}

static bool CaculateSuperArmorChange(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff)
{
	Unit *pCaster = hurtInfo.m_caster->GetUnit();
	Unit *pTarget = hurtInfo.m_target->GetUnit();

	XWoozyComponent* pWoozyComp = static_cast<XWoozyComponent*>(hurtInfo.m_target->GetXComponent(XWoozyComponent::uuID));
	if (pWoozyComp != NULL && pWoozyComp->InTransfer())
	{
		result.SetResult(PJRES_BATI);
		return true;
	}

	if (pTarget->GetAttr(TOTAL_MaxSuperArmor) > 0 && pTarget->IsArmorBroken())
	{
		return true;
	}

	double SAAtk = pCaster->GetAttr(TOTAL_SuperArmorAtk);
	double SADef = pTarget->GetAttr(TOTAL_SuperArmorDef);
	double SADmg = SAAtk * eff->DecSuperArmor - SADef;
	if (SADmg <= 0)
	{
		result.SetResult(PJRES_BATI);
		return true;
	}

	if (pTarget->GetAttr(TOTAL_MaxSuperArmor) <= 0)
	{
		return true;
	}

	if (pTarget->IsArmorBroken() == false)
	{
		double oldBati = pTarget->GetAttr(BASIC_CurrentSuperArmor);
		pTarget->AddAttr(BASIC_CurrentSuperArmor, -SADmg);
		pTarget->CheckAttrState();

		if (pTarget->GetAttr(BASIC_CurrentSuperArmor) > 0)
		{
			result.SetResult(PJRES_BATI);
		}
		else
		{
			if (pTarget->IsEnemy())
			{
				Enemy *pEnemy = (Enemy *)pTarget;
				if (pEnemy->GetTemplate()->WeakStatus && oldBati > 0)
				{
					result.SetResult(PJRES_BATI);
				}
			}
		}
	}

	return true;
}

static double _CheckChangeHPLimit(CombatAttrDef attr, double deltaValue, bool bIsDamage, XObject* pEntity, bool bIgnoreImmortal, bool bForceCantDie)
{
	if (bIsDamage)
		return -XBuffSpecialState::GetActualChangeAttr(attr, -deltaValue, pEntity, bIgnoreImmortal, bForceCantDie);
	else
		return XBuffSpecialState::GetActualChangeAttr(attr, deltaValue, pEntity, bIgnoreImmortal, bForceCantDie);
}

double XCombat::CheckChangeHPLimit(double damage, XObject* pEntity, bool bIgnoreImmortal, bool bForceCantDie)
{
	return _CheckChangeHPLimit(BASIC_CurrentHP, damage, true, pEntity, bIgnoreImmortal, bForceCantDie);
}
double XCombat::CheckChangeHPLimit(CombatAttrDef attr, double value, XObject* pEntity, bool bIgnoreImmortal, bool bForceCantDie)
{
	return _CheckChangeHPLimit(attr, value, false, pEntity, bIgnoreImmortal, bForceCantDie);
}

bool XCombat::ChangeHP(const HurtInfo &hurtInfo, ProjectDamageResult &result, SkillEffect *eff)
{
	if (hurtInfo.m_target == NULL)
		return true;

	Unit *pUnit = hurtInfo.m_target->GetUnit();

	if (pUnit->GetAttr(TOTAL_CurrentXULI) > 0)
	{
		pUnit->AddAttr(BASIC_CurrentXULI, -result.GetValue());

		if (pUnit->GetAttr(BASIC_CurrentXULI) <= 0)
		{
			XStrengthPresevationStopArgs a;
			hurtInfo.m_target->DispatchAction(&a);
		}
	}

	//result.m_value = XCombat::CheckChangeHPLimit(result.m_value, hurtInfo.m_target);

	KillerWatchParam killerParam(pUnit);
	killerParam.KillerID = result.m_CasterID;
	killerParam.KillerSkill = hurtInfo.m_skillId;
	KillerWatcher::CheckKiller(killerParam, -result.GetValue());

	//pUnit->AddAttr(BASIC_CurrentHP, -result.m_value);
	//pUnit->CheckAttrState();

	//pCastUnit->OnAttack(pUnit, result.m_value);
	//pUnit->OnHurt(pCastUnit, result.m_value);

	//XEnmityEventArgs oArgs;
	//oArgs.m_value = (float)result.m_value;
	//oArgs.m_caster = pCastUnit->GetID();
	//pUnit->GetXObject()->DispatchAction(&oArgs);
	XCombat::FireEvent(result.GetValue(), hurtInfo.m_caster == NULL ? NULL : hurtInfo.m_caster->GetUnit(), pUnit, true, hurtInfo.m_skillId);

	if (hurtInfo.m_caster != NULL && hurtInfo.m_caster->GetUnit()->IsRole())
	{
		Role* pRole = static_cast<Role*>(hurtInfo.m_caster->GetUnit());
		if (pRole != NULL)
			result.m_ComboCount = pRole->GetStatistics().GetCombo();
	}

	return true;
}

void XCombat::FireEvent(double damage, Unit* pCaster, Unit* pTarget, bool bHasEnmity, UINT32 skillid)
{
	pTarget->AddAttr(BASIC_CurrentHP, -damage);
	pTarget->CheckAttrState();

	if(pCaster != NULL)
	{	
		pCaster->OnAttack(pTarget, damage);
	}
	
	pTarget->OnHurt(pCaster, damage);

	if(bHasEnmity && pCaster != NULL)
	{
		XEnmityEventArgs oArgs;
		oArgs.m_value = (float)damage;
		oArgs.m_caster = pCaster->GetID();
		oArgs.m_skillid = skillid;
		pTarget->GetXObject()->DispatchAction(&oArgs);
	}
}

static PrjDamageFun DamageHandler[] = {
	&XCombat::CheckState,
	&CheckBossSpecialSkill,
	&CaculateDamage,
	&CaculateSuperArmorChange,
	&XCombat::ApplyBuffEffect,
	&XCombat::ChangeHP,
	NULL,
};

ProjectDamageResult XCombat::ProjectDamage(const HurtInfo &hurtInfo)
{
	ProjectDamageResult result;

	XEntity *pCastEntity = hurtInfo.m_caster;
	Unit *pCaster = pCastEntity->GetUnit();
	result.m_CasterID = hurtInfo.m_caster->GetID();

	SkillEffect effect;
	int skillLevel = pCaster->GetSkillLevelMgr()->GetSkillLevel(hurtInfo.m_skillId);
	XSkillEffectMgr::Instance()->GetSkillEffect(hurtInfo.m_skillId, 
		hurtInfo.m_hitpoint, skillLevel, pCaster->GetSkillLevelMgr()->GetSkillFlags(), effect, pCaster->GetSkillCasterTypeID(),
		Scene::IsPVPCombat(pCaster->GetCurrScene()));

	float ratio = 0.0f;
	result.m_EffectHelper.Set(pCaster, hurtInfo.m_skillId);
	result.m_EffectHelper.GetSkillDamage(ratio);

	if(pCaster->IsRole())
	{
		Role* pRole = static_cast<Role*>(pCaster);
		ratio += CEmblemMgr::Instance()->GetSkillEmblemEffect(hurtInfo.m_skillId, pRole);
	}

	effect.PhyRatio *= (1.0 + ratio);
	effect.MagRatio *= (1.0 + ratio);

	AttributeWatcher watcher1(hurtInfo.m_caster->GetUnit());
	AttributeWatcher watcher2(hurtInfo.m_target->GetUnit());
	watcher1.AddWatchedAttribute(BASIC_CurrentHP);
	watcher1.AddWatchedAttribute(BASIC_CurrentMP);
	watcher1.AddWatchedAttribute(BASIC_CurrentSuperArmor);
	watcher1.AddWatchedAttribute(BASIC_CurrentXULI);

	watcher2.AddWatchedAttribute(BASIC_CurrentHP);
	watcher2.AddWatchedAttribute(BASIC_CurrentMP);
	watcher2.AddWatchedAttribute(BASIC_CurrentSuperArmor);
	watcher2.AddWatchedAttribute(BASIC_CurrentXULI);

	for (int i = 0; DamageHandler[i] != NULL; ++i)
	{
		if (!DamageHandler[i](hurtInfo, result, &effect))
		{
			break;
		}
	}

	watcher1.CheckChangedAttributeAndNotify(true);
	watcher2.CheckChangedAttributeAndNotify(true);

	{
		XAISkillHurtEventArgs oArgs;
		oArgs.SkillId = hurtInfo.m_skillId;
		oArgs.IsCaster = true;
		if (hurtInfo.m_caster)oArgs.CasterUid = hurtInfo.m_caster->GetID();
		if (hurtInfo.m_target) oArgs.TargetUid = hurtInfo.m_target->GetID();
		pCaster->GetXObject()->DispatchAction(&oArgs);
	}

	if (hurtInfo.m_target && hurtInfo.m_target->GetUnit())
	{
		XAISkillHurtEventArgs oArgs;
		oArgs.SkillId = hurtInfo.m_skillId;
		oArgs.IsCaster = false;
		if (hurtInfo.m_caster)oArgs.CasterUid = hurtInfo.m_caster->GetID();
		if (hurtInfo.m_target) oArgs.TargetUid = hurtInfo.m_target->GetID();
		hurtInfo.m_target->GetUnit()->GetXObject()->DispatchAction(&oArgs);
	}

	result.m_IsTargetDead = (hurtInfo.m_target->GetUnit()->GetAttr(BASIC_CurrentHP) <= 0);


	if(result.m_Accept)
	{
		// Send Event to firer, for buff trigger, etc.
		XProjectDamageArgs arg(&hurtInfo, &result);
		pCastEntity->DispatchAction(&arg);

		SecurityStatistic(hurtInfo, result);
	}

	return result;
}

void XCombat::ProjectStart(const HurtInfo &hurtInfo)
{
	SkillStartEffect eff;
	Unit *pCaster = hurtInfo.m_caster->GetUnit();
	int skillLevel = pCaster->GetSkillLevelMgr()->GetSkillLevel(hurtInfo.m_skillId);
	XSkillEffectMgr::Instance()->GetSkillStartEffect(hurtInfo.m_skillId, skillLevel, pCaster->GetSkillLevelMgr()->GetSkillFlags(),
		eff, pCaster->GetSkillCasterTypeID(),
		Scene::IsPVPCombat(pCaster->GetCurrScene()));

	//if (pCaster->IsRole())
	{
		CombatEffectHelper effectHelper(pCaster, hurtInfo.m_skillId);
		effectHelper.GetSkillAddBuff(eff.Buffs, pCaster->GetSkillLevelMgr()->GetSkillFlags());
	}
	AttributeWatcher watcher(pCaster);
	if (eff.IncSuperArmor > 0)
	{
		watcher.AddAttr(BASIC_CurrentSuperArmor, eff.IncSuperArmor);
	}

	double cost = eff.MpCost;
	if (cost > 0)
	{
		cost *= ( 1.0 + hurtInfo.m_caster->BuffComponent()->ModifySkillCost());
	}
	watcher.AddAttr(BASIC_CurrentMP, -cost);
	watcher.CheckChangedAttributeAndNotify(true);

	if(hurtInfo.m_caster->BuffComponent() != NULL)
	{
		hurtInfo.m_caster->BuffComponent()->OnCastSkill(hurtInfo);
	}
	if (hurtInfo.m_caster->SkillComponent() != NULL)
	{
		if (hurtInfo.m_caster->SkillComponent()->CurrentSkill() == NULL || 
			hurtInfo.m_caster->SkillComponent()->CurrentSkill()->MainCore() == NULL || 
			hurtInfo.m_caster->SkillComponent()->CurrentSkill()->MainCore()->ID() != hurtInfo.m_skillId)
			return;
	}
	if (!eff.Buffs.empty())
	{
		//for (unsigned j = 0; j < eff.Buffs.size(); ++j)
		//{
		//	LogDebug("Add ProjectStart Buff (%d %d)", eff.Buffs[j].buffID, eff.Buffs[j].BuffLevel);
		//}
		///> 加buff期间，技能打断，不走后续流程
		if (!AddBuff(hurtInfo, eff.Buffs))
			return;
	}

	//UINT32 flag = 0;
	//if (IsAttackCritical(pCaster))
	//{
	//	flag |= DMGFLAG_CRITICAL;
	//}

	//if (IsAttackStun(pCaster))
	//{
	//	flag |= DMGFLAG_STUN;
	//}

	//DamageTime dt;
	//dt.flag = flag;
	//dt.time = time(NULL);
	//SkillDamageFlag[hurtInfo.m_skilltoken] = dt;

	XAIStartSkillEventArgs oArgs;
	oArgs.SkillId = hurtInfo.m_skillId;
	oArgs.IsCaster = true;
	pCaster->GetXObject()->DispatchAction(&oArgs);
}

void XCombat::ProjectEnd(const HurtInfo &hurtInfo)
{
	SkillStartEffect eff;
	Unit *pCaster = hurtInfo.m_caster->GetUnit();
	int skillLevel = pCaster->GetSkillLevelMgr()->GetSkillLevel(hurtInfo.m_skillId);
	XSkillEffectMgr::Instance()->GetSkillStartEffect(hurtInfo.m_skillId, skillLevel, pCaster->GetSkillLevelMgr()->GetSkillFlags(),
		eff, pCaster->GetSkillCasterTypeID(),
		Scene::IsPVPCombat(pCaster->GetCurrScene()));

	if (eff.IncSuperArmor > 0 && 
		(pCaster->GetUnitType() == KKSG::Category_Role ||
		 pCaster->GetUnitType() == KKSG::Category_DummyRole))
	{
		AttributeWatcher watcher(pCaster);
		watcher.AddAttr(BASIC_CurrentSuperArmor, -eff.IncSuperArmor);
		watcher.CheckChangedAttributeAndNotify(true);
	}

	{
		XAIEndSkillEventArgs oArgs;
		oArgs.SkillId = hurtInfo.m_skillId;
		oArgs.IsCaster = true;
		pCaster->GetXObject()->DispatchAction(&oArgs);
	}
}

void XCombat::Setup()
{
	//if (pClearTimer == NULL)
	//{
	//	pClearTimer = new SkillDamageFlagClearTimer();
	//	pClearTimer->Start();
	//}
}

void XCombat::Cleanup()
{
	//if (pClearTimer != NULL)
	//{
	//	pClearTimer->Stop();
	//	delete pClearTimer;
	//}
}

void XCombat::ProjectExternalDamage(double damage, UINT64 CasterID, XEntity* pTarget, int flag)
{
	if (pTarget == NULL)
		return;

	HurtInfo rawInput;
	Unit* pCastetUnit = Unit::FindUnit(CasterID);
	if(pCastetUnit == NULL)
		rawInput.m_caster = NULL;
	else
		rawInput.m_caster = dynamic_cast<XEntity*>(pCastetUnit->GetXObject());
	rawInput.m_target = pTarget;

	ProjectDamageResult result;

	result.SetValue(damage);
	result.m_CasterID = CasterID;
	result.m_flag |= flag;
	result.m_DefOriginalRatio = 1;

	//AttributeWatcher watcher1(pCastetUnit);
	//AttributeWatcher watcher2(rawInput.m_target->GetUnit());
	//if(pCastetUnit != NULL)
	//{	
	//	watcher1.AddWatchedAttribute(BASIC_CurrentHP);
	//	watcher1.AddWatchedAttribute(BASIC_CurrentMP);
	//	watcher1.AddWatchedAttribute(BASIC_CurrentSuperArmor);
	//	watcher1.AddWatchedAttribute(BASIC_CurrentXULI);
	//}

	//watcher2.AddWatchedAttribute(BASIC_CurrentHP);
	//watcher2.AddWatchedAttribute(BASIC_CurrentMP);
	//watcher2.AddWatchedAttribute(BASIC_CurrentSuperArmor);
	//watcher2.AddWatchedAttribute(BASIC_CurrentXULI);

	///> 职业克制
	if (pTarget->GetUnit()->GetCurrScene())
		result.ScaleValue(pTarget->GetUnit()->GetCurrScene()->GetPvpProfessionBias(pCastetUnit, pTarget->GetUnit()));

	XCombat::ApplyBuffEffect(rawInput, result, NULL);
	XCombat::ChangeHP(rawInput, result, NULL);

	//if(pCastetUnit != NULL)
	//{	
	//	watcher1.CheckChangedAttributeAndNotify(true);
	//}
	//watcher2.CheckChangedAttributeAndNotify(true);
	SecurityStatistic(rawInput, result);
}

void SecurityStatistic(const HurtInfo& hurtInfo, const ProjectDamageResult& result)
{
	if (hurtInfo.m_caster)
	{
		Unit* pCasterUnit = hurtInfo.m_caster->GetUnit();
		if(pCasterUnit->GetSecurityStatistics() != NULL)
			pCasterUnit->GetSecurityStatistics()->OnCastDamage(hurtInfo, result);
	}

	if(hurtInfo.m_target)
	{
		if (hurtInfo.m_target->GetUnit()->GetSecurityStatistics() != NULL)
			hurtInfo.m_target->GetUnit()->GetSecurityStatistics()->OnReceiveDamage(hurtInfo, result);
	}
}