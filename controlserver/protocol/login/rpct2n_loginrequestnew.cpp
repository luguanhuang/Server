#include "pch.h"
#include "login/rpct2n_loginrequestnew.h"
#include "account/logincontrol.h"
#include "account/accountsessionmgr.h"
#include "queuing/queuingmgr.h"
#include "config.h"

// generate by ProtoGen at date: 2016/11/3 2:18:52

RPC_SERVER_IMPLEMETION(RpcT2N_LoginRequestNew, LoginRequestNewArg, LoginRequestNewRes)

void RpcT2N_LoginRequestNew::OnCall(const LoginRequestNewArg &roArg, LoginRequestNewRes &roRes)
{
	// 先检查serverid
	if (!ControlConfig::Instance()->IsValidServerId(roArg.serverid()))
	{
		roRes.set_result(KKSG::ERR_FAILED);
		LogError("Invalid serverid [%u], account[%s]", roArg.serverid(), roArg.openid().c_str());
		return;
	}

	KKSG::ErrorCode nErrCode = CLoginControl::Instance()->BeginLogin(roArg, roRes, m_sessionID);
	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(nErrCode);
		return;
	}

	CLoginRequest* poRequest = CLoginControl::Instance()->GetLoginReq(roArg.openid());
	if(poRequest == NULL)
	{
		roRes.set_result(KKSG::ERR_STATE_ERROR);
		return;
	}

	poRequest->m_dwLoginServerId = roArg.loginzoneid();
	poRequest->m_dwServerId = roArg.serverid();
	poRequest->m_dwRpcDelayed = DelayRpc();
	nErrCode = CLoginControl::Instance()->StartTokenVerify(poRequest);
	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		CUserData oUser((UINT32)nErrCode, NULL);
		CRpc::ReplyDelayRpc(poRequest->m_dwRpcDelayed, oUser);
		CLoginControl::Instance()->CancelLogin(m_sessionID);
		return;
	}
}

void RpcT2N_LoginRequestNew::OnDelayReplyRpc(const LoginRequestNewArg &roArg, LoginRequestNewRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
	roRes.set_function_open(0);
	roRes.mutable_data()->set_is_backflow_server(ControlConfig::Instance()->IsBackFlowServer());
	roRes.mutable_data()->set_backflow_level(ControlConfig::Instance()->GetBackFlowLevel());

	KKSG::LoadAccountData* poAccount = (KKSG::LoadAccountData*)roUserData.m_pUserPtr;
	if (nResult != KKSG::ERR_SUCCESS)
	{
		CLoginControl::Instance()->CancelLogin(m_sessionID);
		return;
	}
	if (poAccount == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		CLoginControl::Instance()->CancelLogin(m_sessionID);
		return;
	}

	roRes.mutable_accountdata()->CopyFrom(*poAccount);
	CLoginRequest* poRequest = CLoginControl::Instance()->GetLoginReq(roArg.openid());
	if(poRequest == NULL)
	{
		roRes.set_result(KKSG::ERR_STATE_ERROR);
		return;
	}

	// 玩家有可以重连的场景, 老的account还保留, 登录流程结束
	if (poRequest->m_sceneTemplateId)
	{
		CAccountSession* poSession = CAccountSessionMgr::Instance()->GetByAccountID(roArg.openid());
		if (poSession)
		{
			CReconnectSession* pRSession = new CReconnectSession;
			pRSession->account = roArg.openid();
			pRSession->loginArg = roArg;
			CAccountSessionMgr::Instance()->SetReconnectSession(m_sessionID, pRSession);

			poSession->SetClientConf(roArg.clientinfo());
			poSession->SetLoginToken(roArg.token());
			poSession->LoadAllRoleData(*poAccount);
			poSession->SetLoginServerId(roArg.loginzoneid());
			poSession->SetServerId(roArg.serverid());
			poSession->SetAccountBackFlow(poRequest->m_accountBackFlow);

			roRes.mutable_rinfo()->set_scenetemplateid(poRequest->m_sceneTemplateId);
			roRes.mutable_rinfo()->set_scenetime(poRequest->m_sceneTime);
			CLoginControl::Instance()->CancelLogin(m_sessionID);
			return;
		}
	}

	///> 正常登录， 没有可重连场景， 判断排队，loginaccount
	///> 登录成功，判断是否需要排队
	if (QueuingMgr::Instance()->IsWaiting(poAccount->account()))
	{
		roRes.set_result(KKSG::ERR_ACCOUNT_QUEUING);
	}
	else
	{
		if (QueuingMgr::Instance()->IsFull())
		{
			SSInfo<<poAccount->account()<<" queuing"<<END;

			QueuingMgr::Instance()->PushWait(poAccount->account());
			roRes.set_result(KKSG::ERR_ACCOUNT_QUEUING);
		}
	}

	CAccountSession* poSession = new CAccountSession;
	if(!poSession->Init(m_sessionID))
	{
		LogWarn("Account [%s] can't login because can not get shm sapce", roArg.openid().c_str());
		roRes.set_result(KKSG::ERR_REGISTER_NUM_LIMIT);
		delete poSession;
		return;
	}

	poSession->SetAccountID(roArg.openid());
	poSession->SetClientConf(roArg.clientinfo());
	poSession->SetLoginToken(roArg.token());
	poSession->LoadAllRoleData(roRes.accountdata());
	poSession->SetLoginServerId(roArg.loginzoneid());
	poSession->SetServerId(roArg.serverid());
	poSession->SetAccountBackFlow(poRequest->m_accountBackFlow);

	CAccountSessionMgr::Instance()->LoginAccount(poSession);
}
