#include "pch.h"
#include "sceneline.h"
#include "sender.hpp"
#include "unit/enemy.h"
#include "unit/unit.h"
#include "scenefinder.h"
#include "unit/enemymanager.h"
#include "foreach.h"
#include "component/XActionSender.h"
#include "scene/ptcg2c_unitappear.h"
#include "scene/ptcg2c_unitdisappear.h"
#include "component/XNetComponent.h"
#include "table/globalconfig.h"
#include "time.h"
#include "unit/roleview.h"
#include "gamelogic/team.h"
#include "gamelogic/teammgr.h"
#include "broadcast/broadcast.h"

SceneLine::SceneLine()
{
	m_ID = -1;
}

SceneLine::~SceneLine() 
{
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		Unit* unit = i->second;
		unit->CleanUpInScene();
	}
	m_units.clear();
	for (auto j = m_role2viewset.begin(); j != m_role2viewset.end(); ++j)
	{
		if (NULL != j->second)
		{
			delete j->second;
		}
	}
	m_role2viewset.clear();
}

void SceneLine::RemoveRole(Role *poRole)
{
	auto i = m_role2viewset.find(poRole->GetID());
	if (i != m_role2viewset.end())
	{
		delete i->second;
		m_role2viewset.erase(i);
	}
	else
	{
		SSWarn << "Remove Role " << poRole << " FAILED!" << END;
	}
}

void SceneLine::AddRole(Role *poRole)
{
	auto i = m_role2viewset.find(poRole->GetID());
	if (i != m_role2viewset.end())
	{
		SSWarn << "Add Role " << poRole << " FAILED!" << END;
		assert(false);
	}

	RoleViewSet* viewset = new RoleViewSet(poRole);
	m_role2viewset.insert(std::make_pair(poRole->GetID(), viewset));
}

void SceneLine::RemoveUnit(Unit* unit)
{
	auto i = m_units.find(unit->GetID());
	if (i != m_units.end())
	{
		m_units.erase(i);
	}
	else
	{
		SSWarn << "Remove Unit " << unit << " FAILED!" << END;
	}
}

void SceneLine::AddUnit(Unit* unit)
{
	auto i = m_units.find(unit->GetID());
	if (i != m_units.end())
	{
		SSWarn << "Add Unit" << unit << " FAILED!" << END;
		assert(false);
	}
	m_units[unit->GetID()] = unit;
}

void SceneLine::Broadcast(Unit* poSender, const CProtocol &roPtc, Unit *pExludeUnit)
{
	/*Role *pRole = NULL;
	for (auto i = m_role2viewset.begin(); i != m_role2viewset.end(); ++i)
	{
		pRole = i->second->m_role;
		if (pRole == pExludeUnit)
			continue;

		pRole->Send(roPtc);
	}*/

	//替换新广播接口
	CBroadCastSender::Broadcast(m_role2viewset, roPtc, pExludeUnit);
}

void SceneLine::FillAll(std::vector<Unit*>& units)
{
	for (auto i = m_role2viewset.begin(); i != m_role2viewset.end(); ++i)
	{
		units.push_back(i->second->m_role);
	}
	for (auto j = m_units.begin(); j != m_units.end(); ++j)
	{
		units.push_back(j->second);
	}
}

Role* SceneLine::IsIn(UINT64 roleid)
{ 
	auto i = m_role2viewset.find(roleid);
	return (i == m_role2viewset.end()) ? NULL : i->second->m_role;
}

void SceneLine::GetCanSee(Role* role, std::vector<Role*>& roles)
{
	for (auto i = m_role2viewset.begin(); i != m_role2viewset.end(); ++i)
	{
		if (role != i->second->m_role)
		{
			roles.push_back(i->second->m_role);
		}
	}
}

void SceneLine::GetCanBeSee(Role* role, std::vector<Role*>& roles, int limit)
{
	for (auto i = m_role2viewset.begin(); i != m_role2viewset.end(); ++i)
	{
		if (role != i->second->m_role)
		{
			roles.push_back(i->second->m_role);
		}
	}
}

void SceneLine::GetUnitsCanSee(Role* role, std::vector<Unit*>& units)
{
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		units.push_back(i->second);
	}
}

void SceneLine::PrintLine(std::stringstream &ss)
{
	ss << "  Line[" << m_ID << "]:\n";
	int k = 0;
	for (auto i = m_role2viewset.begin(); i != m_role2viewset.end(); ++i)
	{
		Role *pRole = i->second->m_role;
		ss << "  " << k++ << ". " << pRole << " @ " << pRole->GetXObject()->GetPosition_p() 
			<< " dir: " << pRole->GetXObject()->GetFaceDegree() << "\n";
	}
	for (auto j = m_units.begin(); j != m_units.end(); ++j)
	{
		Unit *unit = j->second;
		ss << "  " << k++ << ". " << unit << " @ " << unit->GetXObject()->GetPosition_p() 
			<< " dir: " << unit->GetXObject()->GetFaceDegree() << "\n";
	}
}
