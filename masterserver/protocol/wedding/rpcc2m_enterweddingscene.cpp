#include "pch.h"
#include "wedding/rpcc2m_enterweddingscene.h"
#include "marriage/weddingmgr.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/7/17 17:21:11

RPC_SERVER_IMPLEMETION(RpcC2M_EnterWeddingScene, EnterWeddingSceneArg, EnterWeddingSceneRes)

void RpcC2M_EnterWeddingScene::OnCall(const EnterWeddingSceneArg &roArg, EnterWeddingSceneRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Wedding* pWed = WeddingMgr::Instance()->GetWedding(roArg.weddingid());
	if (pWed == NULL)
	{
		roRes.set_result(KKSG::ERR_WEDDING_NOT_EXISTS);
		return;
	}
	roRes.set_result(pWed->DoEnterWedding(pRole));
}

void RpcC2M_EnterWeddingScene::OnDelayReplyRpc(const EnterWeddingSceneArg &roArg, EnterWeddingSceneRes &roRes, const CUserData &roUserData)
{
} 