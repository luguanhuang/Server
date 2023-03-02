#include "pch.h"
#include "sceneupdatehandler.h"
#include "scene.h"
#include "scenepk.h"
#include "gamelogic/XLevelWave.h"
#include "scenepvp.h"
#include "scenegmf.h"

#define PK_TIME 120

SpawnUpdateHandler SpawnUpdateHandler::m_updatehandler;

UINT32 SpawnUpdateHandler::Update(Scene* scene, float deltatime)
{
	XLevelSpawner* spawner = scene->GetSpawner();
	if(!spawner->Update(deltatime))
		return SCENE_RUNNING;

	if (spawner->IsLevelWin())
	{
		scene->SetSceneKillState(true);
		scene->SetSceneState(SCENE_WIN);
		scene->KillAllEnemy(true);
		return SCENE_WIN;
	}
	if (spawner->IsLevelLose())
	{
		scene->SetSceneKillState(true);
		return SCENE_LOSE;
	}
	return SCENE_RUNNING;
}

VsUpdateHandler VsUpdateHandler::m_updatehandler;

UINT32 VsUpdateHandler::Update(Scene* scene, float deltatime)
{
	SceneVsBase* pHandler = scene->GetVsHandler();
	if(NULL == pHandler)
	{
		SSError << " find vs handler null " << END;
		return SCENE_WIN;
	}

	if(pHandler->VsUpdate())
	{
		return SCENE_WIN;
	}

	if(scene->GetSpawner())
	{
		scene->GetSpawner()->Update(deltatime);
	}

	return SCENE_RUNNING;
}

