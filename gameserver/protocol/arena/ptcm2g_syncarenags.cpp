#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/arenarecord.h"
#include "arena/ptcm2g_syncarenags.h"

// generate by ProtoGen at date: 2016/9/11 18:07:18

void PtcM2G_SyncArenaGs::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (NULL == role)
	{
		SSWarn<<"sync arena not find roleid:"<<m_Data.roleid()<<END;
		return;
	}
	if (m_Data.has_optimalrank())
	{
		role->Get<CArenaRecord>()->SetOptimalRank(m_Data.optimalrank().rank());
	}
}
