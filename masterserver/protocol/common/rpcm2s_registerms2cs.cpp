#include "pch.h"
#include "common/rpcm2s_registerms2cs.h"
#include "network/cslink.h"

// generate by ProtoGen at date: 2017/1/19 22:19:06

RPC_CLIENT_IMPLEMETION(RpcM2S_RegisterMS2CS, RegisterMSToCSData, RegisterMSToCSDataRes)

void RpcM2S_RegisterMS2CS::OnReply(const RegisterMSToCSData &roArg, const RegisterMSToCSDataRes &roRes, const CUserData &roUserData)
{
	if (roRes.error() == KKSG::ERR_SUCCESS)
	{
		CSLink::Instance()->AddCenterServer(m_dwConnID, roRes.serverid());
	}
	else
	{
		LogError("register error, centerserverid:%u, error %d", roRes.serverid(), roRes.error());
	}
}

void RpcM2S_RegisterMS2CS::OnTimeout(const RegisterMSToCSData &roArg, const CUserData &roUserData)
{
	LogError("register timeout, connid:%u", m_dwConnID);
	CSLink::Instance()->Close(m_dwConnID);
}
