#ifndef __XACTIONSTATECOMPONENT_H__
#define __XACTIONSTATECOMPONENT_H__

#include "XComponent.h"
#include "state/IXStateTransform.hpp"
#include "state/XStateMachine.h"
#include "XSkillComponent.h"
#include "entity/XEntity.h"
#include "entity/XRole.h"

template<class ActionType>
class XActionStateComponent : public XComponent, public IXStateTransform
{
public:
	XActionStateComponent(void)
		:_selfState(XState_Idle), _entity(NULL), _machine(NULL), _skill(NULL), _is_role(false), _bStopped(false), _bFinished(true), _stateToken(0)
	{
		static_assert(std::is_base_of<XActionArgs, ActionType>::value,
			"ActionType needs to be XActionArgs based.");
	};

	virtual ~XActionStateComponent(void){};

public:
	inline  bool IsPermitted(XStateDefine state)
	{
		return IsFinished() ? true : InnerPermitted(state);
	}

	virtual void OnRejected(XStateDefine current) { }
	virtual void OnGetPermission() { }

	void Stop(XStateDefine next)
	{
		if(!_bStopped)
		{
			_bStopped = true;
			
			Finish();
			Cancel(next);
		}
	}

	inline void StateUpdate(float deltaTime)
	{
		if (SelfState() != _machine->GetDefaultState() && IsFinished())
			Finish();
		else
			ActionUpdate(deltaTime);
	}

	inline XStateDefine SelfState() { return _selfState; }
	inline bool IsFinished() { return _bFinished || _bStopped; }
	inline bool IsStopped() { return _bStopped; }
	inline UINT64 Token() { return _stateToken; }

	/* public for linux gcc
	 *
	 * OnAction -->
	 *		GetPermission --> 
	 *			OnGetPermission()	// Transfer to state
	 *			OnGetAction()	// _args memcpy
	 *		Start -->
	 *			Begin()
	 *		StateUpdate -->
	 *			ActionUpdate()	// update
	 *		Stop -->
	 *			Cancel()
	 *		Finish
	 */
	bool OnAction(IArgs* pargs, void *)
	{
		ActionType* pAction = static_cast<ActionType*>(pargs);

		if (GetPermission(pAction))
		{
			OnGetAction(pAction);
			Start();
		}

		return true;
	}

protected:
	virtual void OnAttachToHost(XObject* pHost)
	{
		XComponent::OnAttachToHost(pHost);

		_entity = dynamic_cast<XEntity*>(GetHost());

		_machine = _entity->StateMachine();
		_skill = _entity->SkillComponent();

		_is_role = dynamic_cast<XRole*>(_entity) != NULL;
	}

	virtual void OnDetachFromHost()
	{
		if (_selfState != XState_Idle && _machine->GetCurrentState() == _selfState)
			_machine->ForceToDefaultState(true);

		_machine = NULL;
		_skill = NULL;

		_entity = NULL;

		XComponent::OnDetachFromHost();
	}

	inline void Finish()
	{
		if (!_bFinished)
		{
			_bFinished = true;
			Cease();
		}
	}

	virtual bool Begin() = 0;
	virtual void Cancel(XStateDefine next) = 0;
	virtual void Cease(){ };

	virtual bool Validate(ActionType* pAction) { return pAction != NULL; }

	bool GetPermission(ActionType* pAction)
	{
		if(Validate(pAction) && _entity->SkillComponent()->CanPerformAction(SelfState(), pAction) && _machine->TransferToState(this))
		{
			_stateToken = pAction->GetToken();
			return true;
		}
		else
			return false;
	}

	virtual bool InnerPermitted(XStateDefine state)
	{
		return _machine->StatePermitted(_selfState, state);
	}

	virtual void OnGetAction(ActionType* pAction)
	{
		//A shallow copy for ActionTypes
		//memcpy(&_args, pAction, sizeof(ActionType));
		_args = *pAction;
	}

	virtual void ActionUpdate(float deltaTime) = 0;

protected:
	XStateDefine _selfState;
	ActionType _args;

	XEntity* _entity;

	XStateMachine* _machine;
	XSkillComponent* _skill;

	bool _is_role;

private:
	void Start()
	{
		_bStopped = false;
		_bFinished = false;

		Begin();
	}

private:
	bool _bStopped;
	bool _bFinished;
	UINT64 _stateToken;
};

#endif	//__XACTIONSTATECOMPONENT_H__