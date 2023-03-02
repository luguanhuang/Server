#include "pch.h"
#include "scene/ptcg2m_scenedestroyed.h"
#include "scene/scenemanager.h"

// generate by ProtoGen at date: 2016/7/26 13:08:53

void PtcG2M_SceneDestroyed::Process(UINT32 dwConnID)
{
	CSceneManager::Instance()->DestroyScene(m_Data.sceneid());
}
