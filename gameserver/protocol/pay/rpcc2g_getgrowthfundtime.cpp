#include "pch.h"
#include "pay/rpcc2g_getgrowthfundtime.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/payv2Mgr.h"

// generate by ProtoGen at date: 2017/5/2 9:42:54

RPC_SERVER_IMPLEMETION(RpcC2G_GetGrowthFundTime, GetGrowthFundTimeArg, GetGrowthFundTimeRes)

void RpcC2G_GetGrowthFundTime::OnCall(const GetGrowthFundTimeArg &roArg, GetGrowthFundTimeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_err(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	roRes.set_err(ERR_SUCCESS);
	roRes.set_lefttime(PayV2Mgr::Instance()->GetGrowthFundLeftTime());
}

void RpcC2G_GetGrowthFundTime::OnDelayReplyRpc(const GetGrowthFundTimeArg &roArg, GetGrowthFundTimeRes &roRes, const CUserData &roUserData)
{
}
