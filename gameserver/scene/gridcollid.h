#ifndef __GRIDCOLLID_H__
#define __GRIDCOLLID_H__

#include "collid.h"

class Grid;
class SceneBase;

class GridCollide : public ICollide
{
public:

	GridCollide();
	~GridCollide();

	void SetScene(SceneBase *pScene); 

	virtual bool MoveCollide(const MoveCollideParam params, float &t);
	virtual bool StandCollide(const StandCollideParam &params);
	float GetHeight(float x, float z);

private:
	Grid *m_pGrid;
};

#endif // __GRIDCOLLID_H__