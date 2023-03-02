#include "pch.h"
#include "securityscenestatistics.h"
#include "securitydamagestatistics.h"
#include "securityattributestatistics.h"
#include "securityaistatistics.h"
#include "scenestatistics.h"
#include "scene.h"
#include "unit/enemy.h"
#include "securitystatistics.h"

XSecuritySceneStatistics::XSecuritySceneStatistics()
{
	_MonsterDamageInfo = new XSecurityDamageStatistics();
	_BossDamageInfo = new XSecurityDamageStatistics();
	_BossHPInfo = new XSecurityHPStatistics();
	_MonsterHPInfo = new XSecurityHPStatistics();
	_MonsterAIInfo = new XSecurityAIStatistics();
	_BossAIInfo = new XSecurityAIStatistics();
}

XSecuritySceneStatistics::~XSecuritySceneStatistics()
{
	delete _MonsterDamageInfo;
	delete _BossDamageInfo;
	delete _BossHPInfo;
	delete _MonsterHPInfo;
	delete _MonsterAIInfo;
	delete _BossAIInfo;

}

void XSecuritySceneStatistics::Reset()
{
	_MonsterDamageInfo->Reset();
	_BossDamageInfo->Reset();
	_BossHPInfo->Reset();
	_MonsterHPInfo->Reset();
	_BossAIInfo->Reset();
	_MonsterAIInfo->Reset();

	_BossMoveDistance = 0;
	_MonsterMoveDistance = 0;

	_Inited = true;
}

void XSecuritySceneStatistics::Merge(const XSecuritySceneStatistics& other)
{
	_MonsterDamageInfo->Merge(*other._MonsterDamageInfo);
	_BossDamageInfo->Merge(*other._BossDamageInfo);
	_BossHPInfo->Merge(*other._BossHPInfo);
	_MonsterHPInfo->Merge(*other._MonsterHPInfo);
	_BossAIInfo->Merge(*other._BossAIInfo);
	_MonsterAIInfo->Merge(*other._MonsterAIInfo);

	_BossMoveDistance += other._BossMoveDistance;
	_MonsterMoveDistance += other._MonsterMoveDistance;
}

void XSecuritySceneStatistics::SendCurrentData(Scene* pScene, RoleBattleRecord* pBattleRecord)
{
	if (pScene == NULL || pBattleRecord == NULL)
		return;

	const std::unordered_map<UINT64, Unit*>& units = pScene->GetUnits();

	s_Statistics->Reset();

	for (auto it = units.begin(); it != units.end(); ++it)
	{
		if (!it->second->IsEnemy())
			continue;

		Enemy* pEnemy = (Enemy*)(it->second);

		if (pEnemy->GetSecurityStatistics() != NULL)
		{
			pEnemy->GetSecurityStatistics()->MergeToScene(s_Statistics);
		}
	}

	s_Statistics->Merge(*this);

	s_Statistics->SendData(pBattleRecord);
}

void XSecuritySceneStatistics::SendData(RoleBattleRecord* pBattleRecord)
{
	_BossHPInfo->SendData("Boss", pBattleRecord->end);
	_MonsterHPInfo->SendData("Monster", pBattleRecord->end);
	_BossDamageInfo->SendEnemyData("Boss", pBattleRecord->end);
	_MonsterDamageInfo->SendEnemyData("Monster", pBattleRecord->end);
	_BossAIInfo->SendBossData(pBattleRecord->end);
	_MonsterAIInfo->SendMonsterData(pBattleRecord->end);
	XSecurityStatistics::Append("BossMoveTotal", (double)_BossMoveDistance, pBattleRecord->end.m_data);
	XSecurityStatistics::Append("MonsterMoveTotal", (double)_MonsterMoveDistance, pBattleRecord->end.m_data);

}

XSecuritySceneStatistics* XSecuritySceneStatistics::s_Statistics = new XSecuritySceneStatistics();

