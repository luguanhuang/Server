#include "pch.h"
#include "guild/rpcg2m_reqguildladderreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/30 10:08:35

RPC_CLIENT_IMPLEMETION(RpcG2M_ReqGuildLadderReward, ReqGuildLadderRewardArg, ReqGuildLadderRewardRes)

void RpcG2M_ReqGuildLadderReward::OnReply(const ReqGuildLadderRewardArg &roArg, const ReqGuildLadderRewardRes &roRes, const CUserData &roUserData)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		return ;
	}

	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Stage, ItemFlow_Stage_Pk);

	for (UINT32 i = 0; i < roArg.item_size(); ++i)
	{
		transition.GiveItem(roArg.item(i).itemid(), roArg.item(i).itemcount());
	}

	for (UINT32 cnt = 0; cnt < roRes.items_size(); ++cnt)
	{
		transition.GiveItem(roRes.items(cnt).itemid(), roRes.items(cnt).itemcount());
	}

	transition.NotifyClient();
}

void RpcG2M_ReqGuildLadderReward::OnTimeout(const ReqGuildLadderRewardArg &roArg, const CUserData &roUserData)
{
}
