#include "pch.h"
#include "militaryrank/ptcm2g_militaryrankchange.h"
#include "unit/rolemanager.h"
#include "gamelogic/military_rank_record.h"
#include "gamelogic/military_rank_manager.h"
#include "unit/roleoutlook.h"
#include "rolesummary/rolesummarybase.h"

// generate by ProtoGen at date: 2017/3/10 10:26:31

void PtcM2G_MilitaryRankChange::Process(UINT32 dwConnID)
{
	Role *role = RoleManager::Instance()->FindByRoleID(m_Data.role_id());
	if (role != NULL)
	{
		UINT32 rank = role->Get<MilitaryRankRecord>()->GetMilitaryRank();
		if (m_Data.military_rank() != rank)
		{
			role->Get<MilitaryRankRecord>()->SetMilitaryRrank(m_Data.military_rank());			
		}
	}
}
