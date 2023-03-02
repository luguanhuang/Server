#include "pch.h"
#include "enemyskill.h"
#include "skill/XSkillCore.h"
#include "table/XSkillEffectMgr.h"
#include "entity/XEntity.h"
#include "skill/XSkillMgr.h"
#include "tablemgr/XEntityInfoLibrary.h"


UINT32 EnemySkillSet::GetSkillLevel(UINT32 skillhash)
{
	auto it = m_LinkedLevels.find(skillhash);
	if (it != m_LinkedLevels.end())
		return it->second;

	return 1; 
}

void EnemySkillSet::RefreshMobLinkedLevels(Unit* pUnit, Unit* pHoster)
{
	m_LinkedLevels.clear();

	if (pUnit == NULL || pHoster == NULL)
		return;

	ISkillSet* pHosterSkillSet = pHoster->GetSkillLevelMgr();
	if (pHosterSkillSet == NULL)
		return;

	const XEntityPresentation::RowData* _present_data = XEntityInfoLibrary::Instance()->PresentID2Data(pUnit->GetPresentID());
	if (NULL == _present_data)
		return;

	vector<SkillList::RowData*> skillsHavingEx;

	for (auto it = _present_data->OtherSkills.begin(); it != _present_data->OtherSkills.end(); ++it)
	{
		UINT32 skillID = XCommon::XHash(it->c_str());
		if (skillID == 0 || (*it) == "E")
			continue;

		SkillList::RowData* rowData = XSkillEffectMgr::Instance()->GetSkillConfig(skillID, 1, pUnit->GetSkillCasterTypeID());
		if (rowData == NULL)
			continue;

		///> 有Ex技能，先放着，看Ex技能有无学会
		if(!rowData->ExSkillScript.empty())
		{
			skillsHavingEx.push_back(rowData);
			continue;
		}

		SetLinkedLevel(skillID, rowData, pHosterSkillSet, pUnit->GetSkillCasterTypeID());
	}

	for (auto it = skillsHavingEx.begin(); it != skillsHavingEx.end(); ++it)
	{
		SkillList::RowData* rowData = *it;

		UINT32 skillID = XCommon::XHash(rowData->SkillScript.c_str());
		///> 有学Ex技能，要屏蔽此技能，LinkLevel设置为0
		if (GetSkillLevel(XCommon::XHash(rowData->ExSkillScript.c_str())) > 0)
		{
			m_LinkedLevels[skillID] = 0;
			continue;
		}

		SetLinkedLevel(skillID, rowData, pHosterSkillSet, pUnit->GetSkillCasterTypeID());
	}
}

void EnemySkillSet::SetLinkedLevel(UINT32 id, SkillList::RowData* rowData, ISkillSet* pHosterSkillSet, UINT32 casterTypeID)
{
	if (rowData == NULL || pHosterSkillSet == NULL || rowData->LinkedSkill.empty())
		return;

	UINT32 linkedHash = XCommon::XHash(rowData->LinkedSkill.c_str());
	UINT32 hosterLevel = pHosterSkillSet->GetSkillLevel(linkedHash);
	///> 关联技能没学，直接等于0
	if	(hosterLevel == 0)
	{
		m_LinkedLevels[id] = 0;
		return;
	}
	UINT32 linkedPreHash = XSkillEffectMgr::Instance()->GetPreSkill(linkedHash, casterTypeID);
	UINT32 actualLevel = hosterLevel;
	///> 关联技能是Ex技能，读其基础技能的等级
	if(linkedPreHash != 0)
	{
		actualLevel = pHosterSkillSet->GetSkillLevel(linkedPreHash);
	}
	m_LinkedLevels[id] = std::min((UINT32)rowData->SkillLevel, actualLevel);
}