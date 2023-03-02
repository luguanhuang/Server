#ifndef __XFLYCOMPONENT_H__
#define __XFLYCOMPONENT_H__

#include "XActionStateComponent.hpp"
#include "entity/XActionDefine.h"

class XFlyComponent : public XComponent
{
public:
	static const UINT32 uuID = XFly_Component;

	XFlyComponent(void);
	virtual ~XFlyComponent(void);

	virtual UINT32 ID() { return XFlyComponent::uuID; }

	inline float CurrentHeight(){ return _current; }
	inline float MinHeight(){ return _min; }
	inline float MaxHeight(){ return _max; }

	virtual void Update(float fDeltaT);

	virtual void OnAttachToHost(XObject* pHost);
	virtual void Attached();

protected:

private:
	float _max;
	float _min;
	float _current;
	float _target;
};

#endif	//__XFLYCOMPONENT_H__