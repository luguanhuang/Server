#include "pch.h"
#include "common/ptcr2g_notifyserverclose.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "config.h"
#include "gamelogic/rolesummarymgr.h"
#include "servermgr/servermgr.h"

// generate by ProtoGen at date: 2016/11/15 16:29:21

void PtcR2G_NotifyServerClose::Process(UINT32 dwConnID)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		return;
	}
	SSWarn << "serverId: " << m_Data.serverid() << " close, remove all relation roles" << END;

	std::list<Role*> removeRoles;
	for (auto it = RoleManager::Instance()->Begin(); it != RoleManager::Instance()->End(); ++it)
	{
		Role* pRole = *it;
		if (pRole->GetFromServerId() != m_Data.serverid())
		{
			continue;
		}
		removeRoles.push_back(pRole);
	}

	for (auto it = removeRoles.begin(); it != removeRoles.end(); ++it)
	{
		Role* pRole = *it;
		SSWarn << "roleId: " << pRole->GetID() << ", name: " << pRole->GetName() << ", force leave cross gs" << END;
		pRole->OnLeaveGs(0);
		UINT64 qwRoleSessionID = pRole->GetSessionID();
		UINT64 roleId = pRole->GetID();
		RoleManager::Instance()->RemoveBySession(qwRoleSessionID);
		RoleSummaryMgr::Instance()->RemoveRoleSummary(roleId);
	}

	ServerMgr::Instance()->RemoveServer(m_Data.serverid());
}
