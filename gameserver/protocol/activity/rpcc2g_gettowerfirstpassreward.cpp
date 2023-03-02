#include "pch.h"
#include "activity/rpcc2g_gettowerfirstpassreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/towerrecord.h"

// generate by ProtoGen at date: 2016/9/19 11:53:29

RPC_SERVER_IMPLEMETION(RpcC2G_GetTowerFirstPassReward, GetTowerFirstPassRewardArg, GetTowerFirstPassRewardRes)

void RpcC2G_GetTowerFirstPassReward::OnCall(const GetTowerFirstPassRewardArg &roArg, GetTowerFirstPassRewardRes &roRes)
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

	UINT32 error = record->GetFirstPassReward(DEFAULT_HARDLEVEL, roArg.floor());
	roRes.set_error((ErrorCode)error);
}

void RpcC2G_GetTowerFirstPassReward::OnDelayReplyRpc(const GetTowerFirstPassRewardArg &roArg, GetTowerFirstPassRewardRes &roRes, const CUserData &roUserData)
{
}
