#include "pch.h"
#include "misc/ptcc2g_sharereport.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2017/7/31 20:31:33

void PtcC2G_ShareReport::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		return;
	}
	TSharePhotoFlow oLog(pRole);
	oLog.m_op = m_Data.op();
	oLog.m_type = m_Data.type();
	oLog.Do();
}
