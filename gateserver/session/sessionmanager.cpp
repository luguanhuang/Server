#include "pch.h"
#include <time.h>
#include "sessionmanager.h"
#include "clientlink.h"
#include "timeutil.h"
#include "config.h"
#include "gslink.h"
#include "netproc.h"
#include "login/ptct2c_loginchallenge.h"
#include "login/ptct2c_keepalivepingreq.h"
#include "login/rpct2g_reconnectgs.h"
#include "common/ptct2c_errornotify.h"
#include "login/ptct2n_logoutntf.h"
#include "network/controllink.h"
#include "LogInit.h"
#include "define.h"
#include "network/rslink.h"
#include "cmdline.h"

#define SESSION_KEEPALIVE_TIME 300
#define SESSION_KEEPALIVE_TRYTIMES 3
#define SESSION_KEEPALIVE_CHECK_INTERVAL (30 * 1000)
#define SESSION_LOGIN_TIMEOUT (20*1000)
//#define SESSION_RECONNECT_EXPIRE_TIMEOUT (7200)

class LoginTimer : public ITimer
{
public:
	SessionInfo *m_sessionInfo;

	LoginTimer(SessionInfo *info) : m_sessionInfo(info) 
	{

	}

	// 登录超时，关闭连接
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		LogWarn("Session [%llu:%d] login timeout, close session", m_sessionInfo->m_sessionID, m_sessionInfo->m_connID);
		SessionManager::Instance()->CloseSession(m_sessionInfo, KKSG::ERR_LOGIN_TIMEOUT);
	}
};

class CloseSocketTimer : public ITimer
{
	std::vector<UINT32> m_ConnIDs;

public:

	std::vector<UINT32> ConnIDs;

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		for (UINT32 i = 0; i < m_ConnIDs.size(); ++i)
		{
			CClientLink::Instance()->Close(m_ConnIDs[i]);
		}

		m_ConnIDs.clear();
		m_ConnIDs.swap(ConnIDs);
	}

	void Start()
	{
		m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	}

	void Stop()
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
	}

	HTIMER m_handler;
};

static CloseSocketTimer CSTimer;

class UpdateAcceptTokenTimer : public ITimer
{
public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		SessionManager::Instance()->SetAcceptTokenNum(GateConfig::Instance()->GetNewConnPerSecond());
	}

	void Start()
	{
		m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	}

	void Stop()
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
	}

	HTIMER m_handler;
};

static UpdateAcceptTokenTimer acceptTimer;

INSTANCE_SINGLETON(SessionManager)

static SessionType SessionCounter = 1;

SessionManager::SessionManager()
{
	m_SessionCount = 0;
	m_keepAliveTimerHandler = CTimerMgr::Instance()->SetTimer(this, 0, SESSION_KEEPALIVE_CHECK_INTERVAL, -1, __FILE__, __LINE__);
	CSTimer.Start();
}

SessionManager::~SessionManager()
{
	CSTimer.Stop();
	CTimerMgr::Instance()->KillTimer(m_keepAliveTimerHandler);
}

void SessionManager::OnNewConnection(UINT32 dwConnID)
{
	// 超过最大连接数限制
	if (GetSessionNum() >= GateConfig::Instance()->GetMaxConnNum())
	{
		LogWarn("over max connection num");
		CClientLink::Instance()->Close(dwConnID);
		return;
	}
	// 每秒连接数过多
	if (m_acceptTokenNum == 0)
	{
		LogWarn("over max accept per second");
		CClientLink::Instance()->Close(dwConnID);
		return;
	}
	// 用掉一个令牌
	m_acceptTokenNum--;

	// 16位gate line，16位serverid， 8位时间戳低八位， 24位自增counter
	SessionType newSessionID = GateConfig::Instance()->GetLine();
	newSessionID <<= SessionGateIDShift;
	UINT64 Temp = GateConfig::Instance()->GetServerID() & 0xFFFF;
	Temp <<= 32;
	Temp |= ((TimeUtil::GetTime() & 0xFF) << 24);
	newSessionID |= Temp;
	newSessionID |= (SessionCounter & 0xFFFFFF);
	SessionCounter++;


	UINT32 dwIP;
	UINT16 wPort;
	CNetProcessor::Instance()->GetRemoteAddr(dwConnID, dwIP, wPort);
	char IPBuf[64] = {0};
	unsigned char *p = (unsigned char *)&dwIP;
	sprintf(IPBuf, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
	LogInfo("a client come from [%s] with session => [%llu : %d]", IPBuf, newSessionID, dwConnID);

	SessionInfo sInfo;
	sInfo.m_connID = dwConnID;
	sInfo.m_sessionID = newSessionID;
	sInfo.m_lastRecvTime = TimeUtil::GetTime();
	sInfo.m_createTime = time(NULL);
	sInfo.m_strIP.assign(IPBuf);
	sInfo.m_recvProto.SetSize(m_maxTpsPerConn);
	m_SessionsInfo.insert(sInfo);
    ++m_SessionCount;
	SessionInfo *pSessionInfo = m_SessionsInfo.find<SessionIndexer>(newSessionID);
	assert(pSessionInfo != NULL);
	StartLoginTimer(pSessionInfo);


	PtcT2C_LoginChallenge challengePtc;
	challengePtc.m_Data.set_challenge("hello");
	challengePtc.m_Data.set_session(newSessionID);
	if (!CClientLink::Instance()->Send(dwConnID, challengePtc))
	{
		LogWarn("send challenge to client failed!");
	}
	pSessionInfo->m_state = SESSION_CHALLENGE;
}

UINT32 SessionManager::GetGameServerId(SessionType sessionId)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<SessionIndexer>(sessionId);
	if (pSessionInfo == NULL)
	{
		LogError("get gameserver id, session id:[%s] not found", str(sessionId));
		return 0;
	}
	return pSessionInfo->m_gameserverId;
}

UINT32 SessionManager::GetGameServerId(UINT32 dwConnId)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<ConnIDIndexer>(dwConnId);
	if (pSessionInfo == NULL)
	{
		LogError("get gameserver id, conn id:[%s] not found", str(dwConnId));
		return 0;
	}
	return pSessionInfo->m_gameserverId;
}

void SessionManager::CloseSession(SessionType sessionID, UINT32 errorcode)
{
	CloseSession(m_SessionsInfo.find<SessionIndexer>(sessionID), errorcode);
}

void SessionManager::CloseSession(SessionInfo *pSessionInfo, UINT32 errorcode)
{
	if (pSessionInfo != NULL)
	{
		SendSessionError(pSessionInfo->m_sessionID, errorcode);
		CSTimer.ConnIDs.push_back(pSessionInfo->m_connID); // 1 秒后close
		SSInfo<<"onclose session id:"<< pSessionInfo->m_sessionID <<" errorcode:"<< errorcode <<END;
	}
}

void SessionManager::ForceCloseSession(SessionInfo *pSessionInfo, UINT32 errorcode)
{
	if (pSessionInfo != NULL)
	{
		SSWarn << "force close session id: " << pSessionInfo->m_sessionID << " errorcode: " << errorcode << END;
		CClientLink::Instance()->Close(pSessionInfo->m_connID);
	}
}

void SessionManager::SendSessionError(SessionType sessionID, UINT32 errorcode)
{
	if (0 != errorcode)
	{
		PtcT2C_ErrorNotify errPtc;
		errPtc.m_Data.set_errorno(errorcode);
		GetSessionSender(sessionID).Send(errPtc);
	}
}

UINT32 SessionManager::GetSessionConnID(SessionType sessionID)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<SessionIndexer>(sessionID);
	if (pSessionInfo == NULL)
	{
		return INVALID_CONN_ID;
	}

	return pSessionInfo->m_connID;
}

SessionSender SessionManager::GetSessionSender(SessionType sessionID)
{
	SessionSender sender(GetSessionConnID(sessionID));
	return sender;
}

SessionType SessionManager::SessionConvert(UINT32 dwConnID, UINT32 dwProtoType)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<ConnIDIndexer>(dwConnID);
	if (pSessionInfo == NULL)
	{
		return INVALID_SESSION_ID;
	}
	pSessionInfo->m_lastRecvTime = TimeUtil::GetTime();
	pSessionInfo->m_keepAliveTryTimes = 0;

	// 每秒最大请求限制
	UINT64 nowMs = TimeUtil::GetMilliSecond();	
	if (nowMs >= pSessionInfo->m_lastStatTimeMs + 3000)
	{
		pSessionInfo->m_protoCount = 0;
		pSessionInfo->m_lastStatTimeMs = nowMs;
	}
	pSessionInfo->m_recvProto.Push(dwProtoType);
	pSessionInfo->m_protoCount++;


	// 超过限制，强关并打印最近收到的协议
	if (pSessionInfo->m_protoCount > m_maxTpsPerConn * 3)
	{
		LogWarn("tps per conn over max, session[%llu], close", pSessionInfo->m_sessionID);
		DumpRecvProto(pSessionInfo);
		ForceCloseSession(pSessionInfo, KKSG::ERR_SESSION_KICKOFF);
		pSessionInfo->m_protoCount = 0;
		pSessionInfo->m_lastStatTimeMs = nowMs;
	}

	return pSessionInfo->m_sessionID;
}

void SessionManager::DumpRecvProto(SessionInfo* pSessionInfo)
{
	if (pSessionInfo == NULL)
	{
		return;
	}
	std::vector<UINT32> protos;
	pSessionInfo->m_recvProto.Get(protos);
	std::ostringstream oss;
	for (auto it = protos.begin(); it != protos.end(); ++it)
	{
		oss << CProtocol::GetProtoName(*it) << ",";
	}
	SSWarn << "Dump Recv Proto, Session:" << pSessionInfo->m_sessionID << ", protos: " << oss.str() << END;
}

SessionType SessionManager::GetSession(UINT32 dwConnID)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<ConnIDIndexer>(dwConnID);
	if (pSessionInfo == NULL)
	{
		return INVALID_SESSION_ID;
	}

	return pSessionInfo->m_sessionID;
}

void SessionManager::StartLoginTimer(SessionInfo *pSessionInfo)
{
	ITimer *pTimer = new LoginTimer(pSessionInfo);
	pSessionInfo->m_loginTimer = pTimer;
	pSessionInfo->m_loginTimerHandler = CTimerMgr::Instance()->SetTimer(pTimer, 0, SESSION_LOGIN_TIMEOUT, 1, __FILE__, __LINE__);
}

void SessionManager::StopLoginTimer(SessionInfo *pSessionInfo)
{
	if (pSessionInfo && pSessionInfo->m_loginTimer != NULL)
	{
		CTimerMgr::Instance()->KillTimer(pSessionInfo->m_loginTimerHandler);
		delete pSessionInfo->m_loginTimer;
		pSessionInfo->m_loginTimer = NULL;
		pSessionInfo->m_loginTimerHandler = INVALID_HTIMER;
	}
}

void SessionManager::StopLoginTimer(SessionType sessionID)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<SessionIndexer>(sessionID);
	if (pSessionInfo != NULL)
	{
		StopLoginTimer(pSessionInfo);
	}
	else
	{
		LogWarn("stop login timer not found session [%llu]", sessionID);
	}
}

void SessionManager::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	std::list<SessionType> NotAliveSessions;
	UINT32 now = TimeUtil::GetTime();
	auto i = m_SessionsInfo.begin();
	for (; i != m_SessionsInfo.end(); i++)
	{
		if (i->m_state == SESSION_DBVERIFY_FAILED)
		{
			NotAliveSessions.push_back(i->m_sessionID);
			continue;
		}

		// 登录状态和鉴权成功没登录角色的keeplive
		if (i->m_state == SESSION_LOGIN || i->m_state == SESSION_DBVERIFY_SUCCESS)
		{
			if (now - i->m_lastRecvTime > SESSION_KEEPALIVE_TIME)
			{
				if (i->m_keepAliveTryTimes >= SESSION_KEEPALIVE_TRYTIMES)
				{
					i->m_state = SESSION_KEEPALIVE_TIMEOUT;
					NotAliveSessions.push_back(i->m_sessionID);
					continue;
				}
				else
				{
					++i->m_keepAliveTryTimes;
					PtcT2C_KeepAlivePingReq keepAlivePtc;
					SessionSender sender(i->m_connID);
					sender.Send(keepAlivePtc);
				}
			}
		}
	}

	for (auto j = NotAliveSessions.begin(); j != NotAliveSessions.end(); ++j)
	{
		UINT64 qwSessionID = *j;
		LogDebug("remove keepalive timeout session [%llu]", qwSessionID);
		CloseSession(*j, KKSG::ERR_ALIVE_TIMEOUT);
	}

	for (auto k = m_SessionsWaitReconnect.begin(); k != m_SessionsWaitReconnect.end(); )
	{
		if ((now > GateConfig::Instance()->GetReconnectTimeout() + k->second.disconnectTime) && k->second.m_keepAliveForever == false)
		{
			PtcT2N_LogoutNtf logoutPtc;
			logoutPtc.m_sessionID = k->first;
			ControlLink::Instance()->SendTo(logoutPtc);
			LogInfo("remove reconnect session [%llu] from waiting queue", k->first);
			m_SessionsWaitReconnect.erase(k++);
		}
		else
		{
			++k;
		}
	}
}

void SessionManager::OnClose(UINT32 dwConnID)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<ConnIDIndexer>(dwConnID);
	if (pSessionInfo == NULL)
	{
		LogWarn("close connid [%u] not found", dwConnID);
		return;
	}

	LogInfo("close session [%llu] at state [%s]", pSessionInfo->m_sessionID, GetSessionStateString(pSessionInfo->m_state));
	StopLoginTimer(pSessionInfo);

	if (pSessionInfo->m_state == SESSION_KEEPALIVE_TIMEOUT)
	{
		// 长期保留断线重连的， gate上session没有了，但是不通知control踢人
		if (pSessionInfo->m_keepAliveForever)
		{
			LogInfo("session [%llu] keep alive timeout, don't send logout notify to ns", pSessionInfo->m_sessionID);
		}
		else
		{
			LogInfo("session [%llu] keep alive timeout, send logout notify to ns", pSessionInfo->m_sessionID);
			PtcT2N_LogoutNtf logoutPtc;
			logoutPtc.m_sessionID = pSessionInfo->m_sessionID;
			ControlLink::Instance()->SendTo(logoutPtc);
		}
	}
	else if (pSessionInfo->m_state == SESSION_DBVERIFY_SUCCESS)
	{
		LogInfo("session [%llu] close when verify success but no role login", pSessionInfo->m_sessionID);

		PtcT2N_LogoutNtf logoutPtc;
		logoutPtc.m_sessionID = pSessionInfo->m_sessionID;
		ControlLink::Instance()->SendTo(logoutPtc);
	}
	else if (pSessionInfo->m_state == SESSION_LOGIN)
	{
		LogInfo("put session [%llu] to reconnect queue", pSessionInfo->m_sessionID);
		WaitReconnSessionInfo info;
		info.disconnectTime = TimeUtil::GetTime();
		info.serverID = pSessionInfo->m_gameserverId;
		info.m_dwGsLine = pSessionInfo->m_dwGsLine;
		info.m_crossGs = pSessionInfo->m_crossGs;
		info.m_keepAliveForever = pSessionInfo->m_keepAliveForever;
		AddReconnectSession(pSessionInfo->m_sessionID, info);

		NotifyReconnect(pSessionInfo->m_sessionID, 0, 0, true, pSessionInfo->m_dwGsLine, pSessionInfo->m_crossGs, 0);
	}

    --m_SessionCount;
	m_SessionsInfo.erase<ConnIDIndexer>(dwConnID);
}

void SessionManager::DestroySessionByNS(SessionType sessionID)
{
	m_SessionsWaitReconnect.erase(sessionID);
	SessionInfo* pSessionInfo = m_SessionsInfo.find<SessionIndexer>(sessionID);
	if (pSessionInfo)
	{
		UINT32 connId = pSessionInfo->m_connID;
		m_SessionsInfo.erase<SessionIndexer>(sessionID);
		CNetProcessor::Instance()->Close(connId);
		--m_SessionCount;
	}
	LogInfo("Force Destroy Session[%llu] By NS", sessionID);
}

bool SessionManager::ReConnectBySession(UINT32 dwConnID, SessionType sessionID, UINT64 roleid, UINT32 sceneid, UINT32 delayid)
{
	UINT32 dwIP;
	UINT16 wPort;
	CNetProcessor::Instance()->GetRemoteAddr(dwConnID, dwIP, wPort);
	char IPBuf[64] = {0};
	unsigned char *p = (unsigned char *)&dwIP;
	sprintf(IPBuf, "%u.%u.%u.%u:%d", p[0], p[1], p[2], p[3], wPort);

	SessionInfo *pSessionInfo = m_SessionsInfo.find<ConnIDIndexer>(dwConnID);
	if (pSessionInfo == NULL)
	{
		return false;
	}

	if (pSessionInfo->m_state != SESSION_CHALLENGE)
	{
		LogWarn("reconnect state error: not in challenge state!");
		return false;
	}

	auto i = m_SessionsWaitReconnect.find(sessionID);
	if (i == m_SessionsWaitReconnect.end())
	{
		SessionInfo *pOldSession = m_SessionsInfo.find<SessionIndexer>(sessionID);
		if (pOldSession != NULL)
		{
			StopLoginTimer(pSessionInfo);
			m_SessionsInfo.erase<ConnIDIndexer>(dwConnID);

			UINT32 dwOldConnID = pOldSession->m_connID;
			SessionInfo sInfo = *pOldSession;
			sInfo.m_connID = dwConnID;
			sInfo.m_sessionID = sessionID;
			sInfo.m_lastRecvTime = TimeUtil::GetTime();
			sInfo.m_state = SESSION_LOGIN;
			sInfo.m_recvProto.SetSize(m_maxTpsPerConn);
			m_SessionsInfo.erase<SessionIndexer>(sessionID);
			m_SessionsInfo.insert(sInfo);
			CNetProcessor::Instance()->Close(dwOldConnID);

			NotifyReconnect(sessionID, roleid, sceneid, false, sInfo.m_dwGsLine, sInfo.m_crossGs, delayid);

			LogInfo("a client reconnect switch connid from [%u] to [%u]", dwOldConnID, dwConnID);
			return true;
		}

		LogInfo("a client reconnect from [%s] FAILED with session [%llu]", IPBuf, sessionID);
		return false;
	}

	UINT32 gameserverid = i->second.serverID;
	UINT32 dwLine = i->second.m_dwGsLine;
	bool crossGs = i->second.m_crossGs;
	m_SessionsWaitReconnect.erase(sessionID);

	// 关掉当前这个连接的定时器
	StopLoginTimer(pSessionInfo);
	m_SessionsInfo.erase<ConnIDIndexer>(dwConnID);

	// 重新插入新的SessionInfo,代替掉当前这个连接的Session
	SessionInfo sInfo;
	sInfo.m_connID = dwConnID;
	sInfo.m_sessionID = sessionID;
	sInfo.m_lastRecvTime = TimeUtil::GetTime();
	sInfo.m_createTime = time(NULL);
	sInfo.m_state = SESSION_LOGIN;
	sInfo.m_gameserverId = gameserverid;
	sInfo.m_dwGsLine = dwLine;
	sInfo.m_crossGs = crossGs;
	sInfo.m_recvProto.SetSize(m_maxTpsPerConn);
	m_SessionsInfo.insert(sInfo);

	NotifyReconnect(sessionID, roleid, sceneid, false, dwLine, crossGs, delayid);

	LogInfo("a client reconnect from [%s] with session [%llu]", IPBuf, sessionID);
	return true;
}

bool SessionManager::ChangeSessionState(SessionType sessionID, SessionState state)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<SessionIndexer>(sessionID);
	if (pSessionInfo != NULL)
	{
		pSessionInfo->m_state = state;
		return true;
	}

	return false;
}

SessionState SessionManager::GetSessionState(SessionType sessionID)
{
	SessionInfo *pSessionInfo = m_SessionsInfo.find<SessionIndexer>(sessionID);
	if (pSessionInfo != NULL)
	{
		return pSessionInfo->m_state;
	}

	return SESSION_INVALID;
}

bool DebugClose(const CmdLineArgs &args, std::string &outputMessage)
{
	auto it = SessionManager::Instance()->Begin();
	if (it != SessionManager::Instance()->End())
	{
		CSTimer.ConnIDs.push_back(it->m_connID);
	}
	return true;
}

bool SessionManager::Init()
{
	CmdLine::RegistCmd("close", DebugClose);
	CmdLine::RegistCmd("session", SessionCmdLine);
	m_maxTpsPerConn = GateConfig::Instance()->GetTpsPerConn();
	acceptTimer.Start();
	return true;
}

void SessionManager::Uninit()
{
	acceptTimer.Stop();
}

bool SessionManager::SessionCmdLine(const CmdLineArgs &args, std::string &outputMessage)
{
	Instance()->DumpSessions(outputMessage);
	return true;
}

void SessionManager::DumpSessions(std::string &outputMessage)
{
	std::stringstream oSS;
	oSS << "Session Dumps:\n";
	oSS << "SessionID  |  ConnID  |  state | connect time | last recv time\n";
	for (auto i = m_SessionsInfo.begin(); i != m_SessionsInfo.end(); ++i)
	{
		oSS << Hex(i->m_sessionID) << " " << ((i->m_connID>>16)&0xFFFF) << ":"
			<< (i->m_connID&0xFFFF) << " " << GetSessionStateString(i->m_state) <<  " "
			<<  i->m_createTime << " " << i->m_lastRecvTime << "\n";
	}
	
	oSS << "Session Wait for Connect:\n";
	for (auto j = m_SessionsWaitReconnect.begin(); j != m_SessionsWaitReconnect.end(); ++j)
	{
		oSS << Hex(j->first) << "\n";
	}

	outputMessage = oSS.str();
	LogInfo("%s", outputMessage.c_str());
}

void SessionManager::RemoveReconnectSession(SessionType sessionID)
{
	m_SessionsWaitReconnect.erase(sessionID);
}

void SessionManager::AddReconnectSession(SessionType sessionID, WaitReconnSessionInfo& info)
{
	m_SessionsWaitReconnect.insert(std::make_pair(sessionID, info));
}

WaitReconnSessionInfo* SessionManager::GetReconnectSession(SessionType sessionID)
{
	auto it = m_SessionsWaitReconnect.find(sessionID);
	return it == m_SessionsWaitReconnect.end() ? NULL : &it->second;
}

void SessionManager::GSShutDownCloseAllSession(UINT32 dwGSId)
{
	for (auto i = m_SessionsInfo.begin(); i != m_SessionsInfo.end(); ++i)
	{
		SessionInfo *pSession = &*i;
		if (pSession->m_gameserverId == dwGSId)
		{
			pSession->m_state = SESSION_GSCLOSED;
			CloseSession(pSession, KKSG::ERR_GS_CLOSED);
		}
	}
}

SessionInfo * SessionManager::FindSession(SessionType sessionID)
{
	return m_SessionsInfo.find<SessionIndexer>(sessionID);
}

SessionInfo* SessionManager::FindByConnID(UINT32 dwConnID)
{
	return m_SessionsInfo.find<ConnIDIndexer>(dwConnID);
}

bool SessionSender::Send(const char *pcData, UINT32 dwLen)
{
	if (m_dwConnID == INVALID_CONN_ID)
		return false;

	return CNetProcessor::Instance()->Send(m_dwConnID, pcData, dwLen);
}

bool SessionSender::Send(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
		return false;

	return CNetProcessor::Instance()->Send(m_dwConnID, roPtc);
}

bool SessionSender::valid()
{
	return m_dwConnID != INVALID_CONN_ID;
}

void SessionManager::NotifyReconnect(SessionType sessionid, UINT64 roleid, UINT32 sceneid, bool isadd, UINT32 dwGsLine, bool isCrossGs, UINT32 delayid)
{
	RpcT2G_ReconnectGs* rpc = RpcT2G_ReconnectGs::CreateRpc();
	rpc->m_oArg.set_isadd(isadd);
	rpc->m_oArg.set_sceneid(sceneid);
	rpc->m_oArg.set_delayrpc(delayid);
	rpc->m_oArg.set_roleid(roleid);
	rpc->m_sessionID = sessionid;
	if (isCrossGs)
	{
		RSLink::Instance()->SendTo(*rpc);
	}
	else
	{
		GSLink::Instance()->SendToLine(dwGsLine, *rpc);
	}
}
