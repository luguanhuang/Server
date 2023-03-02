#include "pch.h"
#include "scenekillinfo.h"
#include "scene.h"
#include "unit/role.h"
#include "sceneconfig.h"
#include "table/KillMonsterScoreTable.h"
#include "unit/enemy.h"
#include "battle/ptcg2c_killenemyscorentf.h"
#include "gamelogic/XLevelWave.h"

SceneKill::SceneKill(Scene* scene)
{
	m_pScene = scene;
	m_killEnemyCount.clear();
	m_scoreEnemy.clear();
	m_killEnemyScore = 0;
	m_isEnd = false;

	Init();
}

void SceneKill::Init()
{
	KillMonsterScoreTable& table = SceneConfig::Instance()->GetKillMonsterScoreTable();

	auto data = table.GetBysceneID(m_pScene->GetSceneTemplateID());
	if (!data)
	{
		return;
	}

	for (auto iter = data->monsterScore.begin(); iter != data->monsterScore.end(); ++iter)
	{
		if (iter->seq[0])
		{
			m_scoreEnemy.insert(std::make_pair(iter->seq[0], iter->seq[1]));
		}
	}
}

void SceneKill::AddKillEnemyInfo(Enemy* enemy)
{
	if (!enemy)
	{
		return;
	}

	if (m_isEnd)
	{
		return;
	}

	++m_killEnemyCount[enemy->GetTemplateID()];

	if (IsScoreEnemy(enemy->GetTemplateID()) && m_pScene->GetSpawner() && m_pScene->GetSpawner()->HasWinConditionScore())
	{
		m_killEnemyScore = CalcKillMonsterScore();
		PtcG2C_KillEnemyScoreNtf ntf;
		ntf.m_Data.set_score(m_killEnemyScore);
		m_pScene->Broadcast(ntf);
	}
}

int SceneKill::GetKillMonsterScore()
{
	return m_killEnemyScore;
}

void SceneKill::SetState(bool isEnd)
{
	m_isEnd = isEnd;
}

bool SceneKill::IsScoreEnemy(UINT32 enemyTemplateID)
{
	auto find = m_scoreEnemy.find(enemyTemplateID);
	if (find == m_scoreEnemy.end())
	{
		return false;
	}

	return true;
}

int SceneKill::CalcKillMonsterScore()
{
	return SceneConfig::Instance()->CalcKillMonsterSocre(m_pScene->GetSceneTemplateID(), m_killEnemyCount);
}
