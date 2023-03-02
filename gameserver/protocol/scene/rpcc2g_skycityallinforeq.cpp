#include "pch.h"
#include "scene/rpcc2g_skycityallinforeq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/vsstate.h"
#include "scene/scenevsbase.h"
#include "scene/sceneskycity.h"

// generate by ProtoGen at date: 2016/10/9 18:09:03

RPC_SERVER_IMPLEMETION(RpcC2G_SkyCityAllInfoReq, SkyCityArg, SkyCityRes)

void RpcC2G_SkyCityAllInfoReq::OnCall(const SkyCityArg &roArg, SkyCityRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		return ;
	}
	if(NULL == pRole->GetCurrScene())
	{
		return;
	}

	auto pHandler = pRole->GetCurrScene()->GetSkyCityHandler();
	if(pHandler)
	{
		pHandler->FillNowData(roRes);
	}
}

void RpcC2G_SkyCityAllInfoReq::OnDelayReplyRpc(const SkyCityArg &roArg, SkyCityRes &roRes, const CUserData &roUserData)
{
}
