#include "pch.h"
#include "SpawnSourceHandler.h"
#include "scene/scene.h"
#include "unit/combatattribute.h"
#include "unit/enemymanager.h"
#include "pb/project.pb.h"
#include "XLevelWave.h"
#include "arenamgr.h"
#include "unit/role.h"
#include "stagemgr.h"
#include "randombossmgr.h"

void SpawnSourceMonsterHandler::FillWave(Role *pRole, Scene *pScene, XLevelWave *pWave, KKSG::UnitAppearance *pData)
{
	EnemyManager::LoadEnemyAppearance(pWave->m_EnemyID, pData);
}

void SpawnSourcePlayerHandler::FillWave(Role *pRole, Scene *pScene, XLevelWave *pWave, KKSG::UnitAppearance *pData)
{
	if (pScene->GetSceneType() == KKSG::SCENE_ARENA)
	{
		CArenaMgr::Instance()->FillBattleInfo(pRole, pData);
	}
	else
	{
		LogWarn("scene spawn [Player] type in scene type [%s] failed!", pScene->SceneTypeStr());
	}
}

void SpawnSourceRandomHandler::FillWave(Role *pRole, Scene *pScene, XLevelWave *pWave, KKSG::UnitAppearance *pData)
{
	if (false)
	{
		int enemyID = RandomBossMgr::Instance()->GetRandomBoss(pWave->m_RandomID);
		EnemyManager::LoadEnemyAppearance(enemyID, pData, true);
	}
	else
	{
		LogWarn("scene spawn [Random] type in scene type [%s] failed!", pScene->SceneTypeStr());
	}
}


