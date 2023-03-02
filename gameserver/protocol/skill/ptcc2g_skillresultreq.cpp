#include "pch.h"
#include "skill/ptcc2g_skillresultreq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "entity/XEntity.h"
#include "entity/XActionDefine.h"
#include "util/XCommon.h"
#include "timeutil.h"
#include "component/XSkillComponent.h"
#include "skill/XSkill.h"
#include "skill/XSkillCore.h"
#include "skill/XSkillMgr.h"

// generate by ProtoGen at date: 2015/12/15 15:06:42

void PtcC2G_SkillResultReq::Process(UINT32 dwConnID)
{
	//client never send.
	return;

	/*Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		//validate check
		//...
		if (pRole->GetCurrScene() == NULL)
		{
			return;
		}

		XEntity* pEntity = dynamic_cast<XEntity*>(pRole->GetXObject());

		if(pEntity)
		{
			UINT32 id = m_Data.skillid();
			if(pEntity->SkillComponent()->IsCasting() && pEntity->SkillComponent()->CurrentSkill()->MainCore()->ID() == id)
			{
				int triggerTime = m_Data.triggertime();
				int resultToken = m_Data.resulttoken();

				XSkillCore* core = XSkillCore::GetSpecificCore(pEntity, pEntity->SkillComponent()->CurrentSkill()->MainCore(), triggerTime, resultToken);

				if(core != NULL)
				{
					vector<Unit*> units;
					Vector3 resultAt(m_Data.resultat());
					Vector3 resultForward = Vector3::FloatToAngle(m_Data.resultforward());

					for (int i = 0; i < m_Data.targetlist().size(); i++)
					{
						XEntity* pTarget = XEntity::ValidEntity(m_Data.targetlist(i));
						if(pTarget && pTarget->GetUnit()->GetCurrScene()) units.push_back(pTarget->GetUnit());
					}

					XSkill::SkillResult(
						pEntity->SkillComponent()->CurrentSkill()->Token(),
						pEntity,
						core,
						NULL,
						triggerTime,
						id,
						resultToken,
						resultForward,
						resultAt,
						units,
						true);
				}
			}
		}
	}*/
}
