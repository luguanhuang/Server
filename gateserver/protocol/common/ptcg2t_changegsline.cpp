#include "pch.h"
#include "common/ptcg2t_changegsline.h"
#include "session/sessionmanager.h"

// generate by ProtoGen at date: 2016/7/11 20:20:03

void PtcG2T_ChangeGsLine::Process(UINT32 dwConnID)
{
	SessionInfo* poSession = SessionManager::Instance()->FindSession(m_Data.sessionid());
	if(poSession != NULL)
	{
		poSession->m_dwGsLine = m_Data.line();
		poSession->m_crossGs = m_Data.is_cross_gs();
		poSession->m_keepAliveForever = m_Data.keepalive();
	}
	else
	{
		LogWarn("Session %llu has been closed", m_Data.sessionid());
	}
	WaitReconnSessionInfo* pRSession = SessionManager::Instance()->GetReconnectSession(m_Data.sessionid());
	if (pRSession)
	{
		pRSession->m_dwGsLine = m_Data.line();
		pRSession->m_crossGs = m_Data.is_cross_gs();
		pRSession->m_keepAliveForever = m_Data.keepalive();
	}
	//SSDebug << "change gs: " << poSession->m_dwGsLine << ", iscross: " << poSession->m_crossGs << END;
}
