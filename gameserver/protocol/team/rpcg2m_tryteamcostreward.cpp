#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/teamrecord.h"
#include "team/rpcg2m_tryteamcostreward.h"

// generate by ProtoGen at date: 2016/11/27 18:37:53

RPC_CLIENT_IMPLEMETION(RpcG2M_TryTeamCostReward, TryTeamCostReward, TryTeamCostRewardRes)

void RpcG2M_TryTeamCostReward::OnReply(const TryTeamCostReward &roArg, const TryTeamCostRewardRes &roRes, const CUserData &roUserData)
{
	Role* role = RoleManager::Instance()->FindByRoleID(m_oArg.roleid());
	if (NULL == role)
	{
		SSWarn<<"role is not exist, roleid:"<<m_oArg.roleid()<<END;
		return;
	}
	if (roRes.ishit())
	{
		role->Get<CTeamRecord>()->SetTeamCostGift((UINT32)(-1));
	}
	else
	{
		role->Get<CTeamRecord>()->SetTeamCostGift(roArg.value());
	}
}

void RpcG2M_TryTeamCostReward::OnTimeout(const TryTeamCostReward &roArg, const CUserData &roUserData)
{
}
