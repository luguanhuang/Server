#include "pch.h"
#include "skill/ptcc2g_skillbulletfirereport.h"
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
#include "scene/scene.h"
#include "skill/XBulletCore.h"
#include "skill/XBullet.h"
#include "skill/XBulletMgr.h"

// generate by ProtoGen at date: 2015/12/16 14:54:31

void PtcC2G_SkillBulletFireReport::Process(UINT32 dwConnID)
{
	//client never send
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
					Scene* scene = pEntity->GetUnit()->GetCurrScene();

					XBullet* bullet = new XBullet(
						true,
						m_Data.bullettoken(),
						new XBulletCore(
						pEntity->SkillComponent()->CurrentSkill()->Token(),
						pEntity->GetID(), 
						XEntity::ValidEntity(m_Data.target()), 
						core, 
						triggerTime,
						id,
						core->GetSoul()->Result[triggerTime].Result_Bullet_FireAngle + m_Data.additionaldegree()));

					scene->GetBulletMgr()->ShootBullet(bullet, true);
				}
			}
		}
	}*/
}
