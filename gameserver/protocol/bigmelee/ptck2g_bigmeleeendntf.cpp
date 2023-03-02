#include "pch.h"
#include "bigmelee/ptck2g_bigmeleeendntf.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "scene/bigmeleefight.h"

// generate by ProtoGen at date: 2017/8/26 11:20:50

void PtcK2G_BigMeleeEndNtf::Process(UINT32 dwConnID)
{
	if(GetGlobalConfig().BigMeleeFightScene.size()<GetGlobalConfig().BigMeleeMaxGames)
		return;
	std::vector<Scene*> scenes;
	SceneManager::Instance()->FindByTemplateID(GetGlobalConfig().BigMeleeFightScene[GetGlobalConfig().BigMeleeMaxGames-1],scenes);
	for (auto iter = scenes.begin(); iter != scenes.end(); iter++)
	{
		Scene* pScene = *iter;
		SceneBMFight*  pFightScene = pScene->GetBMFightHandler();
		if (pFightScene)
		{
			pFightScene->SetResult();
		}
	}
}
