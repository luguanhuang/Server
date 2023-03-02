#include "pch.h"
#include "leaguebattle/rpcc2g_leaguebattlereadyreq.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/sceneleague.h"

// generate by ProtoGen at date: 2017/1/9 8:37:53

RPC_SERVER_IMPLEMETION(RpcC2G_LeagueBattleReadyReq, LeagueBattleReadyReqArg, LeagueBattleReadyReqRes)

void RpcC2G_LeagueBattleReadyReq::OnCall(const LeagueBattleReadyReqArg &roArg, LeagueBattleReadyReqRes &roRes)
{
	roRes.set_result(ERR_FAILED);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	Scene* curScene = pRole->GetCurrScene();
	if (curScene == NULL)
	{
		return;
	}
	SceneLeague* sceneLeague = curScene->GetLeagueBattleHandler();
	if (sceneLeague == NULL)
	{
		return;
	}
	int ret = KKSG::ERR_FAILED;
	if (roArg.type() == KKSG::LBReady_Up)
	{
		ret = sceneLeague->SetRoleUp(pRole);	
	}
	else if (roArg.type() == KKSG::LBReady_Down)
	{
		ret = sceneLeague->SetRoleDown(pRole);
	}
	roRes.set_result((KKSG::ErrorCode)ret);
}

void RpcC2G_LeagueBattleReadyReq::OnDelayReplyRpc(const LeagueBattleReadyReqArg &roArg, LeagueBattleReadyReqRes &roRes, const CUserData &roUserData)
{
}
