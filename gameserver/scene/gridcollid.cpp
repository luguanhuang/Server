#include "pch.h"
#include "gridcollid.h"
#include "scene.h"
#include "grid.h"
#include "unit/unit.h"

GridCollide::GridCollide()
{

}

GridCollide::~GridCollide()
{

}

bool GridCollide::MoveCollide(const MoveCollideParam params, float &t)
{
	if (m_pGrid == NULL)
	{
		t = 1.0f;
		return false;
	}
	else
	{
		t = m_pGrid->LineBlockTest(params.position, params.position + params.delta);
		return t != 1;
	}
}

bool GridCollide::StandCollide(const StandCollideParam &params)
{
	if (m_pGrid == NULL)
	{
		return false;
	}

	return m_pGrid->IsCellBlocked(params.position);
}

void GridCollide::SetScene(SceneBase *pScene)
{
	m_pGrid = pScene->GetGrid();
}

float GridCollide::GetHeight(float x, float z)
{
	if (m_pGrid == NULL)
		return 0;

	return m_pGrid->GetHeight(x, z);
}


