#include "pch.h"
#include "XBuffTriggerByCastSkill.h"
#include "gamelogic/XCombat.h"


XBuffTriggerByCastSkill::XBuffTriggerByCastSkill(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	m_SpecifiedSkillsSet = pBuff->GetRelevantSkills();
}

XBuffTriggerByCastSkill::~XBuffTriggerByCastSkill()
{
}

bool XBuffTriggerByCastSkill::OnCastSkill(const HurtInfo& hurtInfo)
{
	XBuffTrigger::OnCastSkill(hurtInfo);
	m_pRawInput = &hurtInfo;
	Trigger();
	return true;
}

bool XBuffTriggerByCastSkill::CheckTriggerCondition()
{
	if (m_SpecifiedSkillsSet != NULL && (m_pRawInput == NULL || !m_SpecifiedSkillsSet->count(m_pRawInput->m_skillId)))
		return false;

	return true;
}