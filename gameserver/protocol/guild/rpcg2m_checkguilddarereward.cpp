#include "pch.h"
#include "guild/rpcg2m_checkguilddarereward.h"

// generate by ProtoGen at date: 2016/9/11 18:34:28

RPC_CLIENT_IMPLEMETION(RpcG2M_CheckGuildDareReward, CheckGuildDareRewardArg, CheckGuildDareRewardRes)

void RpcG2M_CheckGuildDareReward::OnReply(const CheckGuildDareRewardArg &roArg, const CheckGuildDareRewardRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	UINT32 guildLvl = roRes.guildllvl(); 
	oUser.m_pUserPtr = (void*)&guildLvl;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcG2M_CheckGuildDareReward::OnTimeout(const CheckGuildDareRewardArg &roArg, const CUserData &roUserData)
{
}
