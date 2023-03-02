#include "pch.h"
#include "activity/rpcc2g_getsweeptowerreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/towerrecord.h"

// generate by ProtoGen at date: 2016/9/12 11:53:57

RPC_SERVER_IMPLEMETION(RpcC2G_GetSweepTowerReward, GetSweepTowerRewardArg, GetSweepTowerRewardRes)

void RpcC2G_GetSweepTowerReward::OnCall(const GetSweepTowerRewardArg &roArg, GetSweepTowerRewardRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_error(KKSG::ERR_SUCCESS);
	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_DAILY_PLAY))
	{
		return ;
	}

	CTowerRecord* record = pRole->Get<CTowerRecord>();
	if (!record)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	record->GiveSweepReward(1);
	roRes.set_error(KKSG::ERR_SUCCESS);
}

void RpcC2G_GetSweepTowerReward::OnDelayReplyRpc(const GetSweepTowerRewardArg &roArg, GetSweepTowerRewardRes &roRes, const CUserData &roUserData)
{
}
