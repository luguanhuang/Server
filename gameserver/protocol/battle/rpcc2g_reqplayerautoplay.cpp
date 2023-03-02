#include "pch.h"
#include "battle/rpcc2g_reqplayerautoplay.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "entity/XObject.h"
#include "entity/XActionDefine.h"

// generate by ProtoGen at date: 2016/5/30 17:17:53

RPC_SERVER_IMPLEMETION(RpcC2G_ReqPlayerAutoPlay, ReqAutoPlay, RetAutoPlay)

void RpcC2G_ReqPlayerAutoPlay::OnCall(const ReqAutoPlay &roArg, RetAutoPlay &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	
	if (pRole == NULL)
	{
		roRes.set_succ(false);
		return;
	}

	if (roArg.isautoplay())
	{
		XAIEnableEventArgs autoArgs;
		pRole->GetXObject()->DispatchAction(&autoArgs);
	}
	else
	{
		XAIDisableEventArgs disAutoArgs;
		pRole->GetXObject()->DispatchAction(&disAutoArgs);
	}

	roRes.set_succ(true);
}

void RpcC2G_ReqPlayerAutoPlay::OnDelayReplyRpc(const ReqAutoPlay &roArg, RetAutoPlay &roRes, const CUserData &roUserData)
{
}
