#include "pch.h"
#include "bigmelee/rpcg2k_bmranklistreq.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "scene/bigmeleefight.h"


// generate by ProtoGen at date: 2017/9/13 14:37:29

RPC_CLIENT_IMPLEMETION(RpcG2K_BMRankListReq, BMRankListArg, BMRankListRes)

void RpcG2K_BMRankListReq::OnReply(const BMRankListArg &roArg, const BMRankListRes &roRes, const CUserData &roUserData)
{

	Scene *pScene = SceneManager::Instance()->FindBySceneID(roArg.sceneid());
	if (pScene)
	{
		if (pScene&&pScene->GetSceneType()==KKSG::SCENE_BIGMELEE_FIGHT)
		{
			pScene->GetBMFightHandler()->FillBMRankList(roRes);
		}
	}

}

void RpcG2K_BMRankListReq::OnTimeout(const BMRankListArg &roArg, const CUserData &roUserData)
{
}
