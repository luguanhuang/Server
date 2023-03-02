#include "pch.h"
#include "pay/rpcc2g_getpayallinfo.h"
#include "gamelogic/payv2Mgr.h"
#include "gamelogic/payv2Record.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/8/11 18:00:06

RPC_SERVER_IMPLEMETION(RpcC2G_GetPayAllInfo, GetPayAllInfoArg, GetPayAllInfoRes)

void RpcC2G_GetPayAllInfo::OnCall(const GetPayAllInfoArg &roArg, GetPayAllInfoRes &roRes)
{
	roRes.set_errcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	roRes.set_errcode(ERR_SUCCESS);
	PayV2Mgr::Instance()->GetPayInfo(pRole, roRes.mutable_info());
}

void RpcC2G_GetPayAllInfo::OnDelayReplyRpc(const GetPayAllInfoArg &roArg, GetPayAllInfoRes &roRes, const CUserData &roUserData)
{
}
