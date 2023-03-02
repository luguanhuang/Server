#ifndef __SECURITYAISTATISTICS_H__
#define __SECURITYAISTATISTICS_H__

#include "unit/combatattrdef.h"

class TSecBattleFlow;

class XSecurityAIStatistics
{
public:
	int _PhysicalAttackNum;    
	int _SkillAttackNum;  

	float _LifeTime;

	int _BossCallMonsterTotal; // 召唤小怪总数
	int _BossCallMonsterCount; // 召唤小怪次数

	UINT32 _call_time;

	void Reset();

	void Merge(const XSecurityAIStatistics& other);

	inline void OnPhysicalAttack()
	{
		++_PhysicalAttackNum;
	}

	inline void OnSkillAttack()
	{
		++_SkillAttackNum;
	}

	void OnCallMonster();

	inline void OnExternalCallMonster()
	{
		++_BossCallMonsterTotal;
		++_BossCallMonsterCount;
	}

	inline void SetLifeTime(float life)
	{
		_LifeTime = life;
	}

	void SendBossData(TSecBattleFlow& logFlow);
	void SendMonsterData(TSecBattleFlow& logFlow);
};

#endif