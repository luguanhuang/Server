#include "pch.h"
#include "pk/rpcg2m_pkrankreq.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/scenepk.h"

// generate by ProtoGen at date: 2016/8/25 15:22:15

RPC_CLIENT_IMPLEMETION(RpcG2M_PkRankReq, QMHRPkArg, QMHRPkRes)

void RpcG2M_PkRankReq::OnReply(const QMHRPkArg &roArg, const QMHRPkRes &roRes, const CUserData &roUserData)
{
	UINT32 sceneid = roArg.sceneuniid();
	Scene* pScene = SceneManager::Instance()->FindBySceneID(sceneid);
	if(pScene && pScene->GetVsHandler())
	{
		auto pSubHandler = pScene->GetVsHandler()->GetHandler<ScenePkBase>();
		if(pSubHandler)
		{
			pSubHandler->EndRankUpdateDelay(roRes);
		}
	}
}

void RpcG2M_PkRankReq::OnTimeout(const QMHRPkArg &roArg, const CUserData &roUserData)
{
}
