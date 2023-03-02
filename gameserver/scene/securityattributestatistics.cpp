#include "pch.h"
#include "securityattributestatistics.h"
#include "securitystatistics.h"
#include "utility/tloggerbattle.h"


void XSecurityHPStatistics::Reset()
{
	_TotalValue = 0;
	_MaxValue = 0;
	_MinValue = MAX_FLOAT_VALUE;
}

void XSecurityHPStatistics::Merge(float value)
{
	_TotalValue += value;
	_MaxValue = std::max(_MaxValue, value);
	_MinValue = std::min(_MinValue, value);
}

void XSecurityHPStatistics::Merge(const XSecurityHPStatistics& other)
{
	_TotalValue += other._TotalValue;
	_MaxValue = std::max(_MaxValue, other._MaxValue);
	_MinValue = std::min(_MinValue, other._MinValue);
}

void XSecurityHPStatistics::SendData(const std::string& keywords, TSecBattleFlow& logFlow)
{
	XSecurityStatistics::Append((keywords + "InitHPMax"), _MaxValue, logFlow.m_data);
	XSecurityStatistics::Append((keywords + "InitHPMin"), XSecurityStatistics::GetMinValue(_MinValue), logFlow.m_data);
	XSecurityStatistics::Append((keywords + "InitHPTotal"), _TotalValue, logFlow.m_data);
}

void XSecurityAttributeStatistics::AttrInfo::Reset()
{
	_IncCount = 0;
	_IncMax = 0;
	_IncMin = MAX_FLOAT_VALUE;
	_IncTotal = 0;
	_DecCount = 0;
	_DecMax = 0;
	_DecMin = MAX_FLOAT_VALUE;
	_DecTotal = 0;
}

void XSecurityAttributeStatistics::AttrInfo::OnChange(double delta)
{
	if (delta > 0)
	{
		++_IncCount;
		_IncTotal += (float)delta;
		_IncMax = std::max((float)delta, _IncMax);
		_IncMin = std::min((float)delta, _IncMin);
	}
	else
	{
		++_DecCount;
		_DecTotal += (float)(-delta);
		_DecMax = std::max((float)(-delta), _DecMax);
		_DecMin = std::min((float)(-delta), _DecMin);
	}
}

void XSecurityAttributeStatistics::Reset()
{
	_Hp.Reset();
	_Mp.Reset();
}

bool XSecurityAttributeStatistics::IsUsefulAttr(CombatAttrDef attr)
{
	return attr == BASIC_CurrentMP || attr == BASIC_CurrentHP;
}

void XSecurityAttributeStatistics::OnAttributeChange(CombatAttrDef attr, double delta)
{
	if (attr == BASIC_CurrentHP)
		_Hp.OnChange(delta);
	else if (attr == BASIC_CurrentMP)
		_Mp.OnChange(delta);
}

void XSecurityAttributeStatistics::SendData(TSecBattleFlow& logFlow)
{
	_SendData(_Hp, "HP", logFlow);
	_SendData(_Mp, "MP", logFlow);
}

void XSecurityAttributeStatistics::_SendData(const AttrInfo& info, const std::string& keywords, TSecBattleFlow& logFlow)
{
	XSecurityStatistics::Append(("PlayerHeal" + keywords + "Count"), info._IncCount, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerHeal" + keywords + "Max"), info._IncMax, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerHeal" + keywords + "Min"), XSecurityStatistics::GetMinValue(info._IncMin), logFlow.m_data);
	XSecurityStatistics::Append(("PlayerHeal" + keywords + "Total"), info._IncTotal, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerDamage" + keywords + "Count"), info._DecCount, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerDamage" + keywords + "Max"), info._DecMax, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerDamage" + keywords + "Min"), XSecurityStatistics::GetMinValue(info._DecMin), logFlow.m_data);
	XSecurityStatistics::Append(("PlayerDamage" + keywords + "Total"), info._DecTotal, logFlow.m_data);
}
