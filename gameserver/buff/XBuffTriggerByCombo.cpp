#include "pch.h"
#include "XBuffTriggerByCombo.h"
#include "gamelogic/XCombat.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "entity/XEntity.h"


XBuffTriggerByCombo::XBuffTriggerByCombo(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	m_Base = (UINT32)_GetTriggerParamInt(pBuff->GetBuffInfo(), 0);
	if(m_Base == 0)
		m_Base = 1;
}

XBuffTriggerByCombo::~XBuffTriggerByCombo()
{

}


bool XBuffTriggerByCombo::CheckTriggerCondition()
{
	return m_Count != 0 && m_Count % m_Base == 0;
}

void XBuffTriggerByCombo::OnComboChange(UINT32 comboCount)
{
	m_Count = comboCount;
	Trigger();
}
