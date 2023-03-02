#include "pch.h"
#include "session/ptcg2m_clientsessionchangeg2m.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/6/21 16:50:11

void PtcG2M_ClientSessionChangeG2M::Process(UINT32 dwConnID)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if (pRole == NULL)
	{
		return;
	}
	LogInfo("Role[%llu] name[%s] reconnect change session from[%llu] to[%llu]", pRole->GetID(), pRole->GetName().c_str(), m_Data.oldsession(), m_Data.newsession());
	CRoleManager::Instance()->ConvertSession(pRole, m_Data.newsession());
	pRole->SetIP(m_Data.cinfo().ip());
	pRole->SetToken(m_Data.cinfo().token());
	pRole->SetLoginType(m_Data.cinfo().logintype());
	pRole->SetLoginPlatID(m_Data.cinfo().platid());
}
