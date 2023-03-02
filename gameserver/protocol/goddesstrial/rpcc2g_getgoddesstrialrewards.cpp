#include "pch.h"
#include "goddesstrial/rpcc2g_getgoddesstrialrewards.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/bagtransition.h"
#include "table/globalconfig.h"
#include "gamelogic/teamrecord.h"

// generate by ProtoGen at date: 2016/7/18 20:04:49

RPC_SERVER_IMPLEMETION(RpcC2G_GetGoddessTrialRewards, GetGoddessTrialRewardsArg, GetGoddessTrialRewardsRes)

void RpcC2G_GetGoddessTrialRewards::OnCall(const GetGoddessTrialRewardsArg &roArg, GetGoddessTrialRewardsRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	CTeamRecord* trinfo = pRole->Get<CTeamRecord>();
	int leftReward = trinfo->GetLeftGoddessRewardToday();
	if(leftReward <= 0)
	{
		roRes.set_errorcode(ERR_REWARD_NOTEXIST);
		roRes.set_leftgoddessreward(0);
		return;
	}
	trinfo->AddGoddessRewardToday();
	roRes.set_leftgoddessreward(trinfo->GetLeftGoddessRewardToday());

	BagGiveItemTransition stGiveTransition(pRole);
	stGiveTransition.SetReason(ItemFlow_Goddess, ItemFlow_Goddess_Reward);
	for(auto i = GetGlobalConfig().GoddessTrialRewards.begin(); i != GetGlobalConfig().GoddessTrialRewards.end(); i++)
	{
		stGiveTransition.GiveItem((*i)[0],  (*i)[1]);
	}
	stGiveTransition.NotifyClient();

	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2G_GetGoddessTrialRewards::OnDelayReplyRpc(const GetGoddessTrialRewardsArg &roArg, GetGoddessTrialRewardsRes &roRes, const CUserData &roUserData)
{
}
