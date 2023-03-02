#include "pch.h"
#include "common/rpct2g_registergt2gs.h"

// generate by ProtoGen at date: 2016/7/8 18:12:58

RPC_CLIENT_IMPLEMETION(RpcT2G_RegisterGt2Gs, RegisterGt2GsArg, RegisterGt2GsRes)

void RpcT2G_RegisterGt2Gs::OnReply(const RegisterGt2GsArg &roArg, const RegisterGt2GsRes &roRes, const CUserData &roUserData)
{
	if(roRes.errorcode() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Gate server line [%u] register to gs_%u success", roArg.line(), roRes.gsline());
	}
	else
	{
		LogError("Gate server line [%u] register to gs_%u failed", roArg.line(), roRes.gsline());
	}
}

void RpcT2G_RegisterGt2Gs::OnTimeout(const RegisterGt2GsArg &roArg, const CUserData &roUserData)
{
}
