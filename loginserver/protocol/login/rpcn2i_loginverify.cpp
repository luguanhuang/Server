#include "pch.h"
#include "login/rpcn2i_loginverify.h"
#include "verify/tokenverifymgr.h"
#include "verify/dbgetbespeaktask.h"

// generate by ProtoGen at date: 2016/11/2 20:14:40

RPC_SERVER_IMPLEMETION(RpcN2I_LoginVerify, LoginVerifyArg, LoginVerifyRes)

void RpcN2I_LoginVerify::OnCall(const LoginVerifyArg &roArg, LoginVerifyRes &roRes)
{
	roRes.set_userid("");
	roRes.set_result(KKSG::ERR_LOGIN_VERIFY_FAILED);

	UToken token;
	if (!UToken::FromString(token, roArg.logintoken().data(), roArg.logintoken().size()))
	{
		LogWarn("remove logintoken error, size %d", roArg.logintoken().size());
		return;
	}

	char buf[64];
	token.Format(buf, 64);

	UINT32 serverID = LoginConfig::Instance()->GetServerID();
	if (token.data[4] != serverID)
	{
		LogWarn("send logintoken to wrong loginserver: token[%s] serverID: %d, here serverID: %d", 
			buf, token.data[4], serverID);
		return;
	}

	UserInfo *pUser = TokenVerifyMgr::Instance()->FindUserInfo(token);
	if (pUser == NULL)
	{
		LogWarn("not found token[%s]", buf);
		return;
	}

	if (pUser->isused)
	{
		LogWarn("token[%s] already used", buf);
		return;
	}
	roRes.set_userid(pUser->account);
	roRes.set_isgm(pUser->isgm);
	roRes.set_account_backflow(pUser->accountBackFlow);

	if (roArg.isbackflowserver() && LoginConfig::Instance()->NeedBackFlowCheck())
	{
		DBGetBespeakTask* pTask = new DBGetBespeakTask(DelayRpc(), pUser->account);
		TokenVerifyMgr::Instance()->AddDBTask(pTask);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	LogInfo("verify token[%s], account[%s], userid[%lld] account_back[%u]", buf, pUser->account.c_str(), roArg.uid(), pUser->accountBackFlow);
}

void RpcN2I_LoginVerify::OnDelayReplyRpc(const LoginVerifyArg &roArg, LoginVerifyRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_pUserPtr == NULL)
	{
		roRes.set_result(KKSG::ERR_NOT_BESPEAK);
		return;
	}

	AccountBeSpeakInfo* pInfo = (AccountBeSpeakInfo*)roUserData.m_pUserPtr;
	if (!pInfo->HasBeSpeak(roArg.serverid()) && !pInfo->HasRole(roArg.serverid()))
	{
		LogWarn("Not Bespeak, account[%s] serverid[%u]", pInfo->account.c_str(), roArg.serverid());
		roRes.set_result(KKSG::ERR_NOT_BESPEAK);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	LogInfo("verify account[%s], userid[%lld], account_back[%u]", pInfo->account.c_str(), roArg.uid(), roRes.account_backflow());
}
