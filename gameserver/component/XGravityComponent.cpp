#include "pch.h"
#include "XGravityComponent.h"
#include "entity/XEntity.h"

XGravityComponent::XGravityComponent(void)
{

}

XGravityComponent::~XGravityComponent(void)
{

}

void XGravityComponent::Update(float fDeltaT)
{
	//gravity
	if (!GetEntity()->GravityDisabled())
	{
		GetEntity()->ApplyMove(0, -1, 0);
	}
}
