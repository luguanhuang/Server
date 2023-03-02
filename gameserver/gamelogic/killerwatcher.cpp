#include "pch.h"
#include "unit/unit.h"
#include "killerwatcher.h"


KillerWatcher::KillerWatcher()
{
	Reset();
}

void KillerWatcher::Reset()
{
	m_KillerID = 0;
	m_ExecutionerID = 0;
	m_KillerSkill = 0;
	m_KillerBuff = 0;
}

void KillerWatcher::CheckKiller(const KillerWatchParam& param, double deltaHP)
{
	if(param.pUnit == NULL)
		return;

	double curhp = param.pUnit->GetAttr(BASIC_CurrentHP);
	if(curhp > 0.0 && curhp + deltaHP <= 0.0)
	{
		KillerWatcher* pKiller = param.pUnit->GetKiller();
		pKiller->m_ExecutionerID = param.KillerID;
		pKiller->m_KillerBuff = param.KillerBuff;
		pKiller->m_KillerSkill = param.KillerSkill;

		Unit* pUnitKiller = Unit::FindUnit(param.KillerID);
		if (pUnitKiller && pUnitKiller->GetFinalHostId())
			pKiller->m_KillerID = pUnitKiller->GetFinalHostId();
		else
			pKiller->m_KillerID = param.KillerID;
	}
}
