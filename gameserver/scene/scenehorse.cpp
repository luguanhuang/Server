#include "pch.h"
#include "scene/scenehorse.h"
#include "unit/enemy.h"
#include "foreach.h"
#include "scene/scene.h"
#include "unit/role.h"
#include <vector>
#include "pb/project.pb.h"
#include "horserace/ptcg2c_horsewaittimentf.h"
#include "gamelogic/MultActivityMgr.h"
#include "util/gametime.h"

SceneHorse::SceneHorse(Scene* scene)
{
	m_pScene = scene;
	m_reEnter = false;
}

SceneHorse* SceneHorse::CreateSceneHorse(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SCENE_HORSE)
	{
		SceneHorse* pScene = new SceneHorse(scene);
		return pScene;
	}
	return NULL;
}

void SceneHorse::OnEnterScene(Role* role)
{
	UINT32 nTime = GameTime::GetTime();
	UINT32 nLeft = 0;
	UINT32 nStartTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_HORSE);
	UINT32 nEndTime   = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_HORSE) + GetGlobalConfig().HorseWaitTime;
	if (nTime >= nStartTime && nTime < nEndTime)
	{
		nLeft = nEndTime - nTime;
		PtcG2C_HorseWaitTimeNtf ntf;
		ntf.m_Data.set_time(nLeft);
		role->Send(ntf);
	}
}
