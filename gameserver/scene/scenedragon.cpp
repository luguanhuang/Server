#include "pch.h"
#include "scenedragon.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "foreach.h"
#include "unit/role.h"
#include <vector>
#include "table/globalconfig.h"
#include "gamelogic/towerconfig.h"
#include "gamelogic/XLevelWave.h"
#include "foreach.h"
#include "gamelogic/dragonrecord.h"
#include "scene/sceneteam.h"
#include "event/eventmgr.h"
#include "gamelogic/popwindows.h"
#include "gamelogic/dragonconfig.h"
#include "gamelogic/stagemgr.h"
#include "servermgr/servercrossdatamgr.h"
#include "table/expeditionconfigmgr.h"

SceneDragon::SceneDragon(Scene* scene)
{
	m_pScene = scene;
	m_canrevive = false;
	if (!GSConfig::Instance()->IsCrossGS())
	{
		if (DragonConfig::Instance()->IsReviveExcept(scene->GetSceneTemplateID()))
		{
			m_canrevive = scene->GetSceneInfo()->m_pConf->CanRevive;
		}
		else
		{
			m_canrevive = DragonConfig::Instance()->CanLocalRevive(m_pScene->GetSceneTemplateID());
		}
		SSInfo<<"init dragon scene:"<<m_pScene->GetSceneTemplateID()<<" can revive:"<<m_canrevive<<END;
	}
}

SceneDragon* SceneDragon::CreateSceneDragon(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SCENE_DRAGON)
	{
		SceneDragon* dragon = new SceneDragon(scene);
		return dragon;
	}
	return NULL;
}

void SceneDragon::Init(const KKSG::SceneSwitchData& data)
{
	if (data.has_teamdata())
	{
		UINT32 expID = data.teamdata().teamsyndata().expid();
		KKSG::DragonWeakType type = KKSG::DragonWeakType_Null;
		UINT32 refreshTimes = 0;
		DragonConfig::Instance()->CalcDragonWeakBuffInfo(expID, m_buff.buffID, m_buff.buffLevel, type, refreshTimes);

		std::set<UINT32> serverids;
		const KKSG::TeamSynAll& all = data.teamdata().teamsyndata();
		for (int i = 0; i < all.members_size(); ++i)
		{
			serverids.insert(all.members(i).serverid());
		}
		InitReviveData(serverids);
	}
}

void SceneDragon::InitBattle(const KKSG::CreateBattleParam& data)
{
	if (data.has_teamdata())
	{
		std::set<UINT32> serverids;
		const KKSG::TeamSynAll& all = data.teamdata().teamsyndata();
		for (int i = 0; i < all.members_size(); ++i)
		{
			serverids.insert(all.members(i).serverid());
		}
		InitReviveData(serverids);

		_InitCrossWeakBuff(data);
	}
}

void SceneDragon::InitReviveData(std::set<UINT32>& serverids)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		if (DragonConfig::Instance()->IsReviveExcept(m_pScene->GetSceneTemplateID()))
		{
			m_canrevive = m_pScene->GetSceneInfo()->m_pConf->CanRevive;
		}
		else
		{
			m_canrevive = DragonConfig::Instance()->CanCrossRevive(m_pScene->GetSceneTemplateID(), serverids);
		}
		SSInfo<<"init dragon scene:"<<m_pScene->GetSceneTemplateID()<<" can revive:"<<m_canrevive<<END;
	}
}

void SceneDragon::FinishScene(std::vector<Role*>& roles)
{
	SceneTeam* team = m_pScene->GetSceneTeam();
	if (!team)
	{
		return;
	}
	UINT32 expID = team->GetExpID();

	auto data = DragonConfig::Instance()->GetDragonTableData(expID);
	if (!data)
	{
		return;
	}
	if (data->DragonNestDifficulty == 1 || data->DragonNestDifficulty == 0 || !team->HasNextScene())
	{
		foreach(i in roles)
		{
			CDragonRecord* record = (*i)->Get<CDragonRecord>();
			if (record && !m_pScene->IsHelper((*i)->GetID()))
			{
				record->AddFloor(team->GetExpID());

				(*i)->OnThroughDragon();
			}
			(*i)->Get<StageMgr>()->OnAddDne(team->GetExpID());
		}
	}


	// update global
	DragonConfig::Instance()->UpdateGlobalInfo(m_pScene->GetSceneTemplateID());
}

void SceneDragon::ChangeScene()
{
	SceneTeam* pTeam = m_pScene->GetSceneTeam();
	if (!pTeam)
	{
		return;
	}
	if (m_pScene->GetSceneState()== SCENE_WIN)
	{
		const std::list<Role*>& roles = m_pScene->GetAllRoles();
		if (roles.empty())
		{
			return;
		}
		if (pTeam->HasNextScene())
		{
			//有可以切过去的玩家，有下一个场景
			pTeam->ChangeScene();
		}
	}

}

bool SceneDragon::IsFinalScene(bool isWin)
{
	SceneTeam* pTeam = m_pScene->GetSceneTeam();
	if(isWin && pTeam && !pTeam->HasNextScene())
	{
		return true;
	}

	return false;
}

void SceneDragon::AddBuff(Enemy* pEnemy)
{
	if (!pEnemy)
	{
		return;
	}
	if (m_buff.buffID && FightGroupMgr::Instance()->CanAttack(KKSG::FightRole, pEnemy->GetFightGroup()))
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = m_buff.buffID;
		args.singleBuff.BuffLevel = m_buff.buffLevel;

		pEnemy->GetXObject()->DispatchAction(&args);
	}
}

bool SceneDragon::CheckSendBattleResult()
{
	SceneTeam* pTeam = m_pScene->GetSceneTeam();
	if (!pTeam)
	{
		return false;
	}

	UINT32 expID = pTeam->GetExpID();

	auto data = DragonConfig::Instance()->GetDragonTableData(expID);
	if (!data)
	{
		return false;
	}
	if (data->DragonNestDifficulty == 1 || data->DragonNestDifficulty == 0)
	{
		return true;
	}

	if (pTeam->HasNextScene())
	{
		return false;
	}

	if (!m_pScene->IsSceneWinState())
	{
		return false;
	}

	return true;
}

bool SceneDragon::CheckTeleportChangeScene()
{
	if (m_pScene->GetSceneState() != SCENE_WIN)
	{
		return false;
	}

	if (!m_pScene->GetSceneTeam())
	{
		return false;
	}

	UINT32 expID = m_pScene->GetSceneTeam()->GetExpID();

	auto data = DragonConfig::Instance()->GetDragonTableData(expID);
	if (!data)
	{
		return false;
	}
	if (data->DragonNestDifficulty == 1 || data->DragonNestDifficulty == 0)
	{
		if (m_pScene->GetFinishState() != KKSG::SCENE_FINISH_END || time(0) < m_pScene->GetFinishStateExpireTime() - 5)
		{
			return false;
		}

		return true;
	}

	return true;
}

void SceneDragon::_InitCrossWeakBuff(const KKSG::CreateBattleParam& data)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		std::set<UINT32> serverids;
		const KKSG::TeamSynAll& all = data.teamdata().teamsyndata();
		for (int i = 0; i < all.members_size(); ++i)
		{
			serverids.insert(all.members(i).serverid());
		}

		UINT32 expID = data.teamdata().teamsyndata().expid();

		DragonBuffInfo tempBuffInfo;
		UINT32 finalRefreshTimes = 0;
		KKSG::DragonWeakType type = KKSG::DragonWeakType_Null;
		UINT32 refreshTimes = 0;
		UINT32 serverID = 0;
		for (auto iter = serverids.begin(); iter != serverids.end(); ++iter)
		{
			int passTime = 0;
			int openTime = 0;
			KKSG::ServerCrossData* data = ServerCrossDataMgr::Instance()->GetData(*iter);
			if (data && data->has_teamdata() && data->teamdata().has_globaldragoninfo())
			{
				UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(expID);
				for (int i = 0; i < data->teamdata().globaldragoninfo().sceneid_size(); ++i)
				{
					if (sceneID == data->teamdata().globaldragoninfo().sceneid(i))
					{
						if (data->teamdata().globaldragoninfo().timestamp_size() > i)
						{
							passTime = data->teamdata().globaldragoninfo().timestamp(i);
						}
						break;
					}
				}

				for (int i = 0; i < data->teamdata().globaldragoninfo().weakinfos_size(); ++i)
				{
					if (sceneID == data->teamdata().globaldragoninfo().weakinfos(i).sceneid())
					{
						openTime = data->teamdata().globaldragoninfo().weakinfos(i).opentime();
						break;
					}
				}

				refreshTimes = 0;
				DragonConfig::Instance()->CalcDragonWeakBuffInfo(expID, openTime, passTime, tempBuffInfo.buffID, tempBuffInfo.buffLevel, type, refreshTimes);
				if (refreshTimes > finalRefreshTimes)
				{
					finalRefreshTimes = refreshTimes;
					m_buff = tempBuffInfo;
					serverID = *iter;
				}
			}
		}

		if (m_buff.buffID)
		{
			LogInfo("sceneid:%u, serverid:%u, refreshtimes:%u, buffid:%u, bufflevel:%u", m_pScene->GetSceneID(), serverID, finalRefreshTimes, m_buff.buffID, m_buff.buffLevel);
		}
	}
}

