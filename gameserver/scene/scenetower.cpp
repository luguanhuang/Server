#include "pch.h"
#include "scenetower.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "foreach.h"
#include "unit/role.h"
#include <vector>
#include "battle/ptcg2c_guildgoblinkillntf.h"
#include "table/GuildGoblinTable.h"
#include "guild/guildconfig.h"
#include "guild/guildmgr.h"
#include "tower/ptcg2c_towersceneinfontf.h"
#include "gamelogic/towerrecord.h"
#include "table/globalconfig.h"
#include "gamelogic/towerconfig.h"
#include "gamelogic/XLevelWave.h"
#include "foreach.h"
#include "scene/sceneteam.h"
#include "network/mslink.h"
#include "scene/scenemanager.h"
#include "scene/sceneswitch.h"
#include "unit/rolemanager.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "network/mslink.h"
#include "gamelogic/taskrecord.h"

SceneTower::SceneTower(Scene* scene)
{
	m_pScene = scene;
	m_curFloor = 0;
	m_bestTime = 0;
	m_startFloor = 0;
}

void SceneTower::OnEnterScene(Role* role)
{
	SendTowerInfo(role);
}

void SceneTower::SendTowerInfo(Role* role)
{
	if (!role || m_pScene->GetSceneType() != KKSG::SCENE_TOWER)
	{
		return;
	}
	else
	{
		SendTowerInfoV2(role);
		return;
	}

	SceneTeam* team = m_pScene->GetSceneTeam();
	if (!team)
	{
		return;
	}

	PtcG2C_TowerSceneInfoNtf ntf;
	int floor = 0;
	if (m_pScene->GetFinishState() == KKSG::SCENE_FINISH_NONE)
	{
		floor = team->GetTowerInfo().reachTopFloor + 1;
	}
	else
		floor = team->GetTowerInfo().reachTopFloor;
	ntf.m_Data.set_curtowerfloor(floor);
	SceneInfo* info = SceneConfig::Instance()->FindSceneInfo(m_pScene->GetSceneTemplateID());
	if (!info)
	{
		return;
	}
	ntf.m_Data.set_lefttime(0);
	if (m_pScene->GetSpawner())
	{
		int winTime = m_pScene->GetSpawner()->GetWinConditionTime();
		if (!winTime)
		{
			winTime = m_pScene->GetSpawner()->GetLoseConditionTime();
		}
		ntf.m_Data.set_lefttime(m_pScene->CostTime() < winTime ? (int)(winTime - m_pScene->CostTime()) : 0);
	}
	std::map<int, int> reward;
	GetReward(role, reward);
	
	std::map<int, int> rewardShow;
	for (auto iter = reward.begin(); iter != reward.end(); ++iter)
	{
		rewardShow[iter->first] = iter->second;
	}

	if (team)
	{
		std::map<int, int>* syncReward = team->GetSyncSceneFetchReward(role);
		if (syncReward)
		{
			foreach(i in (*syncReward))
			{
				reward[i->first] += i->second;
			}
		}
	}

	for (auto iter = rewardShow.begin(); iter != rewardShow.end(); ++iter)
	{
		//rewardShow[iter->first] = reward[iter->first];
		KKSG::ItemBrief* brief = ntf.m_Data.add_items();
		brief->set_itemid(iter->first);
		brief->set_itemcount(reward[iter->first]);
	}
	
	//ntf.m_Data.set_totalcoin(reward[GOLD]);
	
	role->Send(ntf);
}

SceneTower* SceneTower::CreateSceneTower(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SCENE_TOWER)
	{
		SceneTower* tower = new SceneTower(scene);
		return tower;
	}
	return NULL;
}

void SceneTower::GetReward(Role* role, std::map<int, int>& rewards)
{
	if (!role)
	{
		return;
	}
	else
	{
		GetRewardV2(role, rewards);
	}

	int openHardLevel = GetHardLevel();
	if (!openHardLevel)
	{
		return;
	}

	int floor = role->Get<CTowerRecord>()->GetSweepFloor(openHardLevel);

	SceneTeam* team = m_pScene->GetSceneTeam();
	if (!team)
	{
		return;
	}
	if (!team->FindMember(role->GetID()))
	{
		return;
	}
	if (team->GetTowerInfo().reachTopFloor)
	{
		int reachTopFloor = team->GetTowerInfo().reachTopFloor;
		TowerConfig::Instance()->GetTowerReward(openHardLevel, floor, reachTopFloor, rewards);
	}
}

int SceneTower::GetHardLevel()
{
	int openHardLevel = 0;
	auto sceneIDS =  GetGlobalConfig().TeamTowerSceneID;
	foreach(i in sceneIDS)
	{
		if ((*i).seq[1] == (int)m_pScene->GetSceneTemplateID())
		{
			openHardLevel = (*i).seq[0];
		}
	}

	return openHardLevel;
}

bool SceneTower::IsNewRecord(Role* role)
{
	if (m_pScene->GetSceneType() != KKSG::SCENE_TOWER)
	{
		return false;
	}

	CTowerRecord* record = role->Get<CTowerRecord>();
	if (!record)
	{
		return false;
	}

	SceneTeam* team = m_pScene->GetSceneTeam();
	if (!team)
	{
		return false;
	}

	RoleTowerInfo info;
	info.bestTime = team->GetTowerInfo().bestTime;
	info.reachTopFloor = team->GetTowerInfo().reachTopFloor;
	info.openHardLevel = GetHardLevel();

	if (!info.openHardLevel)
	{
		return false;
	}

	bool isNew = record->CheckIsBestInfo(info);
	if (isNew)
	{
		record->UpdateBestInfo(info);
	}

	return isNew;
}

bool SceneTower::OnChangeRank(/*std::vector<UINT64>& roleIDs*/)
{
	SceneTeam* team = m_pScene->GetSceneTeam();
	if (!team)
	{
		return false;
	}

	//auto & roleIDs = team->GetAllMemberIDs();
	int reachTopFloor = team->GetTowerInfo().reachTopFloor;
	UINT32 bestTime = team->GetTowerInfo().bestTime;
	int openHardLevel = GetHardLevel();

	if (reachTopFloor)
	{
	}

	return true;
}

bool SceneTower::FinishNoFloor(Role* role)
{
	if (role)
	{
		return FinishNoFloorV2(role);
	}

	bool noFloor = false;

	SceneTeam* pTeam = m_pScene->GetSceneTeam();
	if (pTeam)
	{
		if (pTeam->FindMember(role->GetID()))
		{
			noFloor = pTeam->GetTowerInfo().reachTopFloor == 0;
		}
	}

	return noFloor;

}

int SceneTower::GetTopFloor(Role* role)
{
	SceneTeam* pTeam = m_pScene->GetSceneTeam();

	if (pTeam)
	{
		if (pTeam->FindMember(role->GetID()))
		{
			return pTeam->GetTowerInfo().reachTopFloor;
		}
	}
	return 1;
}

void SceneTower::Init(const KKSG::SceneSwitchData& roData)
{
	if (!roData.roleextralist_size())
	{
		return;
	}
	const KKSG::RoleSceneSwitchExtra& data = roData.roleextralist(0);
	Role* role = RoleManager::Instance()->FindByRoleID(data.roleid());
	if (!role)
	{
		return;
	}
	m_startFloor = data.towerstartfloor();
	auto towerInfo = role->Get<CTowerRecord>()->GetTowerInfo(DEFAULT_HARDLEVEL);
	if (towerInfo)
	{
		m_curFloor = towerInfo->curFloor + 1;
		m_bestTime = towerInfo->curCostTime;
	}
	else
	{
		m_curFloor = 1;
		m_bestTime = 0;
	}
	for (int i = 0; i < data.towerreward_size(); ++i)
	{
		m_mapTowerFetchReward[data.towerreward(i).itemid()] = data.towerreward(i).itemcount();
	}
}

void SceneTower::SendTowerInfoV2(Role* role)
{
	if (!role || m_pScene->GetSceneType() != KKSG::SCENE_TOWER)
	{
		return;
	}

	PtcG2C_TowerSceneInfoNtf ntf;
	ntf.m_Data.set_curtowerfloor(m_curFloor);
	SceneInfo* info = SceneConfig::Instance()->FindSceneInfo(m_pScene->GetSceneTemplateID());
	if (!info)
	{
		return;
	}
	ntf.m_Data.set_lefttime(0);
	if (m_pScene->GetSpawner())
	{
		int winTime = m_pScene->GetSpawner()->GetWinConditionTime();
		if (!winTime)
		{
			winTime = m_pScene->GetSpawner()->GetLoseConditionTime();
		}
		ntf.m_Data.set_lefttime(m_pScene->CostTime() < winTime ? (int)(winTime - m_pScene->CostTime()) : 0);
	}
	std::map<int, int> reward;
	GetRewardV2(role, reward);
	
	for (auto iter = m_mapTowerCurFloorFetchReward.begin(); iter != m_mapTowerCurFloorFetchReward.end(); ++iter)
	{
		reward[iter->first] += iter->second;
	}

	for (auto iter = m_mapTowerFetchReward.begin(); iter != m_mapTowerFetchReward.end(); ++iter)
	{
		reward[iter->first] += iter->second;
	}


	for (auto iter = reward.begin(); iter != reward.end(); ++iter)
	{
		//rewardShow[iter->first] = reward[iter->first];
		KKSG::ItemBrief* brief = ntf.m_Data.add_items();
		brief->set_itemid(iter->first);
		brief->set_itemcount(iter->second);
	}
	
	role->Send(ntf);

}

void SceneTower::AddFetchReward(Role* role, int itemID, int count)
{
	if (!role)
	{
		return;
	}

	m_mapTowerCurFloorFetchReward[itemID] += count;
}

void SceneTower::AddHisFetchReward(Role* role, int itemID, int count)
{
	m_mapTowerFetchReward[itemID] += count;
}

bool SceneTower::CheckSendBattleResult(Role* role)
{
	if (IsRoleLeave(role))
	{
		return true;
	}
	if (m_pScene->IsSceneWinState())
	{
		return TowerConfig::Instance()->IsTopestFloor(DEFAULT_HARDLEVEL, m_curFloor);
	}

	return true;
}

const std::map<int, int>* SceneTower::GetCurFloorFetchReward()
{
	return &m_mapTowerCurFloorFetchReward;
}

const std::map<int, int>* SceneTower::GetHisFloorFetchReward()
{
	return &m_mapTowerFetchReward;
}

void SceneTower::GetFloorReward(Role* role, std::map<int, int>& rewards)
{
	auto data = TowerConfig::Instance()->GetTowerTableData(DEFAULT_HARDLEVEL, m_curFloor);
	if (!data)
	{
		return;
	}

	for (auto iter = data->Reward.begin(); iter != data->Reward.end(); ++iter)
	{
		rewards[iter->seq[0]] += iter->seq[1];
	}
}

void SceneTower::GetHisFloorReward(Role* role, std::map<int, int>& rewards)
{
	TowerConfig::Instance()->GetTowerReward(DEFAULT_HARDLEVEL, m_startFloor - 1, m_curFloor - 1, rewards);
}

void SceneTower::OnFinishScene(Role* role)
{
	if (!role)
	{
		return;
	}

	if (m_pScene->GetSceneState() != SCENE_WIN)
	{
		return;
	}

	UpdateCurFloorInfo(role);

	auto data = TowerConfig::Instance()->GetTowerTableData(DEFAULT_HARDLEVEL, m_curFloor + 1);
	if (!data)
	{
		return;
	}

	CSceneSwitch::RoleList oRoleList;
	oRoleList.push_back(role);
	KKSG::SceneSwitchData oData;
	auto roleExtraData = oData.add_roleextralist();
	roleExtraData->set_roleid(role->GetID());
	roleExtraData->set_towerstartfloor(m_startFloor);
	std::map<int, int> total;
	for (auto iter = m_mapTowerCurFloorFetchReward.begin(); iter != m_mapTowerCurFloorFetchReward.end(); ++iter)
	{
		m_mapTowerFetchReward[iter->first] += iter->second;
	}
	for (auto iter = m_mapTowerFetchReward.begin(); iter != m_mapTowerFetchReward.end(); ++iter)
	{
		auto brief = roleExtraData->add_towerreward();
		brief->set_itemid(iter->first);
		brief->set_itemcount(iter->second);
	}

	CSceneSwitch::ChangeScene(oRoleList, data->SceneID, oData);
}

bool SceneTower::UpdateCurFloorInfo(Role* role)
{
	if (m_pScene->GetSceneType() != KKSG::SCENE_TOWER)
	{
		return false;
	}

	CTowerRecord* record = role->Get<CTowerRecord>();
	if (!record)
	{
		return false;
	}

	RoleTowerInfo info;
	info.curCostTime = m_bestTime + m_pScene->CostTime();
	info.curFloor = m_curFloor;
	info.openHardLevel = DEFAULT_HARDLEVEL;

	if (!info.openHardLevel)
	{
		return false;
	}

	record->UpdateCurInfo(info);

	record->Change();

	return true;
}

void SceneTower::GetRewardV2(Role* role, std::map<int, int>& rewards)
{
	if (!role)
	{
		return;
	}

	//int openHardLevel = GetHardLevel();
	int openHardLevel = DEFAULT_HARDLEVEL;
	if (!openHardLevel)
	{
		return;
	}

	//int floor = role->Get<CTowerRecord>()->GetSweepFloor(openHardLevel);
	int floor = m_startFloor;

	if (m_curFloor)
	{
		int reachTopFloor = m_curFloor;
		if (m_pScene->GetSceneState() != SCENE_WIN)
		{
			reachTopFloor = m_curFloor - 1;
		}
		TowerConfig::Instance()->GetTowerReward(openHardLevel, floor - 1, reachTopFloor, rewards);
	}

}

bool SceneTower::FinishNoFloorV2(Role* role)
{
	if ((m_pScene->GetSceneState() == SCENE_LOSE || m_pScene->GetSceneState() == SCENE_RUNNING) && m_curFloor == m_startFloor)
	{
		return true;
	}

	return false;
}

bool SceneTower::OnChangeRankV2(Role* role/*std::vector<UINT64>& roleIDs*/)
{
	if (!m_curFloor || !role)
	{
		return false;
	}

	if (!CheckSendBattleResult(role))
	{
		return false;
	}

	auto record = role->Get<CTowerRecord>();
	auto towerInfo = record->GetTowerInfo(DEFAULT_HARDLEVEL);
	if (!towerInfo)
	{
		return false;
	}
	RoleTowerInfo info;
	info.openHardLevel = DEFAULT_HARDLEVEL;
	info.curCostTime = towerInfo->curCostTime;
	info.curFloor = towerInfo->curFloor;
	if (record->CheckIsBestInfo(info))
	{
		record->UpdateBestInfo(info);
	}

	return true;
}

bool SceneTower::IsNewRecordV2(Role* role)
{
	auto record = role->Get<CTowerRecord>();
	auto towerInfo = record->GetTowerInfo(DEFAULT_HARDLEVEL);
	if (!towerInfo)
	{
		return false;
	}
	RoleTowerInfo info;
	info.openHardLevel = DEFAULT_HARDLEVEL;
	info.curCostTime = towerInfo->curCostTime;
	info.curFloor = towerInfo->curFloor;
	if (record->CheckIsBestInfo(info))
		return true;

	return false;
}

int SceneTower::GetFinalReachFloor(Role* role)
{
	if (!m_curFloor)
	{
		return 0;
	}

	if (!CheckSendBattleResult(role))
	{
		return 0;
	}

	if (m_pScene->GetSceneState() == SCENE_WIN)
	{
		return m_curFloor;
	}
	else
		return m_curFloor - 1;

}

int SceneTower::GetFinalReachTime(Role* role)
{
	if (!m_curFloor)
	{
		return 0;
	}

	if (!CheckSendBattleResult(role))
	{
		return 0;
	}

	if (m_pScene->GetSceneState() == SCENE_WIN)
	{
		return m_bestTime + m_pScene->CostTime();
	}
	else
		return m_bestTime;

}

void SceneTower::AddLeaveRole(Role* role)
{
	m_leaveRole[role] = 1;
}

bool SceneTower::IsRoleLeave(Role* role)
{
	auto find = m_leaveRole.find(role);
	if (find != m_leaveRole.end())
	{
		return true;
	}

	return false;
}

bool SceneTower::CheckWinDelayEnd()
{
	return !m_leaveRole.empty();
}

void SceneTower::OnEnemyDie(Scene* pScene, Enemy* enemy)
{
	const std::list<Role*>& vecRole = m_pScene->GetAllRoles();
	for (auto i = vecRole.begin(); i != vecRole.end(); ++i)
	{
		Role* pRole = (*i);
		pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_Tower);
	}
}
