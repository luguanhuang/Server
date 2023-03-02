#include "pch.h"
#include "reward/ptcc2g_onlinerewardreport.h"
#include "unit/rolemanager.h"
#include "gamelogic/onlinereward.h"

// generate by ProtoGen at date: 2015/10/21 23:17:45

void PtcC2G_OnlineRewardReport::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	OnlineRewardMgr::Instance()->OnLineNotify(role);
}
