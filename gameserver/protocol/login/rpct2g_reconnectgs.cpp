#include "pch.h"
#include "login/rpct2g_reconnectgs.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "scene/scene.h"
#include "entity/XRole.h"
#include "component/XNetComponent.h"

// generate by ProtoGen at date: 2016/10/15 20:05:37

RPC_SERVER_IMPLEMETION(RpcT2G_ReconnectGs, ReconnectGsArg, ReconnectGsRes)

void RpcT2G_ReconnectGs::OnCall(const ReconnectGsArg &roArg, ReconnectGsRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	role->UpdateOnlineTime();
	role->SetFlag(RSF_ISReconnecting, roArg.isadd());

	if (roArg.isadd())
	{
		role->SetFlag(RSF_ISLoadingScene, false);
		if(NULL != role->GetCurrScene() && role->GetCurrScene()->IsViewGrid())
		{
			XRole* xrole = role->GetXRole();
			xrole->Idled();		
			if(xrole->NetComponent())
			{
				xrole->NetComponent()->ReportMoveAction(xrole->GetPosition_p(), true, xrole->GetFace_p());
			}
		}
	}
	else
	{
		if (roArg.roleid() != role->GetID())
		{
			SSWarn<<"roleid:"<<role->GetID()<<" client:"<<roArg.roleid()<<END;
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
		role->OnClientReconnect(roArg.sceneid());
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcT2G_ReconnectGs::OnDelayReplyRpc(const ReconnectGsArg &roArg, ReconnectGsRes &roRes, const CUserData &roUserData)
{
}
