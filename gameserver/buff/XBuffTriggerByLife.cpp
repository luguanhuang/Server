#include "pch.h"
#include "XBuffTriggerByLife.h"
#include "unit/unit.h"
#include "entity/XEntity.h"


XBuffTriggerByLife::XBuffTriggerByLife(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	m_HPTriggerPercent = _GetTriggerParam(pBuff->GetBuffInfo(), 0);
}

XBuffTriggerByLife::~XBuffTriggerByLife()
{

}

void XBuffTriggerByLife::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	XBuffTrigger::OnAdd(pEntity, pEffectHelper);
	Trigger();
}

bool XBuffTriggerByLife::CheckTriggerCondition()
{
	Unit *pUnit = GetEntity()->GetUnit();
	double maxhp = pUnit->GetAttr(TOTAL_MaxHP);
	double hp = pUnit->GetAttr(BASIC_CurrentHP);
	if (maxhp < 1) maxhp = 1;
	
	return (hp/maxhp < m_HPTriggerPercent);
}

void XBuffTriggerByLife::OnAttributeChanged(CombatAttrDef attr)
{
	if (attr == BASIC_CurrentHP)
	{
		Trigger();
	}
}
