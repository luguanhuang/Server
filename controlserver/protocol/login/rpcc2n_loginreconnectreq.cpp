#include "pch.h"
#include "login/rpcc2n_loginreconnectreq.h"
#include "account/accountsessionmgr.h"
#include "role/role.h"
#include "scene/rpcn2g_leavescene.h"
#include "network/gatelink.h"
#include "network/mslink.h"
#include "network/rslink.h"
#include "login/rpcn2g_notifyroleloginreconnect2gs.h"
#include "login/ptcn2t_notifygtsessionlogin.h"
#include "session/ptcn2t_sessionchange2gt.h"
#include "session/ptcn2r_reportsessioninfo.h"

// generate by ProtoGen at date: 2017/6/19 22:46:12

RPC_SERVER_IMPLEMETION(RpcC2N_LoginReconnectReq, LoginReconnectReqArg, LoginReconnectReqRes)

void RpcC2N_LoginReconnectReq::OnCall(const LoginReconnectReqArg &roArg, LoginReconnectReqRes &roRes)
{
	CReconnectSession* pRSession = CAccountSessionMgr::Instance()->GetReconnectSession(m_sessionID);
	if (pRSession == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		LogError("session[%llu] try reconnect failed, no reconnect session", m_sessionID);
		return;
	}
	const std::string& account = pRSession->account;
	CAccountSession* pAccount = CAccountSessionMgr::Instance()->GetByAccountID(account);
	if (pAccount == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		LogError("session[%llu] account[%s] try reconnect failed, no account session", m_sessionID, account.c_str());
		return;
	}
	CRole* pRole = pAccount->GetRole();
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		LogError("session[%llu] account[%s] try reconnect failed, no role online", m_sessionID, account.c_str());
		return;
	}

	if(pRole->GetState() != ROLE_STATE_IN_GAME)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		LogError("session[%llu] account[%s] try reconnect failed, role changing scene, role[%llu], name[%s]", m_sessionID, account.c_str(),
			pRole->GetID(), pRole->GetName().c_str());
		return;
	}

	UINT32 rpcId = DelayRpc();
	// 不重连, 踢掉旧在线角色, 重新readaccount
	if (roArg.reconnect() == false)
	{
		LogInfo("session[%llu] account[%s] role[%llu] name[%s] refuse reconnect", m_sessionID, account.c_str(), pRole->GetID(), pRole->GetName().c_str());
		RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
		rpc->m_sessionID = pRole->GetSessionID();	// 用旧session踢掉旧的在线角色
		rpc->m_oArg.add_rolelist(pRole->GetID());
		rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_LOGOUT);
		rpc->m_oArg.set_logouttype(KKSG::LOGOUT_RELOGIN_KICK_ACCOUNT);
		rpc->m_oArg.set_rpcid(rpcId);
		rpc->m_oArg.set_account(account);
		rpc->m_oArg.set_line(pRole->GetGsLine());
		pRole->SendMsgToGS(*rpc);
		pRole->SetState(ROLE_STATE_LEAVE_SCENE);
	}
	// 玩家重连: 1.通知gs玩家回来了 2. 成功之后通知gt删了老的session，通知ms session变了
	else
	{
		if (CAccountSessionMgr::Instance()->IsLoginReconnecting(account))
		{
			roRes.set_result(KKSG::ERR_STATE_ERROR);
			LogError("session[%llu] account[%s] role[%llu] name[%s] tyr reconnect failed, state error", m_sessionID, account.c_str(), pRole->GetID(), pRole->GetName().c_str());
			return;
		}
		LogInfo("session[%llu] account[%s] role[%llu] name[%s] start reconnect", m_sessionID, account.c_str(), pRole->GetID(), pRole->GetName().c_str());
		// 玩家跨服中， 需要建立新的的session转发关系
		if (pRole->IsInCrossGs())
		{
			PtcN2R_ReportSessionInfo ptc;
			KKSG::CrossRoleInfo* info = ptc.m_Data.add_roles();
			info->set_session(m_sessionID);
			info->set_roleid(pRole->GetID());
			info->set_name(pRole->GetName());
			ptc.m_Data.set_cross_gsline(pRole->GetGsLine());
			ptc.m_Data.set_is_add(true);
			ptc.m_Data.set_is_reconnect(true);
			RSLink::Instance()->SendTo(ptc);
		}
		RpcN2G_NotifyRoleLoginReconnect2Gs* rpc = RpcN2G_NotifyRoleLoginReconnect2Gs::CreateRpc();
		rpc->m_oArg.set_rpcid(rpcId);
		rpc->m_oArg.set_oldsession(pAccount->GetSessionID());
		rpc->m_oArg.set_newsession(m_sessionID);
		rpc->m_oArg.set_roleid(pRole->GetID());
		rpc->m_oArg.set_sceneid(pRole->GetSceneID());
		rpc->m_oArg.mutable_cinfo()->CopyFrom(pAccount->GetCliConf());
		pRole->SendMsgToGS(*rpc);
	}
}

void RpcC2N_LoginReconnectReq::OnDelayReplyRpc(const LoginReconnectReqArg &roArg, LoginReconnectReqRes &roRes, const CUserData &roUserData)
{
	CReconnectSession* pRSession = CAccountSessionMgr::Instance()->GetReconnectSession(m_sessionID);
	if (pRSession == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		LogError("session[%llu] try reconnect failed, no reconnect session", m_sessionID);
		return;
	}
	const std::string& account = pRSession->account;

	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
	if (nResult != KKSG::ERR_SUCCESS)
	{
		LogError("session[%llu] try reconnect failed, code [%u]", m_sessionID, nResult);
		CAccountSessionMgr::Instance()->RemoveReconnectAccount(m_sessionID);
		return;
	}

	// 玩家拒绝断线重连， 重新登录account, 可以重新选角色
	if (roArg.reconnect() == false)
	{
		do
		{
			KKSG::LoadAccountData* poAccount = (KKSG::LoadAccountData*)roUserData.m_pUserPtr;
			if (poAccount == NULL)
			{
				roRes.set_result(KKSG::ERR_FAILED);
				break;
			}

			CAccountSession* poSession = new CAccountSession;
			if(!poSession->Init(m_sessionID))
			{
				LogWarn("Account [%s] can't login because can not get shm sapce", pRSession->account.c_str());
				roRes.set_result(KKSG::ERR_REGISTER_NUM_LIMIT);
				delete poSession;
				break;
			}

			poSession->SetAccountID(pRSession->account);
			poSession->SetClientConf(pRSession->loginArg.clientinfo());
			poSession->SetLoginToken(pRSession->loginArg.token());
			poSession->LoadAllRoleData(*poAccount);
			poSession->SetLoginServerId(pRSession->loginArg.loginzoneid());
			poSession->SetServerId(pRSession->loginArg.serverid());

			CAccountSessionMgr::Instance()->LoginAccount(poSession);
		} while (0);
	}
	// 重连
	else
	{
		do 
		{
			CAccountSession* pAccount = CAccountSessionMgr::Instance()->GetByAccountID(account);
			if (pAccount == NULL)
			{
				LogError("session[%llu] account[%s] reconnect failed, no account session", m_sessionID, account.c_str());
				roRes.set_result(KKSG::ERR_FAILED);
				break;
			}
			CRole* pRole = pAccount->GetRole();
			if (pRole == NULL)
			{
				roRes.set_result(KKSG::ERR_FAILED);
				LogError("session[%llu] account[%s] reconnect failed, no role online", m_sessionID, account.c_str());
				break;
			}
			SessionType oldSession = pAccount->GetSessionID();
			// 替换session
			CAccountSessionMgr::Instance()->ConvertSession(oldSession, m_sessionID);
			// 通知gate可以删除老session了
			PtcN2T_SessionChange2Gt ptcGt;
			ptcGt.m_sessionID = oldSession;
			ptcGt.m_Data.set_oldsession(oldSession);
			ptcGt.m_Data.set_newsession(m_sessionID);
			ptcGt.m_Data.set_roleid(pRole->GetID());
			GateLink::Instance()->SendToSession(ptcGt);

			// 相当于选角成功
			CAccountSessionMgr::Instance()->NotifyUsedTokenToLoginServer(m_sessionID);
			PtcN2T_NotifyGtSessionLogin ntf;
			ntf.m_sessionID = m_sessionID;
			pRole->Send(ntf);
		} while (0);
	}

	CAccountSessionMgr::Instance()->RemoveReconnectAccount(m_sessionID);
}
