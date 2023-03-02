#include "pch.h"
#include "social/rpcm2g_getflowerrewardstate.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/flowerrecord.h"

// generate by ProtoGen at date: 2016/8/19 10:11:35

RPC_SERVER_IMPLEMETION(RpcM2G_GetFlowerRewardState, GetFlowerRewardStateArg, GetFlowerRewardStateRes)

void RpcM2G_GetFlowerRewardState::OnCall(const GetFlowerRewardStateArg &roArg, GetFlowerRewardStateRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!role)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	bool state = role->Get<CFlowerRecord>()->CheckGetRankReward();
	roRes.set_getstate(state);
	roRes.set_error(KKSG::ERR_SUCCESS);
}

void RpcM2G_GetFlowerRewardState::OnDelayReplyRpc(const GetFlowerRewardStateArg &roArg, GetFlowerRewardStateRes &roRes, const CUserData &roUserData)
{
}
