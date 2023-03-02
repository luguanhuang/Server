#include "pch.h"
#include "XBuffTriggerNearbyMates.h"
#include "timermgr.h"
#include "entity/XEntity.h"
#include "XBuffTrigger.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "scene/scene.h"

XBuffTriggerNearbyMates::XBuffTriggerNearbyMates(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	m_timer = INVALID_HTIMER;
	m_Radius = _GetTriggerParam(pBuff->GetBuffInfo(), 0);
	m_SqrRadius = m_Radius * m_Radius;
}

XBuffTriggerNearbyMates::~XBuffTriggerNearbyMates()
{
	if (m_timer != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_timer);
		m_timer = INVALID_HTIMER;
	}
}

void XBuffTriggerNearbyMates::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	XBuffTrigger::OnAdd(pEntity, pEffectHelper);
	m_pUnit = pEntity->GetUnit();
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	OnTimer(0, 0);
}

void XBuffTriggerNearbyMates::OnRemove(XBuffRemoveReason reason)
{
	CTimerMgr::Instance()->KillTimer(m_timer);
	m_timer = INVALID_HTIMER;
	XBuffTrigger::OnRemove(reason);
}

void XBuffTriggerNearbyMates::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Trigger();
}

bool XBuffTriggerNearbyMates::CheckTriggerCondition()
{
	if(m_pUnit == NULL || m_pUnit->GetCurrScene() == NULL)
		return false;
	m_Mates.clear();

	m_pUnit->GetCurrScene()->GetTeammates(m_pUnit, m_Mates);

	for(auto i = m_Mates.begin(); i < m_Mates.end(); ++i)
	{
		if (Vector3::sqrtMagnitude(GetEntity()->GetPosition_p() - (*i)->GetXObject()->GetPosition_p()) <= m_SqrRadius)
		{
			return true;
		}

	}
	return false;
}
