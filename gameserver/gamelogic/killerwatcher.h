#ifndef __KILLERWATCHER_H__
#define __KILLERWATCHER_H__

#include "unit/combatattrdef.h"

class Unit;

struct KillerWatchParam
{
	UINT64 KillerID;
	UINT32 KillerSkill;
	UINT32 KillerBuff;
	Unit* pUnit;

	KillerWatchParam(Unit* pWatchedUnit)
		: KillerID(0)
		, KillerSkill(0)
		, KillerBuff(0)
		, pUnit(pWatchedUnit)
	{

	}
};

// ������Եı仯��֪ͨ���ͻ���
class KillerWatcher
{
public:

	KillerWatcher();

	void Reset();

	UINT64 GetKillerID() { return m_KillerID; }
	UINT64 GetExecutionerID() { return m_ExecutionerID; }
	UINT64 GetKillerBuff() { return m_KillerBuff; }
	UINT64 GetKillerSkill() { return m_KillerSkill; }

	static void CheckKiller(const KillerWatchParam& param, double deltaHP);
private:

	UINT64 m_KillerID;//Ļ�����Unit(ֻ����skill��buff)	
	UINT64 m_ExecutionerID;//����ִ��Unit(ֻ����skill��buff)	������ٻ�����KillerID��ͬ
	UINT32 m_KillerSkill;
	UINT32 m_KillerBuff;
};

#endif // __KILLERWATCHER_H__