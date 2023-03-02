#ifndef __ALLYSKILL_H__
#define __ALLYSKILL_H__

#include "skilldefine.h"

class RoleSummary;
class XObject;
class XSkillCore;

class DummyRoleSkillSet : public ISkillSet
{
public:
	void InitFromRoleSummary(RoleSummary *pSummary, XObject *pObj);
	virtual UINT32 GetSkillLevel(UINT32 skillhash);
	std::vector<skillInfo>& GetSkills() { return m_skills; }
	virtual void RefreshSelfLinkedLevels(Unit* pUnit);

private:
	std::vector<skillInfo> m_skills;
};

#endif // __ALLYSKILL_H__