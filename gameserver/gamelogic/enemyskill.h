#ifndef __ENEMYSKILL_H__
#define __ENEMYSKILL_H__

#include "skilldefine.h"
#include "SkillList.h"

class Unit;


class EnemySkillSet : public ISkillSet
{
public:
	virtual UINT32 GetSkillLevel(UINT32 skillhash);

	virtual void RefreshMobLinkedLevels(Unit* pUnit, Unit* pHoster);
private:
	void SetLinkedLevel(UINT32 id, SkillList::RowData* rowData, ISkillSet* pHosterSkillSet, UINT32 casterTypeID);
};

#endif // __ENEMYSKILL_H__

