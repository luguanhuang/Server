#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "component/XComponent.h"
#include "XStateDefine.hpp"
#include "IXStateTransform.hpp"
#include "physics/XPhysics.h"

class IXStateTransform;
class XSkillComponent;

class XStateMachine : public XComponent
{
public:
	static const UINT32 uuID = XStateMachine_Component;

	XStateMachine(void);
	virtual ~XStateMachine(void);

	virtual UINT32 ID() { return XStateMachine::uuID; }

	inline IXStateTransform* GetCurrent(){ return _current; }
	inline IXStateTransform* GetDefault(){ return _default; }

	inline XStateDefine GetCurrentState(){ return _current->SelfState(); }
	inline XStateDefine GetDefaultState(){ return _default->SelfState(); }
	inline XStateDefine GetPretState(){ return _pre->SelfState(); }

	inline UINT64 ActionToken() { return _current->Token(); }

	bool CanAct(XStateDefine state);
	bool CanAct(IXStateTransform* next);

	inline bool StatePermitted(XStateDefine src, XStateDefine des) const { return _bStateMap[src][des]; }

	void ForceToDefaultState(bool ignoredeath);

	bool TransferToState(IXStateTransform* next);
	void SetDefaultState(IXStateTransform* def);

	virtual void Update(float fDeltaT);

protected:
	virtual void ActionSubscribe();

	void TransferToDefaultState();

	virtual bool InnerPermission(XStateDefine next){ return true; }

	bool OnLeaveScene(IArgs* pargs, void *);

private:
	IXStateTransform* _current;
	IXStateTransform* _pre;
	IXStateTransform* _default;

	static const bool _bStateMap[][XState_Max];

	XSkillComponent* _skill;
};

#endif	//__STATEMACHINE_H__