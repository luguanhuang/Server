#include "pch.h"
#include "battle/rpcc2g_inspirereq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/sceneinspire.h"

// generate by ProtoGen at date: 2016/10/3 16:16:17

RPC_SERVER_IMPLEMETION(RpcC2G_InspireReq, InspireArg, InspireRes)

void RpcC2G_InspireReq::OnCall(const InspireArg &roArg, InspireRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return ;
	}

	if(pRole->GetCurrScene() && pRole->GetCurrScene()->GetInspire())
	{
		pRole->GetCurrScene()->GetInspire()->HandlerReq(pRole, roArg, roRes);
	}
	else
	{
		roRes.set_errorcode(ERR_FAILED);
	}

}

void RpcC2G_InspireReq::OnDelayReplyRpc(const InspireArg &roArg, InspireRes &roRes, const CUserData &roUserData)
{
}
