#include "pch.h"
#include "login/rpcn2g_notifyroleloginreconnect2gs.h"
#include "unit/rolemanager.h"
#include "unit/roleinfoinit.h"
#include "network/gatelink.h"
#include "common/ptcg2t_changegsline.h"
#include "role/ptcg2c_selectrolentf.h"
#include "table/OpenSystemMgr.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "session/ptcg2m_clientsessionchangeg2m.h"

// generate by ProtoGen at date: 2017/6/20 20:07:04

RPC_SERVER_IMPLEMETION(RpcN2G_NotifyRoleLoginReconnect2Gs, NotifyRoleLoginReconnect2GsArg, NotifyRoleLoginReconnect2GsRes)

void RpcN2G_NotifyRoleLoginReconnect2Gs::OnCall(const NotifyRoleLoginReconnect2GsArg &roArg, NotifyRoleLoginReconnect2GsRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		LogError("role[%llu] oldsession[%llu] newsession[%llu], role not exists", roArg.roleid(), roArg.oldsession(), roArg.newsession());
		roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);
		return;
	}
	Scene* pScene = NULL;
	if (roArg.sceneid() == INVALID_SCENE_ID)
	{
		pScene = pRole->GetCurrScene();
		if (pScene == NULL)
		{
			pScene = pRole->GetLoadingScene();
		}
		LogWarn("invalid sceneid");
	}
	else
	{
		pScene = SceneManager::Instance()->FindBySceneID(roArg.sceneid());	
	}

	if (pScene == NULL)
	{
		LogError("role[%llu] oldsession[%llu] newsession[%llu], scene not exists", roArg.roleid(), roArg.oldsession(), roArg.newsession());
		roRes.set_result(KKSG::ERR_FAILED);
		return;	
	}

	if (pRole->GetCurrScene() != pScene && pRole->GetLoadingScene() != pScene)
	{
		LogError("role[%llu] oldsession[%llu] newsession[%llu], not role scene", roArg.roleid(), roArg.oldsession(), roArg.newsession());
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	pRole->SetClientConf(roArg.cinfo());
	RoleManager::Instance()->ConvertSession(pRole, roArg.oldsession(), roArg.newsession());

	// 新的session接管了这个role，Reconnecting状态没用了
	pRole->SetFlag(RSF_ISReconnecting, false);
	pRole->SetFlag(RSF_ISLoadingScene, false);
	pRole->SetFlag(RSF_ISLoginReconnect, true);

	//  通知MS客户端信息变化
	PtcG2M_ClientSessionChangeG2M oSessionPtc;
	oSessionPtc.m_Data.set_roleid(pRole->GetID());
	oSessionPtc.m_Data.set_oldsession(roArg.oldsession());
	oSessionPtc.m_Data.set_newsession(roArg.newsession());
	oSessionPtc.m_Data.mutable_cinfo()->CopyFrom(roArg.cinfo());
	pRole->SendToMS(oSessionPtc);

	// 通知gt
	PtcG2T_ChangeGsLine oPtc;
	oPtc.m_Data.set_sessionid(pRole->GetSessionID());
	oPtc.m_Data.set_line(GSConfig::Instance()->GetLine());
	oPtc.m_Data.set_is_cross_gs(GSConfig::Instance()->IsCrossGS());
	pRole->Send(oPtc);

	// select role ntf
	PtcG2C_SelectRoleNtf oRoleNtf;
	pRole->DoAllSave();
	*oRoleNtf.m_Data.mutable_roledata() = pRole->GetRoleAllInfo();
	oRoleNtf.m_Data.set_serverid(pRole->GetServerId());
	OpenSystemMgr::Instance()->ExceptCloseSystem(pRole, oRoleNtf.m_Data.mutable_roledata());
	pRole->Send(oRoleNtf);

	// 通知客户端进场景
	pScene->BeforeRoleLoginReconnect(pRole);
	roRes.set_result(KKSG::ERR_SUCCESS);	
}

void RpcN2G_NotifyRoleLoginReconnect2Gs::OnDelayReplyRpc(const NotifyRoleLoginReconnect2GsArg &roArg, NotifyRoleLoginReconnect2GsRes &roRes, const CUserData &roUserData)
{
}
