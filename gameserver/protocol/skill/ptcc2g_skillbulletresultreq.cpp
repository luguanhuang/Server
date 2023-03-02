#include "pch.h"
#include "skill/ptcc2g_skillbulletresultreq.h"
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

// generate by ProtoGen at date: 2015/12/16 15:27:28

void PtcC2G_SkillBulletResultReq::Process(UINT32 dwConnID)
{
	//client never send.
	return;

	/*Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		//validate check
		//...
		if (pRole->GetCurrScene() == NULL || pRole->IsFishing())
		{
			return;
		}

		XEntity* pEntity = dynamic_cast<XEntity*>(pRole->GetXObject());

		if(pEntity)
		{
			Scene* scene = pEntity->GetUnit()->GetCurrScene();

			UINT64 id = m_Data.bulletid();
			XBullet* bullet = scene->GetBulletMgr()->GetClientBullet(KeyOfBullet(pEntity->GetID(), id));

			if(bullet)
			{
				vector<Unit*> units;
				Vector3 resultForward = Vector3::FloatToAngle(m_Data.resultforward());

				for (int i = 0; i < m_Data.targetlist().size(); i++)
				{
					XEntity* pTarget = XEntity::ValidEntity(m_Data.targetlist(i));
					if(pTarget && pTarget->GetUnit()->GetCurrScene()) units.push_back(pTarget->GetUnit());
				}

				XBulletCore* bullet_core = bullet->GetBulletCore();

				if(bullet_core)
				{
					if(m_Data.iscollided())
					{
						if(units.size() > 0)
						{
							XSkill::SkillResult(
								bullet_core->Token(),
								pEntity,
								bullet_core->GetSkillCore(),
								bullet,
								bullet_core->GetSequnce(),
								bullet_core->ResultID(),
								bullet_core->ResultTime(),
								resultForward,
								dynamic_cast<XEntity*>(units[0]->GetXObject()),
								NULL);
						}
					}
					else
					{
						Vector3 resultAt(m_Data.resultat());

						//trigger at end...
						bullet->DeActive();

						XSkill::SkillResult(
							bullet_core->Token(),
							pEntity,
							bullet_core->GetSkillCore(),
							bullet,
							bullet_core->GetSequnce(),
							bullet_core->ResultID(),
							bullet_core->ResultTime(),
							resultForward,
							resultAt,
							units,
							true);
					}
				}
			}
		}
	}*/
}
