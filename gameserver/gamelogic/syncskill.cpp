#include "pch.h"
#include "syncskill.h"
#include "pb/project.pb.h"

void SkillSetSyncToOtherPlayer::Add(const skillInfo &skill)
{
	m_skills.push_back(skill);
}

void SkillSetSyncToOtherPlayer::AddBind(UINT32 skillid)
{
	m_bindskills.push_back(skillid);
}

void SkillSetSyncToOtherPlayer::SaveToUnitAppearance(KKSG::UnitAppearance *pAppearance)
{
	for (int i = 0; i < m_skills.size(); ++i)
	{
		KKSG::SkillInfo *pInfo = pAppearance->add_skills();
		pInfo->set_skillhash(m_skills[i].skillhash);
		pInfo->set_skilllevel(m_skills[i].skilllevel);
		pInfo->set_skillpoint(m_skills[i].skillpoint);
	}
	for (int j = 0; j < m_bindskills.size(); ++j)
	{
		pAppearance->add_bindskills(m_bindskills[j]);
	}
}

void SkillSetSyncToOtherPlayer::FillPreloadRoleData(KKSG::PreloadRoleData* data)
{
	if (!data)
	{
		return;
	}
	for (unsigned i = 0; i < m_skills.size(); ++i)
	{
		data->add_skillhash(m_skills[i].skillhash);
	}
}
