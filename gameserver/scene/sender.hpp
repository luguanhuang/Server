#ifndef __SENDER_H__
#define __SENDER_H__

#include "scene/ptcg2c_unitappear.h"
#include "pb/project.pb.h"
#include "scene/sceneline.h"

struct RoleAppearNotifySender
{
	Unit *poAppearUnit;
	PtcG2C_UnitAppear oAppearToOthersPtc;
	PtcG2C_UnitAppear oAppearToMePtc;
	UINT32 Count;

	void Start(Unit *pUnit, bool isbrief)
	{
		Count = 0;
		poAppearUnit = pUnit;
		KKSG::UnitAppearance *pAppearance = oAppearToOthersPtc.m_Data.add_units();
		poAppearUnit->GetAppearance(*pAppearance, isbrief);
		pAppearance->set_isnewmob(true);
	}
	
	template <typename T>
	void SetNotifyList(T &t)
	{
		for (auto i = t.begin(); i != t.end(); ++i)
		{
			AddNearbyUnit(*i);
		}
	}

	template <typename T>
	void SetNotifyMap(T &t)
	{
		for (auto i = t.begin(); i != t.end(); ++i)
		{
			AddNearbyUnit(i->second);
		}
	}

	void SetNotifyMapNew(const std::unordered_map<UINT64, RoleViewSet*>& roles)
	{
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			AddNearbyUnit((Unit*)i->second->m_role);
		}
	}

	void AddNearbyUnit(Unit *pUnit)
	{
		if (pUnit != poAppearUnit)
		{
			++Count;
			KKSG::UnitAppearance *pAppearance = oAppearToMePtc.m_Data.add_units();
			pUnit->GetAppearance(*pAppearance);
			pUnit->Send(oAppearToOthersPtc);
		}
	}

	void Stop()
	{
		if (Count > 0)
		{
			poAppearUnit->Send(oAppearToMePtc);
		}
	}
};


#endif // __SENDER_H__