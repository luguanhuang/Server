#include "pch.h"
#include "common/rpct2m_registergt2ms.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/7/5 21:11:37

RPC_CLIENT_IMPLEMETION(RpcT2M_RegisterGt2Ms, RegisterGt2MsArg, RegisterGt2MsRes)

void RpcT2M_RegisterGt2Ms::OnReply(const RegisterGt2MsArg &roArg, const RegisterGt2MsRes &roRes, const CUserData &roUserData)
{
	if(roRes.errorcode() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Gate server line [%u] register to ms success", roArg.line());
		MSLink::Instance()->SetRegistered(true);
	}
	else
	{
		LogError("Gate server line [%u] register to ms failed", roArg.line());
	}
}

void RpcT2M_RegisterGt2Ms::OnTimeout(const RegisterGt2MsArg &roArg, const CUserData &roUserData)
{
	LogError("Gate server line [%u] register to ms timeout", roArg.line());
}
