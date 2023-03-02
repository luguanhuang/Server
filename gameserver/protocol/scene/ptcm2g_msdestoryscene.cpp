#include "pch.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/ptcm2g_msdestoryscene.h"

// generate by ProtoGen at date: 2016/11/30 23:36:58

void PtcM2G_MSDestoryScene::Process(UINT32 dwConnID)
{
	Scene* scene = SceneManager::Instance()->FindBySceneID(m_Data.sceneuid());	
	if (NULL != scene)
	{
		if (scene->GetRoleCount() > 0)
		{
			SSWarn<<"role count:"<<scene->GetRoleCount()<<END;
		}
		if (!scene->GetWaitingRoles().empty())
		{
			SSWarn<<"loading role count:"<<scene->GetWaitingRoles().size()<<END;
		}
		scene->KickAllRoleOut();
		scene->SetDontDestory(false);

		SSInfo<<"mapid:"<<scene->GetSceneTemplateID()<<" destroy scene:"<<m_Data.sceneuid()<<END;
	}
}
