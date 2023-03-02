#ifndef __WAYPOINT_H__
#define __WAYPOINT_H__

#include <vector>
#include "math/XVector3.h"

class Unit;

class WayPoint
{
public:
	WayPoint(const std::string& name)
	{
		m_name = name;
	}
	bool LoadData(const std::string& filePath);
	bool HavePoint(int index);
	Vector3 GetPoint(int index);
	bool OnIndexPoint(const Vector3& myPoint , int index);
	int GetClosePointIndex(const Vector3& pos , Unit *unit);
	int GetLastPointIndex();
	int GetNextIndex(int fromIndex , int toIndex);
	void CorrectPathIndex(int &stIndex , int destIndex , Unit *unit);
	
	void CreateNavData(Unit* unit);
	bool NavInited(){ return !m_next.empty(); }

private:

	std::vector<Vector3> m_pointData;
	std::vector<BYTE> m_next;
	std::string m_name;
};

#endif