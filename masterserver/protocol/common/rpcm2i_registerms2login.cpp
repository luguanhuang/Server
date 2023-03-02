#include "pch.h"
#include "common/rpcm2i_registerms2login.h"
#include "network/loginlink.h"

// generate by ProtoGen at date: 2016/8/5 1:17:21

RPC_CLIENT_IMPLEMETION(RpcM2I_RegisterMs2Login, RegisterMs2LoginArg, RegisterMs2LoginRes)

void RpcM2I_RegisterMs2Login::OnReply(const RegisterMs2LoginArg &roArg, const RegisterMs2LoginRes &roRes, const CUserData &roUserData)
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

void RpcM2I_RegisterMs2Login::OnTimeout(const RegisterMs2LoginArg &roArg, const CUserData &roUserData)
{
}
