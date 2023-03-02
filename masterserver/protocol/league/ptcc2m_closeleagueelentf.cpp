#include "pch.h"
#include "league/ptcc2m_closeleagueelentf.h"
#include "leaguebattle/leagueeliminationmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/1/14 15:18:48

void PtcC2M_CloseLeagueEleNtf::Process(UINT32 dwConnID)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	LeagueEliminationMgr::Instance()->RemoveWatcher(pRole->GetID());
}
