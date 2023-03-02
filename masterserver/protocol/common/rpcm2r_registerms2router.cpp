#include "pch.h"
#include "common/rpcm2r_registerms2router.h"

// generate by ProtoGen at date: 2016/10/27 9:25:05

RPC_CLIENT_IMPLEMETION(RpcM2R_RegisterMs2Router, RegisterMs2RouterArg, RegisterMs2RouterRes)

void RpcM2R_RegisterMs2Router::OnReply(const RegisterMs2RouterArg &roArg, const RegisterMs2RouterRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Register To Router Success");
	}
	else
	{
		LogError("Register To Router Failed!");
	}
}

void RpcM2R_RegisterMs2Router::OnTimeout(const RegisterMs2RouterArg &roArg, const CUserData &roUserData)
{
	LogError("Register To Router Timeout");
}
