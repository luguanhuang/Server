#include "pch.h"
#include "role/rpcm2g_reqguilddarerewardrec.h"

// generate by ProtoGen at date: 2016/9/11 18:11:39

RPC_CLIENT_IMPLEMETION(RpcM2G_ReqGuildDareRewardRec, ReqGuildDareRewardRecArg, ReqGuildDareRewardRecRes)

void RpcM2G_ReqGuildDareRewardRec::OnReply(const ReqGuildDareRewardRecArg &roArg, const ReqGuildDareRewardRecRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	std::set<UINT32> rewardSet;
	for (UINT32 cnt = 0; cnt < roRes.rewardrec_size(); ++cnt)
	{
		rewardSet.insert(roRes.rewardrec(cnt));
	}
	oUser.m_pUserPtr = (void*)(&rewardSet);
	//oUser.m_dwUserData = rewardSet.size();
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_ReqGuildDareRewardRec::OnTimeout(const ReqGuildDareRewardRecArg &roArg, const CUserData &roUserData)
{
}
