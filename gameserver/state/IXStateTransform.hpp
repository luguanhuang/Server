#ifndef __IXSTATETRANSFORM_H__
#define __IXSTATETRANSFORM_H__

#include "pch.h"
#include "XStateDefine.hpp"
#include "physics/XPhysics.h"

class IXStateTransform
{
public:
	virtual ~IXStateTransform(){ }

	virtual bool IsPermitted(XStateDefine state) = 0;
	virtual void OnRejected(XStateDefine current) = 0;
	virtual void OnGetPermission() = 0;
	virtual void Stop(XStateDefine next) = 0;
	virtual void StateUpdate(float deltaTime) = 0;
	virtual XStateDefine SelfState() = 0;
	virtual bool IsFinished() = 0;
	virtual UINT64 Token() = 0;
};

#endif	//__IXSTATETRANSFORM_H__