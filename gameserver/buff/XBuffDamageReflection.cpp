#include "pch.h"
#include "XBuffDamageReflection.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "foreach.h"
#include "component/XBuffComponent.h"
#include "gamelogic/XCombat.h"
#include "XBuffSpecialState.h"
#include "unit/attributewatcher.h"
#include "gamelogic/killerwatcher.h"

bool XBuffDamageReflection::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->DamageReflection == 0)
		return false;

	pBuff->AddBuffEffect(new XBuffDamageReflection(pBuffInfo->DamageReflection, pBuff));
	return true;
}

XBuffDamageReflection::XBuffDamageReflection(float ratio, XBuff* _Buff)
{
	m_Ratio = ratio;
	m_pBuff = _Buff;
}

bool XBuffDamageReflection::OnBuffEffect(const HurtInfo& rawInput, ProjectDamageResult& result)
{
	if(rawInput.m_caster == NULL || rawInput.m_target == NULL)
		return true;

	double damage = (result.GetBasicDamage() * result.m_DefOriginalRatio + result.GetTrueDamage()) * m_Ratio;

	if (damage <= 0 || ((result.m_flag & DMGFLAG_REFLECTION) != 0))
		return true;

	AttributeWatcher watcher(rawInput.m_caster->GetUnit());
	watcher.AddWatchedAttribute(BASIC_CurrentHP);

	XCombat::ProjectExternalDamage(damage, rawInput.m_target->GetID(), rawInput.m_caster, DMGFLAG_REFLECTION);

	bool bNotifyClient = (m_pBuff->GetBuffInfo()->DontShowText == false);
	watcher.CheckChangedAttributeAndNotify(true, bNotifyClient, m_pBuff->GetCaster());

	return true;
	//HurtInfo newRawInput;
	//newRawInput.m_caster = rawInput.m_target;
	//newRawInput.m_target = rawInput.m_caster;

	//ProjectDamageResult newResult;
	//newResult.m_value = damage;
	//newResult.m_CasterID = newRawInput.m_caster->GetID();

	//XCombat::ApplyBuffEffect(newRawInput, newResult, NULL);

	//newResult.m_value = XCombat::CheckChangeHPLimit(newResult.m_value, newRawInput.m_target);

	//if (newResult.m_value <= 0)
	//	return;

	//KillerWatchParam killerParam(newRawInput.m_target->GetUnit());
	//killerParam.KillerID = m_pBuff->GetCaster();
	//killerParam.KillerBuff = m_pBuff->GetID();
	//KillerWatcher::CheckKiller(killerParam, -newResult.m_value);

	//AttributeWatcher watcher(newRawInput.m_target->GetUnit());
	//watcher.AddWatchedAttribute(BASIC_CurrentHP);

	//XCombat::FireEvent(newResult.m_value, Unit::FindUnit(m_pBuff->GetCaster()), newRawInput.m_target->GetUnit(), true);
	////watcher.AddAttr(BASIC_CurrentHP, -newResult.m_value);
	//bool bNotifyClient = (m_pBuff->GetBuffInfo()->DontShowText == false);
	//watcher.CheckChangedAttributeAndNotify(true, bNotifyClient, true);
}

XBuffEffectPrioriy XBuffDamageReflection::GetPriority()
{
	return XBuffEffectPrioriy_DamageReflection;
}
