#include "pch.h"
#include "leaguebattle/rpcg2m_leaguerankupdateg2m.h"
#include "scene/sceneleague.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2017/1/12 10:19:39

RPC_CLIENT_IMPLEMETION(RpcG2M_LeagueRankUpdateG2M, LeagueRankUpdateG2MArg, LeagueRankUpdateG2MRes)

void RpcG2M_LeagueRankUpdateG2M::OnReply(const LeagueRankUpdateG2MArg &roArg, const LeagueRankUpdateG2MRes &roRes, const CUserData &roUserData)
{
	Scene* scene = SceneManager::Instance()->FindBySceneID(roArg.sceneid());
	if (scene == NULL)
	{
		LogError("can't find scene[%u]", roArg.sceneid());
		return;
	}
	SceneLeague* handler = scene->GetLeagueBattleHandler();
	if (handler == NULL)
	{
		return;
	}
	handler->EndRankUpdate(&roRes);
}

void RpcG2M_LeagueRankUpdateG2M::OnTimeout(const LeagueRankUpdateG2MArg &roArg, const CUserData &roUserData)
{
	Scene* scene = SceneManager::Instance()->FindBySceneID(roArg.sceneid());
	if (scene == NULL)
	{
		LogError("can't find scene[%u]", roArg.sceneid());
		return;
	}
	SceneLeague* handler = scene->GetLeagueBattleHandler();
	if (handler == NULL)
	{
		return;
	}
	handler->EndRankUpdate(NULL);
}
