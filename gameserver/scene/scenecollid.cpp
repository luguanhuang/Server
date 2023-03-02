#include "pch.h"
#include "scenecollid.h"
#include "unit/unit.h"
#include "entity/XObject.h"

bool SceneCollide::MoveCollide(const MoveCollideParam params, float &t)
{
	// 只有重力影响
	if (params.delta.x == 0 && params.delta.z == 0)
	{
		t = 1.0f;
		return true;
	}

	float ut = 1.0;
	float gt = 1.0;
	//m_unitCollide.MoveCollide(params, ut);
	m_gridCollide.MoveCollide(params, gt);
	t = (ut < gt) ? ut : gt;
	return t < 1.0f;
}

bool SceneCollide::StandCollide(const StandCollideParam &params)
{
	return m_gridCollide.StandCollide(params);
}

void SceneCollide::SetScene(SceneBase *pScene)
{
	m_gridCollide.SetScene(pScene);
	m_unitCollide.SetScene(pScene);
}

float SceneCollide::GetHeight(float x, float z)
{
	return m_gridCollide.GetHeight(x, z);
}

MoveCollideParam::MoveCollideParam(Unit *pUnit_, const Vector3 &delta_)
{
	pUnit = pUnit_;
	position = pUnit_->GetXObject()->GetPosition_p();
	delta = delta_;
	radius = pUnit->GetBoundRaidus();
}

MoveCollideParam::MoveCollideParam( Unit *pUnit_, const Vector3 &position_ , const Vector3 &delta_ )
{
	pUnit = pUnit_;
	position = position_;
	delta = delta_;
	radius = pUnit->GetBoundRaidus();
}


StandCollideParam::StandCollideParam(Unit *pUnit_, const Vector3 &position_)
{
	pUnit = pUnit_;
	position = position_;
	radius = pUnit_->GetBoundRaidus();
}
