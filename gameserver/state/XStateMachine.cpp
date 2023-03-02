#include "pch.h"
#include "XStateMachine.h"
#include "entity/XEntity.h"
#include "component/XSkillComponent.h"

const bool XStateMachine::_bStateMap[][XState_Max] = 
{
	// idle    move   jump   fall   freeze  behit  death  charge
	/***********************************************************************/
	{false,  true,  true,  true,  true,   true,  true,  true  },  // XState_Idle
	{true,   true,  true,  true,  true,   true,  true,  true  },  // XState_Move
	{false,  false, false, true,  true,   true,  true,  false },  // XState_Jump
	{false,  false, false, false, true,   true,  true,  false },  // XState_Fall
	{false , false, false, false, true,   false, true,  false },  // XState_Freeze
	{false,  false, false, false, true,   true,  true,  false },  // XState_BeHit
	{false,  false, false, false, false,  false, false, false },  // XState_Death
	{false,  true,  false, false, true,   true,  true,  true  }   // XState_Charge
};

XStateMachine::XStateMachine(void)
	:_current(0), _pre(0), _default(0), _skill(0)
{

}

XStateMachine::~XStateMachine(void)
{
	_skill = NULL;

	_current = NULL;
	_pre = NULL;
	_default = NULL;
}

void XStateMachine::ActionSubscribe()
{
	RegisterAction(XAction_LeaveScene, DelegateCombination(XStateMachine, OnLeaveScene, this));
}

bool XStateMachine::OnLeaveScene(IArgs* pargs, void *)
{
	ForceToDefaultState(false);
	return true;
}

void XStateMachine::SetDefaultState(IXStateTransform* def)
{
	_default = def;
	_current = _default;
	_pre = _default;
}

bool XStateMachine::CanAct(IXStateTransform* next)
{
	return CanAct(next->SelfState());
}

bool XStateMachine::CanAct(XStateDefine state)
{
	return (state == XState_Death) || (IsEnabled() && InnerPermission(state) && _current->IsPermitted(state));
}

void XStateMachine::Update(float fDeltaT)
{
	_current->StateUpdate(fDeltaT);
	if(_current->IsFinished()) TransferToDefaultState();
}

void XStateMachine::ForceToDefaultState(bool ignoredeath)
{
	if(ignoredeath || _current->SelfState() != XState_Death)
	{
		if (_current != _default)
		{
			_current->Stop(_default->SelfState());
			TransferToDefaultState();
		}
	}
}

void XStateMachine::TransferToDefaultState()
{
	XIdleActionArgs args;
	GetHost()->DispatchAction(&args);
}

bool XStateMachine::TransferToState(IXStateTransform* next)
{
	if(CanAct(next))
	{
		/*
		 * for behit, must cancel last behit presentation first
		 */
		if(_current->SelfState() != next->SelfState() || next->SelfState() != XState_Move)
			_current->Stop(next->SelfState());

		_pre = _current;
		_current = next;
		_current->OnGetPermission();

		return true;
	}
	else
	{
		next->OnRejected(_current->SelfState());
		return false;
	}
}