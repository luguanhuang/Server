#include "pch.h"
#include "common/rpcg2t_registergs2gt.h"

// generate by ProtoGen at date: 2016/7/8 18:08:47

RPC_CLIENT_IMPLEMETION(RpcG2T_RegisterGs2Gt, RegisterGs2GtArg, RegisterGs2GtRes)

void RpcG2T_RegisterGs2Gt::OnReply(const RegisterGs2GtArg &roArg, const RegisterGs2GtRes &roRes, const CUserData &roUserData)
{
	if(roRes.errorcode() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Game server line [%u] register to gt_%u success", roArg.line(), roRes.gtline());
	}
	else
	{
		LogError("Game server line [%u] register to gt_%u failed", roArg.line(), roRes.gtline());
	}
}

void RpcG2T_RegisterGs2Gt::OnTimeout(const RegisterGs2GtArg &roArg, const CUserData &roUserData)
{
}
