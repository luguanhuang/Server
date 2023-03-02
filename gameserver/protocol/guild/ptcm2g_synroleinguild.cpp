#include "pch.h"
#include "guild/ptcm2g_synroleinguild.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/activityrecord.h"

// generate by ProtoGen at date: 2016/8/29 15:41:43

void PtcM2G_SynRoleInGuild::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (pRole != NULL)
	{
		pRole->OnInGuild();
		pRole->Get<CActivityRecord>()->DoAddGuild();
	}
}
