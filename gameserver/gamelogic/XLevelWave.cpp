#include "pch.h"
#include "XLevelWave.h"
#include "util.h"
#include "unit/enemy.h"
#include "XLevelSpawnMgr.h"
#include "unit/enemymanager.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "scene/scene.h"
#include "scene/scenefinder.h"
#include "math/XVector3.h"
#include "unit/role.h"
#include "XLevelScriptMgr.h"
#include "gamelogic/randombossmgr.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "foreach.h"
#include "scene/ptcg2c_executelevelscriptntf.h"
#include "unit/rolemanager.h"
#include "entity/XRole.h"
#include "table/CutSceneReader.h"
#include "robotgenerator.h"
#include "util/XRandom.h"
#include "unit/dummyrole.h"
#include "unit/dummyrolemanager.h"
#include "scene/scenemanager.h"
#include "bossrushmgr.h"
#include "scene/sceneteam.h"
#include "scene/scenebossrush.h"
#include "battle/ptcg2c_levelscriptstatentf.h"
#include "scene/teleportdoor.h"
#include "common/ptcg2c_gserrornotify.h"


using namespace std;

void XLevelWave::ReadFromeFile(std::stringstream& ss)
{
	string strLine;
	std::getline(ss, strLine);
	if (strLine.find("bw") != 0) return;

	while (true)
	{
		std::getline(ss, strLine);

		if (strLine.find("ew") == 0) break;

		ParseInfo(strLine);
	}
}

void XLevelState::Reset()
{
	m_current_scene_id = 0;
	m_total_monster = 0;
	m_total_kill = 0;
	m_total_kill_levelspawn = 0;
	m_isBossDie = false;
	m_refuseRevive = false;
	m_bossNum = 0;
	m_bossDieNum = 0;
	m_targetRoleID = 0;

	m_my_team_alive = 1;     // 应该是当前小队的人数
	m_op_team_alives = 0;
}

void XLevelState::SetExternalString(const std::string& str)
{
	//std::vector<std::string>::iterator iter = m_exstring.begin();
	auto find = m_exstring.find(str);
	if (find != m_exstring.end())
	{
		++find->second.count;
		return;
	}

	XExtString tempExtString(str);
	m_exstring.insert(std::make_pair(str, tempExtString)); 
}

bool XLevelState::QueryExternalString(std::string& str, bool decCount)
{
	bool bFind = false;
	auto find = m_exstring.find(str);
	if (find != m_exstring.end())
	{
		bFind = true;
	}

	if(bFind)
	{
		if (!find->second.count)
		{
			bFind = false;
		}
		if (bFind && decCount)
		{
			--find->second.count;
		}
		//m_exstring.erase(find);
	}

	return bFind;
}

void XLevelState::AddBossDieNum(int num)
{
	m_bossDieNum += num;
}

bool XLevelState::IsBossAllDie()
{
	return m_bossDieNum >= m_bossNum;
}

void XLevelState::SetTargetRoleID(UINT64 roleID)
{
	m_targetRoleID = roleID;
}

UINT64 XLevelState::GetTargetRoleID()
{
	return m_targetRoleID;
}

XLevelCallBack::XLevelCallBack(XLevelSpawner* spawner)
{
	m_pSpawner = spawner;
	m_callBackNum = m_pSpawner->GetLevelCallBackNum();
}

XLevelScriptCallBack::XLevelScriptCallBack(XLevelSpawner* pSpawner):XLevelCallBack(pSpawner)
{
}

void XLevelScriptCallBack::operator()()
{
	m_pSpawner->GenerateCallBackScriptTask(this);
}

void XLevelScriptCallBack::SetCallBackFunc(const std::string& callback)
{
	m_callbackFunc = callback;
}

std::vector<std::string> SplitNotEscape(const std::string &s, const char sep)
{
	int start = 0, end = 0;
	std::vector<std::string> vec;
	std::string str("");
	for (UINT32 i = 0; i < s.length(); ++i)
	{
		if (s.at(i) == (sep))
		{
			end = i;

			if (start == end)
			{
				str.clear();
				vec.push_back(str);
			}
			else if (end > start)
			{
				str = s.substr(start, (end - start));

				vec.push_back(str);
				str.clear();
			}

			++end;
			start = end;
			if (i == (s.length() - 1) && s.at(i) == sep)
			{
				str.clear();
				vec.push_back(str);
			}
		}
		else if (i == (s.length() - 1))
		{
			str = s.substr(start, i - start + 1);
			vec.push_back(str);
		}

	}
	return vec;
}

void XLevelWave::ParseInfo(const std::string& data)
{
	LevelInfoType type = TYPE_NONE;
	if (data.find("id") == 0) type = TYPE_ID;
	else if (data.find("bi")== 0) type = TYPE_BASEINFO;
	else if (data.find("pw")== 0) type = TYPE_PREWAVE;
	else if (data.find("ei")== 0) type = TYPE_EDITOR;
	else if (data.find("mi")== 0) type = TYPE_MONSTERINFO;
	else if (data.find("si")== 0) type = TYPE_SCRIPT;
	else if (data.find("es")== 0) type = TYPE_EXSTRING;
	else if (data.find("st")== 0) type = TYPE_SPAWNTYPE;

	int offset = data[data.size() - 1] == '\r' ? 1 : 0;
	string rawData = data.substr(3, data.size()-3-offset);
	//rawData[rawData.size()-1] = 0; // 去掉行尾的 '\n'

	switch (type)
	{
	case TYPE_NONE:
		{
			break;
		}
	case TYPE_ID:
		{
			m_Id = atoi(rawData.c_str());
		}
		break;
	case TYPE_SPAWNTYPE:
		{
			m_SpawnType = (LevelSpawnType)(atoi(rawData.c_str()));
		}
		break;
	case TYPE_BASEINFO:
		{
			vector<string> strInfos = SplitNotEscape(rawData, ',');

			//m_Time = atoi(strInfos[0].c_str());
			m_Time = (int)(convert<float>(strInfos[0].c_str()) * 1000);
			m_LoopInterval = atoi(strInfos[1].c_str());

			m_EnemyID = atoi(strInfos[2].c_str());

			m_YRotate = atoi(strInfos[5].c_str());

			if(strInfos.size() > 6)
				m_RoundRidus = (float)atof(strInfos[6].c_str());

			if(strInfos.size() > 7)
				m_RoundCount = atoi(strInfos[7].c_str());

			if(strInfos.size() > 8)
			{
				m_RandomID = atoi(strInfos[8].c_str());
				if(Spawn_Source_Player == m_SpawnType && m_RandomID)//深渊PVP的特殊处理
				{
					m_RobotLookupID = m_RandomID;
					m_RandomID = 0;
				}
			}

			if(strInfos.size() > 9)
				//m_DoodadID = atoi(strInfos[9].c_str());
			{
				vector<string> ids = Split(strInfos[9],'|');
				for (size_t i = 0; i < ids.size(); ++i)
				{
					int doodadID = atoi(ids[i].c_str());
					if (doodadID)
						m_DoodadID.push_back(doodadID);
				}
			}

			if(strInfos.size() > 10)
			{
				int percent = atoi(strInfos[10].c_str());
				m_DoodadPercent = percent / 100.0f;
			}
			if(strInfos.size() > 11)
			{
				m_Repeat = strcmp(strInfos[11].c_str(), "True") == 0 ? 1 : 0;
			}
		}
		break;
	case TYPE_PREWAVE:
		{
			vector<string> strInfos = Split(rawData, '|');

			if(strInfos.size() > 0)
			{
				string strPreWave = strInfos[0];

				if (!strPreWave.empty())
				{
					vector<string> strPreWaves = Split(strPreWave, ',');

					for (size_t i = 0; i < strPreWaves.size(); i++)
					{
						int preWave = atoi(strPreWaves[i].c_str());

						m_PreWaves.push_back(preWave);
					}
				}
			}

			if(strInfos.size() > 1)
			{
				int percent = atoi(strInfos[1].c_str());
				m_PreWavePercent = percent / 100.0f;
			}
		}
		break;
	case TYPE_EDITOR:
		break;
	case TYPE_MONSTERINFO:
		{
			vector<string> strFloats = Split(rawData, ',');

			int index = atoi(strFloats[0].c_str());

			// generate game object in scene
			float x = (float)atof(strFloats[1].c_str());
			float y = (float)atof(strFloats[2].c_str());
			float z = (float)atof(strFloats[3].c_str());

			// m_Monsters.insert(make_pair<int, Vector3>(index, Vector3(x, y, z)));//change by lgh
			m_Monsters[index]=Vector3(x, y, z);
			// m_MonsterRotation.insert(make_pair<int, float>(index, (float)atof(strFloats[4].c_str())));change by lgh
			m_MonsterRotation[index]=(float)atof(strFloats[4].c_str());
		}
		break;
	case TYPE_SCRIPT:
		{
			m_Levelscript = rawData;
		}
				break;
	case TYPE_EXSTRING:
		{
			m_ExString = rawData;
		}
		break;
	
	default:
		break;
	}
}

XLevelDynamicInfo::XLevelDynamicInfo()
{
	m_Id = 0;
	m_TotalCount = 0;
	Reset();
}

void XLevelDynamicInfo::Reset()
{
	m_PushInfoTask = false;
	m_GenerateTime = 0;
	m_GenerateCount = 0;
	m_DieCount = 0;
	m_PreWaveFinishTime = 0;
	m_ExstringFinishTime = 0;
	m_RandomizedEnemyID = 0;

	m_enemyIds.clear();
	m_enemyIds.clear();
	
}

void XLevelDynamicInfo::Replace(UINT64 lastID, UINT64 newID)
{
	foreach(i in m_enemyIds)
	{
		if (*i == lastID)
		{
			*i = newID;
			break;
		}
	}
}

void XLevelDynamicInfo::ReplaceRandomizedEnemyID(int newID)
{
	if (m_RandomizedEnemyID)
	{
		m_RandomizedEnemyID = newID;
	}
}

bool XLevelDynamicInfo::IsSpawned(UINT64 enemyID)
{
	bool find = false;
	for (auto iter = m_enemyIds.begin(); iter != m_enemyIds.end(); ++iter)
	{
		if (*iter == enemyID)
		{
			find = true;
			break;
		}
	}

	return find;
}

XLevelWaveInfo::~XLevelWaveInfo()
{
	m_preLevelWaveInfos.clear();
}

void XLevelSpawner::UnInit()
{
	//m_wavesDynamicInfo.clear();
	m_waveInfos.clear();
	
	while(!m_tasks.empty())
	{
		XLevelBaseTask* pTask = m_tasks.front();
		m_tasks.pop();

		delete pTask;
	}

	while(!m_roleMissingTasks.empty())
	{
		XLevelBaseTask* pTask = m_roleMissingTasks.front();
		m_roleMissingTasks.pop();

		delete pTask;
	}

	foreach(i in m_callBackList)
	{
		delete i->second;
	}

	m_callBackList.clear();

	/*
	for(size_t i = 0; i < m_robotSummarys.size(); ++i)
	{
		RobotGenerator::Instance()->FreeRoleSummary(m_robotSummarys[i]);		
	}
	m_robotSummarys.clear();
	*/
}

void XLevelSpawner::Init()
{
	if(!m_dwSceneTemplateID) return;

	std::vector<XLevelWave*> allWave;
	if(!XLevelSpawnMgr::Instance()->GetWaveInfo(m_dwSceneTemplateID, allWave)) return;

	int levelMonsterCount = 0;
	m_oLevelState.Reset();
	for(size_t i = 0; i < allWave.size(); i++)
	{
		m_waveInfos[allWave[i]->m_Id].m_staticLevelWaveInfo = *(allWave[i]);
		XLevelDynamicInfo& dInfo = m_waveInfos[allWave[i]->m_Id].m_dynamicWaveInfo;

		//XLevelDynamicInfo dInfo;
		dInfo.m_Id = allWave[i]->m_Id;
		dInfo.m_TotalCount = (int)allWave[i]->m_Monsters.size() + allWave[i]->m_RoundCount;

		//m_wavesDynamicInfo.insert(make_pair<int, XLevelDynamicInfo>(allWave[i]->m_Id, dInfo));

		auto info = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(allWave[i]->m_EnemyID);
		if (allWave[i]->m_RandomID)
		{
			int enemyID = RandomBossMgr::Instance()->GetRandomBoss(allWave[i]->m_RandomID);
			//m_wavesDynamicInfo[allWave[i]->m_Id].m_RandomizedEnemyID = enemyID;
			dInfo.m_RandomizedEnemyID = enemyID;
			info = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(enemyID);
		}

		if (allWave[i]->m_SpawnType != Spawn_Source_Doodad && info && info->Fightgroup == KKSG::FightEnemy && info->Type != KKSG::Species_Puppet)
		{
			levelMonsterCount += dInfo.m_TotalCount;

			if (info->Type == KKSG::Species_Boss)
			{
				m_oLevelState.m_bossNum += dInfo.m_TotalCount;
			}
		}

		/* 战火
		//pvp刷对手机器人
		if(Spawn_Source_Player == allWave[i]->m_SpawnType &&
			allWave[i]->m_RobotLookupID && !allWave[i]->m_ExString.empty())
		{
			m_robotExString.push_back(allWave[i]->m_ExString);
		}
		*/

	}
	for (auto iter = m_waveInfos.begin(); iter != m_waveInfos.end(); ++iter)
	{
		for(auto j = iter->second.m_staticLevelWaveInfo.m_PreWaves.begin(); j != iter->second.m_staticLevelWaveInfo.m_PreWaves.end(); ++j)
		{
			auto find = m_waveInfos.find(*j);
			if (find != m_waveInfos.end())
			{
				// iter->second.m_preLevelWaveInfos.insert(std::make_pair<int, XLevelWaveInfo*>(*j, &(find->second)));lgh
				iter->second.m_preLevelWaveInfos[*j]=&(find->second);
			}
			else
			{
				LogError("init error,scene:%u,%u,waveid:%d ", m_pScene->GetSceneID(), m_pScene->GetSceneTemplateID(), *j);
			}
		}
	}

	m_oLevelState.m_current_scene_id = m_pScene->GetSceneID();
	m_oLevelState.m_total_monster = levelMonsterCount;
	m_levelScript.Init(this);

	SetupFinishCondition();
}

void XLevelSpawner::SetupFinishCondition()
{
	m_WinConditon.clear();
	m_LoseCondition.clear();
	for(size_t i = 0; i < m_pScene->GetSceneInfo()->m_pConf->WinCondition.size(); i++)
	{
		XLevelWin w;
		w.cond = (XLevelWinCondition)m_pScene->GetSceneInfo()->m_pConf->WinCondition[i][0];
		w.param = m_pScene->GetSceneInfo()->m_pConf->WinCondition[i][1];

		m_WinConditon.push_back(w);
	}

	for(size_t i = 0; i < m_pScene->GetSceneInfo()->m_pConf->LoseCondition.size(); i++)
	{
		XLevelLose w;
		w.cond = (XLevelLoseCondtion)m_pScene->GetSceneInfo()->m_pConf->LoseCondition[i][0];
		w.param = m_pScene->GetSceneInfo()->m_pConf->LoseCondition[i][1];

		m_LoseCondition.push_back(w);
	}
}

void XLevelSpawner::ResetDynamicInfo()
{
	/*for(std::map<int, XLevelDynamicInfo>::iterator it = m_wavesDynamicInfo.begin(); it != m_wavesDynamicInfo.end(); it++)
	{
		(it->second).Reset();
	}*/
	for (auto iter = m_waveInfos.begin(); iter != m_waveInfos.end(); ++iter)
	{
		iter->second.m_dynamicWaveInfo.Reset();
	}
}

void XLevelSpawner::OnMonsterDie(Enemy *pEnemy)
{
	//std::map<int, XLevelDynamicInfo>::iterator it = m_wavesDynamicInfo.find(pEnemy->GetWaveID());
	std::map<int, XLevelWaveInfo>::iterator it = m_waveInfos.find(pEnemy->GetWaveID());

	if(it != m_waveInfos.end())
	{
		std::vector<UINT64>& enemyIDs = it->second.m_dynamicWaveInfo.m_enemyIds;
		bool find = it->second.m_dynamicWaveInfo.IsSpawned(pEnemy->GetID());
		if (find)
		{
			(it->second).m_dynamicWaveInfo.m_DieCount += 1;
			if ((it->second).m_dynamicWaveInfo.m_DieCount <= it->second.m_dynamicWaveInfo.m_TotalCount)
			{
				if(pEnemy->GetCurrScene())
					pEnemy->GetCurrScene()->GenerateEnemyDroppedDoodad(pEnemy, (it->second).m_dynamicWaveInfo.m_DieCount == it->second.m_dynamicWaveInfo.m_TotalCount);
			}

			if (!pEnemy->IsPuppet())
			{
				m_oLevelState.m_total_kill_levelspawn += 1;
			}
		}
	}

	if (pEnemy->IsPuppet())
	{
		return;
	}
	m_oLevelState.m_total_kill += 1;

	if(pEnemy->IsBoss())
	{
		//m_oLevelState.m_isBossDie = true;
		m_oLevelState.AddBossDieNum(1);
	}

	++m_oLevelState.m_killEnemyNum[pEnemy->GetTemplateID()];
	
}

void XLevelSpawner::OnDummyRoleDie( DummyRole *pDummy )
{
	//auto it = m_wavesDynamicInfo.find(pDummy->GetWaveID());
	auto it = m_waveInfos.find(pDummy->GetWaveID());

	if(it != m_waveInfos.end())
	{
		(it->second).m_dynamicWaveInfo.m_DieCount += 1;
	}

}

void XLevelSpawner::OnDoodadFetch(int waveID)
{
	/*XLevelWave* wave = XLevelSpawnMgr::Instance()->GetWaveInfo(m_pScene->GetSceneTemplateID(), waveID);
	if (!wave || wave->m_SpawnType != Spawn_Source_Doodad)
	{
		return;
	}*/

	//std::map<int, XLevelDynamicInfo>::iterator it = m_wavesDynamicInfo.find(waveID);
	std::map<int, XLevelWaveInfo>::iterator it = m_waveInfos.find(waveID);

	if(it == m_waveInfos.end())
	{
		return;
	}
	if (it->second.m_staticLevelWaveInfo.m_SpawnType != Spawn_Source_Doodad)
	{
		return;
	}

	(it->second).m_dynamicWaveInfo.m_DieCount += 1;
}

void XLevelSpawner::OnEnterScene(Role* role)
{
	SyncScriptState(role);
}

void XLevelSpawner::AddRestTime(UINT32 time)
{
	if (!m_executeTimes)
	{
		m_fRestTime += time;
	}
}

void XLevelSpawner::ResetRestTime()
{
	if (!m_executeTimes)
	{
		m_fRestTime = 0.0f;
	}
}

bool XLevelSpawner::IsLevelWin()
{
	bool bCanWin = false;

	for(size_t i = 0; i < m_WinConditon.size(); i++)
	{
		switch(m_WinConditon[i].cond)
		{
			case LevelWin_Boss:
				bCanWin = m_oLevelState.IsBossAllDie();
				//bCanWin = m_oLevelState.m_isBossDie;
				break;
			case LevelWin_Killall:
				if(m_oLevelState.m_total_monster > 0 && m_oLevelState.m_total_monster == m_oLevelState.m_total_kill_levelspawn)
					bCanWin = true;
				break;
			case LevelWin_Kill_N:
				if(m_oLevelState.m_total_kill >= m_WinConditon[i].param)
					bCanWin = true;
				break;
			case LevelWin_PVP:
				break;
			case LevelWin_Time:
				if (m_pScene->CostTime() >= m_WinConditon[i].param)
				{
					bCanWin = true;
				}
				break;
			case LevelWin_BossRush:
				if (m_pScene->GetBRHandler())
				{
					if (m_pScene->GetBRHandler()->IsWin())
					{
						bCanWin = true;
					}
				}
				break;
			case LevelWin_TeamDie:
				{
					if (m_pScene->CostTime() == -1)
					{
						break;
					}
					const std::list<Role*>& roles = m_pScene->GetAllRoles();
					bCanWin = true;
					foreach(i in roles)
					{
						if (!(*i)->IsDead() || (*i)->CanRoleRevive())
						{
							bCanWin = false;
							break;
						}
					}
				}
				break;
			case LevelWin_KillEnemyScore:
				if (m_pScene->GetKillEnemyScore() >= m_WinConditon[i].param)
				{
					bCanWin = true;
				}
				break;
			default:
				break;
		}

		if(bCanWin) break;
	}

	return bCanWin;
}

bool XLevelSpawner::IsLevelLose()
{
	bool bCanLoose = false;

	SceneInfo* data = m_pScene->GetSceneInfo();
	if (!(data && data->m_pConf))
	{
		return true;
	}
	for(size_t i = 0; i < m_LoseCondition.size(); i++)
	{
		switch(m_LoseCondition[i].cond)
		{
		case LevelLose_PlayerDie:
			{
				const std::list<Role*>& roles = m_pScene->GetAllRoles();
				if(roles.empty())
				{
					break;
				}
				else if((*roles.begin())->IsDead() && !(*roles.begin())->CanRoleRevive())
				{
					bCanLoose = true;
				}
			}
			break;
		case LevelLose_TeamDie:
			{
				if (m_pScene->CostTime() == -1)
				{
					break;
				}
				const std::list<Role*>& roles = m_pScene->GetAllRoles();
				bCanLoose = true;
				for (auto i = roles.begin(); i != roles.end(); ++i)
				{
					if (!(*i)->IsDead() || (*i)->CanRoleRevive())
					{
						bCanLoose = false;
						break;
					}
				}
			}
			break;
		case LevelTime_Out:
			if (m_pScene->CostTime() > m_LoseCondition[i].param)
			{
				bCanLoose = true;
			}
			break;
		case LevelLose_NpcDie:
			if (m_oLevelState.m_killEnemyNum[m_LoseCondition[i].param])
			{
				bCanLoose = true;
			}
			break;
		case LevelLose_LeaderDie:
			{
				SceneTeam* pTeam = m_pScene->GetSceneTeam();
				if (!pTeam)
				{
					break;
				}
				UINT64 leaderId = pTeam->GetLeaderID();
				Role* role = RoleManager::Instance()->FindByRoleID(leaderId);
				if (!role)
				{
					break;
				}
				if (role->IsDead() && !role->CanRoleRevive())
				{
					bCanLoose = true;
				}
			}
			break;
		default:
			break;
		}

		if(bCanLoose) break;
	}
	
	return bCanLoose;
}

XLevelWaveInfo* XLevelSpawner::GetLevelWaveInfo(int dwWaveID)
{
	std::map<int, XLevelWaveInfo>::iterator it = m_waveInfos.find(dwWaveID);

	if(it != m_waveInfos.end())
	{
		return &(it->second);
	}

	return NULL;
}

bool XLevelSpawner::Update(float time)
{
	m_fTime += time;
	if (m_executeTimes)
	{
		if (m_fTime < 0.5f)
		{
			ProcessTaskQueue(time);
			return false;
		}
		
		m_fTime -= 0.5f;
	}
	else
	{
		m_fTime = 0.0f;
	}

	time_t curTime = m_pScene->CostTimeMili();
	if (curTime == -1)
	{
		return false;
	}
	if (!_CheckRestTime(time))
	{
		return false;
	}
	++m_executeTimes;

	CheckMissingRoleScript();
	CheckReconnectRoleScript();

	GenerateTask(curTime, time);

	ProcessTaskQueue(time);

	return true;
}

void XLevelSpawner::GenerateTask(time_t curTime, float time)
{
	for(std::map<int, XLevelWaveInfo>::iterator it = m_waveInfos.begin(); it != m_waveInfos.end(); it++)
	{
		XLevelWave* pWave = &it->second.m_staticLevelWaveInfo;
		XLevelDynamicInfo *pdInfo = &it->second.m_dynamicWaveInfo;

		if(pdInfo->m_TotalCount != 0 && pdInfo->m_GenerateCount == pdInfo->m_TotalCount) continue;
		if(pdInfo->m_PushInfoTask) continue;

		bool preWaveFinished = true;

		for(auto iter = it->second.m_preLevelWaveInfos.begin(); iter != it->second.m_preLevelWaveInfos.end(); ++iter)
		{
			XLevelDynamicInfo *pPredInfo = &iter->second->m_dynamicWaveInfo;

			if(pPredInfo)
			{
				if(pPredInfo->m_GenerateCount != pPredInfo->m_TotalCount)
				{
					preWaveFinished = false;
					break;
				}

				if(pPredInfo->m_enemyIds.size() == 1 || pPredInfo->m_dummyIds.size() == 1)//不可能同时为1
				{
					Unit* pUnit = NULL;
					if(pPredInfo->m_enemyIds.size() == 1)
					{
						pUnit = (Unit*)EnemyManager::Instance()->FindEnemy(pPredInfo->m_enemyIds[0]);
					}else if(pPredInfo->m_dummyIds.size() == 1)
					{
						pUnit = (Unit*)DummyRoleManager::Instance()->Find(pPredInfo->m_dummyIds[0]);
					}

					if(pUnit)
					{
						double maxHP = pUnit->GetAttr(TOTAL_MaxHP);
						double currentHP = pUnit->GetAttr(BASIC_CurrentHP);

						if(currentHP > maxHP * pWave->m_PreWavePercent && !(pWave->m_PreWavePercent == 0 && pUnit->IsDead()))
						{
							preWaveFinished = false;
							break;
						}
					}
				}
				else
				{
					if(pPredInfo->m_GenerateCount != pPredInfo->m_DieCount)
					{
						preWaveFinished = false;
						break;
					}
				}
			}
		}

		if(false == preWaveFinished)
		{
			continue;
		}

		bool exStringExists = true;

		if(!pWave->m_ExString.empty())
		{
			if(!m_oLevelState.QueryExternalString(pWave->m_ExString, false))
			{
				exStringExists = false;
			}
		}

		if(exStringExists && pdInfo->m_ExstringFinishTime == 0) pdInfo->m_ExstringFinishTime = curTime;
		if (preWaveFinished && pdInfo->m_PreWaveFinishTime == 0) pdInfo->m_PreWaveFinishTime = curTime;

		bool bCanGenerate = false;

		if(!pWave->m_ExString.empty())
		{
			if(pdInfo->m_ExstringFinishTime > 0 && curTime - pdInfo->m_ExstringFinishTime >= pWave->m_Time)
			{
				bCanGenerate = true;
			}
		}
		else if(!pWave->m_Levelscript.empty() || (pdInfo->m_GenerateCount < pdInfo->m_TotalCount))
		{
			if(preWaveFinished && curTime - pdInfo->m_PreWaveFinishTime >= pWave->m_Time)
			{
				bCanGenerate = true;
			}
		}

		if(bCanGenerate)
		{
			if(!pWave->m_Levelscript.empty())
			{
				GenerateScriptTask(&it->second);
			}
			else
			{
				GenerateNormalTask(&it->second);
				GenerateRoundTask(&it->second);
			}

			if (!pWave->m_Repeat)
				pdInfo->m_PushInfoTask = true;
			if (pWave->m_Repeat && !pWave->m_ExString.empty())
			{
				m_oLevelState.QueryExternalString(pWave->m_ExString, true);
				pdInfo->m_ExstringFinishTime = 0;
			}
		}
	}

}

void XLevelSpawner::ProcessTaskQueue(float time)
{
	while(!m_roleMissingTasks.empty())
	{
		XLevelBaseTask* pTask = m_roleMissingTasks.front();
		m_roleMissingTasks.pop();

		pTask->Execute(time);

		delete pTask;
	}

	for (int i = 0; i < spawn_monster_per_frame; i++)
	{
		if (m_tasks.size() > 0)
		{
			XLevelBaseTask* pTask = m_tasks.front();
			m_tasks.pop();

			pTask->Execute(time);

			delete pTask;
		}
		else
			break;
	}
}

void XLevelSpawner::GenerateNormalTask(XLevelWaveInfo *pWaveInfo)
{
	XLevelWave* pWave = &pWaveInfo->m_staticLevelWaveInfo;
	std::map<int, Vector3>::iterator it2 = pWave->m_Monsters.begin();
	for(; it2 != pWave->m_Monsters.end(); it2++)
	{
		XLevelSpawnTask *pTask = new XLevelSpawnTask(this);
		pTask->m_Id = pWave->m_Id;
		pTask->m_EnemyID = pWave->m_EnemyID;
		pTask->m_MonsterRotate = (int)pWave->m_MonsterRotation[it2->first];
		pTask->m_MonsterIndex = it2->first;
		pTask->m_MonsterPos = it2->second;
		if (pWave->m_RandomID)
		{
			XLevelDynamicInfo *pdInfo = &pWaveInfo->m_dynamicWaveInfo;
			//if (pdInfo)
			{
				pTask->m_EnemyID = pdInfo->m_RandomizedEnemyID;
			}
			/*else
			{
				int enemyID = RandomBossMgr::Instance()->GetRandomBoss(pWave->m_RandomID);
				pTask->m_EnemyID = enemyID;
			}*/
		}

		m_tasks.push(pTask);
	}
}

void XLevelSpawner::GenerateRoundTask(XLevelWaveInfo *pWaveInfo)
{
	XLevelWave* pWave = &pWaveInfo->m_staticLevelWaveInfo;
	if(pWave->m_RoundRidus > 0 && pWave->m_RoundCount > 0)
	{
		float angle = 360.0f/pWave->m_RoundCount;

		const std::list<Role*>& oUnits = m_pScene->GetAllRoles();
		if(oUnits.size() > 0)
		{
			for (int i = 0; i < pWave->m_RoundCount; i++)
			{
				XLevelSpawnTask *pTask = new XLevelSpawnTask(this);
				pTask->m_Id = pWave->m_Id;
				pTask->m_EnemyID = pWave->m_EnemyID;
				if (pWave->m_RandomID)
				{
					XLevelDynamicInfo *pdInfo = &pWaveInfo->m_dynamicWaveInfo;
					//if (pdInfo)
					{
						pTask->m_EnemyID = pdInfo->m_RandomizedEnemyID;
					}
				}
				pTask->m_MonsterIndex = 0; // no use now

				Vector3 dir = Vector3::HorizontalRotate(Vector3(1, 0, 0), angle*i);
				pTask->m_MonsterPos = (*oUnits.begin())->GetXObject()->GetPosition_p() + 
					dir * pWave->m_RoundRidus;
	
				pTask->m_MonsterRotate = (int)angle*i + 180;

				m_tasks.push(pTask);
			}
		}
	}
}

void XLevelSpawner::GenerateScriptTask(XLevelWaveInfo *pWaveInfo)
{
	XLevelWave* pWave = &pWaveInfo->m_staticLevelWaveInfo;
	XLevelScriptTask *pTask = new XLevelScriptTask(this);
	pTask->m_Id = pWaveInfo->m_staticLevelWaveInfo.m_Id;
	pTask->m_strScriptName = pWave->m_Levelscript;
	pTask->m_scene = m_pScene;

	m_tasks.push(pTask);
}

UINT32 XLevelSpawner::GetLevelCallBackNum()
{
	++m_callbackNum;
	if (!m_callbackNum)
	{
		return ++m_callbackNum;
	}

	return m_callbackNum;
}

XLevelCallBack* XLevelSpawner::CreateScriptCallBack()
{
	XLevelCallBack* callback = new XLevelScriptCallBack(this);
	m_callBackList[callback->m_callBackNum] = callback;
	return callback;
}

void XLevelSpawner::GenerateCallBackScriptTask(XLevelScriptCallBack* callBack)
{
	if (!callBack)
	{
		return;
	}
	XLevelScriptTask *pTask = new XLevelScriptTask(this);
	pTask->m_strScriptName = callBack->m_callbackFunc;
	pTask->m_scene = m_pScene;

	m_tasks.push(pTask);
}

void XLevelSpawner::GenerateRoleScriptTask(Role* role, std::string& func, bool isReconnect)
{
	XLevelRoleScriptTask* pTask = new XLevelRoleScriptTask(this);
	pTask->m_strScriptName = func;
	pTask->m_scene = m_pScene;
	pTask->m_role = role;
	pTask->m_isReconnect = isReconnect;

	m_roleMissingTasks.push(pTask);
}

void XLevelSpawner::CheckMissingRoleScript()
{
	bool except = SceneConfig::Instance()->IsPrepareExcept(m_pScene->GetSceneType());
	if (except)
	{
		return;
	}

	XAllRoleMissingExecutScript infos;
	m_levelScript.GetRoleMissingScript(infos);
	if (infos.empty())
	{
		return;
	}
	foreach(i in infos)
	{
		foreach (j in i->m_role)
		{
			GenerateRoleScriptTask(*j, i->m_funcName);
		}
	}
}

void XLevelSpawner::CheckReconnectRoleScript()
{
	bool except = SceneConfig::Instance()->IsPrepareExcept(m_pScene->GetSceneType());
	if (except)
	{
		return;
	}

	XAllRoleMissingExecutScript infos;
	m_levelScript.GetRoleReconnectScript(infos);
	if (infos.empty())
	{
		return;
	}
	foreach(i in infos)
	{
		foreach (j in i->m_role)
		{
			GenerateRoleScriptTask(*j, i->m_funcName, true);
		}
	}

	_ClearReconnectRoles();
}

void XLevelSpawner::SyncScriptState(Role* role)
{
	if (!role)
	{
		return;
	}
	PtcG2C_LevelScriptStateNtf ntf;
	foreach(i in m_levelScript.GetChangeDoorState())
	{
		auto state = ntf.m_Data.add_doorstates();
		state->set_name(i->first);
		state->set_ison(i->second);
	}
	if (ntf.m_Data.doorstates_size())
	{
		role->Send(ntf);
	}
}

bool XLevelSpawner::_CheckRestTime(float time)
{
	if (m_fRestTime <= 0)
	{
		return true;
	}

	m_fRestTime -= time;

	if (m_fRestTime <= 0)
	{
		return true;
	}

	return false;
}

void XLevelSpawner::BossRevive(int waveID, UINT64 lastTempID, /*UINT64 newTempID*/Enemy* enemy)
{
	if (!enemy)
	{
		return;
	}
	XLevelWaveInfo* pWaveInfo = GetLevelWaveInfo(waveID);

	if (pWaveInfo == NULL) return;

	XLevelDynamicInfo* pdInfo = &pWaveInfo->m_dynamicWaveInfo;

	if (pdInfo->m_DieCount)
	{
		--pdInfo->m_DieCount;
	}

	if(enemy->IsBoss())
		m_oLevelState.AddBossDieNum(-1);

	m_oLevelState.m_total_kill -= 1;
	if (pdInfo->IsSpawned(lastTempID))
	{
		-- m_oLevelState.m_total_kill_levelspawn;
	}

	pdInfo->Replace(lastTempID, enemy->GetID());
}

void XLevelSpawner::ReplaceRandmizedEnemyID(int waveID, int enemyID)
{
	if (m_pScene->GetSceneType() != KKSG::SCENE_BOSSRUSH && m_pScene->GetSceneType() != KKSG::SCENE_RESWAR_PVE)
	{
		return;
	}

	XLevelWaveInfo* pWaveInfo = GetLevelWaveInfo(waveID);
	if (pWaveInfo == NULL) return;

	XLevelDynamicInfo* pdInfo = &pWaveInfo->m_dynamicWaveInfo;

	pdInfo->ReplaceRandomizedEnemyID(enemyID);

	if (m_pScene->GetSceneType() == KKSG::SCENE_BOSSRUSH)
	{
		std::stringstream ss;
		//先自己加上 by sherlockzhang
		ss << "playercontinue";
		ss << waveID;
		m_oLevelState.SetExternalString(ss.str());
	}
}

/*战火 功能已关
void XLevelSpawner::AddRobotSummary( RoleSummary* summary )
{
	m_robotSummarys.push_back(summary);
}

void XLevelSpawner::RandomRobotExString()
{
	if(!m_robotExString.empty())
	{
		int index = XRandom::randInt(0,m_robotExString.size());
		if(index < (int)m_robotExString.size())
		{
			m_oLevelState.SetExternalString(m_robotExString[index]);
		}
		m_robotExString.clear();//为了保证只执行一次
	}
}
*/

int XLevelSpawner::GetWinConditionTime()
{
	for(size_t i = 0; i < m_WinConditon.size(); i++)
	{
		switch(m_WinConditon[i].cond)
		{
			case LevelWin_Time:
				return m_WinConditon[i].param;
			default:
				break;
		}

	}

	return 0;
}

int XLevelSpawner::GetLoseConditionTime()
{
	for(size_t i = 0; i < m_LoseCondition.size(); i++)
	{
		switch(m_LoseCondition[i].cond)
		{
			case LevelTime_Out:
				return m_LoseCondition[i].param;
			default:
				break;
		}
	}

	return 0;
}

bool XLevelSpawner::HasWinConditionScore()
{
	for(size_t i = 0; i < m_WinConditon.size(); i++)
	{
		switch(m_WinConditon[i].cond)
		{
			case LevelWin_KillEnemyScore:
				return true;
			default:
				break;
		}

	}

	return false;
}

void XLevelSpawner::FillPreloadInfo(KKSG::SceneCfg& cfg)
{
	//std::vector<XLevelWave*> waves;
	//XLevelSpawnMgr::Instance()->GetWaveInfo(m_dwSceneTemplateID, waves);
	//foreach(i in waves)
	foreach(i in m_waveInfos)
	{
		if (i->second.m_staticLevelWaveInfo.m_SpawnType != Spawn_Source_Monster && i->second.m_staticLevelWaveInfo.m_SpawnType != Spawn_Source_Random)
		{
			continue;
		}
		if (!i->second.m_staticLevelWaveInfo.m_RandomID)
		{
			if (i->second.m_staticLevelWaveInfo.m_EnemyID)
			{
				cfg.add_preloadenemyids(i->second.m_staticLevelWaveInfo.m_EnemyID);
			}
		}
		else
		{
			//XLevelDynamicInfo* info = GetWaveDynamicInfo((*i)->m_Id);
			XLevelDynamicInfo* info = &i->second.m_dynamicWaveInfo;
			if (info)
			{
				cfg.add_preloadenemyids(info->m_RandomizedEnemyID);
			}
		}
	}
}

void XLevelSpawner::PushReconnectRole(Role* role)
{
	if (!role || role->GetCurrScene() != m_pScene)
	{
		return;
	}

	m_reconnectRoles.insert(role->GetID());

	SyncScriptState(role);
}

void XLevelSpawner::OnCallBackFuncFinish(int callBackNum)
{
	if (!callBackNum)
	{
		return;
	}

	auto find = m_callBackList.find(callBackNum);
	if (find != m_callBackList.end())
	{
		(*find->second)();

		delete find->second;

		m_callBackList.erase(callBackNum);
	}
}

XLevelScript::XLevelScript()
{
	m_pSpawner = NULL;
	m_dwSceneTemplateID = 0;
}

void XLevelScript::Init(XLevelSpawner* pSpawner)
{
	m_pSpawner = pSpawner;
	m_dwSceneTemplateID = m_pSpawner->GetScene()->GetSceneTemplateID();

	auto pScriptFuncInfo = XLevelScriptMgr::Instance()->GetLevelScriptInfo(m_dwSceneTemplateID);
	if (pScriptFuncInfo)
	{
		m_funcScripts = *pScriptFuncInfo;
	}

	auto levelScriptInfo = XLevelScriptMgr::Instance()->GetLevelInfos(m_dwSceneTemplateID);
	if (levelScriptInfo)
	{
		foreach(i in (*levelScriptInfo))
		{
			m_levelScriptInfos.push_back(*i);
		}
	}
}

void XLevelScript::Execute(std::string& func)
{
	if (!m_dwSceneTemplateID)
	{
		return;
	}
	/*auto pScripts = XLevelScriptMgr::Instance()->GetLevelScriptInfo(m_dwSceneTemplateID);
	if (!pScripts)
	{
		return;
	}*/
	const std::map<std::string, XLevelFunc>* pScripts = &m_funcScripts;
	auto findFunc = pScripts->find(func);
	if (findFunc == pScripts->end())
	{
		return;
	}
	const std::vector<XLevelCmd>& cmds = findFunc->second.cmds;
	Scene* scene = m_pSpawner->GetScene();
	bool allRoleExecute = false;
	foreach(i in cmds)
	{
		switch(i->type)
		{
		// send client to execute 
		case Level_Cmd_Opendoor:
			{
				if (i->params.size() >= 5)
				{
					float sr = 0, dface = 0;
					Vector3 sPos , dPos;

					vector<string> strInfos = Split(i->params[3], '|');
					if (strInfos.size() >= 4)
					{
						sPos.x = convert<float>(strInfos[0]);
						sPos.y = convert<float>(strInfos[1]);
						sPos.z = convert<float>(strInfos[2]);
						sr = convert<float>(strInfos[3]);
					}

					strInfos.clear();
					strInfos = Split(i->params[4], '|');
					if (strInfos.size() >= 4)
					{
						dPos.x = convert<float>(strInfos[0]);
						dPos.y = convert<float>(strInfos[1]);
						dPos.z = convert<float>(strInfos[2]);
						dface = convert<float>(strInfos[3]);
					}

					TeleportBase* door = scene->AddTeleportDoor(sPos, sr, dPos, dface);
					if (door)
					{
						door->SetLevelScriptFunc(func);
					}
				}

				NoticeClientExecuteScript(scene, func);
				if (scene && i->params.size() >= 2)
				{
					bool isOn = false;
					for (size_t j = 0; j < i->params.size(); ++j)
					{
						if (strcmp(i->params[j].c_str(), "on") == 0)
						{
							isOn = true;
							break;
						}
					}

					this->ChangeWallState(i->params[1], isOn);

					m_doorChangedState.insert(std::make_pair(i->params[1], isOn));
				}
			}
			break;
		case Level_Cmd_OpenDoorEx:
			{
				if (i->params.size() < 6)
					continue;

				float sr = 0, dface = 0;
				Vector3 sPos , dPos;

				vector<string> strInfos = Split(i->params[3], '|');
				if (strInfos.size() >= 4)
				{
					sPos.x = convert<float>(strInfos[0]);
					sPos.y = convert<float>(strInfos[1]);
					sPos.z = convert<float>(strInfos[2]);
					sr = convert<float>(strInfos[3]);
				}

				strInfos.clear();
				strInfos = Split(i->params[4], '|');
				if (strInfos.size() >= 4)
				{
					dPos.x = convert<float>(strInfos[0]);
					dPos.y = convert<float>(strInfos[1]);
					dPos.z = convert<float>(strInfos[2]);
					dface = convert<float>(strInfos[3]);
				}

				TeleportBase* door = scene->AddTeleportDoor(sPos, sr, dPos, dface);

				NoticeClientExecuteScript(scene, func);
				if (scene && i->params.size() >= 2)
				{
					bool isOn = false;
					for (size_t j = 0; j < i->params.size(); ++j)
					{
						if (strcmp(i->params[j].c_str(), "on") == 0)
						{
							isOn = true;
							break;
						}
					}

					this->ChangeWallState(i->params[1], isOn);

					m_doorChangedState.insert(std::make_pair(i->params[1], isOn));
				}

				if (door)
				{
					XLevelCallBack* callback = m_pSpawner->CreateScriptCallBack();
					((XLevelScriptCallBack*)callback)->SetCallBackFunc(i->params[5]);
					door->SetLevelCallBack(callback->m_callBackNum);

					door->SetLevelScriptFunc(func);
				}
			}
			break;
		case Level_Cmd_Notice:
		case Level_Cmd_StopNotice:
		case Level_Cmd_NpcPopSpeek:
		case Level_Cmd_SHOWDIRECTION:
		case Level_Cmd_BUBBLE:
		case Level_Cmd_HIDEBILLBOARD:
		case Level_Cmd_CONTINUEUI:
		case Level_Cmd_JustFx:
		case Level_Cmd_PlayFx:
		//case Level_Cmd_SoloCamera:
			{
				NoticeClientExecuteScript(scene, func);
			}
			break;
		case Level_Cmd_Cutscene:
			{
				if (i->params.size() < 2)
				{
					LogError("scene:%u, %s, show cutscene error.", m_dwSceneTemplateID, func.c_str());
					break;
				}

				NoticeClientExecuteScript(scene, func);

				std::string cutSceneFile;
				int cutSceneTime = 0;
				cutSceneFile = i->params[1];
				if (!cutSceneFile.empty())
				{
					cutSceneTime = CutSceneReader::LoadCutSceneTime((cutSceneFile + ".txt").c_str()) / 1000;
				}

				UINT64 stopAITime = TimeUtil::GetTime() + cutSceneTime;
				stopAITime *= 1000;
				stopAITime += 1500;

				if(i->params.size() >= 6)
				{
					//Vector3 pos = Vector3(i->params[2], i->params[3], i->params[4]);

					// drag to pos
					float x,y,z,face;
					x = convert<float>(i->params[2]);
					y = convert<float>(i->params[3]);
					z = convert<float>(i->params[4]);
					face = convert<float>(i->params[5]);

					Vector3 pos(x,y,z);

					const std::list<Role*>& players = scene->GetAllRoles();
					for (auto j = players.begin(); j != players.end(); ++j)
					{
						(*j)->GetXObject()->TransferLocation(pos, face);
					}

					std::vector<Unit*> dummyroles;
					FindDummyRoleFightGroupCondition cond((UINT32)(KKSG::FightRole));
					scene->GetUnitByCondition(cond , dummyroles);
					for(size_t i = 0; i < dummyroles.size(); ++i)
					{
						if(dummyroles[i] && dummyroles[i]->GetXObject())
						{
							dummyroles[i]->GetXObject()->TransferLocation(pos, face);
						}
					}

				}

				scene->SetStopAITime(stopAITime);

			}
			break;
		case Level_Cmd_Addbuff:
			{
				if (i->params.size() >= 4)
				{
					XAddBuffActionArgs args;
					UINT32 enemyTemID = (UINT32)atoi(i->params[1].c_str());
					args.singleBuff.buffID = atoi(i->params[2].c_str());
					args.singleBuff.BuffLevel = atoi(i->params[3].c_str());

					if (enemyTemID)
					{
						std::vector<Unit*> units;
						FindEnemyIDCondition cond(enemyTemID);
						scene->GetUnitByCondition(cond,units);
						foreach(j in units)
						{
							(*j)->GetXObject()->DispatchAction(&args);
						}
					}
					else
					{
						const std::list<Role*>& units = scene->GetAllRoles();
						foreach(j in units)
						{
							(*j)->GetXObject()->DispatchAction(&args);
						}
					}
				}
				break;
			}
		case Level_Cmd_Removebuff:
			{
				if (i->params.size() >= 3)
				{
					XRemoveBuffActionArgs args;
					UINT32 enemyTemID = (UINT32)atoi(i->params[1].c_str());
					args.nBuffId = atoi(i->params[2].c_str());

					std::vector<Unit*> units;
					FindEnemyIDCondition cond(enemyTemID);
					scene->GetUnitByCondition(cond,units);
					foreach(j in units)
					{
						(*j)->GetXObject()->DispatchAction(&args);
					}

				}
				break;
			}
		case Level_Cmd_KillSpawn:
			{
				if (i->params.size() >= 2)
				{
					int waveID = atoi(i->params[1].c_str());

					scene->KillAllEnemy(waveID);
				}
				break;

			}
		case Level_Cmd_KillAlly:
			{
				if (i->params.size() >= 2)
				{
					int waveID = atoi(i->params[1].c_str());

					scene->KillAllAlly(waveID);
				}
				break;
			}
		case Level_Cmd_KillWave:
			{
				if (i->params.size() >= 2)
				{
					int waveID = atoi(i->params[1].c_str());

					scene->KillWave(waveID);
				}
				break;

			}
		case Level_Cmd_Summon:
			{
				if (i->params.size() >= 4)
				{
					int enemyID = atoi(i->params[1].c_str());
					int enemyNum = atoi(i->params[3].c_str());


					Vector3 pos;
					float rotate = 0;
					if (strcmp(i->params[2].c_str(), "[player]") == 0)
					{
						UINT64 roleID = m_pSpawner->m_oLevelState.GetTargetRoleID();
						Role* tempRole = RoleManager::Instance()->FindByRoleID(roleID);
						if (!tempRole)
						{
							break;
						}
						rotate = tempRole->GetXRole()->GetFaceDegree();
						pos = tempRole->GetXRole()->GetPosition_p();
					}
					else
					{
						int bossID = atoi(i->params[2].c_str());
						std::vector<Unit*> units;
						FindEnemyIDCondition cond(bossID);
						scene->GetUnitByCondition(cond,units);
						if (units.empty())
						{
							break;
						}
						rotate = units[0]->GetXObject()->GetFaceDegree();
						pos = units[0]->GetXObject()->GetPosition_p();

					}
					for (int i = 0; i < enemyNum; ++i)
					{
						Enemy* pUnit = EnemyManager::Instance()->CreateEnemy(enemyID, pos, rotate, 1.0);

						if(pUnit)
						{
							scene->AddUnitAndNotify(pUnit);
						}
					}
				}
				break;

			}
		case Level_Cmd_KillAllSpawn:
			{
				scene->KillAllEnemy();
				break;
			}
		case Level_Cmd_SETEXTSTRING:
			{
				if(i->params.size() >= 2)
				{
					m_pSpawner->m_oLevelState.SetExternalString(i->params[1]);
				}
				break;
			}
		case Level_Cmd_AICommand:
			{
				int monsterid = atoi(i->params[1].c_str());
				string cmd = i->params[2];

				if (monsterid == -1) // send to global ai
				{
					int firerTemplateId = atoi(i->params[3].c_str());

					if (firerTemplateId == 0)
					{
						XAIEventArgs args;
						args.MsgTargetTemplateId = 0;
						args.EventArg = cmd;
						args.Templateid = firerTemplateId;
						args.Pos = Vector3::zero;
						args.FloatArg = 0;
						scene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
					}
					else
					{
						FindEnemyTemplateIdCondition cond0(firerTemplateId);
						std::vector<Unit *> units0;
						scene->GetUnitByCondition(cond0, units0);

						if (units0.size() > 0)
						{
							XAIEventArgs args;
							args.MsgTargetTemplateId = 0;
							args.EventArg = cmd;
							args.Templateid = firerTemplateId;
							args.Pos = units0[0]->GetXObject()->GetPosition();
							args.FloatArg = units0[0]->GetXObject()->GetFaceDegree();
							scene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
						}
					}
				}
				else
				{
					XAIEventArgs args;
					args.MsgTargetTemplateId = monsterid;
					args.EventArg= cmd;
					scene->MsgChannel().SendMsgTo(AISceneMsgChannel::Enemy, AISceneMsgChannel::Cmd, args);
				}
			}
			break;
		case Level_Cmd_AddScenelSpanTime:
			if(i->params.size() >= 2)
			{
				scene->AddTimeSpan(convert<UINT32>(i->params[1]));
			}
			break;
		default:
			break;
		}

		if(i->type == Level_Cmd_HIDEBILLBOARD || (i->type == Level_Cmd_Cutscene && i->params.size() >= 6))
			allRoleExecute = true;
	}
	if(allRoleExecute)
	{
		AddRoleExecutedScript(func);
	}
}

void XLevelScript::Execute(Role* role, std::string& func)
{
	if (!m_dwSceneTemplateID || !role)
	{
		return;
	}
	const std::map<std::string, XLevelFunc>* pScripts = &m_funcScripts;
	auto findFunc = pScripts->find(func);
	if (findFunc == pScripts->end())
	{
		return;
	}
	const std::vector<XLevelCmd>& cmds = findFunc->second.cmds;
	Scene* scene = m_pSpawner->GetScene();
	foreach(i in cmds)
	{
		switch(i->type)
		{
		// send client to execute 
		case Level_Cmd_Opendoor:
		case Level_Cmd_OpenDoorEx:
			{
				if (i->params.size() >= 5)
				{
					float dface = 0;
					Vector3 dPos;

					vector<string> strInfos;

					strInfos = Split(i->params[4], '|');
					if (strInfos.size() >= 4)
					{
						dPos.x = convert<float>(strInfos[0]);
						dPos.y = convert<float>(strInfos[1]);
						dPos.z = convert<float>(strInfos[2]);
						dface = convert<float>(strInfos[3]);
					}

					if (dPos != Vector3(0,0,0))
					{
						role->GetXObject()->TransferLocation(dPos, dface);
					}
					else
					{
						if (!scene->HasTeleportDoor())
						{
							PtcG2C_GSErrorNotify oErrorPtc;
							oErrorPtc.m_Data.set_errorno(KKSG::ERR_TRANSFERR_OTHER_ROLES_LEAVE);
							role->Send(oErrorPtc);
							break;
						}
					}
				}

				NoticeRoleExecuteScript(role, func);
			}
			break;
		case Level_Cmd_Cutscene:
			{
				if (i->params.size() < 2)
				{
					LogError("scene:%u, %s, show cutscene error.", m_dwSceneTemplateID, func.c_str());
					break;
				}

				if(i->params.size() >= 6)
				{
					float x,y,z,face;
					x = convert<float>(i->params[2]);
					y = convert<float>(i->params[3]);
					z = convert<float>(i->params[4]);
					face = convert<float>(i->params[5]);

					Vector3 pos(x,y,z);

					role->GetXObject()->TransferLocation(pos, face);
				}
			}
			break;
		case Level_Cmd_HIDEBILLBOARD:
			{
				NoticeRoleExecuteScript(role, func);
			}
			break;
		default:
			break;
		}
	}

	_AddRoleExecutedScript(func, role->GetID());
}

void XLevelScript::ReconnectExecute(Role* role, std::string& func)
{
	if (!m_dwSceneTemplateID || !role)
	{
		return;
	}
	const std::map<std::string, XLevelFunc>* pScripts = &m_funcScripts;
	auto findFunc = pScripts->find(func);
	if (findFunc == pScripts->end())
	{
		return;
	}
	const std::vector<XLevelCmd>& cmds = findFunc->second.cmds;
	Scene* scene = m_pSpawner->GetScene();
	foreach(i in cmds)
	{
		switch(i->type)
		{
		case Level_Cmd_HIDEBILLBOARD:
			{
				NoticeRoleExecuteScript(role, func);
			}
			break;
		default:
			break;
		}
	}
}

void XLevelScript::ChangeWallState(const std::string& name, bool enable)
{
	foreach(i in m_levelScriptInfos)
	{
		if(strcmp(i->infoName.c_str(), name.c_str()) == 0)
		{
			i->enable = enable;
		}
	}
}

void XLevelScript::GetLevelScriptInfosWithNum(std::vector<XLevelInfo>& infos)
{
	infos.clear();
	foreach(i in m_levelScriptInfos)
	{
		if (i->number)
		{
			infos.push_back(*i);
		}
	}
}

void XLevelScript::GetRoleMissingScript(XAllRoleMissingExecutScript& infos)
{
	if (m_allRoleExecuteScript.empty())
	{
		return;
	}
	const std::list<Role*>& list = m_pSpawner->GetScene()->GetAllRoles();
	if (list.empty())
	{
		return;
	}

	for (auto iter = m_allRoleExecuteScript.begin(); iter != m_allRoleExecuteScript.end(); ++iter)
	{
		XAllRoleMissingExecutScriptInfo oneInfo;
		oneInfo.m_funcName = iter->m_funcName;
		for (auto i = list.begin(); i != list.end(); ++i)
		{
			auto find = iter->m_roleIDs.find((*i)->GetID());
			if (find == iter->m_roleIDs.end())
			{
				oneInfo.m_role.insert(*i);
			}
		}
		if (!oneInfo.m_role.empty())
		{
			infos.push_back(oneInfo);
		}
	}

}

void XLevelScript::GetRoleReconnectScript(XAllRoleMissingExecutScript& infos)
{
	if (m_allRoleExecuteScript.empty())
	{
		return;
	}
	const std::set<UINT64>& set = m_pSpawner->GetReconnectRole();
	if (set.empty())
	{
		return;
	}

	std::set<Role*> roles;
	foreach (i in set)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(*i);
		if(role && role->GetCurrScene() == m_pSpawner->GetScene())
		{
			roles.insert(role);
		}
	}

	for (auto iter = m_allRoleExecuteScript.begin(); iter != m_allRoleExecuteScript.end(); ++iter)
	{
		XAllRoleMissingExecutScriptInfo oneInfo;
		oneInfo.m_funcName = iter->m_funcName;
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			auto find = iter->m_roleIDs.find((*i)->GetID());
			if (find != iter->m_roleIDs.end())
			{
				oneInfo.m_role.insert(*i);
			}
		}
		if (!oneInfo.m_role.empty())
		{
			infos.push_back(oneInfo);
		}
	}
}

void XLevelScript::NoticeClientExecuteScript(Scene* scene, std::string& func)
{
	PtcG2C_ExecuteLevelScriptNtf ntf;
	ntf.m_Data.set_script(func);
	scene->Broadcast(ntf);
}

void XLevelScript::NoticeRoleExecuteScript(Role* role, std::string& func)
{
	PtcG2C_ExecuteLevelScriptNtf ntf;
	ntf.m_Data.set_script(func);
	role->Send(ntf);
}

void XLevelScript::AddRoleExecutedScript(std::string& func)
{
	const std::list<Role*>& list = m_pSpawner->GetScene()->GetAllRoles();
	if (list.empty())
	{
		return;
	}
	if (!m_pSpawner->GetScene()->GetSceneTeam())
	{
		return;
	}
	XAllRoleExecutScriptInfo info;
	info.m_funcName = func;
	foreach(i in list)
	{
		info.m_roleIDs.insert((*i)->GetID());
	}
	foreach(i in m_allRoleExecuteScript)
	{
		if (strcmp(i->m_funcName.c_str(), func.c_str()) == 0)
		{
			m_allRoleExecuteScript.erase(i);
			break;
		}
	}
	m_allRoleExecuteScript.push_back(info);
}

void XLevelScript::_AddRoleExecutedScript(std::string& func, UINT64 roleID)
{
	foreach(i in m_allRoleExecuteScript)
	{
		if (strcmp(i->m_funcName.c_str(), func.c_str()) == 0)
		{
			i->m_roleIDs.insert(roleID);
		}
	}
}

