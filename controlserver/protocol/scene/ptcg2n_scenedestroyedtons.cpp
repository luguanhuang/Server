#include "pch.h"
#include "scene/ptcg2n_scenedestroyedtons.h"
#include "scene/scenemanager.h"

// generate by ProtoGen at date: 2016/11/17 22:30:47

void PtcG2N_SceneDestroyedToNs::Process(UINT32 dwConnID)
{
	CSceneManager::Instance()->DestroyScene(m_Data.sceneid());
}
