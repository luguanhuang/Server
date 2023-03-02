#include "pch.h"
#include "battle/ptcc2g_roledeathinsoloscene.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2015/7/21 15:39:27

void PtcC2G_RoleDeathInSoloScene::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}

	pRole->TriggerDeath();
}
