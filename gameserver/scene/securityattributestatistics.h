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
		float _IncCount;                         // �ظ�����
		float _IncMax;                           // �ظ����ֵ
		float _IncMin;                           // �ظ����ֵ
		float _IncTotal;                         // �ظ�����
		float _DecCount;                         // ���ٴ���
		float _DecMax;                           // �������ֵ
		float _DecMin;                           // �������ֵ
		float _DecTotal;                         // ��������

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