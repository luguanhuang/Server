#include "pch.h"
#include "saveroledatatimer.h"
#include "table/globalconfig.h"
#include "role.h"

void SaveRoleDataTimer::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	m_pRole->SetFlag(RSF_ISRoleDataNeedSave, true);
}

void SaveRoleDataTimer::Start()
{
	m_timeToken = CTimerMgr::Instance()->SetTimer(this, 0,  GetGlobalConfig().RoleSaveInterval * 1000, -1, __FILE__, __LINE__);
}

void SaveRoleDataTimer::Stop()
{
	if (m_timeToken != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_timeToken);
		m_timeToken = INVALID_HTIMER;
	}
}

SaveRoleDataTimer::SaveRoleDataTimer()
{
	m_timeToken = INVALID_HTIMER;
	m_pRole = NULL;
}

SaveRoleDataTimer::~SaveRoleDataTimer()
{
}