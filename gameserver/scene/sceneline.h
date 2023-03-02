#ifndef __SCENELINE_H__
#define __SCENELINE_H__


enum LineSyncMode
{
	SyncMode_Common,
	SyncMode_View,
	SyncMode_Group,
};

#include "unit/role.h"
#include "scene/grid.h"
#include "unit/role.h"
#include "entity/XRole.h"

class Grid;
class Scene;
class Role;
class Unit;
class CProtocol;

#define VIEWGRID_COUNT GetGlobalConfig().ViewGridCount
#define VIEWGRID_ONCELIMIT GetGlobalConfig().ViewGridOnceLimit
#define VIEWGRID_LIMIT GetGlobalConfig().ViewGridTryLimit

struct RoleSet
{
	RoleSet() { }
	///> 按先后顺序
	std::list<Role*> m_set;
	std::unordered_map<UINT64, std::list<Role*>::iterator> m_roleid2role;

	inline std::list<Role*>::iterator Begin() { return m_set.begin(); }
	inline std::list<Role*>::iterator End() { return m_set.end(); }

	inline void AddRole(Role* role)
	{
		auto i = m_roleid2role.find(role->GetID());
		if (i != m_roleid2role.end())
		{
			SSError<<"roleid:"<<role->GetID()<<" has been added"<<END;
			return;
		}
		m_set.push_back(role);
		m_roleid2role.insert(std::make_pair(role->GetID(), --m_set.end()));
	}

	inline bool RemoveRole(Role* role)
	{
		auto i = m_roleid2role.find(role->GetID());
		if (i != m_roleid2role.end())
		{
			m_set.erase(i->second);
			m_roleid2role.erase(i);
			return true;
		}
		return false;
	}

	inline bool IsIn(Role* role)
	{
		auto i = m_roleid2role.find(role->GetID());
		return i != m_roleid2role.end();
	}
};

struct RoleViewSet
{
	Role* m_role;
	UINT64 m_lasttime;
	int m_lastoffset;
	RoleSet m_cansee;
	RoleSet m_canbesee;

	RoleViewSet(Role* role)
	:m_role(role)
	,m_lasttime(0)
	,m_lastoffset(-1)
	{

	}
};

class SceneLine
{
public:
	SceneLine();
	virtual ~SceneLine();

	virtual void Update() { };
	virtual void UpdateViewGrid(Role* role) { };
	virtual void BroadcastSameGroup(UINT32 group, const CProtocol& roPtc) { }
	virtual void BroadcastCanBeSee(Unit* poSender, const CProtocol& roPtc) { }

	virtual void Broadcast(Unit* poSender, const CProtocol& roPtc, Unit* pExludeUnit = NULL);

	virtual void AddRole(Role* poRole);
	virtual void RemoveRole(Role* poRole);
	virtual void AddUnit(Unit* unit);
	virtual void RemoveUnit(Unit* unit);

	virtual void GetCanSee(Role* role, std::vector<Role*>& roles);
	virtual void GetCanBeSee(Role* role, std::vector<Role*>& roles, int limit);
	virtual void GetUnitsCanSee(Role* role, std::vector<Unit*>& units);

	virtual void FillAll(std::vector<Unit*>& units);

	Role* IsIn(UINT64 roleid);

	bool IsFull(UINT32 maxRole) { return m_role2viewset.size() >= maxRole; }
	bool HasSlot(UINT32 maxRole, UINT32 slotcount) { return m_role2viewset.size() + slotcount <= maxRole; }

	void PrintLine(std::stringstream& ss);

public:
	int m_ID;

	///> 玩家的视野列表
	std::unordered_map<UINT64, RoleViewSet*> m_role2viewset;
	std::unordered_map<UINT64, Unit*> m_units;
};

#endif
