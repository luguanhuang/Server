#include "pch.h"
#include "util/XCommon.h"
#include "XMoveComponent.h"
#include "XNetComponent.h"
#include "timeutil.h"
#include "table/globalconfig.h"
#include "scene/scene.h"
#include "action/ptcg2c_syncmovenotify.h"
#include "LogInit.h"
#include "scene/securitystatistics.h"
#include "XBuffComponent.h"

XMoveComponent::XMoveComponent(void)
	:_speed(0),
	 _last_speed(0),
	 _target_speed(0),
	 _acceleration(0),
	 _inertia(false),
	 _overlapped(false),
	 _begin_inertia(false),
	 _move_dir(Vector3::zero)
{
	_selfState = XState_Move;
}

XMoveComponent::~XMoveComponent(void)
{

}

void XMoveComponent::ActionSubscribe()
{
	RegisterAction(XAction_Move, DelegateCombination(XActionStateComponent<XMoveActionArgs>, OnAction, this));
}

void XMoveComponent::OnGetAction(XMoveActionArgs* pAction)
{
	XActionStateComponent<XMoveActionArgs>::OnGetAction(pAction);

	_inertia = _args.Inertia;
	_target_speed = _args.Speed;

	if(!_args.Stoppage) 
	{
		//LogInfo("Role [%llu] Begin sync z: %.3lf", GetEntity()->GetID(), _args.Destination.z);
		if(GetEntity()->NetComponent()) GetEntity()->NetComponent()->ReportMoveAction(_args.Destination, false, Vector3::zero);
	}
}

void XMoveComponent::OnRejected(XStateDefine current)
{

}

bool XMoveComponent::Begin()
{
	if(GetEntity()->StateMachine()->GetPretState() != XState_Move)
		_entity->SyncIncreament();

	_begin_inertia = false;

	if(_target_speed != _speed)
	{
		if(_target_speed > _speed)
			_acceleration = (float)_entity->GetUnit()->GetRunSpeed() * 5.0f;
		else
			_acceleration = (float)(-_entity->GetUnit()->GetRunSpeed() * 6.0f);
	}
	else
		_acceleration = 0;

	//start from where i am
	_move_dir = Vector3::Horizontaled(_args.Destination - _entity->GetPosition_p());
	if(_move_dir.IsZero()) _move_dir = _entity->GetFace_p();

	XRotationActionArgs rargs;
	rargs.TargetDir = _move_dir;
	rargs.Palstance = _entity->GetUnit()->GetRotateSpeed();
	_entity->DispatchAction(&rargs);

	_overlapped = false;

	Vector3 tmp(Vector3::zero);
	CheckMove(tmp);

	return true;
}

void XMoveComponent::Cease()
{
	_begin_inertia = false;
}

void XMoveComponent::Cancel(XStateDefine next)
{
	if(next != XState_Move)
	{
		_speed = 0;
		_last_speed = 0;

		_acceleration = 0;

		if(!_args.StopTowards.IsZero())
		{
			XRotationActionArgs rargs;
			rargs.TargetDir = _args.StopTowards;
			rargs.Palstance = _entity->GetUnit()->GetAutoRotateSpeed();
			_entity->DispatchAction(&rargs);
		}

		if(_args.Stoppage)
		{
			//LogInfo("Role [%llu] End sync z: %.3lf", GetEntity()->GetID(), GetEntity()->GetPosition_p().z);
			if(GetEntity()->NetComponent()) GetEntity()->NetComponent()->ReportMoveAction(GetEntity()->GetPosition_p(), true, _args.StopTowards);
		}
	}

	//LogInfo("Move End At: x %f, y %f, z %f\n", GetEntity()->Position.x, GetEntity()->Position.y, GetEntity()->Position.z);
}

void XMoveComponent::ActionUpdate(float deltaTime)
{
	_last_speed = _speed;

	if(IsFinished()) return;

	if (_speed == 0 && _target_speed == 0) 
		Finish();
	else
	{
		Vector3 movement;

		_speed += deltaTime * _acceleration;

		if((_speed - _target_speed) * _acceleration > 0)
		{
			_speed = _target_speed;
			_acceleration = 0;
		}

		if(_begin_inertia)
		{
			
		}
		else
		{
			movement = _move_dir * ((_speed + _last_speed) * 0.5f) * deltaTime;

			if(GetEntity()->BuffComponent() && GetEntity()->BuffComponent()->IsBuffStateOn(XBuffType_LockFoot)) movement.Zero();

			CheckMove(movement);

			XSecurityStatistics* pSS = _entity->GetUnit()->GetSecurityStatistics();
			if(pSS) pSS->OnMove(Vector3::Magnitude(movement));

			_entity->ApplyMove(movement);
		}
	}
}

void XMoveComponent::CheckMove(Vector3& movement)
{
	if(!_overlapped)
	{
		_overlapped = Overlapped(movement);

		if(_overlapped)
		{
			_target_speed = 0;

			if(_inertia && _speed > 0)
			{
				_acceleration = (float)(-_entity->GetUnit()->GetRunSpeed() * 6.0f);
				_begin_inertia = true;
			}
			else
			{
				_acceleration = 0;
				Finish();
			}
		}
	}
}

bool XMoveComponent::Overlapped(Vector3& movement)
{
	Vector3  v1 = _entity->GetPosition_p() + movement; v1.y = 0;
	Vector3& v2 = _args.Destination; v2.y = 0;

	Vector3 different = (v2 - v1);
	if (Vector3::sqrtMagnitude(different) > 0)
	{
		float angle = Vector3::Angle(different, _move_dir);
		if (angle >= 90)
		{
			movement = _args.Destination - _entity->GetPosition_p();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}
