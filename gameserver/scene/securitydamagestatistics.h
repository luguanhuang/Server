#ifndef __SECURITYDAMAGESTATISTICS_H__
#define __SECURITYDAMAGESTATISTICS_H__

#include "unit/combatattrdef.h"

struct HurtInfo;
struct ProjectDamageResult;
class TSecBattleFlow;

class XSecurityDamageStatistics
{
public:
	float _AttackTotal;          // 总伤害
	float _AttackMax;            // 造成伤害最大值
	float _AttackMin;            // 造成伤害最小值
	int _AttackCount;            // 伤害次数
	int _CriticalAttackCount;    // 暴击次数
	int _InvalidAttackCount;     // 无效攻击次数  如无敌

	float _HurtTotal;            // 总受伤
	float _HurtMax;              // 收到伤害最大值
	float _HurtMin;              // 收到伤害最小值

	void Reset();

	void OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result);

	void OnReceiveDamage(const HurtInfo& rawInput, const ProjectDamageResult& result);

	void Merge(const XSecurityDamageStatistics& other);

	void SendRoleData(TSecBattleFlow& logFlow);

	void SendEnemyData(const std::string& keywords, TSecBattleFlow& logFlow);
};

#endif