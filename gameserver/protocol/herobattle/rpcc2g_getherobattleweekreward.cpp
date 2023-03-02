#include "pch.h"
#include "herobattle/rpcc2g_getherobattleweekreward.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/herobattlerecord.h"

// generate by ProtoGen at date: 2016/12/28 22:16:10

RPC_SERVER_IMPLEMETION(RpcC2G_GetHeroBattleWeekReward, GetHeroBattleWeekRewardArg, GetHeroBattleWeekRewardRes)

void RpcC2G_GetHeroBattleWeekReward::OnCall(const GetHeroBattleWeekRewardArg &roArg, GetHeroBattleWeekRewardRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_errorcode(pRole->Get<CHeroBattleRecord>()->GetWeekNextReward());
	roRes.set_weekprize(pRole->Get<CHeroBattleRecord>()->GetWeekPrize());
	roRes.set_getnextweekprize(pRole->Get<CHeroBattleRecord>()->CanGetNextWeekPrize());
}

void RpcC2G_GetHeroBattleWeekReward::OnDelayReplyRpc(const GetHeroBattleWeekRewardArg &roArg, GetHeroBattleWeekRewardRes &roRes, const CUserData &roUserData)
{
}
