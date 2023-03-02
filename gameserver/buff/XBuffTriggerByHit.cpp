#include "pch.h"
#include "XBuffTriggerByHit.h"
#include "gamelogic/XCombat.h"
#include "entity/XObject.h"
#include "unit/unit.h"
#include "entity/XEntity.h"
#include "XBuffTrigger.h"
#include "skill/XSkillMgr.h"
#include "util/XCommon.h"
#include "component/XBuffComponent.h"


XBuffTriggerByHit::XBuffTriggerByHit(XBuff *pBuff)
	: XBuffTrigger(pBuff)
	, m_pRawInput(NULL)
	, m_pResult(NULL)
{
	m_Type = _GetTriggerParamInt(pBuff->GetBuffInfo(), 0);
	m_SpecifiedSkillsSet = pBuff->GetRelevantSkills();

	switch(m_Type)
	{
	case 2:
		{
			m_CriticalChangeRatio = std::max(0.0, 1.0 + _GetTriggerParam(pBuff->GetBuffInfo(), 1) / 100);
		}
		break;
	case 3:
		{
			m_KillType = _GetTriggerParamInt(pBuff->GetBuffInfo(), 1);
		}
		break;
	//case 4:
	//	{
	//		for (int i = 1;; ++i)
	//		{
	//			string skillScript = _GetTriggerParamStr(pBuff->GetBuffInfo(), i);
	//			if (skillScript.empty())
	//				break;
	//			m_SpecifiedSkillsSet.insert(XCommon::XHash(skillScript.c_str()));
	//		}
	//	}
	//	break;
	case 5:
		{
			m_SpecifiedBuffID = _GetTriggerParamInt(pBuff->GetBuffInfo(), 1);
		}
		break;
	default:
		break;
	}
}

XBuffTriggerByHit::~XBuffTriggerByHit()
{

}

bool XBuffTriggerByHit::OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	if(hurtInfo.m_skillId == 0)
		return true;

	m_pResult = &result;
	m_pRawInput = &hurtInfo;
	Trigger();
	return true;
}

bool XBuffTriggerByHit::CheckTriggerCondition()
{
	if (m_SpecifiedSkillsSet != NULL && (m_pRawInput == NULL || !m_SpecifiedSkillsSet->count(m_pRawInput->m_skillId)))
		return false;

	switch(m_Type)
	{
	case 0:
		return true;
		break;
	case 1:
		// if not normal atk, return false;
		if(m_pRawInput != NULL && GetEntity()->SkillManager()->IsPhysicalAttack(m_pRawInput->m_skillId))
			return true;
		break;
	case 2:
		if (m_pResult != NULL && (m_pResult->m_flag & DMGFLAG_CRITICAL) != 0)
			return true;
		break;
	case 3:
		if (m_pResult == NULL || m_pRawInput == NULL || m_pRawInput->m_target == NULL)
			return false;
		//if (m_pResult->m_IsTargetDead && (m_pRawInput->m_target->GetUnit()->GetKillerID() == GetEntity()->GetID()))
		//	return true;
		if (m_KillType != 0)
		{
			if (m_KillType == 1)
			{
				if (!m_pRawInput->m_target->GetUnit()->IsRole())
					return false;
			}
		}
		if (m_pRawInput->m_target->GetUnit()->GetAttr(BASIC_CurrentHP) - m_pResult->GetValue() <= 0)
			return true;
		break;
	//case 4:
	//	if (m_pRawInput != NULL && m_SpecifiedSkillsSet.count(m_pRawInput->m_skillId) > 0)
	//		return true;
	//	break;
	case 5:
		if(m_pRawInput != NULL && m_pRawInput->m_target != NULL)
		{
			XBuffComponent* pBuffComp = m_pRawInput->m_target->BuffComponent();
			if(pBuffComp != NULL && pBuffComp->GetBuffByID(m_SpecifiedBuffID) != NULL)
			{
				_SetTarget(m_pRawInput->m_target);
				return true;
			}
		}
		break;
	default:
		break;
	}

	return false;
}

void XBuffTriggerByHit::_OnTrigger()
{
	XBuffTrigger::_OnTrigger();
	switch(m_Type)
	{
	case 2:
		if(m_CriticalChangeRatio != 1.0)
			m_pResult->SetValue(m_pResult->GetValue() * m_CriticalChangeRatio);
		break;
	}
}

XBuffEffectPrioriy XBuffTriggerByHit::GetPriority()
{
	if (m_Type == 3)
		return XBuffEffectPrioriy_TriggerByHit_Death;
	return XBuffEffectPrioriy_TriggerByHit;
}
