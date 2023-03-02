#include "pch.h"
#include "reward/rpcc2g_getonlinereward.h"
#include "unit/rolemanager.h"
#include "gamelogic/rewardrecord.h"
#include "gamelogic/onlinereward.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2015/10/20 18:38:39

RPC_SERVER_IMPLEMETION(RpcC2G_GetOnlineReward, GetOnlineRewardArg, GetOnlineRewardRes)

void RpcC2G_GetOnlineReward::OnCall(const GetOnlineRewardArg &roArg, GetOnlineRewardRes &roRes)
{
	roRes.set_errorcode(ERR_UNKNOWN);
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	UINT32 index = OnlineRewardMgr::Instance()->GetTime(roArg.index());
	if ((UINT32)(-1) == index)
	{
		roRes.set_errorcode(ERR_REWARD_NOTEXIST);
		return;
	}

	CRewardRecord* record = role->Get<CRewardRecord>();
	if (record->IsTakenOnline(index))
	{
		roRes.set_errorcode(ERR_REWARD_TAKEN);
		return;
	}
	role->UpdateOnlineTime();
	if (role->GetOnlineTime() < index)
	{
		roRes.set_errorcode(ERR_REWARD_LIMIT);
		return;
	}

	std::vector<ItemDesc> items = OnlineRewardMgr::Instance()->GetOnlineReward(index);
	if (items.empty())
	{
		roRes.set_errorcode(ERR_REWARD_NOTEXIST);
		return;
	}

	record->SetOnline(index);

	BagGiveItemTransition transition(role);
	transition.SetReason(ItemFlow_Reward, ItemFlow_Reward_Online);
	for (auto i = items.begin(); i != items.end(); ++i)
	{
		transition.GiveItem(*i);
	}
	transition.NotifyClient();

	if (roArg.index() == OnlineRewardMgr::Instance()->GetMaxIndex())
	{
		role->Get<CRoleSystem>()->SetSystemOpened(ONLINE_REWARDID, false);
	}

	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2G_GetOnlineReward::OnDelayReplyRpc(const GetOnlineRewardArg &roArg, GetOnlineRewardRes &roRes, const CUserData &roUserData)
{
}
