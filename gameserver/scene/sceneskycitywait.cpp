#include "pch.h"
#include "scene/sceneskycitywait.h"
#include "unit/enemy.h"
#include "foreach.h"
#include "scene/scene.h"
#include "unit/role.h"
#include <vector>
#include "pb/project.pb.h"
#include "skycity/ptcg2c_skycitytimeres.h"
#include "gamelogic/MultActivityMgr.h"

SceneSkyCityWait::SceneSkyCityWait(Scene* scene)
{
	m_pScene = scene;
	m_reEnter = false;
}

SceneSkyCityWait* SceneSkyCityWait::CreateSceneSkyWait(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SKYCITY_WAITING)
	{
		SceneSkyCityWait* pScene = new SceneSkyCityWait(scene);
		return pScene;
	}
	return NULL;
}


void SceneSkyCityWait::OnEnterScene(Role* role)
{
	UINT32 nTime = TimeUtil::GetTime();
	UINT32 nLeft = 0;
	UINT32 nEndTime = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_SKYCITY);
	if (nEndTime > nTime)
	{
		nLeft = nEndTime - nTime;
	}
	PtcG2C_SkyCityTimeRes ntf;
	ntf.m_Data.set_type(KKSG::Waiting);
	ntf.m_Data.set_time(nLeft);
	role->Send(ntf);
}
