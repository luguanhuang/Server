#include "pch.h"
#include "scene/ptcc2g_syncscenefinish.h"
#include "unit/rolemanager.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/scenetower.h"

// generate by ProtoGen at date: 2016/6/1 10:32:49

void PtcC2G_SyncSceneFinish::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	Scene* scene = pRole->GetCurrScene();
	if (scene == NULL)
	{
		return;
	}

	if (scene->GetSceneType() == KKSG::SCENE_TOWER)
	{
		std::vector<Role*> roles;
		roles.push_back(pRole);
		//pRole->GetCurrScene()->TeamMemberCheckFinish(SCENE_LOSE, roles);
		pRole->GetCurrScene()->GetSceneTower()->AddLeaveRole(pRole);

		pRole->GetCurrScene()->EndSyncScene(SCENE_LOSE);
	}
}
