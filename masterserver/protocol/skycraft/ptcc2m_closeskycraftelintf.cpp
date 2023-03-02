#include "pch.h"
#include "skycraft/ptcc2m_closeskycraftelintf.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "skycraft/skycraftelimgr.h"

// generate by ProtoGen at date: 2017/4/11 17:15:28

void PtcC2M_CloseSkyCraftEliNtf::Process(UINT32 dwConnID)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	SkyCraftEliMgr::Instance()->RemoveWatcher(pRole->GetID());
}
