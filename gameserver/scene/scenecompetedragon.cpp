#include "pch.h"
#include "scenecompetedragon.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "foreach.h"
#include "unit/role.h"
#include <vector>
#include "table/globalconfig.h"
#include "gamelogic/towerconfig.h"
#include "gamelogic/XLevelWave.h"
#include "foreach.h"
#include "gamelogic/dragonrecord.h"
#include "scene/sceneteam.h"
#include "event/eventmgr.h"
#include "gamelogic/popwindows.h"
#include "gamelogic/dragonconfig.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/competedragonrecord.h"

SceneCompeteDragon::SceneCompeteDragon(Scene* scene)
{
	m_pScene = scene;
}

SceneCompeteDragon* SceneCompeteDragon::CreateSceneCompeteDragon(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SCENE_COMPETEDRAGON)
	{
		SceneCompeteDragon* dragon = new SceneCompeteDragon(scene);
		return dragon;
	}
	return NULL;
}

void SceneCompeteDragon::FinishScene(std::vector<Role*>& roles)
{
	SceneTeam* team = m_pScene->GetSceneTeam();
	if (!team)
	{
		return;
	}

	if (!team->HasNextScene())
	{
		foreach(i in roles)
		{
			(*i)->Get<CCompeteDragonRecord>()->AddThroughCount();
		}

	}
}

void SceneCompeteDragon::ChangeScene()
{
	SceneTeam* pTeam = m_pScene->GetSceneTeam();
	if (!pTeam)
	{
		return;
	}
	if (m_pScene->GetSceneState()== SCENE_WIN)
	{
		const std::list<Role*>& roles = m_pScene->GetAllRoles();
		if (roles.empty())
		{
			return;
		}
		if (pTeam->HasNextScene())
		{
			//有可以切过去的玩家，有下一个场景
			pTeam->ChangeScene();
		}
	}

}

bool SceneCompeteDragon::CheckSendBattleResult()
{
	SceneTeam* pTeam = m_pScene->GetSceneTeam();
	if (!pTeam)
	{
		return false;
	}

	if (pTeam->HasNextScene())
	{
		return false;
	}

	if (!m_pScene->IsSceneWinState())
	{
		return false;
	}

	return true;
}

