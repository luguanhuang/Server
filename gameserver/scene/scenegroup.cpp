#include "pch.h"
#include "scenegroup.h"
#include "unit/role.h"
#include "gamelogic/fightgroup.h"

UINT32 FightGroupHelper::GetGroup(UINT32 max, UINT32 group, UINT64 uid)
{
	if (UINT64(-1) == uid)
	{
		return group;
	}
	auto i = id2group.find(uid);
	if (i == id2group.end())
	{
		group = max + count++;
		id2group[uid] = group;
		return group;
	}
	else
	{
		return i->second;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
SceneGroup::SceneGroup()
{
	m_netural = NULL;
}

SceneGroup::~SceneGroup()
{
	if (NULL != m_netural)
	{
		delete m_netural;
		m_netural = NULL;
	}
	for (auto i = m_id2oppents.begin(); i != m_id2oppents.end(); ++i)
	{
		delete i->second;
		i->second = NULL;
	}
}

void SceneGroup::AddUnit(Unit* unit)
{
	if (unit->IsRole() && ((Role*)unit)->IsWatcher())
	{
		return;
	}
	auto k = m_units.find(unit);
	if (k != m_units.end())
	{
		SSWarn<<"is in uid:"<<unit->GetID()<<END;
		return;
	}
	m_units.insert(unit);

	///> 中立关系
	if (unit->GetFightGroup() == KKSG::FightNeutral)
	{
		if (NULL == m_netural)
		{
			m_netural = new Opponents();
		}	
		m_netural->Add(unit);
	}

	UINT32 fightgroup = unit->GetFightGroup();
	auto i = m_id2oppents.find(fightgroup);
	if (i == m_id2oppents.end())
	{
		// new fightgroup
		Opponents* opps = new Opponents();
		m_id2oppents[fightgroup] = opps;
		for (auto j = m_units.begin(); j != m_units.end(); ++j)
		{
			Unit* tempunit = *j;
			if (FightGroupMgr::Instance()->CanAttack(tempunit->GetFightGroup(), fightgroup))
			{
				opps->Add(tempunit);
			}
			else
			{
				opps->AddSameGroup(tempunit);
			}
		}
	}
	// new unit
	for (auto j = m_id2oppents.begin(); j != m_id2oppents.end(); ++j)
	{
		UINT32 tempfightgroup = j->first;
		if (FightGroupMgr::Instance()->CanAttack(fightgroup, tempfightgroup))
		{
			j->second->Add(unit);
		}
		else
		{
			j->second->AddSameGroup(unit);
		}
	}
}

bool SceneGroup::RemoveUnit(Unit* unit)
{
	if (unit->IsRole() && ((Role*)unit)->IsWatcher())
	{
		return false;
	}
	auto j = m_units.find(unit);
	if (j != m_units.end())
	{
		m_units.erase(j);

		///> 中立关系
		if (NULL != m_netural)
		{
			m_netural->Remove(unit);
		}
	
		for (auto i = m_id2oppents.begin(); i != m_id2oppents.end(); ++i)
		{
			i->second->Remove(unit);
			i->second->RemoveSameGroup(unit);
		}

		return true;
	}
	else
	{
		SSInfo<<"not find uid:"<<unit->GetID()<<END;	
		return false;
	}
}

void SceneGroup::ChangeUnit(Unit* unit)
{
	if (RemoveUnit(unit))
	{
		AddUnit(unit);
	}
	else
	{
		SSWarn<<"remove failed, uid:"<<unit->GetID()<<END;
	}
}

const std::vector<Unit*>* SceneGroup::GetUnits(UINT32 fightgroup, bool iscollide)
{
	auto i = m_id2oppents.find(fightgroup);
	if (i == m_id2oppents.end())
	{
		return NULL;
	}
	else
	{
		if (NULL == i->second)
		{
			return NULL;
		}			
		else
		{
			return iscollide ? &i->second->collision : &i->second->normal;
		}
	}
}

const std::vector<Unit*>* SceneGroup::GetNetural(bool iscollide)
{
	if (NULL == m_netural)
	{
		return NULL;
	}
	return iscollide ? &m_netural->collision : &m_netural->normal;
}

const std::vector<Unit*>* SceneGroup::GetAllyUnits(UINT32 fightgroup)
{
	auto i = m_id2oppents.find(fightgroup);
	if (i == m_id2oppents.end())
	{
		return NULL;
	}
	else
	{
		return &i->second->samegroup;
	}
}
