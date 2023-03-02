#include "pch.h"
#include "common/ptcg2m_mscrashrecoverg2m.h"
#include "role/rolemanager.h"
#include "scene/scenemanager.h"


// generate by ProtoGen at date: 2016/11/23 20:49:12

void PtcG2M_mscrashrecoverg2m::Process(UINT32 dwConnID)
{
	LogInfo("Crash Recover from cross_gs_%u", m_Data.gsline());

	if(m_Data.rolelist_size() != m_Data.sessionlist_size())
	{
		LogError("Crash Recover from cross_gs_%u error, invalid role/session list size", m_Data.gsline());
	}
	else
	{
		for(INT32 i = 0; i < m_Data.rolelist_size(); ++i)
		{
			UINT64 qwSessionID = m_Data.sessionlist(i);
			const KKSG::RoleInfoOnMs& roInfo = m_Data.rolelist(i);
			if(CRoleManager::Instance()->GetByRoleID(roInfo.roleid()) != NULL)
			{
				continue;
			}

			CRole* poRole = CRoleManager::Instance()->RecoverRole(&roInfo, qwSessionID);
			if(poRole != NULL)
			{
				poRole->SetState(ROLE_STATE_IN_GAME);
				poRole->SetGsLine(m_Data.gsline());
				poRole->SetInCrossGs(m_Data.iscrossgs());
		
			}
		}
	}

}
