#include "pch.h"
#include "survive/rpcc2g_survivereqc2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "globalconfig.h"
#include "gamelogic/rolemisc.h"

// generate by ProtoGen at date: 2017/9/20 17:49:28

RPC_SERVER_IMPLEMETION(RpcC2G_SurviveReqC2G, SurviveReqArg, SurviveReqRes)

void RpcC2G_SurviveReqC2G::OnCall(const SurviveReqArg &roArg, SurviveReqRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_errorcode(ERR_SUCCESS);

	roRes.set_curtopcount(pRole->Get<RoleMisc>()->SurviveGetTopCount());
	roRes.set_needtopcount(GetGlobalConfig().SurviveRewardTopCount);
	roRes.set_givereward(pRole->Get<RoleMisc>()->SurviveGetRewardFlag());
	roRes.set_point(pRole->Get<RoleMisc>()->SurviveGetPoint());
}

void RpcC2G_SurviveReqC2G::OnDelayReplyRpc(const SurviveReqArg &roArg, SurviveReqRes &roRes, const CUserData &roUserData)
{
}
