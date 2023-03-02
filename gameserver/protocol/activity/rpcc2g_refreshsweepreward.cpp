#include "pch.h"
#include "activity/rpcc2g_refreshsweepreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/towerrecord.h"

// generate by ProtoGen at date: 2016/9/12 11:51:44

RPC_SERVER_IMPLEMETION(RpcC2G_RefreshSweepReward, RefreshSweepRewardArg, RefreshSweepRewardRes)

void RpcC2G_RefreshSweepReward::OnCall(const RefreshSweepRewardArg &roArg, RefreshSweepRewardRes &roRes)
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

	int result = 0;
	UINT32 error = record->RefreshSweepReward(1, result);
	roRes.set_error((ErrorCode)error);
	roRes.set_refreshresult(result);
}

void RpcC2G_RefreshSweepReward::OnDelayReplyRpc(const RefreshSweepRewardArg &roArg, RefreshSweepRewardRes &roRes, const CUserData &roUserData)
{
}
