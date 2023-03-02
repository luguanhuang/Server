#include "pch.h"
#include "common/rpct2n_registergt2ns.h"
#include "network/controllink.h"

// generate by ProtoGen at date: 2016/11/2 0:58:27

RPC_CLIENT_IMPLEMETION(RpcT2N_RegisterGt2Ns, RegisterGt2NsArg, RegisterGt2NsRes)

void RpcT2N_RegisterGt2Ns::OnReply(const RegisterGt2NsArg &roArg, const RegisterGt2NsRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Gate server line [%u] register to ns success", roArg.line());
		ControlLink::Instance()->SetRegistered(true);
	}
	else
	{
		LogError("Gate server line [%u] register to ns failed", roArg.line());
	}
}

void RpcT2N_RegisterGt2Ns::OnTimeout(const RegisterGt2NsArg &roArg, const CUserData &roUserData)
{
	LogError("Gate server line [%u] register to ns timeout", roArg.line());
}
