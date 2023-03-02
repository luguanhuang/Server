#include "pch.h"
#include "scene/ptcc2g_leavescenereq.h"
#include "unit/rolemanager.h"
#include "scene/scenemanager.h"
#include "gamelogic/team.h"
#include "gamelogic/teammgr.h"
#include "scene/scene.h"
#include "gamelogic/team.h"
#include "gamelogic/teamrequest.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2014/10/16 11:11:07

void PtcC2G_LeaveSceneReq::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	UINT32 destid = 0;
	Scene* scene = pRole->GetCurrScene();
	if (scene == NULL)
	{
		Scene *pLoadingScene = pRole->GetLoadingScene();
		if (pLoadingScene != NULL)
		{
			// role in enter scene flow, do not interrupt it
			/*
			if (pLoadingScene->GetSceneTemplateID() != pRole->GetLastHallSceneID())
			{
				pLoadingScene->RemoveWaitingRole(pRole);
				pRole->ClearLoadingScene();
				SceneManager::Instance()->ChangeScene(pRole, pRole->GetLastHallSceneID());
				return;
			}
			*/
			// reconnect notify
			//pRole->DoEnterNtf(pLoadingScene, 0);
			SSWarn << pRole << " try to leave scene when loading " << pRole->GetLoadingScene() << END;
		}

		return;
	}

	SSInfo << "role " << pRole << " leave scene " << pRole->GetCurrScene() << END;

	///> 单机情况下记录关卡失败

	if (!scene->IsSyncScene())
	{
		pRole->GetCurrScene()->EndSoloScene(SCENE_LOSE);
	}

	if (scene->GetSceneType() == KKSG::SCENE_GUILD_HALL || scene->GetSceneTemplateID() == GetGlobalConfig().BackFlowActivitySceneID)
	{
		destid = GetGlobalConfig().InitialReturnSceneID;
	}
	else
	{
		destid = pRole->GetPositionKeeper().GetLastHallSceneID();
	}
	SceneManager::Instance()->ChangeScene(pRole, destid);
}
