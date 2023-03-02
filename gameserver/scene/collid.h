#ifndef __COLLID_H__
#define __COLLID_H__

#include "math/XVector3.h"
#include "physics/XPhysics.h"

class Unit;
class Scene;

struct MoveCollideParam
{
	Unit   *pUnit;
	Vector3 position;
	Vector3 delta;
	float   radius;

	MoveCollideParam(Unit *pUnit, const Vector3 &delta_);
	MoveCollideParam(Unit *pUnit, const Vector3 &position , const Vector3 &delta);
};


struct StandCollideParam
{
	Unit   *pUnit;
	Vector3 position;
	float   radius;

	StandCollideParam(Unit *pUnit, const Vector3 &position);
};

class ICollide
{
public:
	virtual ~ICollide() {}
};

class IUnitColliderFilter
{
public:
	IUnitColliderFilter() { MovePercent = 1; CollideUnit = NULL; }
	virtual ~IUnitColliderFilter() {}

	float MovePercent;
	Unit *CollideUnit;
};

#endif // __COLLID_H__