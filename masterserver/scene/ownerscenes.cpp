#include "pch.h"
#include "commondef.h"
#include "ownerscenes.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "network/gslink.h"
#include "garden/garden.h"
#include "garden/garden_manager.h"
#include "garden/garden_utility.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "scene/sceneswitch.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "scene/ptcm2g_msdestoryscene.h"

OwnerScene::OwnerScene()
{
	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);	
}

OwnerScene::~OwnerScene()
{
	if (INVALID_HTIMER != m_timehandler)
	{
		CTimerMgr::Instance()->KillTimer(m_timehandler);
	}
}

void OwnerScene::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 nowtime = (UINT32)time(NULL);
	for (auto i = m_uid2scene.begin(); i != m_uid2scene.end();)
	{
		if (nowtime >= i->second.delayclose)
		{
			SSInfo<<"scene empty close scene:"<<i->second.sceneid<<END;
			// notify gs
			CScene* scene = CSceneManager::Instance()->GetScene(i->second.sceneid);
			if (NULL != scene)
			{
				PtcM2G_MSDestoryScene ptc;
				ptc.m_Data.set_sceneuid(i->second.sceneid);
				GSLink::Instance()->SendToLine(scene->GetLine(), ptc);
			}
			else
			{
				SSWarn<<"not find sceneid:"<<i->second.sceneid<<END;
			}

			auto j = m_sceneid2uid.find(i->second.sceneid);
			if (j != m_sceneid2uid.end())
			{
				m_sceneid2uid.erase(j);
			}
			else
			{
				SSWarn<<"not find sceneid:"<<i->second.sceneid<<END;
			}
			m_uid2scene.erase(i++);	
		}
		else
		{
			++i;
		}
	}
}

OneOwnerScene* OwnerScene::FindScene(UINT64 uid)
{
	auto i = m_uid2scene.find(uid);
	if (i == m_uid2scene.end())
	{
		return NULL;
	}
	return &i->second;
}

//bool OwnerScene::HasPrepareRole(UINT64 uid)
//{
//	auto i = m_uid2scene.find(uid);
//	if (i == m_uid2scene.end())
//	{
//		return false;
//	}
//	return !i->second.prepares.empty();
//}

//void OwnerScene::PushPrepareRole(UINT64 uid, UINT64 roleid)
//{
//	m_uid2scene[uid].prepares.push_back(roleid);
//	SSInfo<<"push prepare scene own:"<<uid<<" role:"<<roleid<<END;
//}

void OwnerScene::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT32 nErrCode = res.result();
	if (NULL == scene || nErrCode != KKSG::ERR_SUCCESS)
	{
		SSError<<"create scene failed, errorcode:"<<nErrCode<<END;
		return;
	}
	UINT64 uid = roParam.owner().uid();
	auto i = m_uid2scene.find(uid);
	if (i == m_uid2scene.end())
	{
		SSError<<"enter scene failed, uid:"<<uid<<END;
		return;
	}

	for (auto j = i->second.prepares.begin(); j != i->second.prepares.end(); ++j)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(*j);
		if (NULL == role)
		{
			continue;
		}

		KKSG::SceneSwitchData data;
		CSceneSwitch::EnterScene(role, scene->GetID(), data);
		LogInfo("Role [%llu] begin to change gs_%u to gs_%u", role->GetID(), role->GetGsLine(), scene->GetLine());
	}
	i->second.prepares.clear();
	i->second.sceneid = scene->GetID();
	
	std::pair<std::map<UINT32, UINT64>::iterator, bool> ret = m_sceneid2uid.insert(std::make_pair(scene->GetID(), i->first));
	if (!ret.second)
	{
		SSWarn<<"sceneid exist, id:"<<scene->GetID()<<END;
	}
} 

void OwnerScene::RemoveScene(UINT32 sceneuid)
{
	auto i = m_sceneid2uid.find(sceneuid);
	if (i != m_sceneid2uid.end())
	{
		auto j = m_uid2scene.find(i->second);
		if (j == m_uid2scene.end())
		{
			SSWarn<<"not find scene, uid:"<<i->second<<END;
		}
		else
		{
			if (!j->second.prepares.empty())
			{
				SSError<<"remove prepare role, count:"<<j->second.prepares.size()<<END;
				j->second.prepares.clear();
			}
			m_uid2scene.erase(j);
			SSInfo<<"remove own scene:"<<sceneuid<<" ownerid:"<<i->second<<END;
		}
		m_sceneid2uid.erase(i);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OwnerScenes::OwnerScenes()
{

}

OwnerScenes::~OwnerScenes()
{
	for (auto i = m_type2scenes.begin(); i!= m_type2scenes.end(); ++i)
	{
		if (NULL != i->second)
		{
			delete i->second;
			i->second = NULL;
		}
	}
	m_type2scenes.clear();
}

OneOwnerScene* OwnerScenes::FindScene(UINT32 type, UINT64 uid)
{
	auto i = m_type2scenes.find(type);
	if (i == m_type2scenes.end())
	{
		return NULL;
	}
	else
	{
		return i->second->FindScene(uid);
	}
}

OneOwnerScene* OwnerScenes::FindSceneBySceneId(UINT32 type, UINT32 sceneid)
{
	auto i = m_type2scenes.find(type);
	if (i == m_type2scenes.end())
	{
		return NULL;
	}
	else
	{
		auto j = i->second->m_sceneid2uid.find(sceneid);
		if (j == i->second->m_sceneid2uid.end())
		{
			return NULL;
		}
		else
		{
			return i->second->FindScene(j->second);
		}
	}
}

void OwnerScenes::EnterScene(UINT32 type, UINT64 uid, UINT64 roleid)
{
	OwnerScene* ownerscene = NULL;
	auto i = m_type2scenes.find(type);
	if (i == m_type2scenes.end())
	{
		ownerscene = new OwnerScene;
		m_type2scenes[type] = ownerscene;
	}
	else
	{
		ownerscene = i->second;
	}

	///> 场景不存在，创建场景
	if (ownerscene->m_uid2scene[uid].prepares.empty())	
	{
		UINT32 destline = INVALID_LINE_ID;
		// create scene 
		KKSG::CreateBattleParam param;
		// init garden info
		if (type == FAMILYGARDEN_MAP_ID)
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(uid);
			if (NULL != summary)
			{
				Garden* garden = GardenManager::Instance()->GetGarden(uid, summary->GetName());
				if (NULL != garden)
				{
					std::vector<GardenPlantStatus> plants;
					garden->GetPlantInfo(plants);
					for (auto i = plants.begin(); i != plants.end(); ++i)
					{
						KKSG::SceneGardenSlot* slot = param.mutable_gardeninfo()->add_plants();
						slot->set_index(i->farmland_id);
						slot->set_plantid(i->sprite_id);
						slot->set_buffid(i->buff_id);
					}
				}
				else
				{
					return;
				}
			}
		}
		else if (type == GUILD_MAP_ID)
		{
			destline = MAIN_HALL_GS_LINE;

			Guild *pGuild  = CGuildMgr::Instance()->GetGuild(uid);
			if (pGuild != NULL)
			{
				GuildGarden* guild_garden = GardenUtility::Instance()->GetGuildGarden(pGuild->GetID());
				if (NULL == guild_garden)
				{
					LogWarn("not find the guild garden,role id is : %llu",roleid);						
					return ;
				}
				std::vector<GardenPlantStatus> plants;
				guild_garden->GetPlantInfo(plants);
				for (auto i = plants.begin(); i != plants.end(); ++i)
				{
					KKSG::SceneGardenSlot* slot = param.mutable_gardeninfo()->add_plants();
					slot->set_index(i->farmland_id);
					slot->set_plantid(i->sprite_id);
					slot->set_buffid(i->buff_id);
				}
			}
		}

		KKSG::SceneOwnerInfo* info = param.mutable_owner();
		info->set_uid(uid);
		CSceneCreator::Instance()->CreateBattleScene(type, ownerscene, param, destline); 
	}
	// push
	ownerscene->m_uid2scene[uid].prepares.push_back(roleid);
	ownerscene->m_uid2scene[uid].ResetState();
}

void OwnerScenes::RemoveScene(UINT32 scenetype, UINT32 sceneuid)
{
	auto i = m_type2scenes.find(scenetype);
	if (i != m_type2scenes.end())
	{
		i->second->RemoveScene(sceneuid);
	}
}

void OwnerScenes::RecoverScene(UINT64 ownerid, UINT32 mapid, UINT32 sceneid)
{
	if (mapid == GUILD_MAP_ID || mapid == FAMILYGARDEN_MAP_ID)
	{
		OwnerScene* temp = NULL;
		auto i = m_type2scenes.find(mapid);
		if (i == m_type2scenes.end())
		{
			temp = new OwnerScene();
			m_type2scenes[mapid] = temp;
		}
		else
		{
			temp = i->second;
		}
		OneOwnerScene onescene;
		onescene.sceneid = sceneid;
		temp->m_uid2scene[ownerid] = onescene;
		temp->m_sceneid2uid[sceneid] = ownerid;
		SSInfo<<"recover scene, mapid:"<<mapid<<" ownerid:"<<ownerid<<" sceneid:"<<sceneid<<END;
	}
}

UINT32 OwnerScenes::GetSceneCount(UINT32 mapid)
{
	auto i = m_type2scenes.find(mapid);
	if (i != m_type2scenes.end())
	{
		return i->second->m_uid2scene.size();
	}
	return 0;
}
