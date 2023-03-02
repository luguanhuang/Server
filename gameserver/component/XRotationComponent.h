#ifndef __XROTATIONCOMPONENT_H__
#define __XROTATIONCOMPONENT_H__

#include "entity/XActionDefine.h"
#include "XComponent.h"

class XEntity;

class XRotationComponent : public XComponent
{
public:
	static const UINT32 uuID = XRotation_Component;

	XRotationComponent(void);
	virtual ~XRotationComponent(void);

	virtual UINT32 ID() { return XRotationComponent::uuID; }

	virtual void Update(float fDeltaT);
	virtual void OnAttachToHost(XObject* pHost);

	inline float To() { return _to; }
	inline bool Rotating() { return _bRotate; }

	float GetMeaningfulFace();
	const Vector3 GetMeaningfulFaceVector3();

	void Cancel();

protected:
	virtual void ActionSubscribe();
	bool OnAction(IArgs* pargs, void *);

	bool Permission(const XRotationActionArgs* e);

private:
	bool _bRotate;
	bool _bAuto;

	float _to;
	float _from;

	float _rotateSpeed;
};

#endif	//__XROTATIONCOMPONENT_H__