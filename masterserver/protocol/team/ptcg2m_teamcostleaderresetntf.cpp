#include "pch.h"
#include "team/ptcg2m_teamcostleaderresetntf.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "team/teammgr.h"
#include "team/team.h"

// generate by ProtoGen at date: 2017/5/17 22:22:24

void PtcG2M_TeamCostLeaderResetNtf::Process(UINT32 dwConnID)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if(pRole)
	{
		if(pRole->HasTeamInMs())
		{
			Team* pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
			if(pTeam && pTeam->GetLeader() == m_Data.roleid() && pTeam->GetCostIndex() != TeamCostInvalidID)
			{
				pTeam->SetCostIndex(TeamCostInvalidID);
			}
		}
	}
}
