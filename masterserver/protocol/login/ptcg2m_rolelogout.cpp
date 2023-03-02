#include "pch.h"
#include "login/ptcg2m_rolelogout.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/11/26 16:38:24

void PtcG2M_RoleLogout::Process(UINT32 dwConnID)
{
	CRoleManager::Instance()->LogoutRole(m_Data.sessionid(), m_Data.level());
}
