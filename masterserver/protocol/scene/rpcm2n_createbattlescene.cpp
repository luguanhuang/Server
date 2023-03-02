#include "pch.h"
#include "scene/rpcm2n_createbattlescene.h"
#include "scene/scenemanager.h"
#include "scene/scenecreate.h"

// generate by ProtoGen at date: 2016/11/4 10:52:58

RPC_CLIENT_IMPLEMETION(RpcM2N_CreateBattleScene, CreateBattleSceneArg, CreateBattleSceneRes)

void RpcM2N_CreateBattleScene::OnReply(const CreateBattleSceneArg &roArg, const CreateBattleSceneRes &roRes, const CUserData &roUserData)
{
	CScene* scene = NULL;
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		scene = CSceneManager::Instance()->GetScene(roRes.sceneinstanceid());

		//临时兼容
		if(scene == NULL)
		{
			scene = CSceneManager::Instance()->CreateScene(roArg.mapid(), roRes.sceneinstanceid(), roRes.sceneline());
		}
	}

	CreateBattleRes res;
	res.set_result(roRes.result());
	CSceneCreator::Instance()->OnCreateBattleReply(roArg.uid(), scene, roArg.param(), res);
}

void RpcM2N_CreateBattleScene::OnTimeout(const CreateBattleSceneArg &roArg, const CUserData &roUserData)
{
	CreateBattleRes res;
	res.set_result(KKSG::ERR_TIMEOUT);
	CSceneCreator::Instance()->OnCreateBattleReply(roArg.uid(), NULL, roArg.param(), res);
}
