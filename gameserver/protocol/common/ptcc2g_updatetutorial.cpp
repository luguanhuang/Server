#include "pch.h"
#include "common/ptcc2g_updatetutorial.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2015/2/2 14:42:56

void PtcC2G_UpdateTutorial::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		pRole->UpdateTutorial(m_Data.tutorialid());
	}
}
