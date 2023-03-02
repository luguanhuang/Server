#include "pch.h"
#include "login/ptcn2t_notifygtsessionlogin.h"
#include "session/sessionmanager.h"

// generate by ProtoGen at date: 2016/11/28 14:10:48

void PtcN2T_NotifyGtSessionLogin::Process(UINT32 dwConnID)
{
	SessionInfo *pSession = SessionManager::Instance()->FindSession(m_sessionID);
	if (pSession)
	{
		pSession->m_state = SESSION_LOGIN;
		LogDebug("session[%lld], login role success", m_sessionID);
	}
	else
	{
		LogWarn("can't find session[%lld]", m_sessionID);
	}
}
