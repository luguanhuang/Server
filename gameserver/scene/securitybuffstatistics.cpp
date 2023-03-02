#include "pch.h"
#include "securitybuffstatistics.h"
#include "securitystatistics.h"
#include "buff/XBuff.h"
#include "unit/unit.h"
#include "utility/tloggerbattle.h"
#include <string>


std::set<CombatAttrDef> XSecurityBuffStatistics::_UsefulAttrs;

bool XSecurityBuffStatistics::s_bInit = XSecurityBuffStatistics::_Init();

bool XSecurityBuffStatistics::_Init()
{
	_UsefulAttrs.insert(BASIC_ATTACK_SPEED);
	_UsefulAttrs.insert(PERCENT_ATTACK_SPEED);
	_UsefulAttrs.insert(BASIC_Critical);
	_UsefulAttrs.insert(PERCENT_Critical);
	_UsefulAttrs.insert(BASIC_RUN_SPEED);
	_UsefulAttrs.insert(PERCENT_RUN_SPEED);
	_UsefulAttrs.insert(BASIC_PhysicalDefMod);
	_UsefulAttrs.insert(PERCENT_PhysicalDefMod);
	_UsefulAttrs.insert(BASIC_PhysicalAtkMod);
	_UsefulAttrs.insert(PERCENT_PhysicalAtkMod);
	_UsefulAttrs.insert(BASIC_MagicAtkMod);
	_UsefulAttrs.insert(PERCENT_MagicAtkMod);

	return true;
}

void XSecurityBuffStatistics::BuffInfo::Reset()
{
	_CountTotal = 0;
	_EffectMax = 0;
	_EffectMin = MAX_FLOAT_VALUE;
	_TimeMax = 0;
	_TimeMin = MAX_UINT_VALUE;
	_TimeTotal = 0;
}

void XSecurityBuffStatistics::BuffInfo::OnChanged(XBuff* buff, double value)
{
	++_CountTotal;
	_EffectMax = std::max(_EffectMax, (float)value);
	_EffectMin = std::min(_EffectMin, (float)value);

	UINT32 actualDuration = buff->GetActualDuration();
	_TimeMax = std::max(_TimeMax, actualDuration);
	_TimeMin = std::min(_TimeMin, actualDuration);
	_TimeTotal += actualDuration;
}

void XSecurityBuffStatistics::BuffInfo::Merge(const BuffInfo& other)
{
	_CountTotal += other._CountTotal;
	_EffectMax = std::max(_EffectMax, other._EffectMax);
	_EffectMin = std::min(_EffectMin, other._EffectMin);
	_TimeMax = std::max(_TimeMax, other._TimeMax);
	_TimeMin = std::min(_TimeMin, other._TimeMin);
	_TimeTotal += other._TimeTotal;
}


void XSecurityBuffStatistics::Reset()
{
	_AttackSpeed.Reset();
	_RunSpeed.Reset();
	_Critical.Reset();
	_PhysicalDef.Reset();
	_Immortal.Reset();
	_ReduceDamage.Reset();
	_Freeze.Reset();
}

void XSecurityBuffStatistics::OnAttributeChanged(Unit* pUnit, XBuff* buff, CombatAttrDef attr, double value)
{
	// 只统计技能buff
	if (pUnit == NULL || buff == NULL || buff->GetSkillID() == 0 || value <= 0)
		return;

	switch(attr)
	{
	case BASIC_ATTACK_SPEED:
	case PERCENT_ATTACK_SPEED:
		_AttackSpeed.OnChanged(buff, value);
		//ProcessNormal(ref _AttackSpeed, s_AttrParam);
		break;
	case BASIC_Critical:
	case PERCENT_Critical:
		_Critical.OnChanged(buff, value);
		//ProcessNormal(ref _Critical, s_AttrParam);
		break;
	case BASIC_RUN_SPEED:
	case PERCENT_RUN_SPEED:
		_RunSpeed.OnChanged(buff, value);
		//ProcessNormal(ref _RunSpeed, s_AttrParam);
		break;
	case BASIC_PhysicalDefMod:
	case PERCENT_PhysicalDefMod:
		_PhysicalDef.OnChanged(buff, value);
		//ProcessNormal(ref _PhysicalDef, s_AttrParam);
		break;
	case BASIC_PhysicalAtkMod:
	case PERCENT_PhysicalAtkMod:
	case BASIC_MagicAtkMod:
	case PERCENT_MagicAtkMod:
		_Attack.OnChanged(buff, value);
		//ProcessNormal(ref _Attack, s_AttrParam);
		break;
	default:
		break;
	}
}

void XSecurityBuffStatistics::OnReduceDamage(XBuff* buff, double value)
{
	if (buff == NULL || buff->GetSkillID() == 0)
		return;

	_ReduceDamage.OnChanged(buff, value * 100);
}

void XSecurityBuffStatistics::OnFreeze(XBuff* buff)
{
	if (buff == NULL || buff->GetSkillID() == 0)
		return;

	_Freeze.OnChanged(buff, 0);
}

void XSecurityBuffStatistics::OnImmortal(XBuff* buff)
{
	if (buff == NULL || buff->GetSkillID() == 0)
		return;

	_Immortal.OnChanged(buff, 0);
}

XSecurityBuffStatistics* XSecurityBuffStatistics::TryGetStatistics(Unit *pUnit)
{
	if (pUnit == NULL || pUnit->GetSecurityStatistics() == NULL)
		return NULL;

	return pUnit->GetSecurityStatistics()->_BuffInfo;
}

void XSecurityBuffStatistics::SendRoleData(TSecBattleFlow& logFlow)
{
	_SendData(_PhysicalDef, "1", logFlow);
	_SendData(_Attack, "2", logFlow);
	_SendData(_Critical, "3", logFlow);
	_SendData(_RunSpeed, "4", logFlow);
	_SendData(_AttackSpeed, "5", logFlow);
	_SendData(_ReduceDamage, "6", logFlow);
	_SendData(_Immortal, "7", logFlow);
	_SendData(_Freeze, "8", logFlow);
}

void XSecurityBuffStatistics::_SendData(const BuffInfo& buffInfo, const std::string& keywords, TSecBattleFlow& logFlow)
{
	XSecurityStatistics::Append(("Skill" + keywords + "Count"), buffInfo._CountTotal, logFlow.m_data);
	XSecurityStatistics::Append(("Skill" + keywords + "EffectMin"), XSecurityStatistics::GetMinValue(buffInfo._EffectMin), logFlow.m_data);
	XSecurityStatistics::Append(("Skill" + keywords + "EffectMax"), buffInfo._EffectMax, logFlow.m_data);
	XSecurityStatistics::Append(("Skill" + keywords + "TimeMin"), (int)XSecurityStatistics::GetMinValue(buffInfo._TimeMin), logFlow.m_data);
	XSecurityStatistics::Append(("Skill" + keywords + "TimeMax"), (int)buffInfo._TimeMax, logFlow.m_data);
	XSecurityStatistics::Append(("Skill" + keywords + "TimeTotal"), (int)buffInfo._TimeTotal, logFlow.m_data);
}
