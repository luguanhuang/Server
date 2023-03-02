#ifndef __UNIT_H__
#define __UNIT_H__

#include "combatattrdef.h"
#include "pb/project.pb.h"
#include "iaiinterface.h"
#include "table/globalconfig.h"
#include "math/XVector3.h"
#include "revive.h"
#include "scene/sceneconfig.h"
#include "combatattribute.h"


// UINT64 的最高2位放UnitType
const UINT32 UnitTypeOffset = 60;
const UINT32 UnitBitMask = 15;

class CProtocol;
class XObject;
class Scene;
class ISkillSet;
class IBroadCastable;
class CombatAttribute;
class CombatAttrListener;
class Unit;
class KillerWatcher;
class XAIComponent;
class AIBehaviorTree;
class SharedData;
class XSecurityStatistics;

std::ostream &operator<<(std::ostream &stream, Unit *pUnit);

class Unit : public IAIInterface
{
	friend std::ostream &operator<<(std::ostream &stream, Unit *);

public:
	Unit();
	virtual ~Unit();

	virtual void Send(const CProtocol &roPtc, bool force = false) {}
	virtual void CleanUpInScene() {}
	void    EnterScene(Scene *scene);
	void    LeaveScene();

	inline KKSG::EntityCategory GetUnitType() { return GetUnitType(m_uID); }
	inline bool IsRole() { return GetUnitType() == KKSG::Category_Role; }
	inline bool IsEnemy()  { return GetUnitType() == KKSG::Category_Enemy;  }
	inline bool IsDummyRole() { return GetUnitType() == KKSG::Category_DummyRole; }
	inline Scene *GetCurrScene() const { return m_currScene; }
	inline XObject *GetXObject() const { return m_pObject; }
	inline float GetRunSpeed() { return (float)GetAttr(TOTAL_RUN_SPEED) * GetScale(); }
	inline float GetWalkSpeed() { return (float)GetAttr(TOTAL_WALK_SPEED) * GetScale(); }
	inline float GetRotateSpeed() { return (float)GetAttr(TOTAL_ROTATION_SPEED); }
	inline float GetAutoRotateSpeed() { return (float)GetAttr(BASIC_AUTOROTATION_SPEED); }
	inline float GetZeroSpeed() { return 1.0f; }
	inline bool IsCollider() { return m_collider; }
	inline bool IsIgnoreDynamicCollide() { return m_ignore_dynamic_collide; }
	inline int GetSummonGroup(){ return m_summongroup; }
	inline bool IsFightGroupNeutral() const { return m_fightgroup == KKSG::FightNeutral; }
	inline bool IsRoleFriendly() const { return (m_fightgroup == KKSG::FightRole || m_fightgroup == KKSG::FightNeutral); }
	inline UINT32 GetDelay() { return m_delay; }
	inline void   SetDelay(UINT32 delay) { m_delay = delay; }

	inline void   SetID(UINT64 uID) { m_uID = uID; }
	inline UINT64 GetID() const { return m_uID; }

	inline float GetBoundRaidus() { return m_boundRadius; }
	inline const Vector3& GetBoundRaidusOffset() { return m_boundRadiusOffset; }
	inline float GetBoundHeight() { return m_boundHeight; }

	inline float GetScale() { return m_scale; }
	inline float IsHuge() { return m_huge; }
	inline float GetFloatingMax() { return m_floatingMax; }
	inline float GetFloatingMin() { return m_floatingMin; }

	inline bool IsDead() const { return m_IsDead; }

	inline int  GetLevel() const { return m_level; }
	inline void SetLevel(int level) { m_level = level; }

	inline const UINT32 GetTemplateID() { return m_uTemplateID; }
	inline const UINT32 GetPresentID() { return m_uPresentID; }

	inline const UINT32 GetFightGroup() const { return m_fightgroup; }
	inline const void SetFightGroup(UINT32 fightgroup) { m_fightgroup = (KKSG::FightGroupType)fightgroup; }
	const void ChangeFightGroup(UINT32 fightgroup);

	inline bool GetDeprecated() const { return m_IsDeprecated; }
	inline void SetDeprecated(bool isdeprecated) { m_IsDeprecated = isdeprecated; }

	double Distance(Unit *pUnit);

	double GetParalyzeAttribute();
	double GetParalyzeDefense();

	inline double GetAttr(CombatAttrDef attrDef) { return m_combatAttr->GetAttr(attrDef); };
	void   SetAttr(CombatAttrDef attrDef, double value);
	void   AddAttr(CombatAttrDef attrDef, double value);
	void   CheckAttrState();
	inline CombatAttribute* GetAttrPtr() { return m_combatAttr; }
	inline ISkillSet *GetSkillLevelMgr() { return m_skillLevelMgr; }
	inline XAIComponent* AIComponent() {return m_ai;}
	inline AIBehaviorTree* BehaviorTree() {return m_behavior;}
	inline SharedData* AIData() {return m_aiData;}
	inline XSecurityStatistics* GetSecurityStatistics() { return m_securityStatistics; }

	void SetAIComponent(XAIComponent* ai){m_ai = ai;}
	void SetBehavior(AIBehaviorTree* tree){m_behavior = tree;}
	void SetAIData(SharedData* data){m_aiData = data;}

	void Revive(float hprecoverper = 1.0f);
	void TriggerDeath();
	void SendDeathNotfiy();
	void PackDeathNotfiy(KKSG::DeathInfo* info);
	void SubHp(Unit* attacker, double hurt);
	int GetHpPercent();
	double GetPreciseHpPercent();
	void ResetHpMp(float hprecoverper = 1.0f);

	virtual bool HasDeathSlowCamera() { return false; }
	virtual void OnTreat(Unit *pTargetUnit, double value) = 0;
	virtual void OnAttack(Unit *pTargetUnit, double value) = 0;
	virtual void OnHurt(Unit* pCastUnit, double value) = 0;
	virtual void OnDied() = 0;
	virtual void OnRevive() {}
	virtual void Update(float delta);
	virtual void ResetFightGroup() { };
	virtual void OnSceneReady();
	virtual UINT32 GetSkillCasterTypeID() { return m_uTemplateID; }
	virtual void GetAppearance(KKSG::UnitAppearance &Appearance, bool isbrief = true);

	virtual void StreamOutput(std::ostream &stream) = 0;
	virtual std::string GetAIString();
	virtual void GetAIArgs(AIDataArgs &args);

	virtual void EnableAI(bool isenable);
	virtual UINT32 GetTag() { return 0;}
	// revive
	SReviveInfo& GetReviveInfo() { return m_reviveinfo; }

	bool IsReviving() {return m_reviveinfo.isreviving; }
	void SetIsReviving(bool isreviving) { m_reviveinfo.isreviving = isreviving; }
	void SetIsReviveLimit(bool islimit) { m_reviveinfo.islimit = islimit; }
	void SetReviveType(KKSG::ReviveType type) { m_reviveinfo.type = type; }

	void AddReviveBuff(BuffDesc buff) { m_reviveinfo.AddBuff(buff); }
	std::vector<BuffDesc>& GetReviveBuff() { return m_reviveinfo.m_buffs; }

	static KKSG::EntityCategory GetUnitType(UINT64 uID);
	static Unit *FindUnit(UINT64 uID);
	static bool IsOppose(const Unit *beattacked, const Unit *attacker);
	static bool IsSameGroup(const Unit *other, const Unit *me);

	bool IsArmorBroken() { return m_IsArmorBroken; }
	void SetArmorBroken(bool flag) { m_IsArmorBroken = flag; }

	void SetHost(Unit* pUnit);
	UINT64 GetHostId() { return m_hostid; }
	UINT64 GetFinalHostId() { return m_finalHostid; }
	Unit* GetHostUnit();
	Unit* GetFinalHostUnit();

	KillerWatcher* GetKiller() { return m_killer; }
	UINT64 GetKillerID();
	UINT64 GetExecutionerID();

	bool IsEnterHall() { return m_entersceneinfo.isenterhall; }
	bool IsEnterPVP() { return m_entersceneinfo.isenterpvp; }
	UINT32 GetEnterSceneType() { return m_entersceneinfo.sceneType; }

	void SetCombatUseLevel(UINT32 lev) { m_combatUseLevel = lev; }
	UINT32 GetCombatUseLevel();

	UINT32 GetSpecialState() { return m_specialstate; }
	bool MakePuppet(bool ismake, bool isnotifyclient = true);
	bool MakeInvisible(bool ismake, bool isnotifyclient = true);

	void ResetBaseAttr(double lifescale, double atkscale = 1.0, double defscale = 1.0, bool notify = false);

	void ClearMobs();

	inline void SetBlockAddHP(bool bBlock) { m_bBlockAddHP = bBlock; }
	inline bool IsBlockAddHP() { return m_bBlockAddHP; }

	bool IsCycleSync() { return m_cyclesync > 0; }
	void SetCycleSync(UINT32 cycle) { m_cyclesync = cycle; }
	void SetLastSync(UINT64 time) { m_lastsync = time; }
	bool IsSyncTimeOut(UINT64 time) { return time >= (m_lastsync + m_cyclesync); }

	void AddSyncAttr(int id, double value);
	void ClearSyncAttr() { m_AttrWatch.clear(); }
	std::unordered_map<int, double>& GetSyncAttrs() { return m_AttrWatch; }


protected:
	void ResetSpecialState() { m_specialstate = 0; }
	void AddSpecialState(KKSG::UnitSpecialState state) { (m_specialstate |= ((UINT32)(1) << state)); }
	void DelSpecialState(KKSG::UnitSpecialState state) { (m_specialstate &= ((~0) ^ ((UINT32)(1) << state))); }
	bool IsInSpecailState(KKSG::UnitSpecialState state) { return (m_specialstate & ((UINT32)(1) << state)) > 0; };
	void NotifyState(UINT32 effectmask);

	void ReviveOnServer();
	void EnterSceneResetAttribute();
	bool OnAttributeRegenerate(IArgs *pargs, void *param);

	virtual void OnEnterScene(Scene *scene) = 0;
	virtual void OnLeaveScene() = 0;

protected:
	int               m_level;
	int				  m_summongroup;
	bool			  m_huge;
	bool              m_collider;
	bool			  m_ignore_dynamic_collide;
	bool              m_IsArmorBroken;
	bool              m_IsDead;
	bool			  m_IsDeprecated;
	float             m_boundRadius;
	Vector3			  m_boundRadiusOffset;
	float             m_boundHeight;
	float			  m_scale;
	float			  m_floatingMin;
	float			  m_floatingMax;
	UINT64            m_uID;
	UINT32            m_uPresentID;
	UINT32            m_uTemplateID;
	UINT32            m_delay;
	HTIMER            m_TimerToken;
	UINT32			  m_fightgroup;
	UINT64			  m_hostid;
	UINT64			  m_finalHostid;
	UINT32			  m_specialstate;
	UINT32            m_combatUseLevel;

	XObject			 *m_pObject;
	Scene			 *m_currScene;
	ISkillSet		 *m_skillLevelMgr;
	CombatAttribute  *m_combatAttr;
	KillerWatcher    *m_killer;
	XAIComponent     *m_ai;
	AIBehaviorTree   *m_behavior;
	SharedData       *m_aiData;

	SReviveInfo		m_reviveinfo;	

	UINT64 m_aiTime;

	EnterSceneInfo m_entersceneinfo;

	XSecurityStatistics *m_securityStatistics;
	bool			  m_bBlockAddHP;

	///> 定频属性同步
	UINT32 m_cyclesync;
	UINT64 m_lastsync;
	std::unordered_map<int, double> m_AttrWatch;
};


#endif // __UNIT_H__
