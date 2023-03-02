#include "pch.h"
#include "rolefashiontimer.h"
#include "unit/role.h"
#include "rolefashion.h"
#include "time.h"
#include "util/gametime.h"

void RoleFashionTimer::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	RoleFashion * record = m_pRole->Get<RoleFashion>();
	record->UpdateAll(nowtime, true);
}

void RoleFashionTimer::Start()
{
	m_timeToken = CTimerMgr::Instance()->SetTimer(this, 0, 5*1000, -1, __FILE__, __LINE__);
}

void RoleFashionTimer::Stop()
{
	if (m_timeToken != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_timeToken);
		m_timeToken = INVALID_HTIMER;
	}
}

RoleFashionTimer::RoleFashionTimer()
{
	m_timeToken = INVALID_HTIMER;
	m_pRole = NULL;
}

RoleFashionTimer::~RoleFashionTimer()
{
}