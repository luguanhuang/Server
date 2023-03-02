#include "pch.h"
#include "waypointmgr.h"
#include "ai/aitreemgr.h"
#include "waypoint.h"

INSTANCE_SINGLETON(WayPointMgr)


WayPointMgr::WayPointMgr()
{

}

WayPointMgr::~WayPointMgr()
{

}

bool WayPointMgr::Init()
{
	std::string path = "gsconf/table/WayPoint/";//≈‰÷√Œƒº˛¬∑æ∂

	std::vector<std::string> files;

	if(!GetFilesBySuffix(path , "xml" , files))
	{
		return false;
	}
	
	bool ret = true;
	for(auto it = files.begin(); it != files.end(); ++it)
	{
		if(!AddOneWayPoint(path , (*it)))
		{
			ret = false;
		}
	}
	if(!ret)
	{
		return false;
	}

	return true;
}

void WayPointMgr::Uninit()
{
	for(auto it = m_wayPointMap.begin(); it != m_wayPointMap.end(); ++it)
	{
		delete (it->second);
	}
	m_wayPointMap.clear();
}

WayPoint* WayPointMgr::GetWayPointByName( const std::string name )
{
	auto it = m_wayPointMap.find(name);
	if(it != m_wayPointMap.end())
	{
		return it->second;
	}
	return NULL;
}

bool WayPointMgr::AddOneWayPoint(const std::string& path , const std::string& name )
{
	if((int)name.size() < 4)
	{
		SSError << " way point name error = " << name.c_str() << END;
		return false;
	}

	std::string keyName(name.substr(0,name.size()-4)); 

	WayPoint * waypoint = new WayPoint(keyName);	
	if(waypoint)
	{
		if(!waypoint->LoadData(path + name))
		{
			delete waypoint;
			return false;
		}else
		{
			m_wayPointMap.insert(std::make_pair(keyName, waypoint));	
		}
	}
	return true;
}
