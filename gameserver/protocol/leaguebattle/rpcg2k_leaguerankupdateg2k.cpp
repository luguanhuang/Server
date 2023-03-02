#include "pch.h"
#include "leaguebattle/rpcg2k_leaguerankupdateg2k.h"
#include "scene/sceneleague.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2017/2/3 20:28:06

RPC_CLIENT_IMPLEMETION(RpcG2K_LeagueRankUpdateG2K, LeagueRankUpdateG2MArg, LeagueRankUpdateG2MRes)

void RpcG2K_LeagueRankUpdateG2K::OnReply(const LeagueRankUpdateG2MArg &roArg, const LeagueRankUpdateG2MRes &roRes, const CUserData &roUserData)
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

void RpcG2K_LeagueRankUpdateG2K::OnTimeout(const LeagueRankUpdateG2MArg &roArg, const CUserData &roUserData)
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
