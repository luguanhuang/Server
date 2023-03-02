#include "pch.h"
#include "common/rpcd2r_registerdb2router.h"

// generate by ProtoGen at date: 2016/12/3 17:01:12

RPC_CLIENT_IMPLEMETION(RpcD2R_RegisterDb2Router, RegisterDb2RouterArg, RegisterDb2RouterRes)

void RpcD2R_RegisterDb2Router::OnReply(const RegisterDb2RouterArg &roArg, const RegisterDb2RouterRes &roRes, const CUserData &roUserData)
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

void RpcD2R_RegisterDb2Router::OnTimeout(const RegisterDb2RouterArg &roArg, const CUserData &roUserData)
{
	LogError("Register To Router Timeout");
}
