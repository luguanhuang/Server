#include "pch.h"

#include "Player_academic_buff_fuhuo.h"
#include "XSkill.h"
#include "scene/scene.h"

bool Player_academic_buff_fuhuo::ReviveAlly(XSkill* pskill, float)
{
	Scene* pScene = pskill->Firer()->GetUnit()->GetCurrScene();

	if(pScene)
	{
		const vector<Unit*> ally = pScene->GetAllDeadSameGroupRoles(pskill->Firer()->GetUnit()->GetFightGroup());
		if (ally.empty()) return true;

		float dis = FLT_MAX;
		Unit* target = NULL;

		auto it = ally.begin();
		while (it != ally.end())
		{
			Unit* pu = *it;

			if( pu && 
				pu->GetXObject() && 
				pu->IsRole() && 
				pu != pskill->Firer()->GetUnit() && 
				pu->IsDead())
			{
				float d = Vector3::sqrtMagnitude(pu->GetXObject()->GetPosition_p() - pskill->Firer()->GetPosition_p());

				if(d < dis)
				{
					dis = d;
					target = pu;
				}
			}

			++it;
		}

		if(target)
		{
			target->Revive(GetGlobalConfig().SkillReviveHpPer);
			pScene->OnSkillRevive(pskill->Firer()->GetUnit(), target);
		}
	}

	return true;
}