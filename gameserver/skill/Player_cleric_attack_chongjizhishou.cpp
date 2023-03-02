#include "pch.h"

#include "Player_cleric_attack_chongjizhishou.h"
#include "XSkill.h"
#include "scene/scene.h"
#include "unit/unit.h"
#include "XSkillCore.h"
#include "entity/XObject.h"
#include "entity/XEntity.h"
#include "math/XVector3.h"
#include "XCombinedSkill.h"
#include "component/XSkillComponent.h"
#include "XSkillMgr.h"
#include "util/XCommon.h"
#include "gamelogic/skilldefine.h"

bool Player_cleric_attack_chongjizhishou::CallBomber(XSkill* pskill, float)
{
	std::vector<UINT64>::const_iterator it = pskill->Firer()->SkillComponent()->GetSkillMobs().begin();

	while (it != pskill->Firer()->SkillComponent()->GetSkillMobs().end())
	{
		XEntity* pEnt = XEntity::ValidEntity(*it);
		if(pEnt)
		{
			XSkill::SkillResult(
				pskill->Token(), 
				pskill->Firer(), 
				pskill->MainCore(), 
				pskill->MainCore()->GetSoul()->Result[0].Index, 
				pskill->MainCore()->ID(), 
				pskill->MainCore()->GetSoul()->Result[0].Token, 
				pEnt->GetFace_p(), 
				pEnt->GetPosition_p());

			pskill->MainCore()->ClearHurtTarget();
		}

		++it;
	}

	return false;
}