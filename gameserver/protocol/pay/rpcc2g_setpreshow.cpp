#include "pch.h"
#include "pay/rpcc2g_setpreshow.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/payv2Record.h"

// generate by ProtoGen at date: 2017/9/7 15:32:38

RPC_SERVER_IMPLEMETION(RpcC2G_SetPreShow, SetPreShowArg, SetPreShowRes)

void RpcC2G_SetPreShow::OnCall(const SetPreShowArg &roArg, SetPreShowRes &roRes)
{
	roRes.set_result(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	if (!pRole->Get<CPayV2Record>()->SetPreShow(roArg))
	{
		return;
	}
	roRes.set_result(ERR_SUCCESS);
}

void RpcC2G_SetPreShow::OnDelayReplyRpc(const SetPreShowArg &roArg, SetPreShowRes &roRes, const CUserData &roUserData)
{
}
