#include "pch.h"
#include "loginactivity/rpcc2g_loginactivity.h"
#include "gamelogic/loginactivity.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2017/4/26 19:24:41

RPC_SERVER_IMPLEMETION(RpcC2G_LoginActivity, LoginActivityArg, LoginActivityRes)

void RpcC2G_LoginActivity::OnCall(const LoginActivityArg &roArg, LoginActivityRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	LoginActivityMgr::Instance()->Pack(pRole,roRes);
}

void RpcC2G_LoginActivity::OnDelayReplyRpc(const LoginActivityArg &roArg, LoginActivityRes &roRes, const CUserData &roUserData)
{
}
