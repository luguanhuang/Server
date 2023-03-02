#ifndef __SCENEGROUPLINE_H__
#define __SCENEGROUPLINE_H__

#include "sceneline.h"

struct SceneGroupSet
{
	UINT32 m_group;
	std::unordered_map<UINT64, Role*> m_roles;
	std::unordered_map<UINT64, Unit*> m_id2unit;

	std::unordered_map<UINT32, std::unordered_map<UINT64, Unit*>> m_cansee;

	bool InView(UINT32 group, UINT64 uid);

	void AddUnit(Unit* unit);
	void RemoveUnit(Unit* unit);

	void BroadCast(const CProtocol& roPtc, Unit* pExludeUnit = NULL);
	void UpdateCanSee(UINT32 group, std::unordered_map<UINT64, Unit*>& newcansee);

	void InnerAppear(Unit* unit);
};

class SceneGroupLine : public SceneLine
{
public:
	SceneGroupLine();
	virtual ~SceneGroupLine();

	virtual void Update();
	virtual void Broadcast(Unit* poSender, const CProtocol& roPtc, Unit* pExludeUnit = NULL);
	virtual void BroadcastCanBeSee(Unit* poSender, const CProtocol& roPtc);
	virtual void BroadcastSameGroup(UINT32 group, const CProtocol& roPtc);

	virtual void AddRole(Role* poRole);
	virtual void RemoveRole(Role* poRole);
	virtual void AddUnit(Unit* unit);
	virtual void RemoveUnit(Unit* unit);

	virtual void GetCanSee(Role* role, std::vector<Role*>& roles);
	virtual void GetCanBeSee(Role* role, std::vector<Role*>& roles, int limit);
	virtual void GetUnitsCanSee(Role* role, std::vector<Unit*>& units);

private:
	SceneGroupSet* GetGroupSet(UINT32 group);
	void Merge(SceneGroupSet& aa, SceneGroupSet& bb);

private:
	UINT64 m_lastupdate;
	std::map<UINT32, SceneGroupSet> m_group2units;
};

#endif