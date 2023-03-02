#include "pch.h"
#include "login/rpcm2d_readaccountdata.h"
#include "idip/idipproc.h"

// generate by ProtoGen at date: 2016/7/9 14:20:59

RPC_CLIENT_IMPLEMETION(RpcM2D_ReadAccountData, ReadAccountDataArg, ReadAccountDataRes)

void RpcM2D_ReadAccountData::OnReply(const ReadAccountDataArg &roArg, const ReadAccountDataRes &roRes, const CUserData &roUserData)
{
	if (roArg.readtype() != KKSG::READ_ACCOUNT_DATA_IDIP)
	{
		LogError("Invalid read account data type [%d]", roArg.readtype());
		return;
	}

	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		oUser.m_pUserPtr   = NULL;
		LogError("Account '%s' load account data failed, error code %d", roArg.account().c_str(), roRes.result());
	}
	else
	{
		oUser.m_pUserPtr   = const_cast<ReadAccountDataRes&>(roRes).mutable_accountdata();
		LogDebug("Account '%s' load account data finished", roArg.account().c_str());
	}

	CIdipProcess::Instance()->OnAsyncReply(roArg.rpcid(), oUser);
}

void RpcM2D_ReadAccountData::OnTimeout(const ReadAccountDataArg &roArg, const CUserData &roUserData)
{
	if (roArg.readtype() != KKSG::READ_ACCOUNT_DATA_IDIP)
	{
		LogError("Invalid read account data type [%d]", roArg.readtype());
		return;
	}

	KKSG::ErrorCode nErrCode = KKSG::ERR_TIMEOUT;
	LogError("Account '%s' load account data timeout", roArg.account().c_str());

	CUserData oUser((UINT32)nErrCode, NULL);
	CIdipProcess::Instance()->OnAsyncReply(roArg.rpcid(), oUser);
}
