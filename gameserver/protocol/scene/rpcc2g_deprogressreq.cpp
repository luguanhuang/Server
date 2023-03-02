#include "pch.h"
#include "scene/rpcc2g_deprogressreq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/DragonExpConfig.h"

// generate by ProtoGen at date: 2016/9/3 17:29:16

RPC_SERVER_IMPLEMETION(RpcC2G_DEProgressReq, DEProgressArg, DEProgressRes)

void RpcC2G_DEProgressReq::OnCall(const DEProgressArg &roArg, DEProgressRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_errcode(ERR_SUCCESS);
	DragonExpConfig::Instance()->FillProgress(pRole, roRes);

}

void RpcC2G_DEProgressReq::OnDelayReplyRpc(const DEProgressArg &roArg, DEProgressRes &roRes, const CUserData &roUserData)
{
}
