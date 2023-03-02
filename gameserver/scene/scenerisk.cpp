#include "pch.h"
#include "scenerisk.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "foreach.h"
#include "unit/role.h"
#include <vector>

SceneRisk::SceneRisk(Scene* scene)
{
	m_pScene = scene;
	m_reEnter = false;
}

SceneRisk* SceneRisk::CreateSceneRisk(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SCENE_RISK)
	{
		SceneRisk* sceneRisk = new SceneRisk(scene);
		return sceneRisk;
	}
	return NULL;
}

