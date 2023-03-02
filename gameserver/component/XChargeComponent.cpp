#include "pch.h"
#include "XChargeComponent.h"
#include "XFlyComponent.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "scene/grid.h"
#include "table/SkillReader.h"
#include "util/XCommon.h"
#include "skill/XSkill.h"
#include "utility/XActionCurve.h"
#include "XRotationComponent.h"
#include "skill/XSkillCore.h"
#include "XBuffComponent.h"

XChargeComponent::XChargeComponent(void)
	:_gravity_disabled(false),
	 _time_elapsed(0),
	 _time_span(0),
	 _land_time(0),
	 _gravity(0),
	 _rticalV(0),
	 _step_speed(0),
	 _offset(0),
	 _height(0),
	 _height_drop(0),
	 _begin_at(Vector3::zero),
	 _step_dir(Vector3::zero),
	 _curve_step_dir(Vector3::zero),
	 _last_offset_forward(0),
	 _last_offset_side(0),
	 _last_offset_up(0),
	 _curve_forward(NULL),
	 _curve_side(NULL),
	 _curve_up(NULL),
	 _distance(FLT_MAX),
	 _target(0)
{
	_selfState = XState_Charge;
}

XChargeComponent::~XChargeComponent(void)
{
	_curve_forward = NULL;
	_curve_side = NULL;
	_curve_up = NULL;
}

void XChargeComponent::ActionSubscribe()
{
	RegisterAction(XAction_Charge, DelegateCombination(XActionStateComponent<XChargeActionArgs>, OnAction, this));
}

void XChargeComponent::OnGetAction(XChargeActionArgs* pAction)
{
	_gravity_disabled = false;

	XActionStateComponent<XChargeActionArgs>::OnGetAction(pAction);
	
	_land_time = 0;

	_distance = FLT_MAX;

	_target = _args.Target;

	if(_args.Data()->Using_Curve)
	{
		_curve_forward = XActionCurveMgr::Instance()->GetCurve(_args.Data()->Curve_Forward);
		_curve_side = XActionCurveMgr::Instance()->GetCurve(_args.Data()->Curve_Side);

		_time_span = _curve_forward->GetTimeSpan() * _args.TimeScale;

		if(_args.Data()->Using_Up)
		{
			_curve_up = XActionCurveMgr::Instance()->GetCurve(_args.Data()->Curve_Up);
			_land_time = _curve_up->GetFirstLandValue() * _args.TimeScale;

			if(_land_time > 0) 
			{
				_entity->DisableGravity();
				_gravity_disabled = true;
			}
		}

		_offset = (_curve_forward->GetLastValue() - _curve_forward->GetFirstValue()) * _entity->GetUnit()->GetScale();
	}
	else
	{
		_time_span = _args.TimeSpan * _args.TimeScale;
		_offset = _args.Data()->Offset;
	}

	_begin_at = GetEntity()->GetPosition_p();

	Calibration();
	HeightDrop();

	_height = _args.Height * _entity->GetUnit()->GetScale();
	_time_elapsed = _args.TimeGone * _args.TimeScale;
}

bool XChargeComponent::Begin() 
{ 
	if(_args.Data()->Using_Curve)
	{
		_last_offset_forward = 0;
		_last_offset_side = 0;
		_last_offset_up = 0;
	}
	else
	{
		_step_speed = _offset / _time_span;

		_rticalV = (_height * 4) / _time_span;
		_gravity = _rticalV / _time_span * 2;
	}

	return true;
}

void XChargeComponent::Cancel(XStateDefine next) 
{
	_time_elapsed = 0;
	_rticalV = 0;
	_gravity = 0;
	_land_time = 0;

	_step_speed = 0;

	_curve_forward = NULL;
	_curve_side = NULL;
	_curve_up = NULL;

	_distance = FLT_MAX;
	_target = 0;
	//#31923
	if(_gravity_disabled) _entity->DisableGravity();
}

void XChargeComponent::ActionUpdate(float deltaTime)
{
	if(_time_elapsed < _time_span)
	{
		Vector3 delta = Vector3::zero;
		float h = 0;

		Calibration();

		if(_args.Data()->Using_Curve)
		{
			GetCurveMove(delta, h, deltaTime);

			if(_args.Data()->Control_Towards && _entity->IsClientManualOn())
			{
				delta += _step_dir * (_args.Data()->Velocity * deltaTime);
			}
		}
		else
		{
			GetNormalMove(delta, h, deltaTime);
		}

		if (_time_elapsed > _time_span)
		{
			_time_elapsed = _time_span;
			Finish();
		}

		h -= _land_time > 0 ? (deltaTime * (_height_drop / _land_time)) : _height_drop;
		if(_args.Data()->Using_Up && _time_elapsed <= _land_time) 
		{
			_gravity_disabled = true;
			_entity->DisableGravity();
		}
		else
		{
			_gravity_disabled = false;
		}

		if (_land_time == 0 && _entity->Fly())
		{
			_height_drop = 0;
		}

		if(_entity->SkillComponent()->CurrentSkill()->MainCore()->GetSoul()->MultipleAttackSupported) CalibrateByMultipleDirection(delta);

		if(GetEntity()->BuffComponent() && GetEntity()->BuffComponent()->IsBuffStateOn(XBuffType_LockFoot)) delta.Zero();

		if(_distance - 0.5f < Vector3::Magnitude(delta)) delta.set(0, delta.y, 0);

		_entity->ApplyMove(delta.x, h, delta.z);
	}
	else
	{
		if(_gravity_disabled) _entity->DisableGravity();
	}
}

void XChargeComponent::Calibration()
{
	XEntity* pTarget = XEntity::ValidEntity(_target);

	if(pTarget)
	{
		Vector3 lookTo = pTarget->GetPosition_p() - GetEntity()->GetPosition_p();
		_distance = Vector3::Magnitude(lookTo);

		XRotationActionArgs rargs;
		rargs.TargetDir = lookTo;
		rargs.SetToken(_entity->SkillComponent()->CurrentSkill()->Token());
		rargs.Palstance = _entity->GetUnit()->GetRotateSpeed();
		_entity->DispatchAction(&rargs);
	}
	else
	{
		_distance = FLT_MAX;
		_target = 0;
	}

	_step_dir = _args.Data()->Control_Towards ? GetControlTowards() : _entity->SkillComponent()->CurrentSkill()->SkillTowardsTo();
	_curve_step_dir = _entity->SkillComponent()->CurrentSkill()->SkillTowardsTo();
}

Vector3 XChargeComponent::GetControlTowards()
{
	if (_entity->IsClientManualOn())
	{
		return _entity->ClientManualForward();
	}
	else
		return _entity->SkillComponent()->CurrentSkill()->SkillTowardsTo();
}

void XChargeComponent::HeightDrop()
{
	if(_args.Data()->StandOnAtEnd)
	{
		Vector3 destination = _begin_at + _offset * _step_dir;

		float y = 0;
		if(GetEntity()->GetUnit()->GetCurrScene()->GetGrid()->TryGetHeight(destination.x, destination.z, y))
		{
			_height_drop = _entity->Fly() ? 0 : _entity->GetPosition_p().y - y;
		}
		else
			_height_drop = 0;

		if(_height_drop < 0) _height_drop = 0;
	}
	else
		_height_drop = 0;
}

void XChargeComponent::GetNormalMove(Vector3& delta, float& h, float deltaTime)
{
	float v1 = _rticalV - _gravity * deltaTime;
	_time_elapsed += deltaTime;
	float v2 = _rticalV - _gravity * deltaTime;

	delta = _step_speed * deltaTime * _step_dir;

	h = (v1 + v2) / 2.0f * deltaTime;

	if(_args.Data()->Rotation_Speed != 0 && !_args.Data()->AimTarget)
	{
		_entity->SetFace(Vector3::HorizontalRotate(_entity->GetFace(), _args.Data()->Rotation_Speed * deltaTime));
	}
}

void XChargeComponent::GetCurveMove(Vector3& delta, float& h, float deltaTime)
{
	_time_elapsed += deltaTime;

	float offset_forward = _curve_forward->Evaluate(_time_elapsed / _args.TimeScale) * _entity->GetUnit()->GetScale();
	float delta_offset_forward = offset_forward - _last_offset_forward;
	_last_offset_forward = offset_forward;

	float offset_side = _curve_side->Evaluate(_time_elapsed / _args.TimeScale) * _entity->GetUnit()->GetScale();
	float delta_offset_side = offset_side - _last_offset_side;
	_last_offset_side = offset_side;

	if(_args.Data()->Using_Up)
	{
		float offset_up = _curve_up->Evaluate(_time_elapsed / _args.TimeScale) * _entity->GetUnit()->GetScale();
		h = offset_up - _last_offset_up;
		_last_offset_up = offset_up;
	}

	Vector3 vforward = delta_offset_forward * _curve_step_dir;
	const Vector3& right = Vector3::Cross(Vector3::up, _entity->GetFace_p());
	const Vector3& vside = delta_offset_side * right;

	delta = vforward + vside; delta.y = 0;
}

void XChargeComponent::CalibrateByMultipleDirection(Vector3& delta)
{
	float factor = _entity->SkillComponent()->CurrentSkill()->MainCore()->GetMultipleDirectionFactor();
	if (factor > 0.25f && factor < 0.75f)
	{
		float dec = (1 - _entity->SkillComponent()->CurrentSkill()->MainCore()->GetSoul()->BackTowardsDecline);
		float rad = ((factor - 0.25f) / 0.5f) * XPI;

		delta = (Vector3::Magnitude(delta) * (1 - sin(rad) * dec)) * Vector3::Normalized(delta);
	}
}
