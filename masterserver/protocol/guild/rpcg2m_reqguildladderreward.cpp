#include "pch.h"
#include "guild/rpcg2m_reqguildladderreward.h"
#include "table/globalconfig.h"
#include "guild/guildmgr.h"
#include "guild/guildladder.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/30 10:08:35

RPC_SERVER_IMPLEMETION(RpcG2M_ReqGuildLadderReward, ReqGuildLadderRewardArg, ReqGuildLadderRewardRes)

void RpcG2M_ReqGuildLadderReward::OnCall(const ReqGuildLadderRewardArg &roArg, ReqGuildLadderRewardRes &roRes)
{
	if (CGuildLadder::Instance()->TryReward(roArg.roleid()))
	{
		foreach (i in GetGlobalConfig().GuildLadderReward)
		{
			KKSG::ItemBrief* pData = roRes.add_items();
			pData->set_itemid(i->seq[0]);
			pData->set_itemcount(i->seq[1]);
		}
	}
}

void RpcG2M_ReqGuildLadderReward::OnDelayReplyRpc(const ReqGuildLadderRewardArg &roArg, ReqGuildLadderRewardRes &roRes, const CUserData &roUserData)
{
}
