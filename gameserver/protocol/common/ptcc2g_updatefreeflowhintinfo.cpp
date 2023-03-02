#include "pch.h"
#include "common/ptcc2g_updatefreeflowhintinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolemisc.h"

// generate by ProtoGen at date: 2017/9/21 19:43:57

void PtcC2G_UpdateFreeflowHintInfo::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	pRole->Get<RoleMisc>()->SetFreeFlowHintTime(m_Data.hint_time());
}
