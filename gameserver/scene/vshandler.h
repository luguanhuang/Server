#ifndef __VSHANDLER_H__
#define __VSHANDLER_H__

#include <time.h>
#include "unit/combatstatistics.h"
#include "math/XVector3.h"

class Role;
class SceneGWB;
class SceneCastle;
class Scene;
class VsLoadMgr
{
public:
	VsLoadMgr();

	void AddRole(UINT64 roleid);
	void DelRole(UINT64 roleid);
	bool HaveRole(UINT64 roleid);
	int LoadedTime(UINT64 roleid);

	UINT32 GetLoadedRoleCount() { return m_load_roles.size(); }

	void Update();

	void CheckSwitchWatcher(UINT32 SwitchCountMax, SceneGWB* gwb);

private:

	std::unordered_map<UINT64, time_t> m_load_roles;	
	time_t m_lastUpTime;
};

class VsRoleData
{
public:
	VsRoleData(UINT64 unitid, UINT32 group)
	{
		m_unitid = unitid;
		m_group = group;
		m_stop = false;
	}

	inline UINT64 GetUnitID(){ return m_unitid; }
	inline UINT32 GetGroup(){ return m_group; }

	inline void SetStatistics(const CombatStatistics& statistics){ m_combatStatistics = statistics; m_stop = true; }
	const CombatStatistics& Statistics(Scene* pScene);

	inline bool Stoped() { return m_stop; }

private:
	UINT64 m_unitid;
	UINT32 m_group;
	bool m_stop;
	CombatStatistics m_combatStatistics;
};

class VsRoleDataMgr
{
public:
	VsRoleDataMgr(Scene* pScene);
	~VsRoleDataMgr();

	void AddUnit(UINT64 unitID, UINT32 group);
	void OnUnitCombatStop(UINT64 unitID = 0);

	UINT32 GetUnitKillCount(UINT64 unitID);
	UINT32 GetUnitDeadCount(UINT64 unitID);
	double GetUnitTotalDamage(UINT64 unitID);

	UINT32 GetGroupKillCount(UINT32 group);
	double GetGroupTotalDamage(UINT32 group);

	UINT64 GetMVPID(UINT32 group);

	void Clear();

	const CombatStatistics* GetStatistics(UINT64 roleId, Scene* pScene);

private:
	std::unordered_map<UINT64, VsRoleData> m_roledata;
	Scene* m_pScene;
};

//仅用于SceneCastle
class VsJvDian
{
public:
	VsJvDian(int type, const Vector3& sPos, float tr, SceneCastle* castle);
	~VsJvDian();

	void Update();
	
private:
	UINT64 CheckBegin();
	bool CheckInterrupt();
	bool CheckEnd();

	void GetRolesInCircle(std::vector<UINT64>& roles);
	bool InCircle(const Vector3& tmpPos);

	UINT64 m_roleID;//正在占领玩家
	UINT32 m_beginTime;//开始占领时间
	UINT32 m_group;//所属分组

	int m_type;
	Vector3 m_pos;
	float m_r;
	SceneCastle* m_pCastle;
};

#endif