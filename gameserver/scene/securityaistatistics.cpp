#include "pch.h"
#include "securityaistatistics.h"
#include "time.h"
#include "utility/tloggerbattle.h"
#include "securitystatistics.h"

void XSecurityAIStatistics::Reset()
{
	_PhysicalAttackNum = 0;
	_SkillAttackNum = 0;
	_LifeTime = 0;
	_call_time = 0;

	_BossCallMonsterTotal = 0;
	_BossCallMonsterCount = 0;
}

void XSecurityAIStatistics::Merge(const XSecurityAIStatistics& other)
{
	_PhysicalAttackNum += other._PhysicalAttackNum;
	_SkillAttackNum += other._SkillAttackNum;
	_LifeTime += other._LifeTime;

	_BossCallMonsterTotal += other._BossCallMonsterTotal;
	_BossCallMonsterCount += other._BossCallMonsterCount;
}

void XSecurityAIStatistics::OnCallMonster()
{
	++_BossCallMonsterTotal;

	if ((UINT32)time(NULL) - _call_time > 1)
	{
		++_BossCallMonsterCount;
		_call_time = (UINT32)time(NULL);
	}
}

void XSecurityAIStatistics::SendBossData(TSecBattleFlow& logFlow)
{
	XSecurityStatistics::Append("BossAttackCount", _PhysicalAttackNum, logFlow.m_data);
	XSecurityStatistics::Append("BossUseSkillCount", _SkillAttackNum, logFlow.m_data);
	XSecurityStatistics::Append("BossTimeTotal", (int)(_LifeTime * 1000), logFlow.m_data);
	//XStaticSecurityStatistics.Append("BossMoveTotal", (int)info._BossMoveDist);
	XSecurityStatistics::Append("BossCallCount", _BossCallMonsterCount, logFlow.m_data);
	XSecurityStatistics::Append("BossCallTotal", _BossCallMonsterTotal, logFlow.m_data);
}

void XSecurityAIStatistics::SendMonsterData(TSecBattleFlow& logFlow)
{
	XSecurityStatistics::Append("MonsterAttackCount", _PhysicalAttackNum, logFlow.m_data);
	XSecurityStatistics::Append("MonsterSkillCount", _SkillAttackNum, logFlow.m_data);
	XSecurityStatistics::Append("MonsterTimeTotal", _LifeTime, logFlow.m_data);
}
