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
		// ���鸴�����������ⳡ���ܷ��鸴��������ĸ���������Ƿ��и������
		Role* pRole = dynamic_cast<Role*>(GetEntity()->GetUnit());
		if(pRole != NULL)
		{
			if (!pRole->CanSceneRevive())
				break;
			if (m_Param0 == 1)
			{
				// ��Ҫ�۳�������
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
