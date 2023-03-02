#include "pch.h"
#include "login/rpct2n_loginrequestnew.h"
#include "session/sessionmanager.h"
#include "network/gslink.h"
#include "network/controllink.h"
#include "login/ptct2n_logoutntf.h"

// generate by ProtoGen at date: 2016/11/3 2:18:52

RPC_CLIENT_IMPLEMETION(RpcT2N_LoginRequestNew, LoginRequestNewArg, LoginRequestNewRes)


static void DelayRepy(SessionInfo* session, UINT32 errorcode, KKSG::LoginRequestNewRes* ret)
{
	CUserData oUserData;
	oUserData.m_dwUserData = errorcode;
	oUserData.m_pUserPtr = ret;
	CRpc::ReplyDelayRpc(session->m_dwRpcID, oUserData);
	session->m_dwRpcID = 0;
}

void RpcT2N_LoginRequestNew::OnReply(const LoginRequestNewArg &roArg, const LoginRequestNewRes &roRes, const CUserData &roUserData)
{
	SessionInfo *pSession = SessionManager::Instance()->FindSession(m_sessionID);

	SessionManager *pSessionManager = SessionManager::Instance();
	if (roRes.result() != ERR_SUCCESS && roRes.result() != ERR_ACCOUNT_QUEUING)
	{
		LogInfo("login result from ns [failed:%d] session=[%llu]", roRes.result(), m_sessionID);
		if (pSession != NULL)
		{
			if (pSession->m_dwRpcID != 0)
			{
				DelayRepy(pSession, roRes.result(), const_cast<LoginRequestNewRes*>(&roRes));
			}
			else
			{
				pSession->m_state = SESSION_DBVERIFY_FAILED;
				SessionManager::Instance()->CloseSession(pSession, roRes.result());
			}
		}
	}
	else
	{
		if (pSession == NULL)
		{
			// 在MS验证期间，客户端连接断开了
			PtcT2N_LogoutNtf logoutPtc;
			logoutPtc.m_sessionID = m_sessionID;
			ControlLink::Instance()->SendTo(logoutPtc);
			LogWarn("login result from ns success, account=[%s] session=[%llu], but session was closed, send logout notify to ns", roArg.openid().c_str(), m_sessionID);
		}
		else
		{
			pSession->m_state = SESSION_DBVERIFY_SUCCESS;
			pSessionManager->StopLoginTimer(m_sessionID);
			if (pSession->m_dwRpcID != 0)
			{
				DelayRepy(pSession, roRes.result(), const_cast<LoginRequestNewRes*>(&roRes));
			}
			LogInfo("login result from ns success, account=[%s] session=[%llu]", roArg.openid().c_str(), m_sessionID);
		}
	}
}

void RpcT2N_LoginRequestNew::OnTimeout(const LoginRequestNewArg &roArg, const CUserData &roUserData)
{
	SessionInfo *pSession = SessionManager::Instance()->FindSession(m_sessionID);
	if (NULL != pSession)
	{
		DelayRepy(pSession, ERR_LOGIN_TIMEOUT, NULL);
	}
	LogWarn("close client session:[%llu] for timeout", m_sessionID);
}
