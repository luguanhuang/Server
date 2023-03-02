#ifndef __XIDLECOMPONENT_H__
#define __XIDLECOMPONENT_H__

#include "XActionStateComponent.hpp"
#include "entity/XActionDefine.h"

class XIdleComponent : public XActionStateComponent<XIdleActionArgs>
{
public:
	static const UINT32 uuID = XIdle_Component;

	XIdleComponent(void);
	virtual ~XIdleComponent(void);

	virtual UINT32 ID() { return XIdleComponent::uuID; }

protected:
	virtual void ActionSubscribe();

	virtual void OnGetAction(XActionArgs* pAction) { }
	virtual bool Begin();
	virtual void Cancel(XStateDefine next);
	virtual void ActionUpdate(float deltaTime);
};

#endif	//__XIDLECOMPONENT_H__