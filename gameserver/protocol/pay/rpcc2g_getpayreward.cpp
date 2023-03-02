#include "pch.h"
#include "pay/rpcc2g_getpayreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/payv2Record.h"
//#include "../share/util/gametime.h" 

// generate by ProtoGen at date: 2017/2/7 19:31:27

RPC_SERVER_IMPLEMETION(RpcC2G_GetPayReward, GetPayRewardArg, GetPayRewardRes)

void RpcC2G_GetPayReward::OnCall(const GetPayRewardArg &roArg, GetPayRewardRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	if (pRecord == NULL)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	if (pRecord->RewardCoolTime() != 0)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	BagGiveItemTransition bagGive(pRole);
	bagGive.SetReason(ItemFlow_Pay, ItemFlow_PayReward);
	bagGive.GiveItem(1, GetGlobalConfig().PayReward);
	bagGive.NotifyClient();

	pRecord->SetRewardTime();
	roRes.set_errorcode(ERR_SUCCESS);
	roRes.set_cdtime(pRecord->RewardCoolTime());
}

void RpcC2G_GetPayReward::OnDelayReplyRpc(const GetPayRewardArg &roArg, GetPayRewardRes &roRes, const CUserData &roUserData)
{
}
