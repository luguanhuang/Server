#ifndef __SCENEBASE_H__
#define __SCENEBASE_H__

#include "sceneconfig.h"
#include "scenepreparelist.h"
#include "linehandler.h"
#include "scenecollid.h"
#include "timeutil.h"

class Role;
class Unit;
class CProtocol;
class ISceneListener;
class ISceneEventListener;
class SceneBase : public IScenePrepareListener
{
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Init(UINT32 sceneID, SceneInfo *pSceneInfo);
	virtual void Uninit();

	virtual void Update(float deltatime);

	bool IsSceneNoRole();
	void OnSceneEmpty();
	inline UINT32 GetEnemyCount() { return m_EnemyCount; }
	inline UINT32 GetRoleCount() { return m_RoleCount; }
	inline UINT32 GetDummyRoleCount(){ return m_DummyRoleCount; }
	inline UINT32 GetUnitCount(){ return m_EnemyCount + m_RoleCount + m_DummyRoleCount; }

	// Scene Info
	///> 场景自增id,不是表中的SceneID
	inline UINT32 GetSceneID() { return m_SceneID; } 
	///> 这个才是表中的SceneID
	inline UINT32 GetSceneTemplateID() { return m_pSceneInfo->m_pConf->id; }
	inline UINT32 CanLoginReconnect() { return m_pSceneInfo->m_pConf->CanReconnect; }

	inline SceneInfo *GetSceneInfo() { return m_pSceneInfo; }
	
	inline bool	  IsStaticScene() { return m_bIsStaticScene; }
	inline void   SetSyncMode(int mode) { m_SyncMode = mode; }
	inline bool   IsSyncScene() { return m_SyncMode == SERVERMODE; }
	inline UINT32 GetSceneType() { return m_pSceneInfo->m_pConf->type; }
	inline Grid*  GetGrid() { return m_pSceneInfo->m_pGrid; }
	inline int    SyncMode() { return m_SyncMode; }
	inline UINT64 GetBattleID() const { return m_BattleID; }

	inline SceneCollide& GetCollide(){ return m_Collide; }

	bool CheckDestory();
	void SetDontDestory(bool flag) { m_dontDestory = flag; }
	bool GetDontDestory() { return m_dontDestory; }

	// Move Function
	bool CheckMoveReach(Unit *pUnit, const Vector3& stPos, const Vector3 &delta);
	bool TryMove(Unit *pUnit, const Vector3 &delta);
	bool SetPosition(Unit *pUnit, const Vector3 &position);

	///> 进场景
	void AddUnitAndNotify(Unit *pUnit);
	void AddRoleAndNotify(Role *pRole);
	void AddNonRoleAndNotify(Unit *pUnit);

	void AddRoleAfterLoadedScene(Role *pRole, UINT32 delayid);
	virtual void OnEnterSceneFinish();
	virtual bool OnClientLoaded(UINT64 qwRoleID, UINT32 delayid);

	///> 离开场景
	void RemoveUnitAndNotify(Unit *pUnit);
	void RemoveWaitingRole(Role *pRole);

	// Debug Function
	const char* SceneTypeStr(UINT32 dwType);
	const char* SceneTypeStr();

	inline void ForEachPlayer(SceneRoleCB cb, void* arg) { m_pUnitHandler->ForEachPlayer(cb, arg); }
	inline const std::unordered_map<UINT64, Role*>& GetWatchers() { return m_pUnitHandler->GetWatchers(); }
	inline const std::list<Role*>& GetAllRoles() { return m_pUnitHandler->GetAllRoles(); }
	inline void GetDummyRoles(std::vector<DummyRole *> &output) { m_pUnitHandler->GetDummyRoles(output); }
	inline void GetUnitByCondition(IFindUnitCondition &cond, std::vector<Unit *> &output) { m_pUnitHandler->GetUnitByCondition(cond, output); }
	///> 所有可见生物(排除Role)
	inline const std::unordered_map<UINT64, Unit*>& GetUnits() { return m_pUnitHandler->GetUnits(); }
	///> 同线的生物
	inline void GetSameLineUnits(Unit* unit, std::vector<Unit*> &output) { m_pUnitHandler->GetSameLineUnits(unit, output); }

	inline void ChangeLine(std::vector<Role*>& roles, UINT32 line = 0) { m_pUnitHandler->ChangeLine(roles, line); }
	inline void ForceOneLine(std::vector<Role*>& roles) { m_pUnitHandler->ForceOneLine(roles); }

	inline ClientLoadingInfo* GetLoadingInfo(UINT64 roleid) { return m_PrepareList->GetLoadingInfo(roleid); }
	inline std::map<UINT64, Role*>& GetWaitingRoles() { return m_RoleWaitLoadScene; }
	inline IScenePrepareList *GetPrepareList() { return m_PrepareList; }

	inline void Broadcast(Unit *poSender, const CProtocol &roPtc, Unit *pExludeUnit = NULL) { m_pUnitHandler->Broadcast(poSender, roPtc, pExludeUnit); }
	inline void Broadcast(const CProtocol &roPtc, Unit *pExludeUnit = NULL) { m_pUnitHandler->Broadcast(roPtc, pExludeUnit); }
	inline void BroadcastWatcher(const CProtocol &roPtc) { return m_pUnitHandler->BroadcastWatcher(roPtc); }
	inline void BroadcastExWatcher(const CProtocol &roPtc, Unit* pExludeUnit = NULL) { return m_pUnitHandler->BroadcastExWatcher(roPtc, pExludeUnit); }
	inline void BroadCastCanBeSee(Unit* sender, const CProtocol& ptc) { return m_pUnitHandler->BroadCastCanBeSee(sender, ptc); }
	inline void BroadCastSameGroup(UINT32 group, const CProtocol& ptc) { return m_pUnitHandler->BroadCastSameGroup(group, ptc); }

	inline bool IsViewGrid() { return m_pUnitHandler->IsViewGrid(); }
	inline Role* IsInScene(UINT64 roleid) { return m_pUnitHandler->IsInScene(roleid); }
	inline void UpdateViewGrid(Unit* unit) { m_pUnitHandler->UpdateViewGrid(unit); }

	inline void UpdateBindRole(Role* role, Role* other) { m_pUnitHandler->UpdateBindRole(role, other); }

	///> 能看见哪些人
	void GetRolesCanSeeSorted(Role* role, std::vector<Role*>& roles);
	inline void GetRolesCanSee(Role* role, std::vector<Role*>& roles) { m_pUnitHandler->GetRolesCanSee(role, roles); }
	///> 能被哪些人看见
	inline void GetRolesCanBeSee(Role* role, std::vector<Role*>& roles, int limit) { m_pUnitHandler->GetRolesCanBeSee(role, roles, limit); }
	///> 能看见的怪
	inline void GetUnitsCanSee(Role* role, std::vector<Unit*>& units) { m_pUnitHandler->GetUnitsCanSee(role, units); }
	inline void AddFinishListener(ISceneListener *pListener) { m_Listeners.insert(pListener); }
	inline void RemoveFinishListener(ISceneListener *pListener) { m_Listeners.erase(pListener); }

	inline void AddEventListener(ISceneEventListener *pListener) { m_EventListeners.insert(pListener); }

	bool IsAllRoleDisconnect();
	UINT32 GetAllDisconnectTime() { return m_allDisconnectTime; }

	bool IsPosInvalid(const Vector3& pos);

    bool IsAllRolesFromSameServerID(){return m_IsSameServerID;}
    UINT32 GetEnterRoleCount(){return m_EnterRoleCount;}
protected:
	UINT32 m_SceneID;
	int    m_SyncMode;
	UINT64 m_BattleID;
	bool   m_bIsStaticScene;
	SceneInfo *m_pSceneInfo;

	bool   m_dontDestory;

	int    m_EnemyCount;
	int    m_RoleCount;
	int    m_DummyRoleCount;

    bool   m_IsSameServerID;
    UINT32 m_FirstRoleServerid;
    UINT32 m_EnterRoleCount;

	///> 等待loading的玩家列表
	IScenePrepareList *m_PrepareList;
	///> 即将进入场景的玩家，等待客户端Load场景完成
	std::map<UINT64, Role*> m_RoleWaitLoadScene;
	///> 玩家和Enemy的列表
	LineHandler *m_pUnitHandler;

	///> 移动碰撞检测模块
	SceneCollide m_Collide;

	std::set<ISceneListener*> m_Listeners;
	std::set<ISceneEventListener*> m_EventListeners;
	bool m_hasRoleLoaded;
	bool m_allDisconnect;
	UINT32 m_allDisconnectTime;
};

UINT32 NewSceneID();

#endif