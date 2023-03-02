#include "pch.h"
#include "team/ptcg2m_teameventnotify.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teammgr.h"

// generate by ProtoGen at date: 2016/8/24 15:11:58

void PtcG2M_TeamEventNotify::Process(UINT32 dwConnID)
{
	switch(m_Data.type())
	{
	case KKSG::TEAM_EVENT_BUYCOUNT:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
			if(NULL == pRole)
			{
				break;
			}
			if(0 == pRole->GetTeamID())
			{
				break;
			}
			Team* pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
			if(NULL == pTeam)
			{
				break;
			}
			TeamMgr::Instance()->SynGetExtraData(pTeam, pRole->GetID());
		}
		break;
	default:
		break;
	}
}
