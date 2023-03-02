#ifndef __SECURITYBUFFSTATISTICS_H__
#define __SECURITYBUFFSTATISTICS_H__

#include "unit/combatattrdef.h"

class XBuff;
class Unit;
class TSecBattleFlow;

class XSecurityBuffStatistics
{
	static std::set<CombatAttrDef> _UsefulAttrs;
	static bool s_bInit;
	static bool _Init();

public:
	class BuffInfo
	{
	public:
		int _CountTotal;
		float _EffectMin;
		float _EffectMax;
		UINT32 _TimeMin;
		UINT32 _TimeMax;
		UINT32 _TimeTotal;

		void Reset();

		void OnChanged(XBuff* buff, double value);

		void Merge(const BuffInfo& other);
	};

public:
	bool IsUsefulAttr(CombatAttrDef attr) { return _UsefulAttrs.find(attr) != _UsefulAttrs.end(); }

	void Reset();

	void OnAttributeChanged(Unit* pUnit, XBuff* buff, CombatAttrDef attr, double value);

	void OnReduceDamage(XBuff* buff, double value);

	// WARNING: 此数值，记录的是本Entity使敌人定身的数值，而非本Entity自身的定身数值
	void OnFreeze(XBuff* buff);

	void OnImmortal(XBuff* buff);

	static XSecurityBuffStatistics* TryGetStatistics(Unit *pUnit);

	void SendRoleData(TSecBattleFlow& logFlow);

private:
	void _SendData(const BuffInfo& buffInfo, const std::string& keywords, TSecBattleFlow& logFlow);

public:
	BuffInfo _AttackSpeed;
	BuffInfo _RunSpeed;
	BuffInfo _Critical;
	BuffInfo _PhysicalDef;
	BuffInfo _Attack;
	BuffInfo _Immortal;
	BuffInfo _ReduceDamage;
	BuffInfo _Freeze;
};

#endif