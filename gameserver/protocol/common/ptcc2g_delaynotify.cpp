#include "pch.h"
#include "common/ptcc2g_delaynotify.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2015/12/5 17:13:46

void PtcC2G_DelayNotify::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		return;
	}

	pRole->SetDelay(m_Data.delay());

	//SSInfo << pRole << " delay is " << m_Data.delay() << END;
}
