#include "pch.h"
#include "login/rpcn2d_nsreadaccountdata.h"
#include "account/logincontrol.h"

// generate by ProtoGen at date: 2016/11/3 1:33:34

RPC_CLIENT_IMPLEMETION(RpcN2D_NsReadAccountData, ReadAccountDataArg, ReadAccountDataRes)

void RpcN2D_NsReadAccountData::OnReply(const ReadAccountDataArg &roArg, const ReadAccountDataRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		oUser.m_pUserPtr   = NULL;
		LogError("Account '%s' serverid[%u] load account failed, error code %d", roArg.account().c_str(), roArg.serverid(), roRes.result());

		if(roArg.readtype() == KKSG::READ_ACCOUNT_DATA_LOGIN)
		{
			CLoginControl::Instance()->CancelLogin(m_sessionID);
		}
	}
	else
	{
		oUser.m_pUserPtr   = const_cast<ReadAccountDataRes&>(roRes).mutable_accountdata();
		LogDebug("Account '%s' serverid[%u] load account finished", roArg.account().c_str(), roArg.serverid());
	}

	if (roArg.readtype() == KKSG::READ_ACCOUNT_DATA_IDIP)
	{
		//CIdipProcess::Instance()->OnAsyncReply(roArg.rpcid(), oUser);
	}
	else
	{
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
	}
}

void RpcN2D_NsReadAccountData::OnTimeout(const ReadAccountDataArg &roArg, const CUserData &roUserData)
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_TIMEOUT;
	LogError("Account '%s' load account timeout", roArg.account().c_str());

	if(roArg.readtype() == KKSG::READ_ACCOUNT_DATA_LOGIN)
	{
		CLoginControl::Instance()->CancelLogin(m_sessionID);
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	if (roArg.readtype() == KKSG::READ_ACCOUNT_DATA_IDIP)
	{
		//CIdipProcess::Instance()->OnAsyncReply(roArg.rpcid(), oUser);
	}
	else
	{
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
	}
}
