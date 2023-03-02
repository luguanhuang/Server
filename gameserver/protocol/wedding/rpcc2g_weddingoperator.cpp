#include "pch.h"
#include "wedding/rpcc2g_weddingoperator.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/scenewedding.h"

// generate by ProtoGen at date: 2017/7/26 16:34:00

RPC_SERVER_IMPLEMETION(RpcC2G_WeddingOperator, WeddingOperatorArg, WeddingOperatorRes)

void RpcC2G_WeddingOperator::OnCall(const WeddingOperatorArg &roArg, WeddingOperatorRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Scene* pScene = pRole->GetCurrScene();
	if (pScene == NULL || pScene->GetSceneType() != KKSG::SCENE_WEDDING)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	SceneWedding* pHandler = pScene->GetWeddingHandler();
	if (pHandler == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	KKSG::ErrorCode result = KKSG::ERR_FAILED;
	switch (roArg.type())
	{
	case KKSG::WeddingOper_Flower:
		result = pHandler->DoFlower(pRole);
		break;
	case KKSG::WeddingOper_Fireworks:
		result = pHandler->DoFireworks(pRole);
		break;
	case KKSG::WeddingOper_ApplyVows:
		result = pHandler->DoApplyVows(pRole);
		break;
	case KKSG::WeddingOper_AgreeVows:
		result = pHandler->DoAgreeVows(pRole);
		break;
	case KKSG::WeddingOper_DisAgreeVows:
		result = pHandler->DoDisagreeVows(pRole);
		break;
	default:
		LogError("invalid oper[%u]", roArg.type());
		break;
	}
	roRes.set_result(result);
}

void RpcC2G_WeddingOperator::OnDelayReplyRpc(const WeddingOperatorArg &roArg, WeddingOperatorRes &roRes, const CUserData &roUserData)
{
}
