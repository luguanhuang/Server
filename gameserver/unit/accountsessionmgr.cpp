#include "pch.h"
#include "accountsessionmgr.h"
#include "cmdline.h"

INSTANCE_SINGLETON(AccountSessionManager)


AccountSessionManager::AccountSessionManager()
{

}

AccountSessionManager::~AccountSessionManager()
{

}

void AccountSessionManager::addSession(const AccountSession &session)
{
	m_sessions.insert(session);
	SSDebug << "Add session [" << session.m_sessionID << "], account [" << session.m_accountID << "]" << END;
}

AccountSession * AccountSessionManager::findBySession(SessionType sessionID)
{
	return m_sessions.find<SessionIndex>(sessionID);
}

AccountSession * AccountSessionManager::findByAccountID(const std::string &accountID)
{
	return m_sessions.find<AccountIndex>(accountID);
}

void AccountSessionManager::removeBySession(SessionType sessionID)
{
	AccountSession *pSession = findBySession(sessionID);
	if (pSession != NULL)
	{
		SSDebug << "Remove session [" << sessionID << "], account [" << pSession->m_accountID << "]" << END;
	}
	else
	{
		SSDebug << "Remove role session [" << sessionID << "], but not exist!" << END;
	}

	m_sessions.erase<SessionIndex>(sessionID);
}

void AccountSessionManager::removeByAccountID(const std::string & accountID)
{
	LogDebug("remove session account [%s] not exist", accountID.c_str());
	m_sessions.erase<AccountIndex>(accountID);
}

bool AccountSessionManager::Init()
{
	return true;
}

void AccountSessionManager::Uninit()
{
	m_sessions.clear();
}