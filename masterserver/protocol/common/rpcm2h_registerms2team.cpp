#include "pch.h"
#include "common/rpcm2h_registerms2team.h"

// generate by ProtoGen at date: 2017/4/24 15:05:17

RPC_CLIENT_IMPLEMETION(RpcM2H_RegisterMs2Team, RegisterMs2HArg, RegisterMs2HRes)

void RpcM2H_RegisterMs2Team::OnReply(const RegisterMs2HArg &roArg, const RegisterMs2HRes &roRes, const CUserData &roUserData)
{
	if(ERR_SUCCESS == roRes.ret())
	{
		SSInfo << " register to team server succeed " << END;
	}
	else
	{
		SSError << " register to team server failed ret = " << roRes.ret() << END;
	}
}

void RpcM2H_RegisterMs2Team::OnTimeout(const RegisterMs2HArg &roArg, const CUserData &roUserData)
{
}
