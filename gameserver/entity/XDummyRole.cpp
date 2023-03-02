#include "pch.h"
#include "XDummyRole.h"
#include "unit/dummyrole.h"
#include "component/XPresentComponent.h"
#include "component/XQuickTimeEventComponent.h"
#include "component/XAIComponent.h"
#include "component/XNavigationComponent.h"
#include "component/XWoozyComponent.h"

XDummyRole::XDummyRole(Unit* pUnit)
	:XEntity(pUnit)
{
}

XDummyRole::~XDummyRole()
{

}

bool XDummyRole::Initilize()
{
	XEntity::Initilize();

	_pqte = static_cast<XQuickTimeEventComponent*>(AttachComponent(new XQuickTimeEventComponent()));
	_ai = static_cast<XAIComponent*>(AttachComponent(new XAIComponent()));
	_pnavigation = static_cast<XNavigationComponent*>(AttachComponent(new XNavigationComponent()));				

	return true;
}
