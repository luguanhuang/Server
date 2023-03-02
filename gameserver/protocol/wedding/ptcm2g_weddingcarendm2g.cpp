#include "pch.h"
#include "wedding/ptcm2g_weddingcarendm2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2017/7/31 23:37:32

void PtcM2G_WeddingCarEndM2G::Process(UINT32 dwConnID)
{
	Role* pRole1 = RoleManager::Instance()->FindByRoleID(m_Data.roleid1());
	Role* pRole2 = RoleManager::Instance()->FindByRoleID(m_Data.roleid2());

	/*
	if (pRole1)
	{
		pRole1->SetWeddingCarPos(false, true);
	}
	else if (pRole2)
	{
		pRole2->SetWeddingCarPos(false, true);
	}
	*/
}
