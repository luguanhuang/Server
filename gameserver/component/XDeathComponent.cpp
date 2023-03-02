#include "pch.h"
#include "XDeathComponent.h"
#include "entity/XObject.h"
#include "XPresentComponent.h"
#include "table/XEntityPresentation.h"
#include "utility/XActionCurve.h"
#include "scene/grid.h"
#include "scene/scene.h"

XDeathComponent::XDeathComponent(void)
	:_curve_h(0),
	 _curve_v(0),
	 _presented(0),
	 _elapsed(0),
	 _last_offset(0),
	 _last_height(0),
	 _deltaH(0),
	 _land_time(0),
	 _land_max(0),
	 _step_dir(Vector3::forward),
	 _destory_token(INVALID_HTIMER)
{
	_selfState = XState_Death;
}

XDeathComponent::~XDeathComponent(void)
{

}

void XDeathComponent::OnGetAction(XRealDeadActionArgs* pAction)
{
	XActionStateComponent<XRealDeadActionArgs>::OnGetAction(pAction);
}

void XDeathComponent::OnAttachToHost(XObject* pHost)
{
	XActionStateComponent<XRealDeadActionArgs>::OnAttachToHost(pHost);
}

void XDeathComponent::Attached()
{
	if (GetEntity()->PresentComponent()->PresentLib()->DeathCurve.size() > 0)
	{
		const string& curve_prefix = GetEntity()->PresentComponent()->CurvePrefix();

		uint size = GetEntity()->PresentComponent()->PresentLib()->DeathCurve.size();

		_curve_h = (size < 1) ? NULL : XActionCurveMgr::Instance()->GetCurve(curve_prefix + GetEntity()->PresentComponent()->PresentLib()->DeathCurve[0]);
		_curve_v = (size < 2) ? NULL : XActionCurveMgr::Instance()->GetCurve(curve_prefix + GetEntity()->PresentComponent()->PresentLib()->DeathCurve[1]);

		_land_time = _curve_h == NULL ? 0 : _curve_h->GetFirstLandValue();
		_land_max = _curve_h == NULL ? 0 :_curve_h->GetMaxValue();
	}
}

void XDeathComponent::OnDetachFromHost()
{
	CTimerMgr::Instance()->KillTimer(_destory_token);
	_destory_token = INVALID_HTIMER;

	_curve_h = 0;
	_curve_v = 0;

	_land_time = 0;
	_land_max = 0;

	XActionStateComponent<XRealDeadActionArgs>::OnDetachFromHost();
}

void XDeathComponent::ActionSubscribe()
{
	RegisterAction(XAction_RealDead, DelegateCombination(XActionStateComponent<XRealDeadActionArgs>, OnAction, this));
	RegisterAction(XAction_Revive, DelegateCombination(XDeathComponent, OnRevive, this));
}

bool XDeathComponent::OnRevive(IArgs* pargs, void *)
{
	if (GetEntity()->StateMachine()->GetCurrentState() == XState_Death)
	{
		CTimerMgr::Instance()->KillTimer(_destory_token);
		_destory_token = INVALID_HTIMER;

		GetEntity()->StateMachine()->ForceToDefaultState(true);
	}
	else
	{
		LogError("OnRevive Failed: [%llu] is not in Death state but state %d", GetEntity()->GetID(), GetEntity()->StateMachine()->GetCurrentState());
	}

	return true;
}

bool XDeathComponent::OnDestroy(IArgs*, void*)
{
	_entity->Died();

	_destory_token = INVALID_HTIMER;
	return true;
}

bool XDeathComponent::Begin()
{
	_entity->SyncIncreament();

	CTimerMgr::Instance()->KillTimer(_destory_token);
	_destory_token = INVALID_HTIMER;

	if(_curve_v)
	{
		_destory_token = XCommon::SetTimer(_curve_v->GetTimeSpan(), DelegateCombination(XDeathComponent, OnDestroy, this), NULL, NULL, __FILE__, __LINE__);

		XEntity* killer = XEntity::ValidEntity(_entity->GetUnit()->GetKillerID());

		_step_dir = killer ?
			(_land_max > 0 ? Vector3::Horizontaled(_entity->GetPosition_p() - killer->GetPosition_p()) : _entity->GetBack()) :
			_entity->GetBack();

		const Vector3& dest = _entity->GetPosition_p() + _curve_v->Evaluate(_land_time) * _entity->GetUnit()->GetScale() * _step_dir;

		float y = 0;
		Scene* pScene = _entity->GetUnit()->GetCurrScene();
		if(pScene)
		{
			if(pScene->GetGrid()->TryGetHeight(dest.x, dest.z, y))
			{
				_deltaH = _entity->GetPosition_p().y - y;
			}
			else
				_deltaH = 0;

			if(_deltaH < 0) _deltaH = 0;

			_last_offset = 0;
			_last_height = 0;

			_elapsed = 0;

			_entity->Dying();
		}
		else
		{
			 return false;
		}
	}
	else
	{
		OnDestroy(0, 0);
	}

	return true;
}

void XDeathComponent::Cancel(XStateDefine next) 
{
	CTimerMgr::Instance()->KillTimer(_destory_token);
	_destory_token = INVALID_HTIMER;

	_land_time = 0;
	_land_max = 0;
};

void XDeathComponent::ActionUpdate(float deltaTime)
{
	_elapsed += deltaTime;

	float delta_x = 0;
	float delta_y = 0;
	float delta_z = 0;

	if(!_curve_v || !_curve_h) return;

	float c_v = _curve_v->Evaluate(_elapsed) * GetEntity()->GetUnit()->GetScale();
	float c_h = _curve_h->Evaluate(_elapsed) * GetEntity()->GetUnit()->GetScale();

	const Vector3& v = _step_dir * (c_v - _last_offset);

	delta_x = v.x;
	delta_y = c_h - _last_height;
	delta_z = v.z;

	_last_height = c_h;
	_last_offset = c_v;

	float deltaH = _land_time > 0 ? ((_elapsed < _land_time) ? -(_deltaH / _land_time) * deltaTime : -sqrt(delta_x * delta_x + delta_z * delta_z)) : 0;

	if(_elapsed < _land_time) GetEntity()->DisableGravity();

	GetEntity()->ApplyMove(delta_x, delta_y + deltaH, delta_z);
};