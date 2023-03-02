#include "pch.h"
#include "scene.h"
#include "commondef.h"
#include "scenemanager.h"
#include "shm/shmmgr.h"


CScene::CScene()
:m_poShmData(NULL)
,m_poConf(NULL)
{
}

CScene::~CScene()
{
	ShmMgr::Instance()->GetSceneShm().Remove(m_poShmData->m_dwID);
}

void CScene::OnCreate()
{
}

void CScene::OnDestroy()
{
}

UINT32 CScene::GetSceneType()
{
	if (m_poConf != NULL)
	{
		return m_poConf->type;
	}

	return 0;
}