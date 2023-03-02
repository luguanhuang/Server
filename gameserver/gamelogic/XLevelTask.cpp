#include "pch.h"
#include "XLevelTask.h"
#include "XLevelWave.h"
#include "unit/enemymanager.h"
#include "unit/enemy.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "scene/scene.h"
#include "scene/ptcg2c_executelevelscriptntf.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "table/GuildGoblinTable.h"
#include "XLevelScriptMgr.h"
#include "XLevelSpawnMgr.h"
#include "rolesummarymgr.h"
#include "robotconfig.h"
#include "unit/dummyrolemanager.h"
#include "unit/dummyrole.h"
#include "table/expeditionconfigmgr.h"
#include "bossrushmgr.h"
#include "dragonexpbase.h"
#include "scene/scenebossrush.h"

XLevelSpawnTask::XLevelSpawnTask(XLevelSpawner* pSpawner):XLevelBaseTask(pSpawner)
{
	m_EnemyID = 0;
	m_MonsterRotate = 0;
	m_MonsterIndex = 0;
	m_scale = 1.0f;
}

void XLevelSpawnTask::Execute(float time)
{
	XLevelWaveInfo* pWaveInfo = m_pSpawner->GetLevelWaveInfo(m_Id);
	if (pWaveInfo == NULL) return;

	XLevelDynamicInfo* pdInfo = &pWaveInfo->m_dynamicWaveInfo;

	//XLevelWave * levelWave = XLevelSpawnMgr::Instance()->GetWaveInfo(m_pSpawner->GetScene()->GetSceneTemplateID(), m_Id);
	XLevelWave* levelWave = &pWaveInfo->m_staticLevelWaveInfo;
	if (!levelWave)
	{
		return;
	}
	if (levelWave->m_SpawnType == Spawn_Source_Doodad)
	{
		pdInfo->m_GenerateCount++;
		m_pSpawner->GetScene()->GenerateDoodad(m_EnemyID, m_Id, m_MonsterPos);
		return;
	}

	//pvp刷对手机器人
	if(Spawn_Source_Player == levelWave->m_SpawnType && levelWave->m_RobotLookupID)
	{
		/*DummyRole* dummy = AddRobot(levelWave->m_RobotLookupID);
		if(dummy)
		{
			pdInfo->m_GenerateCount++;
			pdInfo->m_dummyIds.push_back(dummy->GetID());

			dummy->SetWaveID(m_Id);
			dummy->SetSpawner(m_pSpawner);
		}*/
	}
	else
	{
		Enemy* pUnit = EnemyManager::Instance()->CreateEnemy(m_EnemyID, m_MonsterPos, (float)m_MonsterRotate, m_scale);

		if(pUnit)
		{
			pUnit->SetWaveID(m_Id);
			pUnit->SetSpawner(m_pSpawner);

			pdInfo->m_GenerateCount++;
			pdInfo->m_enemyIds.push_back(pUnit->GetID());

			if(m_pSpawner->GetScene())
			{
				m_pSpawner->GetScene()->ChangeAttrBeforEnterScene(pUnit);

				m_pSpawner->GetScene()->AddUnitAndNotify(pUnit);
			}
		}
	}

	
	if (pdInfo->m_GenerateCount == pdInfo->m_TotalCount)
	{
		pdInfo->m_GenerateTime = time;
	}
}

XLevelScriptTask::XLevelScriptTask(XLevelSpawner* pSpawner):XLevelBaseTask(pSpawner)
{

}

void XLevelScriptTask::Execute(float time)
{
	m_pSpawner->GetLevelScript()->Execute(m_strScriptName);
}

XLevelRoleScriptTask::XLevelRoleScriptTask(XLevelSpawner* pSpawner):XLevelBaseTask(pSpawner)
{
	m_isReconnect = false;
	m_scene = NULL;
	m_role = NULL;
}

void XLevelRoleScriptTask::Execute(float time)
{
	if (!m_isReconnect)
	{
		m_pSpawner->GetLevelScript()->Execute(m_role, m_strScriptName);
	}
	else
	{
		m_pSpawner->GetLevelScript()->ReconnectExecute(m_role, m_strScriptName);
	}
}
