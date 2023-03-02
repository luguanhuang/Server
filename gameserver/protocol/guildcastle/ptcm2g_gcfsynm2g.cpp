#include "pch.h"
#include "guildcastle/ptcm2g_gcfsynm2g.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/scenecastle.h"

// generate by ProtoGen at date: 2016/12/19 10:34:14

void PtcM2G_GCFSynM2G::Process(UINT32 dwConnID)
{
	switch(m_Data.type())
	{
	case KKSG::GCF_SYN_MUL_POINT:
		{
			Scene* pScene = SceneManager::Instance()->FindBySceneID(m_Data.sceneid());
			if(pScene && pScene->GetCastleHandler())
			{
				pScene->GetCastleHandler()->OnMultiPoint(m_Data.mapid(), m_Data.pmulti());
			}
		}
		break;
	case KKSG::GCF_SYN_FIGHT_END:
		{
			Scene* pScene = SceneManager::Instance()->FindBySceneID(m_Data.sceneid());
			if(pScene && pScene->GetCastleHandler())
			{
				pScene->GetCastleHandler()->SetFightEnd();	
			}
		}
		break;
	case KKSG::GCF_SYN_KILL:
		{
			Scene* pScene = SceneManager::Instance()->FindBySceneID(m_Data.sceneid());
			if(pScene && pScene->GetCastleHandler())
			{
				pScene->GetCastleHandler()->AllCountChange(m_Data.roleid(), m_Data.killcount());	
			}
		}
		break;
	default:
		break;
	}
}
