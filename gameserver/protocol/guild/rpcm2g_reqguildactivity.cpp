#include "pch.h"
#include "guild/rpcm2g_reqguildactivity.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/activityrecord.h"

// generate by ProtoGen at date: 2016/9/16 16:40:03

RPC_SERVER_IMPLEMETION(RpcM2G_ReqGuildActivity, ReqGuildActivityAgr, ReqGuildActivityRes)

void RpcM2G_ReqGuildActivity::OnCall(const ReqGuildActivityAgr &roArg, ReqGuildActivityRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		return ;
	}

	roRes.set_roleidd(roArg.roleid());
	roRes.set_guildactivity(pRole->Get<CActivityRecord>()->GetWeekValue());
}

void RpcM2G_ReqGuildActivity::OnDelayReplyRpc(const ReqGuildActivityAgr &roArg, ReqGuildActivityRes &roRes, const CUserData &roUserData)
{
}
