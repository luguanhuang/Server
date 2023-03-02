#ifndef __SCENEGROUP_H__
#define __SCENEGROUP_H__

#include "unit/unit.h"

struct FightGroupHelper
{
	UINT32 count;
	std::map<UINT64, UINT32> id2group;
	FightGroupHelper()
	{
		count = 0;
	}

	UINT32 GetGroup(UINT32 max, UINT32 group, UINT64 uid);
};

struct Opponents
{
	std::vector<Unit*> normal;
	std::vector<Unit*> collision;
	std::vector<Unit*> samegroup;

	void AddSameGroup(Unit* unit)
	{
		for (auto i = samegroup.begin(); i != samegroup.end(); ++i)
		{
			if (*i == unit)
			{
				return;
			}
		}
		samegroup.push_back(unit);
	}
	void RemoveSameGroup(Unit* unit)
	{
		for (auto i = samegroup.begin(); i != samegroup.end(); ++i)
		{
			if (unit == *i)
			{
				samegroup.erase(i);
				break;
			}
		}
	}

	void Add(Unit* unit)
	{
		normal.push_back(unit);
		if (unit->IsCollider())
		{
			collision.push_back(unit);
		}
	}

	void Remove(Unit* uint)
	{
		for (auto i = normal.begin(); i != normal.end(); ++i)
		{
			if (uint == *i)
			{
				normal.erase(i);
				break;
			}
		}
		for (auto j = collision.begin(); j != collision.end(); ++j)
		{
			if (uint == *j)
			{
				collision.erase(j);
				break;
			}
		}
	}
};

class Unit;
class SceneGroup
{
public:
	SceneGroup();
	~SceneGroup();

	void AddUnit(Unit* unit);
	bool RemoveUnit(Unit* unit);
	void ChangeUnit(Unit* unit);

	const std::vector<Unit*>* GetUnits(UINT32 fightgroup, bool iscollide);		
	const std::vector<Unit*>* GetNetural(bool iscollide);		
	const std::vector<Unit*>* GetAllyUnits(UINT32 fightgroup);		

private:
	std::set<Unit*> m_units;
	Opponents* m_netural;
	std::unordered_map<UINT32, Opponents*> m_id2oppents;
};

#endif