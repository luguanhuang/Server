#ifndef __SECURITYATTRIBUTESTATISTICS_H__
#define __SECURITYATTRIBUTESTATISTICS_H__

#include "unit/combatattrdef.h"
class TSecBattleFlow;

class XSecurityHPStatistics
{
public:
	float _TotalValue;
	float _MaxValue;
	float _MinValue;

	void Reset();

	void Merge(float value);
	void Merge(const XSecurityHPStatistics& other);
	void SendData(const std::string& keywords, TSecBattleFlow& logFlow);
};

class XSecurityAttributeStatistics
{
public:
	class AttrInfo
	{
	public:
		float _IncCount;                         // 回复次数
		float _IncMax;                           // 回复最大值
		float _IncMin;                           // 回复最大值
		float _IncTotal;                         // 回复总量
		float _DecCount;                         // 减少次数
		float _DecMax;                           // 减少最大值
		float _DecMin;                           // 减少最大值
		float _DecTotal;                         // 减少总量

		void Reset();

		void OnChange(double delta);
	};

	void Reset();

	bool IsUsefulAttr(CombatAttrDef attr);
	void OnAttributeChange(CombatAttrDef attr, double delta);

	AttrInfo _Hp;
	AttrInfo _Mp;

	void SendData(TSecBattleFlow& logFlow);
private:
	void _SendData(const AttrInfo& info, const std::string& keywords, TSecBattleFlow& logFlow);

};

#endif