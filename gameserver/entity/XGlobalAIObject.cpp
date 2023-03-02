#include "pch.h"
#include "entity/XGlobalAIObject.h"
#include "unit/unit.h"
#include "component/XAIComponent.h"

XGlobalAIObject::XGlobalAIObject(Unit* pUnit)
	:XEntity(pUnit)
{

}

XGlobalAIObject::~XGlobalAIObject()
{

}

bool XGlobalAIObject::Initilize()
{
	_ai = static_cast<XAIComponent*>(AttachComponent(new XAIComponent()));
	return true;
}

void XGlobalAIObject::Uninitilize()
{

}

void XGlobalAIObject::OnCreated()
{
	//_ai->Attached();
	XEntity::OnCreated();
}