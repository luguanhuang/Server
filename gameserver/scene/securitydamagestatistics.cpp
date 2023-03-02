#include "pch.h"
#include "securitydamagestatistics.h"
#include "gamelogic/XCombat.h"
#include "securitystatistics.h"
#include "utility/tloggerbattle.h"

void XSecurityDamageStatistics::Reset()
{
	_AttackTotal = 0;
	_AttackMax = 0;
	_AttackMin = MAX_FLOAT_VALUE;
	_AttackCount = 0;
	_CriticalAttackCount = 0;
	_InvalidAttackCount = 0;

	_HurtTotal = 0;
	_HurtMax = 0;
	_HurtMin = MAX_FLOAT_VALUE;
}

void XSecurityDamageStatistics::OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result)
{
	if (result.GetValue() >= 0)
	{
		++_AttackCount;
		_AttackTotal += (float)result.GetValue();
		_AttackMax = std::max((float)result.GetValue(), _AttackMax);

		if (result.GetValue() > 0)
			_AttackMin = std::min((float)result.GetValue(), _AttackMin);

		if ((result.m_flag & DMGFLAG_CRITICAL) != 0)
			++_CriticalAttackCount;

		if (result.m_Result == PJRES_IMMORTAL)
			++_InvalidAttackCount;
	}
}

void XSecurityDamageStatistics::OnReceiveDamage(const HurtInfo& rawInput, const ProjectDamageResult& result)
{
	if (result.GetValue() >= 0)
	{
		_HurtTotal += (float)result.GetValue();
		if (result.GetValue() > 0)
		{
			_HurtMax = std::max((float)result.GetValue(), _HurtMax);
			_HurtMin = std::min((float)result.GetValue(), _HurtMin);
		}
	}
}

void XSecurityDamageStatistics::Merge(const XSecurityDamageStatistics& other)
{
	_AttackTotal += other._AttackTotal;
	_AttackMax = std::max(_AttackMax, other._AttackMax);
	_AttackMin = std::min(_AttackMin, other._AttackMin);
	_AttackCount += other._AttackCount;
	_CriticalAttackCount += other._CriticalAttackCount;
	_InvalidAttackCount += other._InvalidAttackCount;

	_HurtTotal += other._HurtTotal;
	_HurtMax = std::max(_HurtMax, other._HurtMax);
	_HurtMin = std::min(_HurtMin, other._HurtMin);
}

void XSecurityDamageStatistics::SendRoleData(TSecBattleFlow& logFlow)
{
	XSecurityStatistics::Append("PlayerDpsCount", _AttackCount, logFlow.m_data);
	XSecurityStatistics::Append("PlayerAtkMissTotal", _InvalidAttackCount, logFlow.m_data);
	XSecurityStatistics::Append("PlayerCritCount", _CriticalAttackCount, logFlow.m_data);
	XSecurityStatistics::Append("PlayerDamageMax", _AttackMax, logFlow.m_data);
	XSecurityStatistics::Append("PlayerDamageMin", XSecurityStatistics::GetMinValue(_AttackMin), logFlow.m_data);
	XSecurityStatistics::Append("PlayerDpsTotal", _AttackTotal, logFlow.m_data);
}

void XSecurityDamageStatistics::SendEnemyData(const std::string& keywords, TSecBattleFlow& logFlow)
{
	XSecurityStatistics::Append(keywords + "MissCount", _InvalidAttackCount, logFlow.m_data);
	XSecurityStatistics::Append(keywords + "AttackMax", _AttackMax, logFlow.m_data);
	XSecurityStatistics::Append(keywords + "AttackMin", XSecurityStatistics::GetMinValue(_AttackMin), logFlow.m_data);
	XSecurityStatistics::Append(keywords + "AttackTotal", _AttackTotal, logFlow.m_data);
	XSecurityStatistics::Append(keywords + "DamageMax", _HurtMax, logFlow.m_data);
	XSecurityStatistics::Append(keywords + "DamageMin", XSecurityStatistics::GetMinValue(_HurtMin), logFlow.m_data);
	XSecurityStatistics::Append(keywords + "DamageTotal", _HurtTotal, logFlow.m_data);
}
