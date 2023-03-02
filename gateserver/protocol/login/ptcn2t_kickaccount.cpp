#include "pch.h"
#include "login/ptcn2t_kickaccount.h"
#include "session/sessionmanager.h"

// generate by ProtoGen at date: 2016/11/3 10:24:07


const char* GetKickTypeDesc(KKSG::KickType nType)
{
	switch(nType)
	{
	case KKSG::KICK_NORMAL: return "normal";
	case KKSG::KICK_RELOGIN: return "relogin";
	case KKSG::KICK_GMFORBID: return "forbidden";
	case KKSG::KICK_SERVER_SHUTDOWN: return "gs_shutdown";
	case KKSG::KICK_HG: return "hg_forceexit";
	default: break;
	}
	return "unknown";
}

void PtcN2T_KickAccount::Process(UINT32 dwConnID)
{
	//新的踢人协议

	LogInfo("Ns kick session [%llu], type [%s]", m_sessionID, GetKickTypeDesc(m_Data.reason()));
	if(SessionManager::Instance()->GetSessionState(m_sessionID) != SESSION_INVALID)
	{
		if (m_Data.reason() == KKSG::KICK_RELOGIN)
		{
			SessionManager::Instance()->ChangeSessionState(m_sessionID, SESSION_RELOGIN_KICKOFF);
			SessionManager::Instance()->CloseSession(m_sessionID, KKSG::ERR_SESSION_KICKOFF);
		}
		else if (m_Data.reason() == KKSG::KICK_GMFORBID)
		{
			SessionManager::Instance()->ChangeSessionState(m_sessionID, SESSION_GMFORBID_KICKOFF);
			SessionManager::Instance()->CloseSession(m_sessionID, KKSG::ERR_LOGIN_FORBID);
		}
		else if (m_Data.reason() == KKSG::KICK_NORMAL)
		{
			SessionManager::Instance()->ChangeSessionState(m_sessionID, SESSION_GSCLOSED);
			SessionManager::Instance()->CloseSession(m_sessionID, KKSG::ERR_SUCCESS);
		}
		else if (m_Data.reason() == KKSG::KICK_CHANGE_PROFESSION)
		{
			SessionManager::Instance()->ChangeSessionState(m_sessionID, SESSION_GSCLOSED);
			SessionManager::Instance()->CloseSession(m_sessionID, KKSG::ERR_CHANGEPRO_KICK);
		}
		else if (m_Data.reason() == KKSG::KICK_HG)
		{
			SessionManager::Instance()->ChangeSessionState(m_sessionID, SESSION_GSCLOSED);
			SessionManager::Instance()->CloseSession(m_sessionID, KKSG::ERR_SUCCESS);
		}
		else
		{
			SessionManager::Instance()->ChangeSessionState(m_sessionID, SESSION_GSCLOSED);
			SessionManager::Instance()->CloseSession(m_sessionID, KKSG::ERR_GS_CLOSED);
		}
	}
	SessionManager::Instance()->RemoveReconnectSession(m_sessionID);
}
