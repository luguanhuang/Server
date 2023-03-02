#ifndef __XGRAVITYCOMPONENT_H__
#define __XGRAVITYCOMPONENT_H__

#include "XComponent.h"

class XGravityComponent : public XComponent
{
public:
	static const UINT32 uuID = XGravity_Component;

	XGravityComponent(void);
	virtual ~XGravityComponent(void);

	virtual UINT32 ID() { return XGravityComponent::uuID; }
	virtual void Update(float fDeltaT);
};

#endif	//__XGRAVITYCOMPONENT_H__