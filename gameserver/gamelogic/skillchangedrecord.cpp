#include "pch.h"
#include "skillchangedrecord.h"
#include "unit/role.h"
#include "unit/unit.h"
#include "entity/XActionDefine.h"
#include "entity/XObject.h"
#include "define/itemdef.h"
#include "gamelogic/bag.h"

SkillChangedRecord::SkillChangedRecord(Role *pRole)
{
	m_slotchanged = false;
	m_changed = false;
	m_pRole = pRole;
	m_skillpoint = m_pRole->GetSkillPoint();

	pRole->GetRoleSkillLevelMgr()->SetListener(this);
}

SkillChangedRecord::~SkillChangedRecord()
{
	if (m_pRole->GetRoleSkillLevelMgr()->GetListener() == this)
	{
		m_pRole->GetRoleSkillLevelMgr()->SetListener(NULL);
	}
}

void SkillChangedRecord::OnAddSkill(UINT32 skillHash, int skillLevel)
{
	m_changed = true;
	m_oPtcNtf.m_Data.add_newskill(skillHash);
	m_oPtcNtf.m_Data.add_newskilllevel(skillLevel);

	XAddSkillEventArgs args;
	args.SkillHash = skillHash;
	m_pRole->GetXObject()->DispatchAction(&args);
}

void SkillChangedRecord::OnRemoveSkill(UINT32 skillHash)
{
	m_changed = true;
	m_oPtcNtf.m_Data.add_removeskill(skillHash);

	XRemoveSkillEventArgs args;
	args.SkillHash = skillHash;
	m_pRole->GetXObject()->DispatchAction(&args);
}

void SkillChangedRecord::OnChangedSkillLevel(UINT32 skillHash, int skillLevel)
{
	m_changed = true;
	m_oPtcNtf.m_Data.add_changedskillhash(skillHash);
	m_oPtcNtf.m_Data.add_changedskilllevel(skillLevel);
}

void SkillChangedRecord::SendNtf()
{
	if(m_changed || m_skillpoint != m_pRole->GetSkillPoint())
	{
		m_oPtcNtf.m_Data.set_skillpoint(m_pRole->GetSkillPoint());
		UINT32 pointtwo = m_pRole->Get<RoleSkillSet>()->GetSkillPointTypeTwo();
		m_oPtcNtf.m_Data.set_skillpointtwo((UINT32)m_pRole->Get<Bag>()->CountVirtualItem(pointtwo));

		if (m_slotchanged)
		{
			RoleSkillSet *pSkillMgr = m_pRole->GetRoleSkillLevelMgr();
			for (int i = 0; i < KKSG::Attack_Max; ++i)
			{
				m_oPtcNtf.m_Data.add_skillslot(pSkillMgr->GetBindSkill(i));
			}
		}

		m_pRole->Send(m_oPtcNtf);
		m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Skill);
	}
}

void SkillChangedRecord::OnSkillSlotChanged()
{
	m_slotchanged = true;
}

