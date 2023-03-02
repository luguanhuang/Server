#ifndef __SCENECOLLID_H__
#define __SCENECOLLID_H__

#include "collid.h"
#include "unitcollid.h"
#include "gridcollid.h"

class Scene;

class SceneCollide : public ICollide
{
public:

	void SetScene(SceneBase *pScene);

	virtual bool MoveCollide(const MoveCollideParam params, float &t);
	virtual bool StandCollide(const StandCollideParam &params);
	float GetHeight(float x, float z);

private:
	UnitCollide m_unitCollide;
	GridCollide m_gridCollide;
};

#endif // __SCENECOLLID_H__