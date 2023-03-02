#include "pch.h"

#include "Player_warrior_charge.h"
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

bool Player_warrior_charge::checkInput(XSkill* pskill, float)
{
	if(pskill->Casting())
	{
		if(!pskill->Firer()->IsClientManualOn())
		{
			pskill->Cease();
		}
	}

	return true;
}