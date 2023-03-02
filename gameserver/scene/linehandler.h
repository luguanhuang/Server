#ifndef __LINEHANDLER_H__
#define __LINEHANDLER_H__

#include "sceneline.h"

class Role;
class Unit;
class SceneBase;
class SceneLine;
class CProtocol;
class DummyRole;
struct RoleSet;
class IFindUnitCondition;

typedef void (*SceneRoleCB)(Role* pRole, void* arg);

class LineHandler
{
public:
	LineHandler(SceneBase* scenebase);
	~LineHandler();

	void Uninit();

	void Update(float deltatime);

	// broadcast
	void BroadCastSameGroup(UINT32 group, const CProtocol& ptc);
	void BroadCastCanBeSee(Unit* sender, const CProtocol& ptc);
	void Broadcast(Unit *poSender, const CProtocol &roPtc, Unit *pExludeUnit = NULL);
	void Broadcast(const CProtocol &roPtc, Unit *pExludeUnit = NULL);
	///> 非观战者
	void BroadcastExWatcher(Unit *poSender, const CProtocol &roPtc, Unit *pExludeUnit = NULL);
	void BroadcastExWatcher(const CProtocol &roPtc, Unit *pExludeUnit = NULL);
	///> 观战者
	void BroadcastWatcher(Unit* poSender, const CProtocol &roPtcL);
	void BroadcastWatcher(const CProtocol &roPtcL);

	void AddUnitAndNotify(Unit *pUnit);
	void RemoveUnitAndNotify(Unit *pUnit);

	void GetUnitByCondition(IFindUnitCondition &cond, std::vector<Unit *> &output);
	void ForEachPlayer(SceneRoleCB cb, void* arg);
	void GetDummyRoles(std::vector<DummyRole *> &output);
	void GetSameLineUnits(Unit* unit, std::vector<Unit*> &units);

	const std::unordered_map<UINT64, Role*>& GetWatchers() { return m_Wathcers; }
	const std::list<Role*>& GetAllRoles() { return NULL == m_allrole ? m_emptyrole : m_allrole->m_set; }

	Role* IsInScene(UINT64 roleid);
	///> 能看见哪些人
	void GetRolesCanSee(Role* role, std::vector<Role*>& roles);
	///> 能被哪些人看见
	void GetRolesCanBeSee(Role* role, std::vector<Role*>& roles, int limit);
	///> 能看见的怪
	void GetUnitsCanSee(Role* role, std::vector<Unit*>& units);

	void UpdateBindRole(Role* role, Role* other);
	void UpdateViewGrid(Unit* unit);

	///> 找不到内部构造
	SceneLine* GetLine(UINT32 line);
	void ChangeLine(std::vector<Role*>& roles, UINT32 line = 0);
	void ForceOneLine(std::vector<Role*>& roles);

	UINT32 GetWatcherNum() { return m_Wathcers.size(); }

	inline bool IsViewGrid() { return SyncMode_View == m_syncmode; }
	inline bool IsCommonSync() { return SyncMode_Common == m_syncmode; }
	inline bool IsGroupSync() { return SyncMode_Group == m_syncmode; }
	inline const std::unordered_map<UINT64, Unit*>& GetUnits() { return m_NonRolesInScene; }
	void DumpLine(std::stringstream &ss);

private:
	void AddRoleAndNotify(Role *pRole);
	void AddWatcherAndNotify(Role *pRole);
	void AddNonRoleAndNotify(Unit *pUnit);

	void RemoveRoleAndNotify(Role *pRole);
	void RemoveNonRoleAndNotify(Unit *pUnit);

	void RemoveFromLine(Role* role, SceneLine* line);
	void AddToLine(Role* role, SceneLine* line);

	void AddAndNotify(Unit* pUnit);
	void Disappear(Role* role, Unit* unit);

	SceneLine *GetUnitLine(UINT64 uid);
	bool AddUnitLine(Unit* pUnit, SceneLine* line);
	SceneLine *GetAvaliableLine();
	///> 找剩余人数满足leftslot的线
	SceneLine *GetAvaliableLine(UINT32 leftslot);

	void ForceOneLine(std::vector<Role*>& roles, SceneLine* sceneline);

private:
	LineSyncMode m_syncmode;
	SceneBase* m_scene;
	UINT32 m_MaxRoleInLine;
	UINT32 m_CurrenMaxLineID;

	RoleSet* m_allrole;
	static std::list<Role*> m_emptyrole;

	std::vector<SceneLine*> m_Lines;
	std::unordered_map<UINT64, Role*> m_Wathcers;
	std::unordered_map<UINT64, Unit*> m_NonRolesInScene;
	std::unordered_map<UINT64, SceneLine*> m_Unit2Line;
};

#endif // __LINEHANDLER_H__
