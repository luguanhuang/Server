#include "pch.h"
#include "login/ptcg2m_roleloginreconnectntf.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/6/21 17:26:57

void PtcG2M_RoleLoginReconnectNtf::Process(UINT32 dwConnID)
{
	UINT64 qwRoleID = m_Data.roleid();
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
	if (pRole == NULL)
	{
		return;
	}
	LogInfo("role[%llu] name[%s] openid[%s] loginreconnect", pRole->GetID(), pRole->GetName().c_str(), pRole->GetAccount().c_str());
	pRole->OnLoginReconnect();
}
