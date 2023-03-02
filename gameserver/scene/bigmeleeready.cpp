#include "pch.h"
#include "bigmeleeready.h"
#include "unit/enemy.h"
#include "foreach.h"
#include "scene/scene.h"
#include "unit/role.h"
#include <vector>
#include "bigmelee/ptcg2c_bmreadytimentf.h" 
#include "gamelogic/MultActivityMgr.h"
#include "util/gametime.h"

SceneBMReady::SceneBMReady(Scene* scene)
{
	m_pScene = scene;
	m_timerId = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);

}

SceneBMReady::~SceneBMReady()
{
	CTimerMgr::Instance()->KillTimer(m_timerId);
}

SceneBMReady* SceneBMReady::CreateSceneBMReady(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SCENE_BIGMELEE_READY)
	{
		SceneBMReady* pScene = new SceneBMReady(scene);
		return pScene;
	}
	return NULL;
}




void SceneBMReady::SyncTime()
{
	UINT32 nTime = GameTime::GetTime();
	UINT32 nLeft = 0;
	UINT32 nEndTime = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_BIGMELEE);
	if (nEndTime >= nTime)
	{
		nLeft = nEndTime - nTime;
	}else
	{
		m_pScene->SetDontDestory(false);
		return;
	}
	PtcG2C_BMReadyTimeNtf ntf;
	ntf.m_Data.set_time(nLeft);
	m_pScene->Broadcast(ntf);
}

void SceneBMReady::OnEnterScene(Role* role)
{
	UINT32 nTime = GameTime::GetTime();
	UINT32 nLeft = 0;
	UINT32 nEndTime = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_BIGMELEE);
	if (nEndTime >= nTime)
	{
		nLeft = nEndTime - nTime;
	}else
	{
		return;
	}
	PtcG2C_BMReadyTimeNtf ntf;
	ntf.m_Data.set_time(nLeft);
	role->Send(ntf);
}


void SceneBMReady::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	SyncTime();
}