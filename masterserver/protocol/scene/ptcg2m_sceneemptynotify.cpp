#include "pch.h"
#include "scene/scenemanager.h"
#include "scene/ownerscenes.h"
#include "scene/ptcg2m_sceneemptynotify.h"

// generate by ProtoGen at date: 2016/11/30 23:36:19

void PtcG2M_SceneEmptyNotify::Process(UINT32 dwConnID)
{
	OneOwnerScene* onescene = CSceneManager::Instance()->FindOwnerSceneBySceneId(m_Data.mapid(), m_Data.sceneuid());
	if (NULL != onescene)
	{
		onescene->SetClose();
	}
	SSInfo<<"mapid:"<<m_Data.mapid()<<" sceneid:"<<m_Data.sceneuid()<<END;
}
