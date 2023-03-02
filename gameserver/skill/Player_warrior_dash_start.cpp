#include "pch.h"

#include "Player_warrior_dash_start.h"
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

bool Player_warrior_dash_start::canfirecharge(XSkill* pskill, float)
{
	XCombinedSkill* pCombined = dynamic_cast<XCombinedSkill*>(pskill);

	if(pCombined)
	{
		XSkillCore* core = pskill->Firer()->SkillComponent()->SkillManager()->GetSkill(XCommon::XHash(pCombined->MainCore()->GetSoul()->Combined[2].Name.c_str()));

		if ((!core || core->Level() == 0) ||
			!pskill->Firer()->IsClientManualOn())
		{
			pCombined->ShutDown();
		}
	}

	return true;
}