#include "pch.h"
#include "allyskill.h"
#include "rolesummarymgr.h"
#include "entity/XObject.h"
#include "skill/XSkillCore.h"
#include "table/XSkillEffectMgr.h"
#include "entity/XEntity.h"
#include "skill/XSkillMgr.h"


void DummyRoleSkillSet::InitFromRoleSummary(RoleSummary *pSummary, XObject *pObj)
{
	m_skills = pSummary->oSkills.m_skills;
}

UINT32 DummyRoleSkillSet::GetSkillLevel(UINT32 skillhash)
{
	auto it = m_LinkedLevels.find(skillhash);
	if (it != m_LinkedLevels.end())
		return it->second;

	for (unsigned int i = 0; i < m_skills.size(); ++i)
	{
		if (m_skills[i].skillhash == skillhash)
		{
			return m_skills[i].skilllevel;
		}
	}

	return 0;
}

void DummyRoleSkillSet::RefreshSelfLinkedLevels(Unit* pUnit)
{
	m_LinkedLevels.clear();
	if (pUnit == NULL)
		return;

	const std::vector<XSkillCore*>& skillList = static_cast<XEntity*>(pUnit->GetXObject())->SkillManager()->GetSkillOrder();

	for (auto it = skillList.begin(); it != skillList.end(); ++it)
	{
		SkillList::RowData* rowData = XSkillEffectMgr::Instance()->GetSkillConfig((*it)->ID(), 1, pUnit->GetSkillCasterTypeID());
		if (rowData == NULL)
			continue;

		if (rowData->LinkedSkill != "")
		{
			UINT32 linkedHash = XCommon::XHash(rowData->LinkedSkill.c_str());

			for (unsigned int i = 0; i < m_skills.size(); ++i)
			{
				if (m_skills[i].skillhash == linkedHash)
				{
					m_LinkedLevels[(*it)->ID()] = m_skills[i].skilllevel;
					break;
				}
			}
		}
	}
}
