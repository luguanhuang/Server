#include "pch.h"

#include "Player_assassin_dash.h"
#include "XSkill.h"
#include "component/XBuffComponent.h"

bool Player_assassin_dash::PowerDash(XSkill* pskill, float)
{
	if(pskill->Firer() && pskill->Firer()->BuffComponent())
	{
		if(pskill->Firer()->BuffComponent()->GetBuffByID(36111))
			pskill->Cease();
	}

	return true;
}