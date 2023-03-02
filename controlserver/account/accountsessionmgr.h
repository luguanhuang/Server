#ifndef _H_Account_Session_Mgr_H__
#define _H_Account_Session_Mgr_H__

#include "timer.h"
#include "multiindex_map.h"
#include "pb/enum.pb.h"
#include "accountsession.h"
#include "role/roleidgenerator.h"

struct CReconnectSession
{
	std::string account;
	KKSG::LoginRequestNewArg loginArg;
};

class CAccountSessionMgr
{
	CAccountSessionMgr();
	~CAccountSessionMgr();
	DECLARE_SINGLETON(CAccountSessionMgr)

public:
	bool Init();
	void Uninit();
	void RecoverFromShm();


	void LoginAccount(CAccountSession* session);
	void LogoutAccount(const std::string& accountID, KKSG::KickType nKickType);
	CAccountSession* ConvertSession(SessionType oldSession, SessionType newSession);

	CRole* LoginRole(UINT64 sessionID, const KKSG::RoleAllInfo& roInfo);
	void   LogoutRole(UINT64 sessionID);

	void AddSession(CAccountSession* session);
	CAccountSession *GetBySession(SessionType sessionID);
	CAccountSession *GetByAccountID(const std::string & accountID);
	void RemoveBySession(SessionType sessionID);
	void RemoveByAccountID(const std::string & accountID);
	void PrintSessions();

	void IncOnlineNum(KKSG::PlatType nPlat);
	void DecOnlineNum(KKSG::PlatType nPlat);
	UINT32 GetOnlineNum(KKSG::PlatType nPlat) const;
	UINT32 GetAllOnlineNum() const;

	KKSG::ErrorCode BeginCreateRole(SessionType qwSession, KKSG::RoleType nType, const std::string& strName);
	void OnCreateRoleReply(SessionType qwSession, KKSG::ErrorCode nResult, const KKSG::RoleAllInfo& roRoleInfo);
	void OnCreateRoleTimeout(SessionType qwSession);

	UINT64 NewRoleID(UINT32 serverID);
	UINT64 GetRoleIDByIndex(SessionType qwSession, INT32 nIndex);

	void KickoutPlayerOnGsLine(UINT32 dwLine, bool isCross = false);
	void KickoutAllCrossPlayer();
	void KickoutPlayerOnGateLine(UINT32 dwLine);
	void PackSessionClientInfo(SessionType qwSessionID, KKSG::ClientInfo& roCliInfo, UINT32& roleNum);
	void NotifyUsedTokenToLoginServer(SessionType qwSessionID);

	void SetRegisterAccount(UINT32 num) { m_registerAccount = num; }
	UINT32 GetRegisterAccount() { return m_registerAccount; }

	void OnMysqlConnectLost();
	void OnlineReport();

	CReconnectSession* GetReconnectSession(SessionType session);
	void SetReconnectSession(SessionType session, CReconnectSession* pRSession);
	void RemoveReconnectAccount(SessionType session);
	bool IsLoginReconnecting(const std::string& account);
	void SetLoginReconnecting(const std::string& account);

private:
	class OnlineReportTimer : public ITimer
	{
	public:
		OnlineReportTimer() {}
		virtual ~OnlineReportTimer() {}

		virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	};

	class MysqlLostKickTimer : public ITimer
	{
	public:
		MysqlLostKickTimer() {}
		virtual ~MysqlLostKickTimer() {}

		virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
		std::vector<UINT32>	m_oLineList;
	};

	HTIMER				m_hReportTimer;
	OnlineReportTimer	m_oReportTimer;
	UINT32				m_aOnlineArray[KKSG::PlatType_ARRAYSIZE];

private:
	struct SessionIndex
	{
		typedef SessionType key;
		key Key(const CAccountSession* s) { return s->GetSessionID(); }
	};

	struct AccountIndex
	{
		typedef std::string key;
		key Key(const CAccountSession* s) { return s->GetAccountID(); }
	};
private:
	typedef mutliindex_map_ptr<CAccountSession, SessionIndex, AccountIndex> CAccountSessionMap;
	CAccountSessionMap	m_sessions;
	CRoleIDGenerator	m_oRoleIDGen;
	UINT32 m_registerAccount;

	std::unordered_map<SessionType, CReconnectSession*> m_reconnectSession;
	std::unordered_set<std::string> m_reconnectingAccount;
};

#endif
