#include "pch.h"
#include "XBuffTriggerByBeHit.h"
#include "gamelogic/XCombat.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "entity/XEntity.h"


XBuffTriggerByBeHit::XBuffTriggerByBeHit(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	m_Type = _GetTriggerParamInt(pBuff->GetBuffInfo(), 0);
	m_Param0 = _GetTriggerParamInt(pBuff->GetBuffInfo(), 1);
}

XBuffTriggerByBeHit::~XBuffTriggerByBeHit()
{

}

bool XBuffTriggerByBeHit::OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	if(hurtInfo.m_skillId == 0)
		return true;

	m_pRawInput = &hurtInfo;
	m_pResult = &result;

	Trigger();
	return true;
}

XBuffEffectPrioriy XBuffTriggerByBeHit::GetPriority()
{
	return XBuffEffectPrioriy_TriggerByBeHit;
}

bool XBuffTriggerByBeHit::CheckTriggerCondition()
{
	switch(m_Type)
	{
	case 0:
		// always true
		return true;
		break;
	case 1:
		// 无敌成功，如果有参数1和2，则当有参数12 Buff时 才返回true
		if (m_pResult->m_Result != PJRES_IMMORTAL)
			return false;
		if (m_Param0 != 0)
		{
			XBuff* pTargetBuff = GetEntity()->BuffComponent()->GetBuffByID(m_Param0);
			if (pTargetBuff != NULL)
				return true;
		}
		break;
	}
	return false;
}
