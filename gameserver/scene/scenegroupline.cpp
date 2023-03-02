#include "pch.h"
#include "scenegroupline.h"
#include "timeutil.h"
#include "scene/ptcg2c_unitappear.h"
#include "scene/ptcg2c_unitdisappear.h"
#include "table/globalconfig.h"
#include "component/XActionSender.h"

#define SCENEGROUP_UPDATE GetGlobalConfig().SceneGroupUpdate

bool SceneGroupSet::InView(UINT32 group, UINT64 uid)
{
	auto i = m_cansee[group].find(uid);
	if (i != m_cansee[group].end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SceneGroupSet::AddUnit(Unit* unit)
{
	m_id2unit[unit->GetID()] = unit;
	if (unit->IsRole())
	{
		m_roles[unit->GetID()] = (Role*)unit;
	}
}

void SceneGroupSet::RemoveUnit(Unit* unit)
{
	m_id2unit.erase(unit->GetID());

	m_roles.erase(unit->GetID());
	
	for (auto l = m_cansee.begin(); l != m_cansee.end(); ++l)
	{
		l->second.erase(unit->GetID());
	}
}

void SceneGroupSet::BroadCast(const CProtocol& roPtc, Unit* pExludeUnit)
{
	for (auto i = m_id2unit.begin(); i != m_id2unit.end(); ++i)
	{
		if (i->second != pExludeUnit)
		{
			i->second->Send(roPtc);	
		}
	}
}

void SceneGroupSet::InnerAppear(Unit* unit)
{
	// appear to me, sync same group
	if (unit->IsRole())
	{
		PtcG2C_UnitAppear appeartome;
		for (auto i = m_id2unit.begin(); i != m_id2unit.end(); ++i)
		{
			if (i->second != unit)
			{
				i->second->GetAppearance(*appeartome.m_Data.add_units());
			}
		}
		for (auto l = m_cansee.begin(); l != m_cansee.end(); ++l)
		{
			for (auto j = l->second.begin(); j != l->second.end(); ++j)
			{
				if (j->second != unit)
				{
					j->second->GetAppearance(*appeartome.m_Data.add_units());
				}
			}
		}
		if (0 != appeartome.m_Data.units_size())
		{
			unit->Send(appeartome);
		}
	}

	// appear to other, sync same group
	PtcG2C_UnitAppear appeartoother;
	unit->GetAppearance(*appeartoother.m_Data.add_units());
	BroadCast(appeartoother, unit);
}

void SceneGroupSet::UpdateCanSee(UINT32 group, std::unordered_map<UINT64, Unit*>& newcansee)
{
	std::unordered_map<UINT64, Unit*>& onegourp = m_cansee[group];	

	std::unordered_map<UINT64, Unit*> enter = newcansee;
	std::unordered_map<UINT64, Unit*> leave;

	std::unordered_map<UINT64, Unit*> pregroup;
	onegourp.swap(pregroup);
	onegourp.swap(newcansee);

	for (auto i = pregroup.begin(); i != pregroup.end(); ++i)
	{
		auto j = enter.find(i->first);
		if (j == enter.end())
		{
			leave[i->first] = i->second;
		}
		else
		{
			enter.erase(j);
		}
	}

	PtcG2C_UnitAppear appear;
	for (auto k = enter.begin(); k != enter.end(); ++k)
	{
		Unit* unit = k->second;
		unit->GetAppearance(*appear.m_Data.add_units());

		SSDebug<<"appear:"<<unit->GetID()<<END;
	}
	if (!enter.empty())
	{
		BroadCast(appear);
	}

	PtcG2C_UnitDisappear disappear;
	for (auto l = leave.begin(); l != leave.end(); ++l)
	{
		SSDebug<<"disappear:"<<l->first<<END;

		disappear.m_Data.set_uid(l->first);
		disappear.m_Data.set_forcedisappear(true);
		BroadCast(disappear);

		disappear.m_Data.Clear();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
SceneGroupLine::SceneGroupLine()
{
	m_lastupdate = 0;
}

SceneGroupLine::~SceneGroupLine()
{

}

void SceneGroupLine::Update()
{
	UINT64 timestamp = TimeUtil::GetMilliSecond();
	if (timestamp >= (m_lastupdate + SCENEGROUP_UPDATE))
	{
		m_lastupdate = timestamp;
		for (auto i = m_group2units.begin(); i != m_group2units.end(); ++i)
		{
			for (auto j = m_group2units.begin(); j != m_group2units.end(); ++j)
			{
				if (i->first != j->first)
				{
					Merge(i->second, j->second);	
				}
			}
		}
	}

	for (auto i = m_group2units.begin(); i != m_group2units.end(); ++i)
	{
		SceneGroupSet* groupset = &i->second;

		// sync package
		XActionSender sender;
		// same group
		sender.PackageData(groupset->m_id2unit);
		// can see
		for (auto j = groupset->m_cansee.begin(); j != groupset->m_cansee.end(); ++j)
		{
			sender.PackageData(j->second);
		}
		sender.Broadcast(groupset->m_roles);
	}
}

void SceneGroupLine::Broadcast(Unit* poSender, const CProtocol& roPtc, Unit* pExludeUnit)
{
	if (NULL == poSender)
	{
		for (auto i = m_group2units.begin(); i != m_group2units.end(); ++i)
		{
			SceneGroupSet* groupset = &i->second;
			groupset->BroadCast(roPtc, pExludeUnit);
		}
	}
	else
	{
		for (auto i = m_group2units.begin(); i != m_group2units.end(); ++i)
		{
			SceneGroupSet* groupset = &i->second;
			///> 同一阵营或者在其他阵营的视野内
			if (poSender->GetFightGroup() == i->first || groupset->InView(poSender->GetFightGroup(), poSender->GetID()))
			{
				groupset->BroadCast(roPtc, pExludeUnit);
			}
		}
	}
}

void SceneGroupLine::BroadcastCanBeSee(Unit* poSender, const CProtocol& roPtc)
{
	Broadcast(poSender, roPtc, poSender);
}

void SceneGroupLine::AddRole(Role* poRole)
{
	SceneLine::AddRole(poRole);

	UINT32 fightgroup = poRole->GetFightGroup();
	SceneGroupSet* groupset = GetGroupSet(poRole->GetFightGroup());
	if (NULL == groupset)
	{
		SceneGroupSet tempgroupset;
		tempgroupset.m_group = fightgroup;
		m_group2units[fightgroup] = tempgroupset;
		groupset = &m_group2units[fightgroup];
	}
	groupset->AddUnit(poRole);
	groupset->InnerAppear(poRole);
}

void SceneGroupLine::RemoveRole(Role* poRole)
{
	SceneLine::RemoveRole(poRole);

	///> remove role and canbesee
	UINT32 fightgroup = poRole->GetFightGroup();
	for (auto i = m_group2units.begin(); i != m_group2units.end(); ++i)
	{
		i->second.RemoveUnit(poRole);
	}
}

void SceneGroupLine::AddUnit(Unit* unit)
{
	SceneLine::AddUnit(unit);

	UINT32 fightgroup = unit->GetFightGroup();
	SceneGroupSet* groupset = GetGroupSet(unit->GetFightGroup());
	if (NULL == groupset)
	{
		SceneGroupSet tempgroupset;
		tempgroupset.m_group = fightgroup;
		m_group2units[fightgroup] = tempgroupset;
		groupset = &m_group2units[fightgroup];
	}
	groupset->AddUnit(unit);
	groupset->InnerAppear(unit);
}

void SceneGroupLine::RemoveUnit(Unit* unit)
{
	SceneLine::RemoveUnit(unit);

	///> remove unit and canbesee
	UINT32 fightgroup = unit->GetFightGroup();
	for (auto i = m_group2units.begin(); i != m_group2units.end(); ++i)
	{
		i->second.RemoveUnit(unit);
	}
}

void SceneGroupLine::GetCanSee(Role* role, std::vector<Role*>& roles)
{
	SceneGroupSet* groupset = GetGroupSet(role->GetFightGroup());
	if (NULL != groupset)
	{
		for (auto i = groupset->m_cansee.begin(); i != groupset->m_cansee.end(); ++i)
		{
			for (auto l = i->second.begin(); l != i->second.end(); ++l)
			{
				if (l->second->IsRole())
				{
					roles.push_back((Role*)l->second);
				}
			}
		}
		for (auto j = groupset->m_id2unit.begin(); j != groupset->m_id2unit.end(); ++j)
		{
			Unit* unit = j->second;
			if (unit->IsRole() && role != unit)
			{
				roles.push_back((Role*)unit);
			}
		}
	}
}

void SceneGroupLine::GetCanBeSee(Role* role, std::vector<Role*>& roles, int limit)
{
	for (auto i = m_group2units.begin(); i != m_group2units.end(); ++i)
	{
		SceneGroupSet* groupset = &i->second;
		///> 同一阵营或者在其他阵营的视野内
		if (role->GetFightGroup() == i->first || groupset->InView(role->GetFightGroup(), role->GetID()))
		{
			for (auto j = groupset->m_id2unit.begin(); j != groupset->m_id2unit.end(); ++j)
			{
				Unit* unit = j->second;
				if (unit->IsRole() && role != unit)
				{
					roles.push_back((Role*)unit);
				}
			}
		}
	}
}

void SceneGroupLine::GetUnitsCanSee(Role* role, std::vector<Unit*>& units)
{
	SceneGroupSet* groupset = GetGroupSet(role->GetFightGroup());
	if (NULL != groupset)
	{
		for (auto i = groupset->m_cansee.begin(); i != groupset->m_cansee.end(); ++i)
		{
			for (auto l = i->second.begin(); l != i->second.end(); ++l)
			{
				if (!l->second->IsRole())
				{
					units.push_back(l->second);
				}
			}
		}
		for (auto j = groupset->m_id2unit.begin(); j != groupset->m_id2unit.end(); ++j)
		{
			Unit* unit = j->second;
			if (!unit->IsRole())
			{
				units.push_back(j->second);
			}
		}
	}
}

void SceneGroupLine::Merge(SceneGroupSet& aa, SceneGroupSet& bb)
{
	std::unordered_map<UINT64, Unit*> aacanseeunit;
	std::unordered_map<UINT64, Unit*> bbcanseeunit;
	for (auto i = aa.m_id2unit.begin(); i != aa.m_id2unit.end(); ++i)
	{
		Unit* unitaa = i->second;
		if (unitaa->IsEnemy() && unitaa->IsDead())
		{
			continue;
		}
		for (auto j = bb.m_id2unit.begin(); j != bb.m_id2unit.end(); ++j)
		{
			Unit* unitbb = j->second;
			if (unitbb->IsEnemy() && unitbb->IsDead())
			{
				continue;
			}
			///> 视野内
			float distance = Vector3::Distance(unitaa->GetXObject()->GetPosition(), unitbb->GetXObject()->GetPosition());
			if (distance <= (float)GetGlobalConfig().ViewGroupLen)
			{
				if (!unitaa->IsDead())
				{
					aacanseeunit[unitbb->GetID()] = unitbb;
				}
				if (!unitbb->IsDead())
				{
					bbcanseeunit[unitaa->GetID()] = unitaa;
				}
			}
		}
	}
	// aa leave/enter
	aa.UpdateCanSee(bb.m_group, aacanseeunit);
	// bb leave/enter
	bb.UpdateCanSee(aa.m_group, bbcanseeunit);
}

SceneGroupSet* SceneGroupLine::GetGroupSet(UINT32 group)
{
	auto i = m_group2units.find(group);
	if (i != m_group2units.end())
	{
		return &i->second;
	}
	return NULL;
}

void SceneGroupLine::BroadcastSameGroup(UINT32 group, const CProtocol& roPtc)
{
	SceneGroupSet* groupset = GetGroupSet(group);
	if (NULL != groupset)
	{
		for (auto i = groupset->m_roles.begin(); i != groupset->m_roles.end(); ++i)
		{
			Role* role = i->second;
			role->Send(roPtc);
		}
	}
}
