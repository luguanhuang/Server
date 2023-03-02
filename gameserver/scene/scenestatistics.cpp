#include "pch.h"
#include "scenestatistics.h"
#include "unit/role.h"
#include "scene.h"
#include "sceneteam.h"
#include "unit/dummyrole.h"
#include "unit/dummyrolemanager.h"
#include "unit/enemy.h"
#include "gamelogic/rolesummary.h"
#include "securityscenestatistics.h"
#include "securitystatistics.h"
#include "unit/enemy.h"
#include "scene/scenemortuary.h"

SceneStatistics::SceneStatistics()
{
	m_securityStatistics = new XSecuritySceneStatistics();
	Reset();
}

SceneStatistics::~SceneStatistics()
{
	for (auto i = m_roleid2record.begin(); i != m_roleid2record.end(); ++i)
	{
		if (NULL != i->second)
		{
			delete i->second;
			i->second = NULL;
		}
	}
	m_roleid2record.clear();
	delete m_securityStatistics;
}

void SceneStatistics::Reset()
{
	m_ischeated = false;
	m_TotalBehit = 0;
	m_TotalDamage = 0;
	m_securityStatistics->Reset();
}

void SceneStatistics::OnEnterSceneFinally(Scene *pScene, Role* role)
{
	if (role->IsWatcher())
	{
		return;
	}
	auto i = m_roleid2record.find(role->GetID());
	if (i == m_roleid2record.end())
	{
		RoleBattleRecord* record = new RoleBattleRecord(role);
		m_roleid2record[role->GetID()] = record;
		record->Init(pScene);
	}
	else
	{
		SSWarn<<"find role exist:"<<role->GetID()<<" scene:"<<pScene->GetSceneTemplateID()<<END;
	}

	if (role->GetSecurityStatistics())
	{
		role->GetSecurityStatistics()->OnStart();
	}
	///> 工会擂台战场景中阵营会切换，不统计阵营关系
	if (!SceneConfig::Instance()->IsChangeFightGroupScene(pScene->GetSceneType()))
	{
		if (m_players.size() >= 8)
		{
			SSWarn<<"players out of range"<<END;
			return;
		}
		PlayerInfo info;
		info.PlayerOpenID = role->GetAccount();
		info.PlayerBattlePoint = role->GetAttr(TOTAL_POWERPOINT);
		info.PlayerSide = role->GetFightGroup();
		info.PlayerType = 2;
		info.roleid = role->GetID();

		m_players[info.roleid] = info;
	}
}

void SceneStatistics::OnLeaveScene(Scene *pScene, Role* role)
{
	auto i = m_roleid2record.find(role->GetID());
	if (i == m_roleid2record.end())
	{
		SSInfo<<"not find role:"<<role->GetID()<<" scene:"<<pScene->GetSceneTemplateID()<<END;
	}
	else
	{
		RoleBattleRecord* record = i->second;
		if (!role->IsWatcher())
		{
			///> 关卡结算方式
			UINT32 scenestate = pScene->GetSceneState();
			if (SCENE_RUNNING == scenestate)
			{
				record->end.m_endtype = 2;
			}
			else if (SCENE_LOSE == scenestate)
			{
				record->end.m_endtype = 1;
			}
			else if (SCENE_WIN == scenestate)
			{
				record->end.m_endtype = 0;
			}

			///> 关卡内玩家交互关系
			record->start.FillPlayers(role->GetFightGroup(), m_players);
			///> 场景结算信息
			record->end.FillEnd(pScene);

		    ///> 怪物死亡信息
			if (pScene->IsSyncScene())
			{
				record->end.FillMonsterKill(m_monsterinfo);
				record->end.FillTimeSpan(pScene->GetTimeSpan());
			}
			///> 死亡复活
			if (NULL != pScene->GetMortuary())
			{
				record->end.FillPlayerDeath(pScene->GetMortuary()->GetDeathCount(role->GetID()), pScene->GetMortuary()->GetReviveCount(role->GetID()));
			}

			if (!record->Send())
			{
				SSInfo<<"client info not received, roleid:"<<role->GetID()<<END;
			}
		}
	
		delete record;
		i->second = NULL;
		m_roleid2record.erase(i);
	}
}

void SceneStatistics::OnEnemyDie(Scene* pScene, Enemy* enemy)
{
	if (enemy->GetSecurityStatistics() != NULL)
		enemy->GetSecurityStatistics()->Dump(GetSecurityStatistics());

	if (pScene->GetSceneState() == SCENE_RUNNING)
	{
		if (!FightGroupMgr::Instance()->CanAttack(KKSG::FightRole, enemy->GetFightGroup()))
		{
			return;
		}
		if (enemy->IsBoss())
		{
			++m_monsterinfo.BossKillCount;
		}
		else	
		{
			++m_monsterinfo.MonsterCount1;
		}
	}
}

void SceneStatistics::OnAddNonRole(Scene *pScene, Unit* unit)
{
	if (unit->IsEnemy())
	{
		if (!FightGroupMgr::Instance()->CanAttack(KKSG::FightRole, unit->GetFightGroup()))
		{
			return;
		}
		++m_monsterinfo.MonsterCount;	
		if (((Enemy*)(unit))->IsBoss())
		{
			++m_monsterinfo.BossCount;	
		}
	}
	else if (unit->IsDummyRole())
	{
		AddDummyRole(unit);	
	}
}

RoleBattleRecord* SceneStatistics::GetRecord(UINT64 roleid)
{
	auto i = m_roleid2record.find(roleid);
	if (i == m_roleid2record.end())
	{
		return NULL;
	}
	else
	{
		return i->second;
	}
}

void SceneStatistics::SetPkBeginRank(UINT64 roleid, UINT32 rank)
{
	RoleBattleRecord* record = GetRecord(roleid);
	if (NULL == record)
	{
		SSWarn<<"not find roleid:"<<roleid<<END;
		return;
	}
	record->start.FillPkRank(ToString(rank));
}

void SceneStatistics::SetPkEndRank(UINT64 roleid, int change, UINT32 rank)
{
	RoleBattleRecord* record = GetRecord(roleid);
	if (NULL == record)
	{
		SSWarn<<"not find roleid:"<<roleid<<END;
		return;
	}
	record->end.FillPk(ToString(change), ToString(rank));
}

void SceneStatistics::SetClientReceived(UINT64 roleid)
{
	RoleBattleRecord* record = GetRecord(roleid);
	if (NULL == record)
	{
		SSWarn<<"client received not find roleid:"<<roleid<<END;
		return;
	}
	record->m_clientreceived = true;
}

void SceneStatistics::AddDummyRole(Unit* unit)
{
	PlayerInfo info;
	info.PlayerOpenID = "robot";
	info.PlayerBattlePoint = unit->GetAttr(TOTAL_POWERPOINT);
	info.PlayerSide = unit->GetFightGroup();
	info.PlayerType = 1;
	info.roleid = unit->GetID();

	m_players[info.roleid] = info;
}

void SceneStatistics::AddItem(UINT64 roleid, UINT32 itemid, UINT32 itemcount)
{
	RoleBattleRecord* record = GetRecord(roleid);
	if (NULL == record)
	{
		SSWarn<<"client received not find roleid:"<<roleid<<END;
		return;
	}
	record->AddItem(itemid, itemcount);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneStatistics::DoRank(UINT32 scenetype, std::vector<Role*>& roles)
{
	if (scenetype == KKSG::SCENE_ABYSSS || scenetype == KKSG::SCENE_NEST || scenetype == KKSG::SCENE_DRAGON
		|| scenetype == KKSG::SCENE_ACTIVITY_ONE || scenetype == KKSG::SCENE_ACTIVITY_TWO)
	{
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = *i;
			const CombatStatistics& statistics = role->GetStatistics();

			m_damagerank.insert(std::make_pair((float)statistics.GetTotalDamage(), role->GetID()));
			m_treatrank.insert(std::make_pair((float)statistics.GetTotalTreat(), role->GetID()));
			m_ppptrank.insert(std::make_pair((UINT32)role->GetAttr(TOTAL_POWERPOINT), role->GetID()));
		}
	}
}

///> 深渊和机器人组队的结算，为了给机器人凑结算数据(目前只支持一个机器人)
DummyRole* SceneStatistics::DoRankRobot(UINT32 scenetype, SceneTeam* team)
{
	if (scenetype == KKSG::SCENE_ABYSSS && NULL != team)
	{
		const std::vector<RoleSummary*>& robots = team->GetRobots();
		if (!robots.empty() && NULL != robots[0])
		{
			DummyRole* robot = DummyRoleManager::Instance()->Find(robots[0]->qwRobotID);
			if (NULL != robot)
			{
				const CombatStatistics& statistics = robot->GetStatistics();
				m_damagerank.insert(std::make_pair((float)statistics.GetTotalDamage(), robot->GetID()));
				m_treatrank.insert(std::make_pair((float)statistics.GetTotalTreat(), robot->GetID()));
				m_ppptrank.insert(std::make_pair((UINT32)robot->GetAttr(TOTAL_POWERPOINT), robot->GetID()));
			}
			return robot;
		}
	}
	return NULL;
}

UINT32 SceneStatistics::GetRank(UINT64 roleid, UINT32 type)
{
	std::multimap<float, UINT64>* temp = NULL;
	if (0 == type)
	{
		temp = &m_damagerank;
	}
	else if (2 == type)
	{
		temp = &m_treatrank;
	}
	if (NULL == temp)
	{
		return 0; 
	}
	UINT32 count = 0;
	for (auto i = temp->begin(); i != temp->end(); ++i)
	{
		if (i->second == roleid)
		{
			if (2 == type && 0 == i->first)
			{
				return (temp->size() - 1);
			}
			else
			{
				return (temp->size() - count - 1);
			}
		}
		++count;
	}
	return count;
}

UINT32 SceneStatistics::GetPPTRank(UINT64 roleid)
{
	std::multimap<UINT32, UINT64>* temp = &m_ppptrank;
	if (NULL == temp)
	{
		return 0;
	}
	UINT32 count = 0;
	for (auto i = temp->begin(); i != temp->end(); ++i)
	{
		if (i->second == roleid)
		{
			if (0 == i->first)
			{
				return (temp->size() - 1);
			}
			else
			{
				return (temp->size() - count - 1);
			}
		}
		++count;
	}
	return count;
}
