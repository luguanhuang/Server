#include "pch.h"
#include "social/rpcm2g_getflowerrewardstate.h"

// generate by ProtoGen at date: 2016/8/19 10:11:35

RPC_CLIENT_IMPLEMETION(RpcM2G_GetFlowerRewardState, GetFlowerRewardStateArg, GetFlowerRewardStateRes)

void RpcM2G_GetFlowerRewardState::OnReply(const GetFlowerRewardStateArg &roArg, const GetFlowerRewardStateRes &roRes, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = roRes.error();
	oUserData.m_pUserPtr = (void*)&roRes;

	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}

void RpcM2G_GetFlowerRewardState::OnTimeout(const GetFlowerRewardStateArg &roArg, const CUserData &roUserData)
{
}
