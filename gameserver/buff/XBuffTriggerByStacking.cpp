#include "pch.h"
#include "XBuffTriggerByStacking.h"
#include "gamelogic/XCombat.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "entity/XEntity.h"


XBuffTriggerByStacking::XBuffTriggerByStacking(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	m_Base = (UINT32)_GetTriggerParamInt(pBuff->GetBuffInfo(), 0);
	if(m_Base == 0)
		m_Base = 1;
}

XBuffTriggerByStacking::~XBuffTriggerByStacking()
{

}


bool XBuffTriggerByStacking::CheckTriggerCondition()
{
	return m_pBuff->GetStackCount() != 0 && m_pBuff->GetStackCount() % m_Base == 0;
}

void XBuffTriggerByStacking::OnAppend(XEntity*pEntity)
{
	Trigger();
}
