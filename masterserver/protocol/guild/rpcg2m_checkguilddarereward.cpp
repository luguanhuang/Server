#include "pch.h"
#include "guild/rpcg2m_checkguilddarereward.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/9/11 18:34:28

RPC_SERVER_IMPLEMETION(RpcG2M_CheckGuildDareReward, CheckGuildDareRewardArg, CheckGuildDareRewardRes)

void RpcG2M_CheckGuildDareReward::OnCall(const CheckGuildDareRewardArg &roArg, CheckGuildDareRewardRes &roRes)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roArg.roleid());
	if (pGuild == NULL)
	{
		roRes.set_result(false);
		roRes.set_guildllvl(0);
		return ;
	}

	if (pGuild->GetDareTimes() < roArg.rewardtype())
	{
		roRes.set_result(false);
		roRes.set_guildllvl(0);
	}
	else
	{
		roRes.set_result(true);
		roRes.set_guildllvl(pGuild->GetLevel());
	}

}

void RpcG2M_CheckGuildDareReward::OnDelayReplyRpc(const CheckGuildDareRewardArg &roArg, CheckGuildDareRewardRes &roRes, const CUserData &roUserData)
{
}
