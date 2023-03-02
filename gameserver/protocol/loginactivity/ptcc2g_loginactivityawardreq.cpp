#include "pch.h"
#include "loginactivity/ptcc2g_loginactivityawardreq.h"
#include "gamelogic/loginactivity.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2017/4/26 19:27:55

void PtcC2G_LoginActivityAwardReq::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
		return;
	LoginActivityMgr::Instance()->GiveAward(pRole);
}
