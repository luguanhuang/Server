#include "pch.h"
#include "XBuffTriggerAlways.h"
#include "timermgr.h"


XBuffTriggerAlways::XBuffTriggerAlways(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	m_timer = INVALID_HTIMER;
}

XBuffTriggerAlways::~XBuffTriggerAlways()
{
	if (m_timer != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_timer);
		m_timer = INVALID_HTIMER;
	}
}

void XBuffTriggerAlways::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	XBuffTrigger::OnAdd(pEntity, pEffectHelper);
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	OnTimer(0, 0);
}

void XBuffTriggerAlways::OnRemove(XBuffRemoveReason reason)
{
	CTimerMgr::Instance()->KillTimer(m_timer);
	m_timer = INVALID_HTIMER;
	XBuffTrigger::OnRemove(reason);
}

void XBuffTriggerAlways::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Trigger();
}