#include "pch.h"
#include "scene/ptcc2g_addlevelscriptextstring.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "gamelogic/XLevelWave.h"

// generate by ProtoGen at date: 2015/12/23 11:03:05

void PtcC2G_AddLevelScriptExtString::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if (pRole->GetCurrScene() == NULL || pRole->GetCurrScene()->GetSpawner() == NULL)
	{
		return;
	}

	pRole->GetCurrScene()->GetSpawner()->m_oLevelState.SetExternalString(m_Data.extstring());

	 
}
