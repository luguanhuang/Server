#include "pch.h"
#include "skill/ptcc2g_castskill.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "entity/XEntity.h"
#include "entity/XActionDefine.h"
#include "util/XCommon.h"
#include "timeutil.h"
#include "scene/scene.h"
#include "component/XActionReceiver.h"
#include "skill/XSkillMgr.h"

// generate by ProtoGen at date: 2014/12/3 19:53:01

void PtcC2G_CastSkill::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		if (pRole->GetCurrScene() == NULL) return;

		XEntity* entity = dynamic_cast<XEntity*>(pRole->GetXObject());

		if(entity)
		{
			if(m_Data.has_skillid())
			{
				if (!pRole->GetCurrScene()->CanCastSkill())
				{
					XSkillMgr* pSkillmgr = entity->SkillManager();

					if(!pRole->GetCurrScene()->CanCastNormalSkill() || !pSkillmgr || !pSkillmgr->IsPhysicalAttack(m_Data.skillid()))
						return;
				}

				if(m_Data.has_manualface())
					XActionReceiver::OnSkillReceived(pRole, m_Data.target(), m_Data.skillid(), m_Data.has_slot() ? m_Data.slot() : -1, true, m_Data.manualface() / 10.0f);
				else
					XActionReceiver::OnSkillReceived(pRole, m_Data.target(), m_Data.skillid(), m_Data.has_slot() ? m_Data.slot() : -1);
			}
			else
			{
				if (!pRole->GetCurrScene()->CanCastSkill())
				{
					if(!pRole->GetCurrScene()->CanCastNormalSkill() || m_Data.slot() != 0)
						return;
				}

				if(m_Data.has_manualface())
					XActionReceiver::OnSkillSlotReceived(pRole, m_Data.target(), m_Data.slot(), true, m_Data.manualface() / 10.0f);
				else
					XActionReceiver::OnSkillSlotReceived(pRole, m_Data.target(), m_Data.slot());
			}
		}
	}
}
