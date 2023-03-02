#include "pch.h"
#include "util/XCommon.h"
#include "entity/XObject.h"
#include "XRotationComponent.h"
#include "skill/XSkillCore.h"
#include "XSkillComponent.h"
#include "entity/XEntity.h"
#include "state/XStateDefine.hpp"
#include "state/XStateMachine.h"
#include "XNetComponent.h"
#include "skill/XSkill.h"

XRotationComponent::XRotationComponent(void)
	:_bRotate(false),
	 _bAuto(false),
	 _to(0),
	 _from(0),
	 _rotateSpeed(0)
{

}

XRotationComponent::~XRotationComponent(void)
{

}

void XRotationComponent::OnAttachToHost(XObject* pHost)
{
	XComponent::OnAttachToHost(pHost);

	_rotateSpeed = 0;
}

void XRotationComponent::ActionSubscribe()
{
	RegisterAction(XAction_Rotation, DelegateCombination(XRotationComponent, OnAction, this));
}

bool XRotationComponent::Permission(const XRotationActionArgs* e)
{
	 bool uncontrol = XStateMgr::IsUnControlledState(GetEntity()->StateMachine()->GetCurrentState());

	 if(GetEntity()->SkillComponent()->IsCasting())
	 {
		 return e->GetToken() == GetEntity()->SkillComponent()->CurrentSkill()->Token() || (GetEntity()->SkillComponent()->CurrentSkill()->MainCore()->CanRotate() && !uncontrol);
	 }
	 else
	 {
		 return e->GetToken() == GetEntity()->StateMachine()->ActionToken() || !uncontrol;
	 }
}

bool XRotationComponent::OnAction(IArgs* pargs, void *)
{
	XRotationActionArgs* pr = static_cast<XRotationActionArgs*>(pargs);

	if(pr->TargetDir.IsZero()) return true;

	if(Permission(pr))
	{
		Vector3 to = pr->TargetDir; to.y = 0;
		if(to.IsZero()) return false;
		to.Normalize();

		const Vector3& from = GetHost()->GetFace_p();

		_from = Vector3::AngleToFloat(from);
		float angle = Vector3::Angle(from, to);

		if(Vector3::Clockwise(from, to))
		{
			_to = _from + angle;
		}
		else
		{
			_to = _from - angle;
		}

		_rotateSpeed = pr->Palstance;
		_bRotate = true;

		return true;
	}

	return false;
}

void XRotationComponent::Update(float fDeltaT)
{
	if(_bRotate)
	{
		if(fabs(_from - _to) > 1.f)
		{
			//if(_rotateSpeed == 0) LogDebug("Entity [%llu] with zero rotation speed.", GetHost()->GetID()); ///> 策划配置异常，导致日志打印过多。策划修复有风险和时间，先改成debug
			_from += (_to - _from) * std::min(1.0f, fDeltaT * _rotateSpeed);
		}
		else
		{
			_bRotate = false;
			_from = _to;
		}

		GetHost()->InnerSetFace(_from);
	}
}

float XRotationComponent::GetMeaningfulFace()
{
	return (_bRotate && _rotateSpeed > 0) ? _to : GetEntity()->GetFaceDegree();
}

const Vector3 XRotationComponent::GetMeaningfulFaceVector3()
{
	return (_bRotate && _rotateSpeed > 0) ? Vector3::FloatToAngle(_to) : GetEntity()->GetFace_p();
}

void XRotationComponent::Cancel()
{
	_bRotate = false;
	_to = GetEntity()->GetFaceDegree();
}