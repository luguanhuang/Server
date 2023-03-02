#include "pch.h"
#include "cmdline.h"
#include "timeutil.h"
#include "config.h"
#include "appid.h"
#include "accountsessionmgr.h"
#include "role/rolemanager.h"
#include "scene/rpcn2g_leavescene.h"
#include "login/ptcn2t_kickaccount.h"
#include "network/gslink.h"
#include "logincontrol.h"
#include "network/gatelink.h"
#include "util/dbutil.h"
#include "common/ptcn2d_onlinenumntf.h"
#include "login/ptcn2i_usedlogintokennotify.h"
#include "network/dblink.h"
#include "network/mslink.h"
#include "network/loginlink.h"
#include "time.h"
#include "queuing/queuingmgr.h"
#include "role/ptcn2m_newrolecreated.h"
#include "logutil.h"
#include "shm/shmmgr.h"
#include "common/ptcn2r_nsclosentf.h"
#include "network/rslink.h"
#include "common/ptcn2i_updateserverstate2login.h"
#include "config.h"
#include "table/globalconfig.h"
#include "account/ptcn2d_accountlogout.h"


INSTANCE_SINGLETON(CAccountSessionMgr)


#define STATE_LOG_INTERVAL	(1 * 60 * 1000)


CAccountSessionMgr::CAccountSessionMgr()
:m_hReportTimer(INVALID_HTIMER)
,m_registerAccount(0)
{
	memset(m_aOnlineArray, 0, sizeof(m_aOnlineArray));
}

CAccountSessionMgr::~CAccountSessionMgr()
{
}

void CAccountSessionMgr::AddSession(CAccountSession* session)
{
	m_sessions.insert(session);
	IncOnlineNum(session->GetPlat());
}

CAccountSession * CAccountSessionMgr::GetBySession(SessionType sessionID)
{
	return m_sessions.find<SessionIndex>(sessionID);
}

CAccountSession * CAccountSessionMgr::GetByAccountID(const std::string &accountID)
{
	return m_sessions.find<AccountIndex>(accountID);
}

void CAccountSessionMgr::RemoveBySession(SessionType sessionID)
{
	CAccountSession *pSession = GetBySession(sessionID);
	if (pSession != NULL)
	{
		LogDebug("remove session account [%s] serverid [%u]", pSession->GetAccountID().c_str(), pSession->GetServerId());
	}
	else
	{
		LogWarn("remove session [%s] not exist", str(sessionID));
		return;
	}

	DecOnlineNum(pSession->GetPlat());
	m_sessions.erase<SessionIndex>(sessionID);
	delete pSession;
}

void CAccountSessionMgr::RemoveByAccountID(const std::string & accountID)
{
	CAccountSession *pSession = GetByAccountID(accountID);
	if (pSession != NULL)
	{
		LogDebug("remove session account [%s] serverid [%u] ", accountID.c_str(), pSession->GetServerId());
	}
	else
	{
		LogWarn("remove session account [%s] not exist", accountID.c_str());
		return;
	}

	DecOnlineNum(pSession->GetPlat());
	m_sessions.erase<AccountIndex>(accountID);
	delete pSession;
}

static bool SessionCmdLine(const CmdLineArgs &args, std::string &outputMessage)
{
	CAccountSessionMgr *pManager = CAccountSessionMgr::Instance();
	pManager->PrintSessions();
	return true;
}

void CAccountSessionMgr::PrintSessions()
{
	std::stringstream ss;
	ss << "\n---------- login sessions --------------\n";
	int j = 0;
	for (auto i = m_sessions.begin(); i != m_sessions.end(); ++i, ++j)
	{
		auto poSession = *i;
		ss << j << ". account:" << poSession->GetAccountID() << ", session:" << str(poSession->GetSessionID()) << "\n";
	}

	LogInfo("%s", ss.str().c_str());
}

void CAccountSessionMgr::IncOnlineNum(KKSG::PlatType nPlat)
{
	if(!KKSG::PlatType_IsValid(nPlat))
	{
		return;
	}

	m_aOnlineArray[nPlat]++;
}

void CAccountSessionMgr::DecOnlineNum(KKSG::PlatType nPlat)
{
	if(!KKSG::PlatType_IsValid(nPlat))
	{
		return;
	}

	if(m_aOnlineArray[nPlat] == 0)
	{
		return;
	}

	m_aOnlineArray[nPlat]--;
}

UINT32 CAccountSessionMgr::GetOnlineNum(KKSG::PlatType nPlat) const
{
	if(!KKSG::PlatType_IsValid(nPlat))
	{
		return 0;
	}

	return m_aOnlineArray[nPlat];
}

UINT32 CAccountSessionMgr::GetAllOnlineNum() const
{
	UINT32 count = 0;
	for (int i = 0; i <= KKSG::PlatType_MAX; ++i)
	{
		count += m_aOnlineArray[i];
	}
	return count;
}

bool CAccountSessionMgr::Init()
{
	CmdLine::RegistCmd("session", SessionCmdLine);

	m_hReportTimer = CTimerMgr::Instance()->SetTimer(&m_oReportTimer, 0, STATE_LOG_INTERVAL, -1, __FILE__, __LINE__);
	if (INVALID_HTIMER == m_hReportTimer)
	{
		LogWarn("Set ReportTimer failed");
		return false;
	}

	m_oRoleIDGen.Init();
	RecoverFromShm();
	return true;
}

void CAccountSessionMgr::Uninit()
{
	if(m_hReportTimer != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_hReportTimer);
		m_hReportTimer = INVALID_HTIMER;
	}
	m_sessions.clear();
}

void CAccountSessionMgr::RecoverFromShm()
{
	std::vector<CShmAccountSession*> shmAccount;
	ShmMgr::Instance()->GetAccountShm().GetAll(shmAccount);
	if (shmAccount.empty())
	{
		return;
	}
	LogWarn("Recover %u account session from shm", shmAccount.size());

	for (size_t i = 0; i < shmAccount.size(); ++i)
	{
		CShmAccountSession* shmData = shmAccount[i];
		CAccountSession* poSession = new CAccountSession;
		poSession->Init(shmData);
		UINT64 roleId = poSession->GetRoleID();
		if (roleId)
		{
			CRole* poRole = CRoleManager::Instance()->GetByRoleID(roleId);
			poSession->SetRole(poRole);
		}

		AddSession(poSession);
		LogInfo("Recover account[%s] serverid [%u] ", poSession->GetAccountID().c_str(), poSession->GetServerId());
	}
}

CAccountSession* CAccountSessionMgr::ConvertSession(SessionType oldSession, SessionType newSession)
{
	CAccountSession* pOldAccount = GetBySession(oldSession);
	if (pOldAccount == NULL)
	{
		return NULL;
	}
	CRole* pOldRole = pOldAccount->GetRole();
	CRole* pNewRole = NULL;
	if (pOldRole)
	{
		pNewRole = CRoleManager::Instance()->ConvertSession(oldSession, newSession);
	}

	CAccountSession* pNewAccount = new CAccountSession;
	if (!pNewAccount->Init(newSession))
	{
		delete pNewAccount;
		return NULL;
	}

	CShmAccountSession* pTempShm = pNewAccount->m_poShmData;
	*pNewAccount = *pOldAccount;
	memcpy(pTempShm, pOldAccount->m_poShmData, sizeof(CShmAccountSession));
	pNewAccount->m_poShmData = pTempShm;
	pNewAccount->SetSessionID(newSession);
	pNewAccount->SetRole(pNewRole);

	RemoveBySession(oldSession);
	AddSession(pNewAccount);
	return pNewAccount;
}

void CAccountSessionMgr::LoginAccount(CAccountSession* session)
{
	if (session == NULL)
	{
		return;
	}
	AddSession(session);
	CAccountSession* poAccount = GetByAccountID(session->GetAccountID());
	poAccount->SetState(ACCOUNT_STATE_IN_LOBBY);

	CLoginControl::Instance()->CancelLogin(session->GetSessionID());
	LogInfo("Account [%s], session [%llu] serverid [%u] Login", session->GetAccountID().c_str(), session->GetSessionID(), session->GetServerId());

}

void CAccountSessionMgr::LogoutAccount(const std::string& accountID, KKSG::KickType nKickType)
{
	CAccountSession* poAccount = GetByAccountID(accountID);
	if(poAccount == NULL)
	{
		return;
	}

	UINT32 rolecount = poAccount->GetProfessionCount();

	//if(nKickType != KKSG::KICK_HG)
	{
		PtcN2T_KickAccount oKick;
		oKick.m_sessionID = poAccount->GetSessionID();
		oKick.m_Data.set_reason(nKickType);
		GateLink::Instance()->SendToSession(oKick);
	}

	std::string strAccountID = accountID;
	UINT32 serverId = poAccount->GetServerId();

	CRoleManager::Instance()->LogoutRole(poAccount->GetSessionID());
	LogInfo("Account [%s], session [%llu] serverid[%u] Logout", strAccountID.c_str(), poAccount->GetSessionID(), poAccount->GetServerId());
	RemoveByAccountID(strAccountID);

	QueuingMgr::Instance()->PopWait(strAccountID, rolecount);

	// account leave from queue, send msg to db detect if account is empty(delete it)
	if (0 == rolecount)
	{
		PtcN2D_AccountLogout ptc;
		ptc.m_Data.set_account(strAccountID);
		ptc.m_Data.set_serverid(serverId);
		DBLink::Instance()->SendTo(ptc);
		LogInfo("Try to remove empty account [%s] serverId[%u]", strAccountID.c_str(), serverId);
	}
}

CRole* CAccountSessionMgr::LoginRole(UINT64 sessionID, const KKSG::RoleAllInfo& roInfo)
{
	CAccountSession* poAccount = GetBySession(sessionID);
	if(poAccount == NULL)
	{
		return NULL;
	}

	CRole* poRole = CRoleManager::Instance()->LoginRole(&roInfo, poAccount->GetCliConf(), sessionID);
	if(poRole == NULL)
	{
		return NULL;
	}

	poAccount->SetRole(poRole);
	return poRole;
}

void CAccountSessionMgr::LogoutRole(UINT64 sessionID)
{
	CAccountSession* poAccount = GetBySession(sessionID);
	if(poAccount == NULL)
	{
		return;
	}

	CRoleManager::Instance()->LogoutRole(sessionID);
	poAccount->SetRole(NULL);
}

KKSG::ErrorCode CAccountSessionMgr::BeginCreateRole(SessionType qwSession, KKSG::RoleType nType, const std::string& strName)
{
	CAccountSession* poAccount = GetBySession(qwSession);
	if(poAccount == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	if(!MSLink::Instance()->IsConnected())
	{
		return KKSG::ERR_MS_UNREADY;
	}

	//角色数量检查
	INT32 nSlot = MapProfessionToSlot(nType);
	if (!poAccount->IsProfessionValid(nSlot))
	{
		return KKSG::ERR_FAILED;
	}
	if(!poAccount->IsProfessionSlotEmpty(nSlot))
	{
		return KKSG::ERR_ACCOUNT_ROLE_FULL;
	}

	if (nSlot + 1 > GetGlobalConfig().MaxRole)
	{
		return KKSG::ERR_ROLE_NOTEXIST;
	}

	if(CLoginControl::Instance()->IsNameLocked(strName))
	{
		return KKSG::ERR_NAME_EXIST;
	}

	poAccount->SetState(ACCOUNT_STATE_CREATE_ROLE);
	return KKSG::ERR_SUCCESS;
}

UINT64 CAccountSessionMgr::NewRoleID(UINT32 serverID)
{
	return m_oRoleIDGen.NewRoleID(serverID);
}

void CAccountSessionMgr::OnCreateRoleReply(SessionType qwSession, KKSG::ErrorCode nResult, const KKSG::RoleAllInfo& roRoleInfo)
{
	CAccountSession* poAccount = GetBySession(qwSession);
	if(poAccount == NULL)
	{
		return;
	}

	poAccount->SetState(ACCOUNT_STATE_IN_LOBBY);
	if(nResult == KKSG::ERR_SUCCESS)
	{
		LogInfo("Account [%s], serverid [%u] session [%llu] create role [%llu] success",
			poAccount->GetAccountID().c_str(), poAccount->GetServerId(), qwSession, roRoleInfo.brief().roleid());
		INT32 nSlot = MapProfessionToSlot(roRoleInfo.brief().type());
		poAccount->SetRoleId(nSlot, roRoleInfo.brief().roleid());

		PtcN2M_NewRoleCreated oPtc;
		oPtc.m_Data.mutable_roledata()->CopyFrom(roRoleInfo);
		oPtc.m_Data.mutable_clientinfo()->CopyFrom(poAccount->GetCliConf());
		oPtc.m_Data.set_totalrolenum(poAccount->GetProfessionCount());
		MSLink::Instance()->SendTo(oPtc);
	}
	else
	{
		LogError("Account [%s], serverid[%u] session [%llu] create role failed, error code %d",
			poAccount->GetAccountID().c_str(), poAccount->GetServerId(), qwSession, nResult);
	}
}

void CAccountSessionMgr::OnCreateRoleTimeout(SessionType qwSession)
{
	CAccountSession* poAccount = GetBySession(qwSession);
	if(poAccount == NULL)
	{
		return;
	}

	poAccount->SetState(ACCOUNT_STATE_IN_LOBBY);
	LogError("Account '%s' serverid [%u] create role timeout", poAccount->GetAccountID().c_str(), poAccount->GetServerId());
}

UINT64 CAccountSessionMgr::GetRoleIDByIndex(SessionType qwSession, INT32 nIndex)
{
	CAccountSession* poAccount = GetBySession(qwSession);
	if(poAccount == NULL)
	{
		return 0;
	}
	return poAccount->GetRoleIdBySlot(nIndex);
}

void CAccountSessionMgr::KickoutPlayerOnGsLine(UINT32 dwLine, bool isCross)
{
	std::vector<std::string> oKickList;
	for(CAccountSessionMap::iterator it = m_sessions.begin(); it != m_sessions.end(); ++it)
	{
		CAccountSession* poSession = *it;
		if(poSession->GetRole() == NULL) continue;
		if(poSession->GetRole()->GetGsLine() != dwLine) continue;

		if (poSession->GetRole()->IsInCrossGs() == isCross)
		{
			oKickList.push_back(poSession->GetAccountID());
		}
	}

	if(!oKickList.empty())
	{
		LogWarn("Gameserver line [%u] crash, Kickout all account and roles in %s_%u", dwLine, GetGsPrefix(isCross), dwLine);
	}

	for(std::vector<std::string>::size_type i = 0; i < oKickList.size(); ++i)
	{
		LogoutAccount(oKickList[i], KKSG::KICK_SERVER_SHUTDOWN);
	}
}

void CAccountSessionMgr::KickoutAllCrossPlayer()
{
	std::vector<std::string> oKickList;
	for(CAccountSessionMap::iterator it = m_sessions.begin(); it != m_sessions.end(); ++it)
	{
		CAccountSession* poSession = *it;
		if(poSession->GetRole() == NULL) continue;
		if (poSession->GetRole()->IsInCrossGs())
		{
			oKickList.push_back(poSession->GetAccountID());
		}
		if(poSession->GetRole()->GetState() == ROLE_STATE_ENTER_SCENE && poSession->GetRole()->GetDestIsCross())
		{
			oKickList.push_back(poSession->GetAccountID());
		}
	}

	if(!oKickList.empty())
	{
		LogWarn("Router server close, Kickout all cross roles");
	}

	for(std::vector<std::string>::size_type i = 0; i < oKickList.size(); ++i)
	{
		LogoutAccount(oKickList[i], KKSG::KICK_SERVER_SHUTDOWN);
	}
}

void CAccountSessionMgr::KickoutPlayerOnGateLine(UINT32 dwLine)
{
	std::vector<CAccountSession*> oKickList;
	for(CAccountSessionMap::iterator it = m_sessions.begin(); it != m_sessions.end(); ++it)
	{
		CAccountSession* poSession = *it;
		UINT32 dwRsLine = (UINT32)((poSession->GetSessionID() >> SessionGateIDShift) & 0xFF);
		if(dwRsLine != dwLine) continue;

		oKickList.push_back(poSession);
	}

	if(!oKickList.empty())
	{
		LogWarn("Gateserver line [%u] crash, Kickout all account and roles in gt_%u", dwLine, dwLine);
	}

	for(std::vector<CAccountSession*>::size_type i = 0; i < oKickList.size(); ++i)
	{
		CAccountSession* poSession = oKickList[i];
		CRole* poRole = poSession->GetRole();
		if(poRole == NULL || poRole->GetGsLine() == INVALID_LINE_ID)
		{
			LogoutAccount(poSession->GetAccountID(), KKSG::KICK_NORMAL);
		}
		else
		{
			RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
			rpc->m_sessionID = poSession->GetSessionID();
			rpc->m_oArg.add_rolelist(poRole->GetID());
			rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_LOGOUT);
			rpc->m_oArg.set_logouttype(KKSG::LOGOUT_ACCOUNT_NORMAL);
			rpc->m_oArg.set_rpcid(0);
			rpc->m_oArg.set_account(poSession->GetAccountID());
			rpc->m_oArg.set_line(poRole->GetGsLine());
			poRole->SendMsgToGS(*rpc);
		}
	}
}

void CAccountSessionMgr::PackSessionClientInfo(SessionType qwSessionID, KKSG::ClientInfo& roCliInfo, UINT32& roleNum)
{
	roleNum = 0;
	CAccountSession* poAccount = GetBySession(qwSessionID);
	if(poAccount == NULL)
	{
		return;
	}
	
	roCliInfo.CopyFrom(poAccount->GetCliConf());
	roleNum = poAccount->GetProfessionCount();
}

void CAccountSessionMgr::NotifyUsedTokenToLoginServer(SessionType qwSessionID)
{
	CAccountSession* poAccount = GetBySession(qwSessionID);
	if(poAccount == NULL)
	{
		return;
	}

	if(poAccount->GetLoginToken().empty())
	{
		return;
	}

	PtcN2I_UsedLoginTokenNotify oPtc;
	oPtc.m_Data.set_serverid(poAccount->GetServerId());	// 更新lastserver
	oPtc.m_Data.set_logintoken(poAccount->GetLoginToken());
	LoginLink::Instance()->SendTo(poAccount->GetLoginServerId(), oPtc);

	poAccount->SetLoginToken("");
}

void CAccountSessionMgr::OnMysqlConnectLost()
{
	const std::map<UINT32, GateServerInfo>& roGateMap = GateLink::Instance()->GetGateInfoMap();
	if(roGateMap.empty()) return;

	MysqlLostKickTimer* poKickTimer = new MysqlLostKickTimer();

	std::map<UINT32, GateServerInfo>::const_iterator it = roGateMap.begin();
	std::map<UINT32, GateServerInfo>::const_iterator itEnd = roGateMap.end();
	for(; it != itEnd; ++it)
	{
		poKickTimer->m_oLineList.push_back(it->first);
	}

	CTimerMgr::Instance()->SetTimer(poKickTimer, 0, 2000, poKickTimer->m_oLineList.size(), __FILE__, __LINE__);
}

void CAccountSessionMgr::OnlineReport()
{
	PtcN2D_OnlineNumNtf ptc;

	ptc.m_Data.set_timekey(TimeUtil::GetTime());
	ptc.m_Data.set_appid(ControlConfig::Instance()->GetAppType());
	ptc.m_Data.set_serverid(ControlConfig::Instance()->GetServerID());
	ptc.m_Data.set_zoneid(ControlConfig::Instance()->GetServerID());
	ptc.m_Data.set_iosnum(0);
	ptc.m_Data.set_androidnum(0);

	UINT32 dwRoleCount = CRoleManager::Instance()->GetRoleCount();
	if(ControlConfig::Instance()->GetPlatType() == KKSG::PLAT_IOS)
	{
		ptc.m_Data.set_iosnum(dwRoleCount); 
	}
	else if(ControlConfig::Instance()->GetPlatType() == KKSG::PLAT_ANDROID)
	{
		ptc.m_Data.set_androidnum(dwRoleCount); 
	}


	DBLink::Instance()->SendTo(ptc);

	// 通知login本服状态
	PtcN2I_UpdateServerState2Login ntf;
	ntf.m_Data.set_online_role(dwRoleCount);
	ntf.m_Data.set_serverid(ControlConfig::Instance()->GetServerID());
	auto& allServerIds = ControlConfig::Instance()->GetServerIds();
	for (auto it = allServerIds.begin(); it != allServerIds.end(); ++it)
	{
		ntf.m_Data.add_serverids(*it);
	}

	UINT32 registerNum = CAccountSessionMgr::Instance()->GetRegisterAccount();
	if (registerNum != 0)
	{
		ntf.m_Data.set_register_account(registerNum);
	}
	std::vector<UINT32> loginIds;;
	LoginLink::Instance()->GetAllLogins(loginIds);
	if (loginIds.empty())
	{
		return;
	}
	UINT32 dbIndex = rand() % loginIds.size();
	for (size_t i = 0; i < loginIds.size(); ++i)
	{
		if (i == dbIndex)
		{
			ntf.m_Data.set_write_db(true);
		}
		else
		{
			ntf.m_Data.set_write_db(false);
		}
		LoginLink::Instance()->SendTo(loginIds[i], ntf);
	}
}

void CAccountSessionMgr::MysqlLostKickTimer::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 dwLine = m_oLineList.back();
	m_oLineList.pop_back();

	LogInfo("Mysql Connection Lost, Kick out role on gt_%u", dwLine);
	CAccountSessionMgr::Instance()->KickoutPlayerOnGateLine(dwLine);

	if(m_oLineList.empty())
	{
		delete this;
	}
}

void CAccountSessionMgr::OnlineReportTimer::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CAccountSessionMgr::Instance()->OnlineReport();
}

CReconnectSession* CAccountSessionMgr::GetReconnectSession(SessionType session)
{
	auto it = m_reconnectSession.find(session);
	return it == m_reconnectSession.end() ? NULL : it->second;
}

void CAccountSessionMgr::SetReconnectSession(SessionType session, CReconnectSession* pRSession)
{
	if (pRSession == NULL)
	{
		return;
	}
	auto it = m_reconnectSession.find(session);
	if (it != m_reconnectSession.end())
	{
		LogError("already has reconnect session[%llu]", session);
		delete it->second;
		m_reconnectSession.erase(it);
	}
	m_reconnectSession.insert(std::make_pair(session, pRSession));
}

void CAccountSessionMgr::RemoveReconnectAccount(SessionType session)
{
	auto it = m_reconnectSession.find(session);
	if (it == m_reconnectSession.end())
	{
		return;
	}
	m_reconnectingAccount.erase(it->second->account);
	delete it->second;
	m_reconnectSession.erase(it);
}

bool CAccountSessionMgr::IsLoginReconnecting(const std::string& account)
{
	auto it = m_reconnectingAccount.find(account);
	return it != m_reconnectingAccount.end();
}

void CAccountSessionMgr::SetLoginReconnecting(const std::string& account)
{
	m_reconnectingAccount.insert(account);
}
