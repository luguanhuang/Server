#include "pch.h"
#include "XIdleComponent.h"

XIdleComponent::XIdleComponent(void)
{
	_selfState = XState_Idle;
}

XIdleComponent::~XIdleComponent(void)
{

}

void XIdleComponent::ActionSubscribe()
{
	RegisterAction(XAction_Idle, DelegateCombination(XActionStateComponent<XIdleActionArgs>, OnAction, this));
}

bool XIdleComponent::Begin() 
{ 
	return true;
}

void XIdleComponent::Cancel(XStateDefine next) 
{

};

void XIdleComponent::ActionUpdate(float deltaTime)
{

};
