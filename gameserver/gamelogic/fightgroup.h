#ifndef __FIGHTGROUP_H__
#define __FIGHTGROUP_H__

#include "table/FightGroupTable.h"


class FightGroupMgr
{
	FightGroupMgr();
	~FightGroupMgr();
	DECLARE_SINGLETON(FightGroupMgr);
public:
	bool Init();
	void Uninit();
	// can attack has direction, func returns whether beattacked can attack attacker	
	bool CanAttack(UINT32 beattacked, UINT32 attacker);
	bool InGroup(UINT32 group, std::set<UINT32>& groups);
	inline UINT32 GetGroupMax() { return m_groupmax; };

	void Test();

private:
	UINT32 m_groupmax;
	int** m_fightgroups;
	FightGroupTable m_table;
};

#endif