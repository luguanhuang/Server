#ifndef __WAYPOINTMGR_H__
#define __WAYPOINTMGR_H__

#include <string>
#include <unordered_map>

class WayPoint;

class WayPointMgr 
{
	WayPointMgr();
	~WayPointMgr();
	DECLARE_SINGLETON(WayPointMgr)
public:
	bool Init();
	void Uninit();
	
	WayPoint* GetWayPointByName(const std::string name);

private:
	bool AddOneWayPoint(const std::string& path , const std::string& name);

	std::unordered_map<std::string , WayPoint*> m_wayPointMap;
	
};

#endif