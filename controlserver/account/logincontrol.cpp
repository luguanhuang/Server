#include "pch.h"
#include "logincontrol.h"
#include "timeutil.h"
#include "accountsessionmgr.h"
#include "scene/gsmanager.h"
#include "login/rpcn2i_loginverify.h"
#include "network/loginlink.h"
#include "config.h"
#include "login/rpcn2d_nsreadaccountdata.h"
#include "scene/rpcn2g_leavescene.h"
#include "network/dblink.h"
#include "network/gslink.h"
#include "role/rolemanager.h"
#include "account/whitelistmgr.h"
#include "network/mslink.h"


INSTANCE_SINGLETON(CLoginControl)


CLoginControl::CLoginControl()
{
}

CLoginControl::~CLoginControl()
{
}

bool CLoginControl::Init()
{
	return true;
}

void CLoginControl::Uninit()
{
}

KKSG::ErrorCode CLoginControl::BeginLogin(const KKSG::LoginRequestNewArg& roArg, KKSG::LoginRequestNewRes& roRes, UINT64 qwSessionID)
{
	const std::string& strAccount = roArg.openid();
	const std::string& strToken = roArg.token();
	if(!MSLink::Instance()->IsConnected() || !MSLink::Instance()->GetVersioned())
	{
		return KKSG::ERR_MS_UNREADY;
	}
	if(!LoginLink::Instance()->IsConnected())
	{
		return KKSG::ERR_MS_UNREADY;
	}
	if(CGsManager::Instance()->GetGsNum() == 0)
	{
		LogError("No any game server connected, can not login");
		return KKSG::ERR_GS_UNREADY;
	}
	if(CGsManager::Instance()->GetGsInfo(MAIN_HALL_GS_LINE) == NULL)
	{
		LogError("No main hall game server, can not login");
		return KKSG::ERR_GS_UNREADY;
	}

	if(strAccount.empty() || qwSessionID == INVALID_SESSION_ID)
	{
		LogError("Account '%s' with session %llu is invalid, login request was denied", strAccount.c_str(), qwSessionID);
		return KKSG::ERR_ACCOUNT_INVALID;
	}

	if(ControlConfig::Instance()->UseWhiteList() && !CWhiteListMgr::Instance()->IsOpenIDInWhiteList(strAccount))
	{
		return KKSG::ERR_LOGIN_NOT_IN_WHITE_LIST;
	}

	std::string version = ControlConfig::Instance()->GetVersion().CheckVersion(roArg.ios(), roArg.android(), roArg.pc());
	if (!version.empty())
	{
		roRes.set_version(version);
		LogWarn("Version check failed");
		return KKSG::ERR_VERSION_FAILED;
	}


	CLoginRequest* poReq = GetLoginReq(strAccount);
	if(poReq != NULL)
	{
		if(poReq->IsValid())
		{
			LogError("Account '%s' with session %llu is logining, repeated login request with same account was denied", strAccount.c_str(), qwSessionID);
			return KKSG::ERR_FAILED;
		}
		else
		{
			CancelLogin(strAccount);
		}
	}

	poReq = GetLoginReq(qwSessionID);
	if(poReq != NULL)
	{
		if(poReq->IsValid())
		{
			LogError("Account '%s' with session %llu is logining, repeated login request with same session was denied", strAccount.c_str(), qwSessionID);
			return KKSG::ERR_FAILED;
		}
		else
		{
			CancelLogin(qwSessionID);
		}
	}


	CLoginRequest oReq;
	oReq.m_strAccount = strAccount;
	oReq.m_strToken   = strToken;
	oReq.m_qwSessionID = qwSessionID;
	oReq.m_qwBeginTime = TimeUtil::GetMilliSecond();
	oReq.m_qwEndTime = TimeUtil::GetMicroSecond();
	oReq.m_nState = LOGIN_STATE_LOADING_ACCOUNT_DATA;
	std::pair<CSession2RequestMap::iterator, bool> pr = m_oSession2ReqMap.insert(std::make_pair(qwSessionID, oReq));
	m_oAccount2ReqMap.insert(std::make_pair(strAccount, &pr.first->second));

	return KKSG::ERR_SUCCESS;
}

void CLoginControl::CancelLogin(UINT64 qwSessionID)
{
	CLoginRequest* poReq = GetLoginReq(qwSessionID);
	if(poReq == NULL) return;

	m_oAccount2ReqMap.erase(poReq->m_strAccount);
	m_oSession2ReqMap.erase(poReq->m_qwSessionID);
}

void CLoginControl::CancelLogin(const std::string& strAccount)
{
	CLoginRequest* poReq = GetLoginReq(strAccount);
	if(poReq == NULL) return;

	m_oAccount2ReqMap.erase(poReq->m_strAccount);
	m_oSession2ReqMap.erase(poReq->m_qwSessionID);
}

CLoginRequest* CLoginControl::GetLoginReq(UINT64 qwSessionID)
{
	CSession2RequestMap::iterator it = m_oSession2ReqMap.find(qwSessionID);
	return it == m_oSession2ReqMap.end() ? NULL : &it->second;
}

CLoginRequest* CLoginControl::GetLoginReq(const std::string& strAccount)
{
	CAccount2RequestMap::iterator it = m_oAccount2ReqMap.find(strAccount);
	return it == m_oAccount2ReqMap.end() ? NULL : it->second;
}

void CLoginControl::OnSessionTerminated(UINT64 qwSessionID)
{
	CLoginRequest* poReq = GetLoginReq(qwSessionID);
	if(poReq != NULL)
	{
		LogDebug("Account '%s' serverid [%u] with session %llu is closed when logining, Login will be canceled",
			poReq->m_strAccount.c_str(), poReq->m_dwServerId, qwSessionID);

		m_oAccount2ReqMap.erase(poReq->m_strAccount);
		m_oSession2ReqMap.erase(poReq->m_qwSessionID);
	}
}

KKSG::ErrorCode CLoginControl::StartTokenVerify(CLoginRequest* poReq)
{
	if(poReq == NULL) return KKSG::ERR_FAILED;

	RpcN2I_LoginVerify* rpc = RpcN2I_LoginVerify::CreateRpc();
	rpc->m_oArg.set_uid(poReq->m_qwSessionID);
	rpc->m_oArg.set_logintoken(poReq->m_strToken);
	rpc->m_oArg.set_serverid(poReq->m_dwServerId);
	rpc->m_oArg.set_isbackflowserver(ControlConfig::Instance()->IsBackFlowServer());

	//UINT32 serverID;
	//memcpy(&serverID, pFlow->m_token.c_str()+16, 4);
	//SSWarn << "login server: " << poReq->m_dwLoginServerId << END;
	if(!LoginLink::Instance()->SendTo(poReq->m_dwLoginServerId, *rpc))
	{
		LogError("send token verify for account=[%s] severid [%u] to login server failed",
			poReq->m_strAccount.c_str(), poReq->m_dwServerId);
		return KKSG::ERR_FAILED;
	}

	LogDebug("start token verify for account=[%s] serverid [%u]", poReq->m_strAccount.c_str(), poReq->m_dwServerId);
	return KKSG::ERR_SUCCESS;
}

void CLoginControl::OnTokenVerfiyCompleted(UINT64 qwSessionID, const std::string& strUserID, KKSG::ErrorCode nErrCode, const KKSG::LoginVerifyRes* roRes)
{
	CLoginRequest* poReq = GetLoginReq(qwSessionID);
	if(poReq == NULL)
	{
		return;
	}

	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		LogError("account=[%s] serverid[%u] token verify failed, error=[%d]", poReq->m_strAccount.c_str(), poReq->m_dwServerId, nErrCode);
		CUserData oUser((UINT32)nErrCode, NULL);
		CRpc::ReplyDelayRpc(poReq->m_dwRpcDelayed, oUser);
		CancelLogin(qwSessionID);
		return;
	}
	if (roRes != NULL)
	{
		poReq->m_accountBackFlow = roRes->account_backflow();
	}

	CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetByAccountID(poReq->m_strAccount);
	if(poAccount == NULL)
	{
		RpcN2D_NsReadAccountData *rpc = RpcN2D_NsReadAccountData::CreateRpc();
		rpc->m_sessionID = poReq->m_qwSessionID;
		rpc->m_oArg.set_account(poReq->m_strAccount);
		rpc->m_oArg.set_serverid(poReq->m_dwServerId);
		rpc->m_oArg.set_rpcid(poReq->m_dwRpcDelayed);
		DBLink::Instance()->SendTo(*rpc);
		return;
	}

	///> 踢掉旧账号
	CRole* poRole = poAccount->GetRole();
	if(poRole == NULL || (poRole->GetGsLine() == INVALID_LINE_ID && poRole->GetState() == ROLE_STATE_INVALID))
	{
		CAccountSessionMgr::Instance()->LogoutAccount(poReq->m_strAccount, KKSG::KICK_RELOGIN);
		RpcN2D_NsReadAccountData *rpc = RpcN2D_NsReadAccountData::CreateRpc();
		rpc->m_sessionID = poReq->m_qwSessionID;
		rpc->m_oArg.set_account(poReq->m_strAccount);
		rpc->m_oArg.set_serverid(poReq->m_dwServerId);
		rpc->m_oArg.set_rpcid(poReq->m_dwRpcDelayed);
		DBLink::Instance()->SendTo(*rpc);
		return;
	}

	///> 简单起见，正在切换场景的不允许登录
	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		UINT32 dwNow = TimeUtil::GetTime();
		if(poRole->GetStateAdjustBeginTime() == 0)
		{
			poRole->SetStateAdjustBeginTime(dwNow);
		}
		else
		{
			if(dwNow >= poRole->GetStateAdjustBeginTime() && dwNow - poRole->GetStateAdjustBeginTime() >= 60 * 3)
			{
				//做超时处理
				if(poRole->GetState() == ROLE_STATE_LEAVE_SCENE)
				{
					poRole->SetState(ROLE_STATE_IN_GAME);
					poRole->ClearSwitchInfo();
					poRole->SetStateAdjustBeginTime(0);
					LogInfo("account=[%s] serverid [%u] role=[%llu] name=[%s] adjust state from LEAVE_SCENE to IN_GAME", poReq->m_strAccount.c_str(), poReq->m_dwServerId, poRole->GetID(), poRole->GetName().c_str());
				}
				else if(poRole->GetState() == ROLE_STATE_ENTER_SCENE)
				{
					poRole->SetGsLine(poRole->GetDestGsLine());
					poRole->SetInCrossGs(poRole->GetDestIsCross());
					poRole->SetMapID(poRole->GetDestMapID());
					poRole->SetSceneID(poRole->GetDestSceneID());
					poRole->SetIsWatcher(poRole->GetDestIsWatcher());

					poRole->SetState(ROLE_STATE_IN_GAME);
					poRole->ClearSwitchInfo();
					poRole->SetStateAdjustBeginTime(0);
					LogInfo("account=[%s] serverid [%u] role=[%llu] name=[%s] adjust state from ENTER_SCENE to IN_GAME", poReq->m_strAccount.c_str(), poReq->m_dwServerId, poRole->GetID(), poRole->GetName().c_str());
				}
			}
		}

		if(poRole->GetState() != ROLE_STATE_IN_GAME)
		{
			LogError("account=[%s] serverid [%u] role=[%llu] name=[%s] is changing scene, state=[%d], the login request was denied",
				poReq->m_strAccount.c_str(), poReq->m_dwServerId, poRole->GetID(), poRole->GetName().c_str(), poRole->GetState());
			CUserData oUser(ERR_STATE_ERROR, NULL);
			CRpc::ReplyDelayRpc(poReq->m_dwRpcDelayed, oUser);
			CancelLogin(qwSessionID);
			return;
		}
	}

	if(poRole->GetStateAdjustBeginTime() != 0)
	{
		poRole->SetStateAdjustBeginTime(0);
	}

	// 有一个正常的role在场景中

	// 断线重连， 不踢role和account
	if (poRole->GetServerID() == poReq->m_dwServerId && poRole->CanLoginReconnect())
	{
		poReq->m_sceneTemplateId = poRole->GetMapID();
		poReq->m_sceneTime = 100;		// TODO
		RpcN2D_NsReadAccountData *rpc = RpcN2D_NsReadAccountData::CreateRpc();
		rpc->m_sessionID = poReq->m_qwSessionID;
		rpc->m_oArg.set_account(poReq->m_strAccount);
		rpc->m_oArg.set_serverid(poReq->m_dwServerId);
		rpc->m_oArg.set_rpcid(poReq->m_dwRpcDelayed);
		rpc->m_oArg.set_readtype(KKSG::READ_ACCOUNT_DATA_LOGIN);
		DBLink::Instance()->SendTo(*rpc);
	}
	else
	{
		///> 踢掉旧在线角色
		RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
		rpc->m_sessionID = poRole->GetSessionID();	// 用旧session踢掉旧的在线角色
		rpc->m_oArg.add_rolelist(poRole->GetID());
		rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_LOGOUT);
		rpc->m_oArg.set_logouttype(KKSG::LOGOUT_RELOGIN_KICK_ACCOUNT);
		rpc->m_oArg.set_rpcid(poReq->m_dwRpcDelayed);
		rpc->m_oArg.set_account(poReq->m_strAccount);
		rpc->m_oArg.set_line(poRole->GetGsLine());
		poRole->SendMsgToGS(*rpc);
		poRole->SetState(ROLE_STATE_LEAVE_SCENE);
	}
}

void CLoginControl::LockName(const std::string& strName)
{
	m_oNameLocked.insert(strName);
}

void CLoginControl::UnlockName(const std::string& strName)
{
	m_oNameLocked.erase(strName);
}

bool CLoginControl::IsNameLocked(const std::string& strName) const
{
	return m_oNameLocked.find(strName) != m_oNameLocked.end();
}

bool CLoginRequest::IsValid() const
{
	UINT64 qwNowTime = TimeUtil::GetMilliSecond();
	if(qwNowTime > m_qwBeginTime && qwNowTime - m_qwBeginTime >= 60 * 1000)
	{
		return false;
	}

	return true;
}