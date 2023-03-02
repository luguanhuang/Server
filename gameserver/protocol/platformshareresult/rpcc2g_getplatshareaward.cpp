#include "pch.h"
#include "platformshareresult/rpcc2g_getplatshareaward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/platformshare_record.h"

// generate by ProtoGen at date: 2017/5/31 21:54:00

RPC_SERVER_IMPLEMETION(RpcC2G_GetPlatShareAward, GetPlatShareAwardArg, GetPlatShareAwardRes)

void RpcC2G_GetPlatShareAward::OnCall(const GetPlatShareAwardArg &roArg, GetPlatShareAwardRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (role == NULL)
	{
		LogWarn("no role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	role->Get<PlatformShareRecord>()->GetWeeklyShareReward();
	
}

void RpcC2G_GetPlatShareAward::OnDelayReplyRpc(const GetPlatShareAwardArg &roArg, GetPlatShareAwardRes &roRes, const CUserData &roUserData)
{
}
