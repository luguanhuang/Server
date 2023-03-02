#ifndef __XMOVECOMPONENT_H__
#define __XMOVECOMPONENT_H__

#include "XActionStateComponent.hpp"
#include "entity/XActionDefine.h"

class XMoveComponent : public XActionStateComponent<XMoveActionArgs>
{
public:
	static const UINT32 uuID = XMove_Component;

	XMoveComponent(void);
	virtual ~XMoveComponent(void);

	virtual UINT32 ID() { return XMoveComponent::uuID; }

	inline float GetSpeed(){ return _speed; }

protected:
	virtual void ActionSubscribe();

	virtual bool Begin();
	virtual void Cease();
	virtual void Cancel(XStateDefine next);
	virtual void ActionUpdate(float deltaTime);
	virtual void OnGetAction(XMoveActionArgs* pAction);

	virtual void OnRejected(XStateDefine current);

private:
	void CheckMove(Vector3& movement);
	bool Overlapped(Vector3& movement);

private:
	float _speed;
	float _last_speed;
	float _target_speed;

	float _acceleration;

	bool _inertia;
	bool _overlapped;
	bool _begin_inertia;

	Vector3 _move_dir;
};

#endif	//__XMOVECOMPONENT_H__