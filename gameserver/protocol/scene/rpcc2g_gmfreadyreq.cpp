#include "pch.h"
#include "scene/rpcc2g_gmfreadyreq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/scenegmf.h"
#include "idip/idiprecord.h"

// generate by ProtoGen at date: 2016/10/1 16:23:39

RPC_SERVER_IMPLEMETION(RpcC2G_GmfReadyReq, GmfReadyArg, GmfReadyRes)

void RpcC2G_GmfReadyReq::OnCall(const GmfReadyArg &roArg, GmfReadyRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);	
	if(NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		return;
	}

	if(pRole->Get<CIdipRecord>()->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		roRes.set_errcode(ERR_FAILED);
		return;
	}

	if(pRole->GetCurrScene() && pRole->GetCurrScene()->GetGWBHandler())
	{
		int ret = pRole->GetCurrScene()->GetGWBHandler()->HandlerJoinRequest(pRole, roArg, roRes);
		roRes.set_errcode((ErrorCode)(ret));
	}
}

void RpcC2G_GmfReadyReq::OnDelayReplyRpc(const GmfReadyArg &roArg, GmfReadyRes &roRes, const CUserData &roUserData)
{
}
