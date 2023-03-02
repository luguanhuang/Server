#ifndef __SESSIONMANAGER_H__
#define __SESSIONMANAGER_H__

#include "multiindex_map.h"
#include "cmdline.h"
#include "commondef.h"
#include "protowindow.h"



enum SessionState
{
	SESSION_INVALID,
	SESSION_CHALLENGE,
	SESSION_DBVERIFY,			// 登录验证中
	SESSION_DBVERIFY_FAILED,	// 登录验证失败
	SESSION_DBVERIFY_SUCCESS,	// 登录验证成功，选角色或者排队中
	SESSION_LOGIN,				// 登录角色成功

	SESSION_KEEPALIVE_TIMEOUT,	// 连接超时, 客户端导致的断开，需要通知ns断开
	SESSION_RELOGIN_KICKOFF,	// 服务器主动断开, 重登陆
	SESSION_GSCLOSED,			// 服务器主动断开, GS关闭
	SESSION_GMFORBID_KICKOFF,	// 服务器主动断开, 封禁踢人
};

inline const char *GetSessionStateString(SessionState state)
{
	switch(state) {
	case SESSION_INVALID: return "[invalid]";
	case SESSION_CHALLENGE: return "[challenge]";
	case SESSION_DBVERIFY: return "[db verify]";
	case SESSION_DBVERIFY_FAILED: return "[db verify failed]";
	case SESSION_DBVERIFY_SUCCESS: return "[db verify success]";
	case SESSION_LOGIN: return "[login]";
	case SESSION_RELOGIN_KICKOFF: return "[ReloginKickOff]";
	case SESSION_KEEPALIVE_TIMEOUT: return "[KeepaliveTimeout]";
	case SESSION_GMFORBID_KICKOFF: return "[GMForbidKickOff]";
	default:return "[Unknow]";
	}
	return "[unknow]";
}

struct SessionInfo
{
	SessionInfo()
	{
		m_dwRpcID = 0;
		m_connID = 0;
		m_gameserverId = 0;
		m_sessionID = 0;
		m_loginTimer = 0;
		m_lastRecvTime = 0;
		m_lastStatTimeMs = 0;
		m_protoCount = 0;
		m_keepAliveTryTimes = 0;
		m_createTime = 0;
		m_expireTime = 0;
		m_state = SESSION_INVALID;
		m_loginTimerHandler = INVALID_HTIMER;
		m_dwGsLine = INVALID_LINE_ID;
		m_crossGs = false;
		m_keepAliveForever = false;
	}

	UINT32 m_dwRpcID;
	UINT32 m_connID;
	UINT32 m_gameserverId;
	UINT32  m_lastRecvTime;
	UINT64 m_lastStatTimeMs;
	UINT32 m_protoCount;
	UINT32 m_keepAliveTryTimes;
	time_t m_createTime;
	time_t m_expireTime;
	ITimer *m_loginTimer;
	HTIMER  m_loginTimerHandler;
	SessionState m_state;
	SessionType m_sessionID;
	UINT32		m_dwGsLine;
	bool		m_crossGs;	// 是否跨服GS
	std::string	m_strIP;
	ProtoWindow m_recvProto;
	bool		m_keepAliveForever;	// 是否永久保活
};

struct SessionIndexer
{
	typedef SessionType key;
	key Key(const SessionInfo &info) { return info.m_sessionID; }
};

struct ConnIDIndexer
{
	typedef UINT32 key;
	key Key(const SessionInfo &info) { return info.m_connID; }
};

class CProtocol;

class SessionSender
{
public:
	SessionSender(UINT32 dwConnID) : m_dwConnID(dwConnID){}
	bool Send(const char *pcData, UINT32 dwLen);
	bool Send(const CProtocol& roPtc);
	bool valid();
	UINT32 GetConnID() { return m_dwConnID; }

private:
	UINT32 m_dwConnID;
};

struct WaitReconnSessionInfo
{
	UINT32 disconnectTime;
	UINT32 serverID;
	UINT32	m_dwGsLine;
	bool m_crossGs;
	bool m_keepAliveForever;

	WaitReconnSessionInfo()
		:disconnectTime(0)
		,serverID(0)
		,m_dwGsLine(0)
		,m_crossGs(false)
		,m_keepAliveForever(false)
	{
	}
};

class SessionManager : public ITimer
{
	SessionManager();
	virtual ~SessionManager();
	DECLARE_SINGLETON(SessionManager)

public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	SessionType GetSession(UINT32 dwConnID);
	SessionType SessionConvert(UINT32 dwConnID, UINT32 dwProtoType);
	bool ChangeSessionState(SessionType sessionID, SessionState state);
	SessionState GetSessionState(SessionType sessionID);

	void OnNewConnection(UINT32 dwConnID);
	void OnClose(UINT32 dwConnID);
	
	void CloseSession(SessionType sessionID, UINT32 errorcode);
	void CloseSession(SessionInfo *sessionInfo, UINT32 errorcode);
	void SendSessionError(SessionType sessionID, UINT32 errorcode);
	void ForceCloseSession(SessionInfo *sessionInfo, UINT32 errorcode);

	void GSShutDownCloseAllSession(UINT32 dwGSId);
	UINT32 GetSessionConnID(SessionType sessionID);
	SessionSender GetSessionSender(SessionType sessionID);

	SessionInfo *FindSession(SessionType sessionID);
	SessionInfo *FindByConnID(UINT32 dwConnID);

	void StartLoginTimer(SessionInfo *sessionInfo);
	void StopLoginTimer(SessionInfo *sessionInfo);
	void StopLoginTimer(SessionType sessionID);
	int  GetSessionCount() { return m_SessionCount; }

	void AddReconnectSession(SessionType sessionID, WaitReconnSessionInfo& info);
	void RemoveReconnectSession(SessionType sessionID);
	WaitReconnSessionInfo* GetReconnectSession(SessionType sessionID);
	bool ReConnectBySession(UINT32 dwConnID, SessionType sessionID, UINT64 roleid, UINT32 sceneid, UINT32 delayid);
	void DestroySessionByNS(SessionType sessionID);
	void NotifyReconnect(SessionType sessionid, UINT64 roleid, UINT32 sceneid, bool isadd, UINT32 dwGsLine, bool isCrossGs, UINT32 delayid);

	UINT32 GetGameServerId(SessionType sessionId);
	UINT32 GetGameServerId(UINT32 dwConnId);


	std::list<SessionInfo>::iterator Begin() { return m_SessionsInfo.begin(); }	
	std::list<SessionInfo>::iterator End() { return m_SessionsInfo.end(); }

	static bool SessionCmdLine(const CmdLineArgs &args, std::string &outputMessage);	

	UINT32 GetSessionNum() { return m_SessionsInfo.size(); }
	void SetAcceptTokenNum(UINT32 num) { m_acceptTokenNum = num; }

	void DumpRecvProto(SessionInfo* pSessionInfo);

private:
	void DumpSessions(std::string &outputMessage);

	int m_SessionCount;
	mutliindex_map<SessionInfo, SessionIndexer, ConnIDIndexer> m_SessionsInfo;
	std::unordered_map<UINT64, WaitReconnSessionInfo> m_SessionsWaitReconnect;
	HTIMER m_keepAliveTimerHandler;

	UINT32 m_acceptTokenNum;
	UINT32 m_maxTpsPerConn;
};

//为了不让audio和gs的serverid重复，需要分段，比如gs取1-10W， audio取100W-200W

#endif
