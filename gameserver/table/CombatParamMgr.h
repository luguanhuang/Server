#ifndef __COMBATPARAMMGR_H__
#define __COMBATPARAMMGR_H__

#include "CombatParamTable.h"
#include "SuperArmorRecoveryCoffTable.h"

class Unit;

class CombatParamMgr
{
	CombatParamMgr();
	~CombatParamMgr();
	DECLARE_SINGLETON(CombatParamMgr)

public:

	bool Init();
	void Uninit();
	CombatParamTable::RowData *GetCombatParamConf(Unit* unit);
	SuperArmorRecoveryCoffTable::RowData *GetSuperArmorCoff(int monsterType, int behitState);

	inline double GetCombatValue(int combatParam, double value) { return value / (value + combatParam); }
private:

	CombatParamTable::RowData *GetCombatParamConf(int Level);

	CombatParamTable m_oTable;
	SuperArmorRecoveryCoffTable m_oSATable;
};

#endif // __COMBATPARAMMGR_H__

