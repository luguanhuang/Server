#ifndef __XMANIPULATIONCOMPONENT_H__
#define __XMANIPULATIONCOMPONENT_H__

#include "entity/XActionDefine.h"
#include "XComponent.h"

class XEntity;

class XManipulationComponent : public XComponent
{
public:
	static const UINT32 uuID = XManipulation_Component;

	XManipulationComponent(void);
	virtual ~XManipulationComponent(void);

	virtual UINT32 ID() { return XManipulationComponent::uuID; }
	virtual void OnAttachToHost(XObject* pHost);
	virtual void OnDetachFromHost();

	virtual void Update(float fDeltaT);

protected:
	virtual void ActionSubscribe();

	bool ManipulationOn(IArgs* pargs, void *);
	bool ManipulationOff(IArgs* pargs, void *);

private:
	std::map<int, const XManipulationData*> _item;
};

#endif	//__XMANIPULATIONCOMPONENT_H__