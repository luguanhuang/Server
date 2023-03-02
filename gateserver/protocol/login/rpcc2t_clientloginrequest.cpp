#include "pch.h"
#include "login/rpcc2t_clientloginrequest.h"
#include "network/gslink.h"
#include "network/mslink.h"
#include "network/controllink.h"
#include "clientlink.h"
#include "session/sessionmanager.h"
#include "login/rpct2n_loginrequestnew.h"
#include "config.h"

// generate by ProtoGen at date: 2015/10/7 15:03:26

RPC_SERVER_IMPLEMETION(RpcC2T_ClientLoginRequest, LoginArg, LoginRes)

void RpcC2T_ClientLoginRequest::OnCall(const LoginArg &roArg, LoginRes &roRes)
{
	roRes.set_result(KKSG::ERR_FAILED);

	UINT32 serverID = roArg.gameserverid();

	if(!MSLink::Instance()->IsRegistered())
	{
		roRes.set_result(ERR_MS_UNREADY);
		SessionManager::Instance()->CloseSession(m_sessionID, 0);
		LogWarn("Master server link is not registered");
		return;
	}

	SessionInfo *pSession = SessionManager::Instance()->FindSession(m_sessionID);
	if (pSession == NULL || pSession->m_state != SESSION_CHALLENGE)
	{
		roRes.set_result(ERR_STATE_ERROR);
		SessionManager::Instance()->CloseSession(m_sessionID, 0);
		LogWarn("client not in challenge state, state=[%d]", SessionManager::Instance()->GetSessionState(m_sessionID));
		return;
	}
	
	//version check on control
	//std::string version = GateConfig::Instance()->GetVersion().CheckVersion(roArg.ios(), roArg.android(), roArg.pc());
	//if (!version.empty())
	//{
	//	SessionManager::Instance()->CloseSession(m_sessionID, 0);
	//	roRes.set_result(ERR_VERSION_FAILED);
	//	roRes.set_version(version);
	//	LogWarn("Version check failed");
	//	return;
	//}

	///> 更新sessioninfo里面的gameserverid
	pSession->m_gameserverId = serverID;
	pSession->m_state = SESSION_DBVERIFY;
	pSession->m_dwRpcID = DelayRpc();

	RpcT2N_LoginRequestNew* rpc = RpcT2N_LoginRequestNew::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_openid(roArg.openid());
	rpc->m_oArg.set_token(roArg.token());
	rpc->m_oArg.set_loginzoneid(roArg.loginzoneid());
	rpc->m_oArg.set_serverid(roArg.gameserverid());
	*rpc->m_oArg.mutable_clientinfo() = roArg.clientinfo();
	rpc->m_oArg.mutable_clientinfo()->set_ip(pSession->m_strIP);
	rpc->m_oArg.set_ios(roArg.ios());
	rpc->m_oArg.set_android(roArg.android());
	rpc->m_oArg.set_pc(roArg.pc());
	//if (!rpc->m_oArg.mutable_clientinfo()->has_clientversion())
	//{
	//	std::string v = GateConfig::Instance()->GetVersion().GetClientVersion(roArg.ios(), roArg.android(), roArg.pc());
	//	if (!v.empty())
	//	{
	//		rpc->m_oArg.mutable_clientinfo()->set_clientversion(v);
	//	}
	//}
	ControlLink::Instance()->SendTo(*rpc);
	LogInfo("recv login request, account=[%s] session=[%llu] server[%u], send login request to ns", roArg.openid().c_str(), m_sessionID, serverID);
}

void RpcC2T_ClientLoginRequest::OnDelayReplyRpc(const LoginArg &roArg, LoginRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nErrCode = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nErrCode);
	if(nErrCode == KKSG::ERR_SUCCESS || nErrCode == KKSG::ERR_ACCOUNT_QUEUING)
	{
		KKSG::LoginRequestNewRes* ret = (KKSG::LoginRequestNewRes*)roUserData.m_pUserPtr;
		if(ret != NULL)
		{
			roRes.set_function_open(ret->function_open());
			roRes.mutable_accountdata()->CopyFrom(ret->accountdata());
			roRes.mutable_data()->CopyFrom(ret->data());
			if (ret->has_rinfo())
			{
				roRes.mutable_rinfo()->CopyFrom(ret->rinfo());
			}		
		}
	}
	else
	{
		if (roUserData.m_pUserPtr)
		{
			KKSG::LoginRequestNewRes* ret = (KKSG::LoginRequestNewRes*)roUserData.m_pUserPtr;
			roRes.set_version(ret->version());
		}
		SessionManager::Instance()->CloseSession(m_sessionID, 0);
	}
}
