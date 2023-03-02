#ifndef __SECURITYDAMAGESTATISTICS_H__
#define __SECURITYDAMAGESTATISTICS_H__

#include "unit/combatattrdef.h"

struct HurtInfo;
struct ProjectDamageResult;
class TSecBattleFlow;

class XSecurityDamageStatistics
{
public:
	float _AttackTotal;          // ���˺�
	float _AttackMax;            // ����˺����ֵ
	float _AttackMin;            // ����˺���Сֵ
	int _AttackCount;            // �˺�����
	int _CriticalAttackCount;    // ��������
	int _InvalidAttackCount;     // ��Ч��������  ���޵�

	float _HurtTotal;            // ������
	float _HurtMax;              // �յ��˺����ֵ
	float _HurtMin;              // �յ��˺���Сֵ

	void Reset();

	void OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result);

	void OnReceiveDamage(const HurtInfo& rawInput, const ProjectDamageResult& result);

	void Merge(const XSecurityDamageStatistics& other);

	void SendRoleData(TSecBattleFlow& logFlow);

	void SendEnemyData(const std::string& keywords, TSecBattleFlow& logFlow);
};

#endif