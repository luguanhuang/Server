#ifndef __SYNCSKILL_H__
#define __SYNCSKILL_H__

#include "skilldefine.h"

namespace KKSG
{
	class UnitAppearance;
	class PreloadRoleData;
}

class SkillSetSyncToOtherPlayer
{
	friend class DummyRoleSkillSet;
public:
	void Add(const skillInfo &skill);
	void AddBind(UINT32 skillid);

	const std::vector<UINT32>& GetBindSkills() { return m_bindskills; }
	const std::vector<skillInfo>& GetSkills() { return m_skills; }

	void Clear() { m_skills.clear(); m_bindskills.clear();}

	void SaveToUnitAppearance(KKSG::UnitAppearance *pAppearance);
	void FillPreloadRoleData(KKSG::PreloadRoleData* data);

private:
	std::vector<UINT32> m_bindskills;
	std::vector<skillInfo> m_skills;
};

#endif // __SYNCSKILL_H__
