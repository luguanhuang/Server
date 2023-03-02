#ifndef __SCENE_H__
#define __SCENE_H__

#include "math/XMath.h"
#include "gamelogic/stagerankmgr.h"
#include "aimsgchannel.h"
#include "unit/globalaiunit.h"
#include "gamelogic/fightgroup.h"
#include "scenegroup.h"
#include "commondef.h"
#include "gamelogic/teamreward.h"
#include "scenebase.h"
#include "scenecb.h"
#include "sceneweekend4v4duck.h"
#include "scenetransctrl.h"

enum SceneState
{
	SCENE_RUNNING,
	SCENE_WIN,
	SCENE_LOSE,
};

enum HEnterSceneType
{
	HEST_NONE,
	HEST_JUST_CREATED,//刚刚创建(代码层面)
	HEST_HAVE_CREATED,//已经创建(代码层面)
};

struct SceneGuildInfo
{
	UINT64 guildID;
	UINT32 guildIcon;
	string guildName;
	SceneGuildInfo()
	{
		guildID = 0;
		guildIcon = 0;
		guildName.clear();
	}
};

class DummyRole;
class Enemy;
class IFindUnitCondition;
class XBulletMgr;
class SceneMortuary;
class XLevelSpawner;
class TeamSelectReward;
class WayPoint;
class TeleportDoor;
class TeleportBase;
class SceneDoodad;
class SceneTower;
class SceneDragon;
class SceneCompeteDragon;
class SceneTeam;
class CDESceneHandler;
class CSceneBossRush;
class SceneRisk;
class SceneInspire;
class SceneSkyCityWait;
class ISceneUpdateHandler;
class ScenePk;
class SceneCustomPk;
class SceneInvFigtht;
class ScenePvp;
class SceneGMF;
class SceneGWB;
class SceneCastle;
class SceneSkyCity;
class SceneVsBase;
class WorldBossHandler;
class SceneGarden;
class SceneResWar;
class SceneStatistics;
class SceneResWarPVE;
struct XLevelDoodad;
struct PickDoodadInfo;
struct RoleBoxRewardInfo;
struct PickServerModeDoodadInfo;
class SceneGuildCamp;
class SceneKill;
class SceneInherit;
class SceneHorse;
class SceneHorseRace;
class SceneHeroBattle;
class SceneLeague;
class ScenePkTwo;
class SceneMorphInfo;
class SceneMobaBattle;
class SceneWeekEnd4v4MonsterFight;
class SceneWeekEnd4v4GhostAction;
class SceneWeekEnd4v4Duck;
class SceneWeekEnd4v4LiveChallenge;
class SceneWeekEnd4v4CrazyBomb;
class SceneWeekEnd4v4HorseRacing;
class SceneCustomPkTwo;
class SceneBMReady;
class SceneBMFight;
class SceneWedding;
class SceneBFReady;
class SceneBFFight;


class Scene : public SceneBase, public ISceneEventListener
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init(UINT32 sceneID, SceneInfo* pSceneInfo);
	virtual void Uninit();
	// update 
	virtual void Update(float deltaTime);
	void RunningSceneUpdate(float deltaTime);
	void FinishSceneUpdate(float deltaTime);

	virtual void OnPreEnterScene(Scene* pScene, Role* role);
	virtual void OnPreEnterSceneFinally(Scene* pScene, Role* pRole);
	virtual void OnEnterSceneFinally(Scene* pScene, Role* role);
	virtual void OnLeaveScene(Scene* pScene, Role* role);

	virtual void OnAddNonRole(Scene* pScene, Unit* unit);
	virtual void OnRemoveUnit(Scene* pScene, Unit* unit);
	virtual void OnRemoveWaitingRole(Scene* pScene, Role* role);

	virtual void OnRoleReconnect(Role* role);
	void BeforeRoleLoginReconnect(Role* pRole);	// 通过登录重连回到场景中的role

	void InitHandler(UINT32 scenetype);
	void InitBattleParam(const KKSG::CreateBattleParam& roParam);
	void InitSceneSwitchData(const KKSG::SceneSwitchData& roData);
	void InitRoleSwitchData(const KKSG::SceneSwitchData& roData, Role* pRole, HEnterSceneType type);

	// Scene Info
	inline SceneState GetSceneState() { return m_state; }
	inline void SetSceneState(SceneState state) { m_state = state; }
	inline bool IsSceneWinState() {return m_state == SCENE_WIN;}
	inline void SetSceneReady() { m_isready = true; }
	inline bool IsSceneReady() { return m_isready; }
	inline bool IsPvpOne() { return m_ispvpone; }

	void OnSceneReady();
	void NotifySceneState();
	void NotifyRoleWait(const std::vector<Role*>& loaded, const std::vector<std::string>& unloaded);

	bool CanCastSkill();
	bool CanCastNormalSkill();

	inline WayPoint* GetWayPoint() { return m_way; }
	inline SceneMortuary *GetMortuary() { return m_pMortuary; }
	inline XLevelSpawner* GetSpawner() { return m_pSpawner; }
	inline SceneStatistics* Statistics() { return m_statistics; }
	inline SceneTeam* GetSceneTeam() { return m_sceneTeam; }
	inline SceneVsBase* GetVsHandler() { return m_vshandler; }
	inline CDESceneHandler* GetDEHandler() { return m_DEHandler; }
	inline CSceneBossRush* GetBRHandler() { return m_BRHandler; }
	inline SceneInspire* GetInspire() { return m_pInspire; }
	inline SceneRisk* GetSceneRisk() { return m_sceneRisk; }
	inline SceneTower* GetSceneTower() { return m_pTower; }
	inline SceneSkyCityWait* GetSkyCityWait() { return m_pSceneWait; }
	inline SceneResWarPVE* GetSceneResWarPVE() { return m_pSceneResWarPVE; }
	inline SceneBMReady* GetSceneBMReady() {return m_pSceneBMReady; }
	inline WorldBossHandler* GetWorldBossHandler() { return m_worldbosshandler; }
	inline SceneDragon* GetSceneDragon() { return m_pDragon; }
	inline SceneCompeteDragon* GetSceneCompeteDragon() { return m_pCompeteDragon; }
	inline SceneGarden* GetSceneGarden() { return m_garden; }
	inline SceneHorse* GetSceneHorseWait() { return m_pHorseWait; }
	inline SceneMorphInfo* GetSceneMorphInfo(){return m_pSceneMorphInfo;}
	inline SceneWedding* GetWeddingHandler() { return m_pWedding; }
	inline SceneBFReady* GetSceneBFReady(){return m_pSceneBFReady; }


	ScenePk* GetPkHandler();
	ScenePkTwo* GetPkTwoHandler();
	SceneCustomPk* GetCustomPkHandler();
	SceneCustomPkTwo* GetCustomPkTwoHandler();
	ScenePvp* GetPvpHandler();
	SceneGWB* GetGWBHandler();
	SceneCastle* GetCastleHandler();
	SceneResWar*  GetResWarHandler();
	SceneSkyCity* GetSkyCityHandler();
	SceneHorseRace* GetHorseRaceHandler();
	SceneHeroBattle* GetHeroBattleHandler();
	SceneInvFigtht* GetInvFightHandler();
	SceneLeague* GetLeagueBattleHandler();
	SceneMobaBattle* GetMobaBattleHandler();
    SceneWeekEnd4v4MonsterFight* GetMonsterFightHandler();
    SceneWeekEnd4v4GhostAction* GetGhostActionHandler();
    SceneWeekEnd4v4Duck* GetDuckHandler();
    SceneWeekEnd4v4LiveChallenge* GetLiveChallengeHandler();
    SceneWeekEnd4v4CrazyBomb* GetCrazyBombHandler();
    SceneWeekEnd4v4HorseRacing* GetWeekEnd4v4HorseRacingHandler();
	SceneBMFight* GetBMFightHandler();
	SceneBFFight* GetBFFightHandler();


	///> guild brief
	SceneGuildInfo* GetSceneGuildInfo(UINT64 guildID);
	SceneTransCtrl* GetSceneTrans() { return m_transctrl; }

	void ChangeAttrBeforEnterScene(Unit *pUnit);

	bool IsHelper(UINT64 roleid);
	bool IsSelectBoxExcept(UINT64 roleid, UINT32 rolelevel);
	int UseTicket(UINT64 roleID);

	UINT64 GetOwnerID() { return m_ownerID; }
	void SetOwnerID(UINT64 ownerID) { m_ownerID = ownerID; }


	void SetTarja(UINT64 nRoleID) 
	{
		m_setTarja.insert(nRoleID);
	}
	
	UINT64 GetTarja(UINT64 nRoleID) 
	{
		auto iter = m_setTarja.find(nRoleID);
		if(iter!=m_setTarja.end())
		{
			return *iter;
		}
		return 0;
	}

	bool IsTarja()
	{
		return !m_setTarja.empty();
	}

	///> box
	inline bool IsCanDrawBox()const { return m_pSceneInfo->m_pConf->CanDrawBox; }
	inline TeamSelectReward* GetTeamSelectReward() const { return m_teamSelectReward; }
	void OnSetRank(UINT64 roleid, UINT32 rolelevel, UINT32 star, bool isrobot);
	//RoleBoxRewardInfo FillBoxRobot(UINT64 robotid, UINT32 star);
	const Sequence<UINT32, 2>* GetPeerItem();

	// rank
	StageRankInfo* GetStageRankInfo(UINT64 roleid);
	void SetStageRankInfo(UINT64 roleid, const StageRankInfo& info);
	int GetRoleRank(UINT64 qwRoleID) const;

	///> scene finish
	bool CheckWinDealy(SceneState state);
	bool CheckWinDelayEnd();
	bool CheckFinishEnd(SceneState state);
	void ChangeFinishState(int newState, int countdown);
	int GetFinishState() { return m_finishState; }

	bool CheckLeaveBeforeFinish(Role* role);
	void OnLeaveBeforeFinish(Role* role);
	UINT32 GetFinishStateExpireTime(){return m_finishStateExpireTime;}
	///> 获得评星相关npc的剩余血量
	UINT32 GetRankNpcHpPercent();

	///> time
	time_t CostTime();
	time_t CostTimeMili();
	int GetTimeSpan();
	/*
		单个关卡通关时间
		保证龙本连续通关只返回当前关卡的通关时间
	*/
	int GetOneSceneTimeSpan(){ return (int)(m_SceneFinishTime.tv_sec - m_sceneStartTime.tv_sec); }
	int GetAddedTimeSpan();
	void SetTimeSpan(int time);
	void AddTimeSpan(int addedTime);
	void InitSceneStartTime();

	void EndSoloScene(SceneState state, UINT32 timespan = 0);
	void EndSyncScene(SceneState state);

	void OnRoleDead(Role *pRole);
	void OnRoleRevive(Role* pRole);
	void OnEnemyDead(Enemy* enemy);
	void OnDummyRoleDead(DummyRole* pDummy);
	void OnDummyRoleRevive(DummyRole* pDummy);

	void KickAllRoleOut();
	void GetPlayersNotHelper(std::vector<Role *>& output);
	void GetTeammates(Unit* pUnit, std::vector<Unit*>& teammates);

	void ReviveDeathRole();

	//获取场景Unit
	Unit* FindSceneUnit(UINT64 unitID);
	Role* FindSceneRole(UINT64 roleID);
	Role* FindSceneRoleNotWatcher(UINT64 roleID);
	Role* FindSceneRoleCond(UINT64 roleID, bool includeLoading, bool includeWatcher);

	bool CheckDynamicBlock(const Vector3& from, const Vector3& to);
	bool CheckSequenceBlock(const Vector3& from, const Vector3& to, const Vector3& center, int& currentIdx, int& crosstype, float& degree);

	inline SceneDoodad* GetSceneDoodad(){return m_pSceneDoodad;}
	void FillDoodadInfo(KKSG::SceneCfg* pData);
	void SetPickedDoodad(UINT32 waveID);
	const std::list<PickDoodadInfo>& GetDoodadsInfo();
	std::list<PickServerModeDoodadInfo>& GetServerDoodadsInfo();

	void KillAllEnemy(bool isend = false);
	void KillWave(int waveID);
	void KillAllEnemy(int waveID);
	void KillAllAlly(int waveID);
	void ReviveEnemy(Enemy* pUnit, int waveID, UINT64 lastID);
	int GetKillEnemyScore();
	void SetSceneKillState(bool state);

	///> enemy drop
	void NotifyDoodadAppearOnEnter(Role* role);
	void GenerateEnemyDroppedDoodad(Enemy* pEnemy, bool isLastEnemy);
	void GenerateDoodad(UINT32 doodadID, UINT32 waveID, Vector3& pos);
	void FetchDoodad(Role* role, const KKSG::EnemyDoodadInfo& arg, KKSG::RollInfoRes& roRes);

	///> door
	void SyncLevelScriptState(Role* role);
	void DelTeleportDoor(TeleportBase* tele);
	TeleportBase* AddTeleportDoor(const Vector3& sPos, float tr , const Vector3& dPos, float dface);
	void UpdateTeleport();
	void ClearAllTeleport();
	bool HasTeleportDoor();

	///> global AI
	void SetStopAITime(UINT64 time) { m_stopAITime = time; }
	UINT64 GetStopAITime() { return m_stopAITime; }
	GlobalAIUnit *GetGlobalAI() { return m_globalAIUnit; }
	AISceneMsgChannel& MsgChannel() { return m_AIMsgChannel; }

	void AddEnterFight(UINT64 uid);
	int GetEnterFightTime(UINT64 uid);

	///> watcher
	bool IsWatchEnd();
	UINT32 ChangeWatchRole(Role* role, UINT64 viewID);
	void SetLiveID(UINT32 liveID){m_liveID = liveID;}
	UINT32 GetLiveID(){return m_liveID;}
	UINT32 GetAbyssID(){return m_abyssID;};
	UINT32 GetWatchRoleNum();
	UINT32 GetTotalWatchNum();
	UINT32 GetTotalCommendNum();
	bool GetSendLiveNotice(){return m_sendLiveNotice;}
	void SetSendLiveNotice(bool bSend){m_sendLiveNotice = bSend;}
	void DebugAddWatchNum(int num){m_debugAddWatchNum = num;}

	///> fight group
	UINT32 GetFightGroup(Unit* unit);
	UINT64 GetFightGroupUId(Unit* unit);
	inline SceneGroup* GetSceneGroup() { return m_scenegroup; }
	///> 只读数据，使用时不可以出现可能改变该数据的函数调用(TriggerDeath等函数)
	inline const std::vector<Unit*>* GetOpponents(UINT32 fightgroup) { return NULL == m_scenegroup ? NULL : m_scenegroup->GetUnits(fightgroup, false); }
	inline const std::vector<Unit*>* GetOpponentUnitWithCollision(UINT32 fightgroup){ return NULL == m_scenegroup ? NULL : m_scenegroup->GetUnits(fightgroup, true); };
	inline const std::vector<Unit*>* GetAllys(UINT32 fightgroup){ return NULL == m_scenegroup ? NULL : m_scenegroup->GetAllyUnits(fightgroup); };

	std::vector<Unit*> GetAllDeadSameGroupRoles(UINT32 fightgroup);

	inline const std::vector<Unit*>* GetNeturals() { return NULL == m_scenegroup ? NULL : m_scenegroup->GetNetural(false); }
	inline const std::vector<Unit*>* GetNeturalsWithCollision() { return NULL == m_scenegroup ? NULL : m_scenegroup->GetNetural(true); };

	// Create and Destroy Function
	static Scene *CreateScene(UINT32 sceneTemplateID, SceneInfo *pSceneInfo, UINT32 dwSceneInstanceID);
	static Scene *CreateScene(UINT32 sceneTemplateID, SceneInfo *pSceneInfo, UINT64 ownerID, UINT32 dwSceneInstanceID);
	static void DestroyScene(Scene *scene);
	static bool IsPVPCombat(Scene* scene);

	void OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value);

	void DumpScene(std::stringstream &ss);

	void CheckKickRole();

	void OnSkillRevive(Unit* firer, Unit* target);

	float GetPvpProfessionBias(Unit* pCaster, Unit* pReceiver);
protected:
	void FinishScene();
	void OnSceneFinish();
	void AddKillEnemyInfo(Enemy* enemy);
	float GetSequenceDegree(const Vector3& face, const Vector3& rankdir);
	void CheckInitDeath(Role* role);

private:
	std::set<UINT64> m_setTarja;
	bool m_isready;
	bool m_ispvpone;
	bool m_hasPvpBias;
	UINT32 m_abyssID;
	UINT64 m_ownerID;
	SceneState m_state;
	int m_finishState;
	int m_finishStateExpireTime;
	UINT32 m_addedTimeSpan;
	UINT32 m_serialScenesCostTime;
	timeval m_sceneStartTime;
	timeval m_SceneFinishTime;

	SceneStatistics* m_statistics;
	FightGroupHelper m_grouphelper;

	///> 场景中阵营关系
	SceneGroup* m_scenegroup;
	///> 导航点
	WayPoint* m_way;
	///> 玩家关卡星级
	std::map<UINT64, StageRankInfo>  m_StageRankInfo;

	///> 刷怪模块
	XLevelSpawner* m_pSpawner;
	///> 场景状态更新
	ISceneUpdateHandler* m_updatehandler;

	///> 玩家死亡复活管理
	SceneMortuary* m_pMortuary;
	///> 掉落
	SceneDoodad* m_pSceneDoodad;

	std::vector<TeleportBase*> m_teleports;
	
	// global AI
	UINT64 m_stopAITime;
	GlobalAIUnit* m_globalAIUnit;
	AISceneMsgChannel m_AIMsgChannel;

	///> 组队奖励
	TeamSelectReward* m_teamSelectReward;
	SceneTeam* m_sceneTeam;
	///> PK 
	SceneVsBase* m_vshandler;
	///> 玩法
	SceneTower* m_pTower;
	SceneDragon* m_pDragon;
	SceneCompeteDragon* m_pCompeteDragon;
	WorldBossHandler* m_worldbosshandler;
	CDESceneHandler* m_DEHandler;	
	CSceneBossRush* m_BRHandler;
	SceneInspire* m_pInspire;
	SceneRisk* m_sceneRisk;
	SceneSkyCityWait* m_pSceneWait;
	SceneResWarPVE* m_pSceneResWarPVE;
	SceneGarden* m_garden;
	SceneInherit* m_inherit;
	SceneHorse* m_pHorseWait;
	SceneGuildCamp* m_pGuildCamp;
	SceneWedding* m_pWedding;
	///> 击杀信息
	SceneKill* m_pSceneKill;
	SceneMorphInfo* m_pSceneMorphInfo;
	SceneBMReady* m_pSceneBMReady;
	SceneBFReady* m_pSceneBFReady;

	SceneTransCtrl* m_transctrl;

	UINT32 m_liveID;
	bool m_sendLiveNotice;
	UINT32 m_debugAddWatchNum;
	std::unordered_map<UINT64, UINT64> m_WatcherViewRole;

	///> 玩家所在公会简单信息
	std::vector<SceneGuildInfo> m_sceneGuildInfos;
	std::unordered_map<UINT64, int> m_makeEnemyEenterFight;
	UINT32 m_lastKickRoleTime;

	std::set<UINT64> m_switchDeadRoles;
};

std::ostream &operator<<(std::ostream &stream, Scene *scene); 

#endif // __SCENE_H__
