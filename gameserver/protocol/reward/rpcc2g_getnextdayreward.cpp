#include "pch.h"
#include "reward/rpcc2g_getnextdayreward.h"
#include "unit/rolemanager.h"
#include "gamelogic/rewardrecord.h"
#include "gamelogic/onlinereward.h"
#include "gamelogic/bagtransition.h"
// generate by ProtoGen at date: 2015/10/20 18:39:10

RPC_SERVER_IMPLEMETION(RpcC2G_GetNextDayReward, GetNextDayRewardArg, GetNextDayRewardRes)

void RpcC2G_GetNextDayReward::OnCall(const GetNextDayRewardArg &roArg, GetNextDayRewardRes &roRes)
{
	roRes.set_errorcode(ERR_UNKNOWN);
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
}

void RpcC2G_GetNextDayReward::OnDelayReplyRpc(const GetNextDayRewardArg &roArg, GetNextDayRewardRes &roRes, const CUserData &roUserData)
{
}
