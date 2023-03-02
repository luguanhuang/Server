#include "pch.h"
#include "lottery/rpcc2g_querylotterycd.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/lotteryrecord.h"

// generate by ProtoGen at date: 2015/5/13 11:41:30

RPC_SERVER_IMPLEMETION(RpcC2G_QueryLotteryCD, QueryLotteryCDArg, QueryLotteryCDRes)

void RpcC2G_QueryLotteryCD::OnCall(const QueryLotteryCDArg &roArg, QueryLotteryCDRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	LotteryRecord *pLR = pRole->Get<LotteryRecord>();
	roRes.set_cooldown(pLR->GetDragonCoinFreeBuyCoolDown());
	roRes.set_goldbuycount(pLR->GetGoldFreeBuyCount());
	roRes.set_goldbuycooldown(pLR->GetGoldFreeBuyCoolDown());
	roRes.set_goldbaodi(pLR->GetGoldMinimumRewardCount()+1);
	roRes.set_coinbaodi(pLR->GetDragonCoinMinimumRewardCount()+1);
}

void RpcC2G_QueryLotteryCD::OnDelayReplyRpc(const QueryLotteryCDArg &roArg, QueryLotteryCDRes &roRes, const CUserData &roUserData)
{
}
