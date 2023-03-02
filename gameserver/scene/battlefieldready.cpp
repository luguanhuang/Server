#include "pch.h"
#include "battlefieldready.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "unit/role.h"
#include <vector>

#include "gamelogic/MultActivityMgr.h"
#include "util/gametime.h"
#include "gamelogic/battlefieldgsmgr.h"


SceneBFReady::SceneBFReady(Scene* scene)
{
	m_pScene	= scene;


}

SceneBFReady::~SceneBFReady()
{
}

SceneBFReady* SceneBFReady::CreateSceneBFReady(Scene* scene)
{
	BattleFieldGSMgr::Instance()->SetSceneID(scene->GetSceneID());
	if (scene->GetSceneType() == KKSG::SCENE_BATTLEFIELD_READY)
	{
		SceneBFReady* pScene = new SceneBFReady(scene);
		return pScene;
	}
	return NULL;
}
