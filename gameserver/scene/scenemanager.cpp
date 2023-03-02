#include "pch.h"
#include "scene.h"
#include "scenemanager.h"
#include "unit/role.h"
#include "sceneconfig.h"
#include "pb/project.pb.h"
#include "unit/enemy.h"
#include "unit/enemymanager.h"
#include "entity/XObject.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "enterscenecondition.h"
#include "common/ptcg2c_gserrornotify.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guildconfig.h"
#include "gamelogic/noticemgr.h"
#include "scenesecurityzone.h"
#include "foreach.h"
#include "sceneswitch.h"
#include "scene/ptcg2m_scenecreated.h"
#include "scene/ptcg2m_scenedestroyed.h"
#include "scene/ptcg2n_scenedestroyedtons.h"
#include "network/mslink.h"
#include "apollovoice/ApolloVoiceMgr.h"
#include "gamelogic/towerrecord.h"
#include "crossscene/ptcg2k_crossscenedestoryed.h"
#include "network/worldlink.h"
#include "network/controllink.h"

INSTANCE_SINGLETON(SceneManager)

class CheckSceneTimer : public ITimer
{
public:
	void Start()
	{
		m_TimerHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	}

	void Stop()
	{
		CTimerMgr::Instance()->KillTimer(m_TimerHandler);
	}

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		SceneManager::Instance()->CheckScene();
	}

	HTIMER m_TimerHandler;
};

Scene *SceneManager::CreateScene(SceneInfo *poInfo, UINT32 dwSceneInstanceID)
{
	Scene *newScene = Scene::CreateScene(poInfo->m_pConf->id, poInfo, dwSceneInstanceID);
	
	SceneManager::Instance()->AddScene(newScene);

	if(!poInfo->m_pConf->IsStaticScene)
	{
		PtcG2M_SceneCreated oPtc;
		oPtc.m_Data.set_mapid(poInfo->m_pConf->id);
		oPtc.m_Data.set_sceneid(newScene->GetSceneID());
		oPtc.m_Data.set_line(GSConfig::Instance()->GetLine());
		MSLink::Instance()->SendTo(oPtc);
	}
	return newScene;
}

Scene *SceneManager::CreateScene(SceneInfo *poInfo, UINT64 ownerID, UINT32 dwSceneInstanceID)
{
	Scene *newScene = Scene::CreateScene(poInfo->m_pConf->id, poInfo, ownerID, dwSceneInstanceID);
	
	SceneManager::Instance()->AddScene(newScene);

	if(!poInfo->m_pConf->IsStaticScene)
	{
		if (!GSConfig::Instance()->IsCrossGS())
		{
			PtcG2M_SceneCreated oPtc;
			oPtc.m_Data.set_mapid(poInfo->m_pConf->id);
			oPtc.m_Data.set_sceneid(newScene->GetSceneID());
			oPtc.m_Data.set_line(GSConfig::Instance()->GetLine());
			MSLink::Instance()->SendTo(oPtc);
		}
	}
	return newScene;
}

void SceneManager::CreateStaticScene(UINT32 sceneID, SceneInfo *pSceneInfo, void *)
{
	if (pSceneInfo->m_pConf->IsStaticScene)
	{
		Scene* pScene = NULL;
		switch(pSceneInfo->m_pConf->type)
		{
		case KKSG::SCENE_HALL:
			{
				if (GSConfig::Instance()->IsHallGS())
				{
					pScene = CreateScene(pSceneInfo);
				}
				break;
			}
		default:
			{
				pScene = CreateScene(pSceneInfo);
				break;
			}
		}
		if (NULL == pScene)
		{
			SSWarn<<"new scene failed, scene id:"<<pSceneInfo->m_pConf->id<<END;
			return;
		}
	}
	return;
}

SceneManager::SceneManager()
{
	m_CheckSceneTimer = NULL;
	m_pSecurityZone = NULL;
}

SceneManager::~SceneManager()
{

}

bool SceneManager::Init()
{
	CmdLine::RegistCmd("scene", CmdLineScene);
	CheckSceneTimer *timer = new CheckSceneTimer();
	timer->Start();
	m_CheckSceneTimer = timer;

	m_EnterCondtions.push_back(new EnterSceneNormalCondition());
	m_EnterCondtions.push_back(new EnterSceneCountCondition());
	m_EnterCondtions.push_back(new EnterSceneFatigueCondition());
	m_EnterCondtions.push_back(new EnterSceneTimeLimitCondition());
	m_EnterCondtions.push_back(new EnterGuildSceneCondition());
	m_EnterCondtions.push_back(new EnterTowerSceneCondition());
	m_EnterCondtions.push_back(new EnterHolidaySceneCondition());

	SceneConfig::Instance()->TravelAllSceneInfo(CreateStaticScene, NULL);

	m_pSecurityZone = new SceneSecurityZone();
	m_pSecurityZone->Load("table/RegionSkillList.txt", "table/RegionList.txt");

	return true;
}

void SceneManager::Uninit()
{
	CheckSceneTimer *timer = (CheckSceneTimer *)m_CheckSceneTimer;
	timer->Stop();
	delete timer;

	foreach(i in m_AllScenes)
	{
		Scene::DestroyScene(i->second);
	}

	foreach(i in m_EnterCondtions)
	{
		delete *i;
	}

	m_EnterCondtions.clear();
	m_AllScenes.clear();
	m_listStaticScene.clear();

	if (m_pSecurityZone != NULL)
	{
		delete m_pSecurityZone;
		m_pSecurityZone = NULL;
	}
}

void SceneManager::AddScene(Scene *scene)
{
	m_AllScenes.insert(std::make_pair(scene->GetSceneID(), scene));
	if (scene->IsStaticScene())
	{
		m_listStaticScene.push_back(scene);
	}
}

void SceneManager::RemoveScene(Scene *scene)
{
	m_AllScenes.erase(scene->GetSceneID());
	if (scene->GetSceneState() != SCENE_RUNNING)
	{
		m_listStaticScene.remove(scene);
	}
}

Scene * SceneManager::FindStaticScene(UINT32 sceneTemplateID)
{
	foreach(i in m_listStaticScene)
	{
		Scene *scene = *i;
		if (scene->GetSceneTemplateID() == sceneTemplateID)
		{
			return scene;
		}
	}

	return NULL;
}

void SceneManager::FindByTemplateID(UINT32 sceneTemplateID, std::vector<Scene*>& scenes)
{
	foreach(i in m_AllScenes)
	{
		if (i->second->GetSceneTemplateID() == sceneTemplateID)
		{
			scenes.push_back(i->second);
		}
	}
}

int SceneManager::CheckEnterSceneCondition(Role *poRole, SceneInfo *pSceneInfo, UINT32* arg, UINT64 ownerID)
{
	///> 进入场景的条件
	foreach (cond in m_EnterCondtions)
	{
		int ret =(*cond)->CanEnterScene(poRole, pSceneInfo, arg);
		if (ret != KKSG::ERR_SUCCESS)
		{
			return ret;
		}
	}

	return KKSG::ERR_SUCCESS;
}

int SceneManager::CheckWatcherEnterSceneCondition(Role *poRole, SceneInfo *poInfo)
{
	// 正在Loading场景
	if (poRole->GetLoadingScene() != NULL)
	{
		LogWarn("role is loading scene, can't switch scene");
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

void SceneManager::TakeEnterSceneCost(Role *poRole, SceneInfo *pSceneInfo, bool iswin)
{
	///> 进入场景的消耗
	foreach (cond in m_EnterCondtions)
	{
		(*cond)->TakeEnterSceneCost(poRole, pSceneInfo, iswin);
	}
}

bool SceneManager::ChangeScene(Role *poRole, UINT32 toSceneTemplateID, UINT64 ownerID,UINT32 abyssID)
{
	CSceneSwitch::RoleList oRoleList;
	oRoleList.push_back(poRole);
	KKSG::SceneSwitchData oData;
	oData.set_sceneowner(ownerID);
	KKSG::RoleSceneSwitchExtra* extra = oData.add_roleextralist();
	extra->set_roleid(poRole->GetID());
	extra->set_towerstartfloor(poRole->Get<CTowerRecord>()->GetStartFloor(1));
	extra->set_abyssid(abyssID);
	return CSceneSwitch::ChangeScene(oRoleList, toSceneTemplateID, oData);
}

//bool SceneManager::ChangeScene(Role *poRole, Scene *pScene)
//{
//	int ret = CheckEnterSceneCondition(poRole, pScene->GetSceneInfo());
//	if (ret != KKSG::ERR_SUCCESS)
//	{
//		if (ret != KKSG::ERR_FAILED)
//		{
//			PtcG2C_GSErrorNotify oErrorPtc;
//			oErrorPtc.m_Data.set_errorno(ret);
//			poRole->Send(oErrorPtc);
//		}
//		return false;
//	}

//	EnterScene(poRole, pScene);
//	return true;
//}

void SceneManager::EnterScene(Role *poRole, Scene *pScene, bool isWatchRole/* = false*/)
{
	Scene *currScene = poRole->GetCurrScene();
	if (currScene != NULL)
	{
		currScene->RemoveUnitAndNotify(poRole);
	}

	Scene* loadingScene = poRole->GetLoadingScene();
	if (loadingScene)
	{
		loadingScene->RemoveWaitingRole(poRole);
		poRole->ClearLoadingScene();
	}

	if(isWatchRole)
	{
		poRole->SetWatcher(true);
	}
	else
	{
		poRole->SetWatcher(false);
	}

	pScene->AddUnitAndNotify(poRole);
}

Scene* SceneManager::FindOrCreateScene(UINT32 dwSceneTemplateID, Role *poRole, const KKSG::SceneSwitchData& roSwitchData, UINT32 dwSceneInstanceID)
{
	if(poRole == NULL)
	{
		return NULL;
	}

	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(dwSceneTemplateID);
	if (pSceneInfo == NULL)
	{
		LogError("Invalid scene template id %u", dwSceneTemplateID);
		return NULL;
	}

	Scene* poScene = FindorCreateScene(pSceneInfo, poRole, roSwitchData.sceneowner(), dwSceneInstanceID);
	if(poScene != NULL)
	{
		poScene->InitSceneSwitchData(roSwitchData);
	}
	return poScene;
}

Scene * SceneManager::FindorCreateScene(SceneInfo *poInfo, Role *pRole, UINT64 ownerID, UINT32 dwSceneInstanceID)
{
	if (poInfo->m_pConf->IsStaticScene)
	{
		Scene* pScene = FindStaticScene(poInfo->m_pConf->id);
		if (pScene == NULL)
		{
			SSWarn<<"pScene is NULL, scene template id:"<<poInfo->m_pConf->id<<END;
			pScene = CreateScene(poInfo, dwSceneInstanceID);
		}
		return pScene;
	}
	if (poInfo->m_pConf->type == KKSG::SCENE_GUILD_HALL)
	{
		auto iter = m_allGuildScene.find(pRole->getGuildId()); 
		if (iter == m_allGuildScene.end())
		{
			Scene *pScene = CreateScene(poInfo, dwSceneInstanceID);
			pScene->SetDontDestory(true);
			m_allGuildScene[pRole->getGuildId()] = pScene;
			return pScene;
		}
		else
		{
			return iter->second;
		}
	}
	else
	{
		return CreateScene(poInfo, ownerID, dwSceneInstanceID);
	}
}

void SceneManager::DumpScene(std::string &outputMessage)
{
	std::stringstream ss;
	ss << "Scene All:\n";
	for (auto i = m_AllScenes.begin(); i != m_AllScenes.end(); ++i)
	{
		Scene* scene = i->second;
		ss<<"scene id:"<<scene->GetSceneTemplateID()<<" role count:"<<scene->GetRoleCount()<<" enemy count:"<<scene->GetEnemyCount()<<"\n";
	}

	ss << "\nDump Scene\n static scene:\n";
	ForeachCall(m_listStaticScene, DumpScene, ss);

	ss << "\nBattle Scene:\n";
	foreach(j in m_AllScenes)
	{
		Scene *pScene = j->second;
		if (!pScene->IsStaticScene())
		{
			pScene->DumpScene(ss);
		}
	}

	outputMessage = ss.str();
	LogDebug("%s", outputMessage.c_str());
}


bool SceneManager::CmdLineScene(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() == 1)
	{
		SceneManager::Instance()->DumpScene(outputMessage);
		return true;
	}
	else if (args[1] == "notice")
	{
		if (args.size() >= 3)
		{
			UINT32 priority = 0;
			if (args.size() == 4)
			{
				priority = convert<UINT32>(args[3]);
			}
			NoticeMgr::Instance()->RawNotice(5, args[2], priority);
		}

		return true;
	}
	else
	{
		std::stringstream ss;
		for (unsigned i = 1; i < args.size(); ++i)
		{
			Scene *pScene = Instance()->FindBySceneID(convert<UINT32>(args[i]));
			if (pScene)
			{
				pScene->DumpScene(ss);
			}
		}

		outputMessage = ss.str();
		LogInfo("%s", outputMessage.c_str());
		return true;
	}
}

void SceneManager::CheckScene()
{
	for (auto i = m_AllScenes.begin(); i != m_AllScenes.end();)
	{
		Scene *pScene = i->second;
		if (pScene->CheckDestory())
		{
			//notify ms or world
			if (GSConfig::Instance()->IsCrossGS())
			{
				PtcG2K_CrossSceneDestoryed ntf;
				ntf.m_Data.set_scene_id(pScene->GetSceneID());
				WorldLink::Instance()->SendTo(ntf);
			}
			else
			{
				PtcG2M_SceneDestroyed oPtc;
				oPtc.m_Data.set_sceneid(pScene->GetSceneID());
				MSLink::Instance()->SendTo(oPtc);

				PtcG2N_SceneDestroyedToNs oNtf;
				oNtf.m_Data.set_sceneid(pScene->GetSceneID());
				ControlLink::Instance()->SendTo(oNtf);
			}

			Scene::DestroyScene(pScene);
			m_AllScenes.erase(i++);
		}
		else
		{
			++i;
		}
	}
}

void SceneManager::Update(float deltaTime)
{
	foreach(i in m_AllScenes)
	{
		i->second->Update(deltaTime);
	}
}

Scene * SceneManager::FindBySceneID(UINT32 sceneUID)
{
	auto i = m_AllScenes.find(sceneUID);
	if (i == m_AllScenes.end())
	{
		return NULL;
	}

	return i->second;
}


Scene* SceneManager::CreateScene(UINT32 dwSceneTemplateID, UINT32 dwSceneInstanceID, const KKSG::CreateBattleParam& roParam)
{
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(dwSceneTemplateID);
	if (pSceneInfo == NULL)
	{
		LogError("Scene template id %u is not exist", dwSceneTemplateID);
		return NULL;
	}

	if(pSceneInfo->m_pConf->IsStaticScene)
	{
		LogError("Scene template id %u is static scene, can't create dynamicly", dwSceneTemplateID);
		return NULL;
	}


	Scene* poScene = CreateScene(pSceneInfo, dwSceneInstanceID);
	if(poScene == NULL)
	{
		return NULL;
	}

	LogDebug("Create scene tid [%u] uid [%u] success", dwSceneTemplateID, dwSceneInstanceID);
	poScene->SetDontDestory(true);		//不设置就会被干掉
	poScene->InitBattleParam(roParam);
	return poScene;
}

UINT32 SceneManager::GetHallRoleCount()
{
	for (auto i = m_listStaticScene.begin(); i != m_listStaticScene.end(); ++i)
	{
		Scene* scene = *i;
		if (scene->GetSceneType() == KKSG::SCENE_HALL)
		{
			return scene->GetRoleCount();
		}
	}
	return 0;
}
