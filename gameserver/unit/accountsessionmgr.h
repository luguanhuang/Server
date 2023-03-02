#ifndef __ACCOUNTSESSIONMANAGER_H__
#define __ACCOUNTSESSIONMANAGER_H__

#include "multiindex_map.h"
#include "clientconfinfo.h"

struct AccountSession
{
	SessionType m_sessionID;
	std::string m_accountID;			//tencent open id
	ClientConfInfo	m_cliConf;

	AccountSession()
		:m_sessionID(INVALID_SESSION_ID)
	{
	}
};

struct SessionIndex
{
	typedef SessionType key;
	key Key(const AccountSession &s) { return s.m_sessionID; }
};

struct AccountIndex
{
	typedef std::string key;
	key Key(const AccountSession &s) { return s.m_accountID; }
};


class AccountSessionManager
{
	AccountSessionManager();
	~AccountSessionManager();
	DECLARE_SINGLETON(AccountSessionManager)

public:
	bool Init();
	void Uninit();

	void addSession(const AccountSession &session);
	AccountSession *findBySession(SessionType sessionID);
	AccountSession *findByAccountID(const std::string & accountID);
	void removeBySession(SessionType sessionID);
	void removeByAccountID(const std::string & accountID);

private:
	mutliindex_map<AccountSession, SessionIndex, AccountIndex> m_sessions;
};

#endif
