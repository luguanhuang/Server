#include "pch.h"

#include "XFreezeComponent.h"
#include "XFlyComponent.h"
#include "math/XVector3.h"
#include "entity/XEntity.h"
#include "scene/scene.h"
#include "scene/grid.h"
#include "unit/unit.h"
#include "pb/enum.pb.h"

XFreezeComponent::XFreezeComponent(void)
	:_elapsed(0), _height_drop(0)
{
	_selfState = XState_Freeze;
}

XFreezeComponent::~XFreezeComponent(void)
{

}

void XFreezeComponent::ActionSubscribe()
{
	RegisterAction(XAction_Freeze, DelegateCombination(XActionStateComponent<XFreezeActionArgs>, OnAction, this));
}

void XFreezeComponent::OnGetAction(XFreezeActionArgs* pAction)
{
	XActionStateComponent<XFreezeActionArgs>::OnGetAction(pAction);
}

bool XFreezeComponent::Begin()
{
	GetEntity()->SyncIncreament();

	const Vector3& pos = GetEntity()->GetPosition_p();

	float y = _entity->GetUnit()->GetCurrScene()->GetGrid()->GetHeight(pos.x, pos.z);

	_height_drop = pos.y - (_entity->Fly() ? y + _entity->Fly()->CurrentHeight() : y);
	if(_height_drop < 0) _height_drop = 0;

	_elapsed = 0;

	TrytoTirggerQTE();

	if(_args.Duration <= 0) Finish();

	return true;
}

void XFreezeComponent::Cancel(XStateDefine next) 
{
	TrytoTirggerQTE(true);
};

void XFreezeComponent::ActionUpdate(float deltaTime)
{
	if (IsFinished()) return;

	_elapsed += deltaTime;
    /*
     * we do nothing just maintain this state
     */
	float time = _args.Duration;

    float h = (deltaTime * (_height_drop / time));
    GetEntity()->ApplyMove(0, -h, 0);

    if (_elapsed > time) Finish();
};

void XFreezeComponent::TrytoTirggerQTE(bool bEnd)
{
	if(GetEntity()->QuickTimeEventComponent())
	{
		KKSG::XQTEState qte = KKSG::QTE_None;

		if(!bEnd)
		{
			qte = KKSG::QTE_HitFreeze;
		}

		XSkillQTEEventArgs args;
		args.State = qte;
		args.On = !bEnd;
		GetEntity()->DispatchAction(&args);
	}
}
