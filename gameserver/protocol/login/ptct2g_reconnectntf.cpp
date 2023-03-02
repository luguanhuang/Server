#include "pch.h"
#include "login/ptct2g_reconnectntf.h"
#include "gamelogic/rolesummarymgr.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2016/1/18 10:44:42

void PtcT2G_ReconnectNtf::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	role->UpdateOnlineTime();
	role->SetFlag(RSF_ISReconnecting, m_Data.isadd());

	if (m_Data.isadd())
	{
		//RoleSummaryMgr::Instance()->UpdateRoleSummary(role, SUMMARY_TIMELOGIN);
	}
	else
	{
		role->OnClientReconnect(m_Data.sceneid());
	}
}
