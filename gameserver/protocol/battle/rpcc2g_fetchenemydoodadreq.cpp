#include "pch.h"
#include "battle/rpcc2g_fetchenemydoodadreq.h"
#include "crpc.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2015/11/25 15:23:00

RPC_SERVER_IMPLEMETION(RpcC2G_FetchEnemyDoodadReq, EnemyDoodadInfo, RollInfoRes)

void RpcC2G_FetchEnemyDoodadReq::OnCall(const EnemyDoodadInfo &roArg, RollInfoRes &roRes)
{
	roRes.set_errcode((ErrorCode)KKSG::ERR_INVALID_REQUEST);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}

	Scene *pScene = pRole->GetCurrScene();
	if (pScene == NULL || !pScene->IsSyncScene())
	{
		return;
	}

	pScene->FetchDoodad(pRole, roArg, roRes);

}

void RpcC2G_FetchEnemyDoodadReq::OnDelayReplyRpc(const EnemyDoodadInfo &roArg, RollInfoRes &roRes, const CUserData &roUserData)
{
}
