#include "pch.h"
#include "common/ptcn2g_updatestartuptypen2gntf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolemisc.h"

// generate by ProtoGen at date: 2016/12/21 21:29:40

void PtcN2G_UpdateStartUpTypeN2GNtf::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	pRole->Get<RoleMisc>()->SetStartUpType(m_Data.type());
}
