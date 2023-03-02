#include "pch.h"
#include "XEnemy.h"
#include "unit/enemy.h"
#include "component/XPresentComponent.h"
#include "component/XQuickTimeEventComponent.h"
#include "component/XAIComponent.h"
#include "component/XNavigationComponent.h"
#include "component/XSuperArmorComponent.h"
#include "component/XWoozyComponent.h"
#include "component/XFlyComponent.h"
#include "component/XManipulationComponent.h"

XEnemy::XEnemy(Unit* pUnit)
	:XEntity(pUnit)
{

}

XEnemy::~XEnemy()
{
	//LogInfo("XEnemy::~XEnemy UnitId[%llu],  this[%x]", GetID(), this);
}

bool XEnemy::Initilize()
{
	XEntity::Initilize();

	//enemy initialization goes here
	//...

	Enemy* enemy = dynamic_cast<Enemy*>(GetUnit());
	if(enemy != NULL)
	{
		switch(enemy->Type())
		{
		case KKSG::Species_Boss:
		case KKSG::Species_Opposer:
		case KKSG::Species_Elite:
			{
				_ai = static_cast<XAIComponent*>(AttachComponent(new XAIComponent()));
				_pnavigation = static_cast<XNavigationComponent*>(AttachComponent(new XNavigationComponent()));				

				AttachComponent(new XManipulationComponent());
			}break;
		default: 
			break;
		}
	}

	if (enemy->GetTemplate()->MaxSuperArmor > 0)
	{
		/*
		 * must be attached after XSkillComponent.
		 */
		AttachComponent(new XSuperArmorComponent());
	}

	AttachComponent(new XWoozyComponent());
	if(GetUnit()->GetFloatingMax() > 0) 
	{
		_pfly = static_cast<XFlyComponent*>(AttachComponent(new XFlyComponent()));
	}

	return true;
}
