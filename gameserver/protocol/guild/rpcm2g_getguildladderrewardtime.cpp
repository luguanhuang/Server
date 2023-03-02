#include "pch.h"
#include "guild/rpcm2g_getguildladderrewardtime.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/activityrecord.h"

// generate by ProtoGen at date: 2016/10/13 21:04:11

RPC_SERVER_IMPLEMETION(RpcM2G_GetGuildLadderRewardTime, GetGuildLadderRewardTimeArg, GetGuildLadderRewardTimeRes)

void RpcM2G_GetGuildLadderRewardTime::OnCall(const GetGuildLadderRewardTimeArg &roArg, GetGuildLadderRewardTimeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		roRes.set_time(0);
		return ;
	}

	roRes.set_time(pRole->Get<CActivityRecord>()->GetGuildLadderTime());
}

void RpcM2G_GetGuildLadderRewardTime::OnDelayReplyRpc(const GetGuildLadderRewardTimeArg &roArg, GetGuildLadderRewardTimeRes &roRes, const CUserData &roUserData)
{
}
