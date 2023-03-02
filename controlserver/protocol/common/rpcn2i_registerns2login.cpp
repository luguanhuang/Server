#include "pch.h"
#include "common/rpcn2i_registerns2login.h"
#include "network/loginlink.h"

// generate by ProtoGen at date: 2016/11/3 11:34:29

RPC_CLIENT_IMPLEMETION(RpcN2I_RegisterNs2Login, RegisterMs2LoginArg, RegisterMs2LoginRes)

void RpcN2I_RegisterNs2Login::OnReply(const RegisterMs2LoginArg &roArg, const RegisterMs2LoginRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		if(LoginLink::Instance()->AddLoginServer(GetConnID(), roRes.loginsvrid()) == ERR_SUCCESS)
		{
			LogInfo("Register to login server [%u] success", roRes.loginsvrid());
		}
	}
	else
	{
		LogError("Register to login server [%u] failed, error code [%d]", roRes.loginsvrid(), roRes.result());
	}
}

void RpcN2I_RegisterNs2Login::OnTimeout(const RegisterMs2LoginArg &roArg, const CUserData &roUserData)
{
}
