#include "pch.h"
#include "scenemanager.h"
#include "role/rolemanager.h"
#include "util.h"
#include "config.h"
#include "shm/shmmgr.h"

INSTANCE_SINGLETON(CSceneManager)

#define SERVERMODE 1


CSceneManager::CSceneManager()
:m_dwSceneID(INVALID_SCENE_ID)
{
}

CSceneManager::~CSceneManager()
{
}

bool CSceneManager::Init()
{
	RecoverFromShm();
	return true;
}

void CSceneManager::Uninit()
{
}

void CSceneManager::RecoverFromShm()
{
	std::vector<CShmScene*> shmScenes;
	ShmMgr::Instance()->GetSceneShm().GetAll(shmScenes);
	if (shmScenes.empty())
	{
		return;
	}
	LogWarn("Recover %u scene from shm", shmScenes.size());

	for (size_t i = 0; i < shmScenes.size(); ++i)
	{
		CShmScene* shmData = shmScenes[i];
		const SceneConf* poConf = CSceneConfig::Instance()->GetSceneConf(shmData->m_dwMap);
		if(poConf == NULL)
		{
			LogError("Scene template id %u is invalid", shmData->m_dwMap);
			continue;
		}
		CScene* poNew = new CScene();
		poNew->SetShmSceneData(shmData);
		poNew->SetConf(poConf);
		m_oSceneMap[shmData->m_dwID] = poNew;

		if (shmData->m_dwID > m_dwSceneID)
		{
			m_dwSceneID = shmData->m_dwID;
		}
	}
}

CScene* CSceneManager::CreateScene(UINT32 dwTemplateID, UINT32 dwInstanceID, UINT32 dwLine)
{
	const SceneConf* poConf = CSceneConfig::Instance()->GetSceneConf(dwTemplateID);
	if(poConf == NULL)
	{
		LogError("Scene template id %u is invalid", dwTemplateID);
		return NULL;
	}

	if(GetScene(dwInstanceID) != NULL)
	{
		return NULL;
	}

	CScene* poNew = new CScene();
	poNew->SetShmSceneData(ShmMgr::Instance()->GetSceneShm().Get(dwInstanceID));
	poNew->SetID(dwInstanceID);
	poNew->SetMapID(dwTemplateID);
	poNew->SetLine(dwLine);
	poNew->SetConf(poConf);

	m_oSceneMap[dwInstanceID] = poNew;
	return poNew;
}

void CSceneManager::DestroyScene(UINT32 dwSceneID)
{
	CSceneMap::iterator it = m_oSceneMap.find(dwSceneID);
	if(it != m_oSceneMap.end())
	{
		delete it->second;
		m_oSceneMap.erase(it);
	}
}

CScene* CSceneManager::GetScene(UINT32 dwSceneID)
{
	CSceneMap::iterator it = m_oSceneMap.find(dwSceneID);
	return it == m_oSceneMap.end() ? NULL : it->second;
}

void CSceneManager::GetScene(UINT32 dwMapID, std::vector<CScene*>& scenes)
{
	for (auto iter = Begin(); iter != End(); ++iter)
	{
		if (iter->second && iter->second->GetMapID() == dwMapID)
		{
			scenes.push_back(iter->second);
		}
	}
}

void CSceneManager::GetSceneByType(UINT32 type, std::vector<CScene*>& scenes)
{
	for (auto iter = Begin(); iter != End(); ++iter)
	{
		if (iter->second && iter->second->GetSceneType() == type)
		{
			scenes.push_back(iter->second);
		}
	}
}

UINT32 CSceneManager::NewID()
{
	++m_dwSceneID;
	if(m_dwSceneID == INVALID_SCENE_ID)
	{
		++m_dwSceneID;
	}

	return m_dwSceneID;
}

bool CSceneManager::IsDynamicScene(UINT32 dwTemplateID)
{
	const SceneConf* poConf = CSceneConfig::Instance()->GetSceneConf(dwTemplateID);
	if(poConf == NULL)
	{
		return false;
	}
	///> 由于组队限制，导致工会场景只能在主城创建
	if(poConf->type == KKSG::SCENE_GUILD_HALL)
	{
		return false;
	}
	if(poConf->syncMode != SERVERMODE)
	{
		return false;
	}

	return !poConf->IsStaticScene;
}

void CSceneManager::DestroySceneOnGsLine(UINT32 dwLine)
{
	for(CSceneMap::iterator it = m_oSceneMap.begin(); it != m_oSceneMap.end();)
	{
		CScene* poScene = it->second;
		if(poScene->GetLine() == dwLine)
		{
			delete poScene;
			m_oSceneMap.erase(it++);
		}
		else
		{
			++it;
		}
	}
}