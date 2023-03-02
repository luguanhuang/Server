#include "pch.h"
#include "dragonguild/rpcm2g_getrolepower.h"

// generate by ProtoGen at date: 2017/9/5 14:39:27

RPC_CLIENT_IMPLEMETION(RpcM2G_GetRolePower, GetRolePowerArg, GetRolePowerRes)

void RpcM2G_GetRolePower::OnReply(const GetRolePowerArg &roArg, const GetRolePowerRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = roRes.rolepower();
	UINT32 result = roRes.result();
	oUser.m_pUserPtr = (void *)&result;
	if (result != KKSG::ERR_SUCCESS)
	{
		oUser.m_dwUserData = roRes.cdtime();
	}
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_GetRolePower::OnTimeout(const GetRolePowerArg &roArg, const CUserData &roUserData)
{
}
