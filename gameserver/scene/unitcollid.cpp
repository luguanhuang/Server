#include "pch.h"
#include "unitcollid.h"
#include "unit/unit.h"
#include "scene.h"
#include "scene/grid.h"
#include "entity/XObject.h"
#include "scenefinder.h"

UnitCollide::UnitCollide()
{
	m_pScene = NULL;
}

UnitCollide::~UnitCollide()
{

}

void UnitCollide::SetScene(SceneBase *pScene)
{
	m_pScene = pScene;
}

bool UnitCollide::Collide(const Vector3 &position, float r1, const Vector3& other, float r2)
{
	Vector3 a = position - other;
	return Vector3::Dot(a, a) <= (r1+r2)*(r1+r2);
}


