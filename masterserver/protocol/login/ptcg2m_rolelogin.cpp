#include "pch.h"
#include "login/ptcg2m_rolelogin.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/11/26 16:38:19

void PtcG2M_RoleLogin::Process(UINT32 dwConnID)
{
	UINT64 qwRoleID = m_Data.roledata().roleid();
	CRole* poExist = CRoleManager::Instance()->GetByRoleID(qwRoleID);
	if(poExist != NULL)
	{
		CRoleManager::Instance()->LogoutRole(poExist->GetSessionID(), 0);
	}

	CRole* poRole = CRoleManager::Instance()->LoginRole(m_Data.mutable_roledata(), m_Data.sessionid());

	//登录后进入主城GS1
	if(poRole != NULL)
	{
		poRole->SetGsLine(MAIN_HALL_GS_LINE);
		poRole->SetState(ROLE_STATE_IN_GAME);
	}
}
