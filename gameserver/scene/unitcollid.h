#ifndef __UNITCOLLID_H__
#define __UNITCOLLID_H__

#include "math/XVector3.h"
#include "collid.h"

class SceneBase;

class UnitCollide : public ICollide
{
public:
	UnitCollide();
	~UnitCollide();

	void SetScene(SceneBase *pScene);

private:

	bool Collide(const Vector3 &position, float r1, const Vector3& other, float r2);

	SceneBase *m_pScene;
};

#endif // __UNITCOLLID_H__