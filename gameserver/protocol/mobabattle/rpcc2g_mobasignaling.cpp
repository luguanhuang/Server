#include "pch.h"
#include "mobabattle/rpcc2g_mobasignaling.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/scenemobabattle.h"
#include "scene/sceneherobattle.h"

// generate by ProtoGen at date: 2017/6/13 19:39:42

RPC_SERVER_IMPLEMETION(RpcC2G_MobaSignaling, MobaSignalingArg, MobaSignalingRes)

void RpcC2G_MobaSignaling::OnCall(const MobaSignalingArg &roArg, MobaSignalingRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Scene* scene = pRole->GetCurrScene();
	if (scene == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	SceneMobaBattle* mobaBattle = scene->GetMobaBattleHandler();
	if (mobaBattle != NULL)
	{
		roRes.set_errorcode(mobaBattle->SendSignal(pRole, roArg.type()));	
		return;
	}
	SceneHeroBattle* heroBattle = scene->GetHeroBattleHandler();
	if (heroBattle != NULL)
	{
		roRes.set_errorcode(heroBattle->SendSignal(pRole, roArg.type()));
		return;
	}
	roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
}

void RpcC2G_MobaSignaling::OnDelayReplyRpc(const MobaSignalingArg &roArg, MobaSignalingRes &roRes, const CUserData &roUserData)
{
}
