#include "pch.h"
#include "garden/rpcm2g_gardenplayevent.h"

// generate by ProtoGen at date: 2016/10/14 15:11:36

RPC_CLIENT_IMPLEMETION(RpcM2G_GardenPlayEvent, GardenPlayEventArg, GardenPlayEventRes)

void RpcM2G_GardenPlayEvent::OnReply(const GardenPlayEventArg &roArg, const GardenPlayEventRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_GardenPlayEvent::OnTimeout(const GardenPlayEventArg &roArg, const CUserData &roUserData)
{
}
