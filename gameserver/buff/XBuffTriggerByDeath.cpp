#include "pch.h"
#include "XBuffTriggerByDeath.h"
#include "gamelogic/XCombat.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "entity/XEntity.h"
#include "gamelogic/spriterecord.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/sceneconfig.h"
#include "SceneTable.h"


XBuffTriggerByDeath::XBuffTriggerByDeath(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	m_Type = _GetTriggerParamInt(pBuff->GetBuffInfo(), 0);
	m_Param0 = _GetTriggerParamInt(pBuff->GetBuffInfo(), 1);
}

XBuffTriggerByDeath::~XBuffTriggerByDeath()
{

}

bool XBuffTriggerByDeath::CheckTriggerCondition()
{
	switch(m_Type)
	{
	case 0:
		// 精灵复活触发条件，检测场景能否精灵复活，及若消耗复活次数，是否还有复活次数
		Role* pRole = dynamic_cast<Role*>(GetEntity()->GetUnit());
		if(pRole != NULL)
		{
			if (!pRole->CanSceneRevive())
				break;
			if (m_Param0 == 1)
			{
				// 需要扣场景次数
				if(!pRole->CanReviveLimit())
					break;
			}
			return true;
		}
		break;
	}
	return false;
}

void XBuffTriggerByDeath::_OnTrigger()
{
	switch(m_Type)
	{
	case 0:
		// Mark this entity will revive in 2 sec
		SSInfo << "Trigger by death" << END;
		Role* pRole = dynamic_cast<Role*>(GetEntity()->GetUnit());
		if(pRole != NULL)
		{
			pRole->SetIsReviveLimit(m_Param0 == 1);
			pRole->SetIsReviving(true);
			pRole->SetReviveType(KKSG::ReviveSprite);
		}
		break;
	}
}

void XBuffTriggerByDeath::OnRealDead(const XRealDeadActionArgs* arg)
{
	Trigger();
}
