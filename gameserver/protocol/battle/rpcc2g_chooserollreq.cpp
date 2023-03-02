#include "pch.h"
#include "battle/rpcc2g_chooserollreq.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/scenedoodad.h"

// generate by ProtoGen at date: 2016/8/22 17:03:42

RPC_SERVER_IMPLEMETION(RpcC2G_ChooseRollReq, ChooseRollReqArg, ChooseRollReqRes)

void RpcC2G_ChooseRollReq::OnCall(const ChooseRollReqArg &roArg, ChooseRollReqRes &roRes)
{
	roRes.set_error((ErrorCode)KKSG::ERR_INVALID_REQUEST);
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

	if (pScene->GetSceneDoodad())
	{
		pScene->GetSceneDoodad()->ChooseRoll(pRole, roArg.choosetype(), roArg.info());
	}

	roRes.set_error(KKSG::ERR_SUCCESS);

}

void RpcC2G_ChooseRollReq::OnDelayReplyRpc(const ChooseRollReqArg &roArg, ChooseRollReqRes &roRes, const CUserData &roUserData)
{
}
