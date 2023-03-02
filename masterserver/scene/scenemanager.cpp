#include "pch.h"
#include "scenemanager.h"
#include "role/rolemanager.h"
#include "util.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"

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
	m_id2lineselector[SelectLine_Hall] = &LineSelectorHall::GlobalSelectorHall;
	m_id2lineselector[SelectLine_Dynamic] = &LineSelectorDynamic::GlobalSelectorDynamic;
	m_id2lineselector[SelectLine_WorldBoss] = &LineSelectorWorldBoss::GlobalSelectorWorldBoss;
	m_id2lineselector[SelectLine_Specific] = &LineSelectorSpecific::GlobalSelectorSpecific;
	m_id2lineselector[SelectLine_GuildBoss] = &LineSelectorGuildBoss::GlobalSelectorGuildBoss;
	m_id2lineselector[SelectLine_Owner] = &LineSelectorOwner::GlobalSelectorOwner;
	return true;
}

void CSceneManager::Uninit()
{
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
	poNew->SetID(dwInstanceID);
	poNew->SetMapID(dwTemplateID);
	poNew->SetLine(dwLine);
	poNew->SetConf(poConf);

	m_oSceneMap[dwInstanceID] = poNew;
	LogInfo("CreateScene sceneMap's size is %u, templateID:%u, SceneID:%u", m_oSceneMap.size(), dwTemplateID, dwInstanceID);
	return poNew;
}

void CSceneManager::DestroyScene(UINT32 dwSceneID)
{
	CSceneMap::iterator it = m_oSceneMap.find(dwSceneID);
	if(it != m_oSceneMap.end())
	{
		m_owerscenes.RemoveScene(it->second->GetMapID(), dwSceneID);

		delete it->second;
		m_oSceneMap.erase(it);
		LogInfo("DestroyScene sceneMap's size is %u, sceneID:%u", m_oSceneMap.size(), dwSceneID);
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

//UINT32 CSceneManager::NewID()
//{
//	++m_dwSceneID;
//	if(m_dwSceneID == INVALID_SCENE_ID)
//	{
//		++m_dwSceneID;
//	}

//	return m_dwSceneID;
//}

bool CSceneManager::IsDynamicScene(UINT32 dwTemplateID)
{
	const SceneConf* poConf = CSceneConfig::Instance()->GetSceneConf(dwTemplateID);
	if(poConf == NULL)
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
			CGuildBossMgr::Instance()->OnGsClose(poScene->GetID());
			m_owerscenes.RemoveScene(poScene->GetMapID(), poScene->GetID());
			delete poScene;
			m_oSceneMap.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

UINT32 CSceneManager::FillLineDest(std::vector<CRole*>& roles, UINT32 destid, UINT32 mapid, const KKSG::SceneSwitchData* data, LineDest& dest)
{
	ILineSelector* selector = NULL;
	if(destid == INVALID_SCENE_ID)
	{
		if(CSceneManager::IsDynamicScene(mapid))
		{
			if (mapid == WORLDBOSS_MAP_ID)
			{
				selector = m_id2lineselector[SelectLine_WorldBoss];
				((LineSelectorWorldBoss*)selector)->m_roles = roles;
			}
			else if (mapid == GUILDBOSS_MAP_ID)
			{
				selector = m_id2lineselector[SelectLine_GuildBoss];
				((LineSelectorGuildBoss*)selector)->m_roles = roles;
			}
			else if (mapid == FAMILYGARDEN_MAP_ID || mapid == GUILD_MAP_ID || KKSG::SCENE_LEISURE == dest.type)
			{
				selector = m_id2lineselector[SelectLine_Owner];
				((LineSelectorOwner*)selector)->m_mapid = mapid;
				((LineSelectorOwner*)selector)->m_uid = NULL == data ? 0 : data->sceneowner();
				((LineSelectorOwner*)selector)->m_roles = roles;
			}
			else
			{
				selector = m_id2lineselector[SelectLine_Dynamic];
			}
		}
		else
		{
			selector = m_id2lineselector[SelectLine_Hall];
		}
	}
	else
	{
		selector = m_id2lineselector[SelectLine_Specific];
	}

	return NULL == selector ? KKSG::ERR_FAILED : selector->SelectLine(destid, dest);
}

OneOwnerScene* CSceneManager::FindOwnerScene(UINT32 type, UINT64 uid)
{
	return m_owerscenes.FindScene(type, uid);
}

OneOwnerScene* CSceneManager::FindOwnerSceneBySceneId(UINT32 type, UINT32 sceneid)
{
	return m_owerscenes.FindSceneBySceneId(type, sceneid);
}

bool CSceneManager::EnterOwnerScene(UINT32 type, UINT64 uid, UINT64 roleid)
{
	m_owerscenes.EnterScene(type, uid, roleid);
	return true;
}

UINT32 CSceneManager::GetSceneCount(UINT32 mapid)
{
	return m_owerscenes.GetSceneCount(mapid);
}
