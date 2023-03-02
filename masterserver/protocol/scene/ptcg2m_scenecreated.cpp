#include "pch.h"
#include "scene/ptcg2m_scenecreated.h"
#include "scene/scenemanager.h"

// generate by ProtoGen at date: 2016/11/3 23:44:10

void PtcG2M_SceneCreated::Process(UINT32 dwConnID)
{
	CSceneManager::Instance()->CreateScene(m_Data.mapid(), m_Data.sceneid(), m_Data.line());
}
