#ifndef __XDEATHCOMPONENT_H__
#define __XDEATHCOMPONENT_H__

#include "XActionStateComponent.hpp"
#include "entity/XActionDefine.h"

class XActionCurve;

class XDeathComponent : public XActionStateComponent<XRealDeadActionArgs>
{
public:
	static const UINT32 uuID = XDeath_Component;

	XDeathComponent(void);
	virtual ~XDeathComponent(void);

	virtual UINT32 ID() { return XDeathComponent::uuID; }

	virtual XPhysicsLayer CollisionLayer() { return XLayer_NoneEntity; }

protected:
	virtual void ActionSubscribe();
	virtual void OnAttachToHost(XObject* pHost);
	virtual void Attached();
	virtual void OnDetachFromHost();

	bool OnRevive(IArgs* pargs, void *);
	bool OnDestroy(IArgs*, void*);

	virtual bool Begin();
	virtual void Cancel(XStateDefine next);
	virtual void ActionUpdate(float deltaTime);

	virtual void OnGetAction(XRealDeadActionArgs* pAction);

private:
	XActionCurve* _curve_h;
	XActionCurve* _curve_v;

	bool _presented;

	float _elapsed;
	float _last_offset;
	float _last_height;
	float _deltaH;

	float _land_time;
	float _land_max;

	Vector3 _step_dir;

	HTIMER _destory_token;
};

#endif	//__XDEATHCOMPONENT_H__