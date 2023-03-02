#include "pch.h"
#include "common/rpcn2r_registerctrl2router.h"

// generate by ProtoGen at date: 2016/11/5 13:58:49

RPC_CLIENT_IMPLEMETION(RpcN2R_RegisterCtrl2Router, RegisterCtrl2RouterArg, RegisterCtrl2RouterRes)

void RpcN2R_RegisterCtrl2Router::OnReply(const RegisterCtrl2RouterArg &roArg, const RegisterCtrl2RouterRes &roRes, const CUserData &roUserData)
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

void RpcN2R_RegisterCtrl2Router::OnTimeout(const RegisterCtrl2RouterArg &roArg, const CUserData &roUserData)
{
	LogError("Register To Router Timeout");
}
