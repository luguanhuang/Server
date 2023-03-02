#ifndef __XLEVEL_WAVE_H__
#define __XLEVEL_WAVE_H__

#include "math/XVector3.h"
#include "XLevelTask.h"
#include "XLevelScriptMgr.h"

class Role;
enum LevelInfoType
{
	TYPE_NONE,
    TYPE_ID,
    TYPE_BASEINFO,
    TYPE_PREWAVE,
    TYPE_EDITOR,
    TYPE_MONSTERINFO,
	TYPE_SCRIPT,
	TYPE_EXSTRING,
	TYPE_SPAWNTYPE,
};

enum LevelSpawnType
{
	Spawn_Source_Monster,
	Spawn_Source_Player,
	Spawn_Source_Random,
	Spawn_Source_Doodad,
	Spawn_Source_Robot,
};

enum XLevelWinCondition
{
	LevelWin_None,
	LevelWin_Boss,
	LevelWin_Killall,
	LevelWin_Kill_N,
	LevelWin_Time,
	LevelWin_PVP,
	LevelWin_BossRush,
	LevelWin_TeamDie,
	LevelWin_KillEnemyScore,
};

enum XLevelLoseCondtion
{
	LevelLose_None,
	LevelLose_PlayerDie,
	LevelLose_TeamDie,
	LevelTime_Out,
	LevelLose_NpcDie,
	LevelLose_LeaderDie,
};

struct XLevelWin
{
	XLevelWinCondition cond;
	int param;
};

struct XLevelLose
{
	XLevelLoseCondtion cond;
	int param;
};

class RoleSummary;

class XLevelWave
{
public:
	 int m_Id;
	 LevelSpawnType m_SpawnType;
	 int m_Time;
	 int m_LoopInterval;
	 int m_EnemyID;
	 int m_YRotate;
	 int m_RandomID;

	 std::string m_ExString;
	 std::vector<int> m_PreWaves;
	 float m_PreWavePercent;

	 std::map<int, Vector3> m_Monsters;
	 std::map<int, float> m_MonsterRotation;

	 float m_RoundRidus;
	 int m_RoundCount;

	 std::vector<int> m_DoodadID;
	 float m_DoodadPercent;
	 int m_Repeat;

	 std::string m_Levelscript;
	 
	 int m_RobotLookupID;//机器人配置ID

	 XLevelWave(): m_Id(0), m_Time(0), m_LoopInterval(0), m_EnemyID(0), m_YRotate(0), m_PreWavePercent(0.0f), m_RoundRidus(0), m_RoundCount(0), m_DoodadPercent(0), m_Repeat(0), m_RobotLookupID(0) {}
	 void ReadFromeFile(std::stringstream& ss);

protected:
	void ParseInfo(const std::string &data);

};

class XLevelDynamicInfo
{
public:
	XLevelDynamicInfo();
	void Reset();
	void Replace(UINT64 lastID, UINT64 newID);
	void ReplaceRandomizedEnemyID(int newID);
	bool IsSpawned(UINT64 enemyID);

public:
	int m_Id;
	bool m_PushInfoTask;
	float m_GenerateTime;
	int m_TotalCount;
	int m_GenerateCount;
	int m_DieCount;
	time_t m_PreWaveFinishTime;
	time_t m_ExstringFinishTime;
	int m_RandomizedEnemyID;

	std::vector<UINT64> m_enemyIds;
	std::vector<UINT64> m_dummyIds;
};

class XLevelWaveInfo
{
public:
	~XLevelWaveInfo();

	//int m_Id;
	XLevelWave m_staticLevelWaveInfo;
	XLevelDynamicInfo m_dynamicWaveInfo;

	std::map<int ,XLevelWaveInfo*> m_preLevelWaveInfos;
};

struct XLevelState
{
public:
	struct XExtString
	{
		XExtString(const std::string& str)
		{
			count = 1;
			extString = str;
		}

		std::string extString;
		UINT32 count;
	};

	unsigned int m_current_scene_id;
	int m_total_monster;
	int m_total_kill;
	int m_total_kill_levelspawn;
	bool m_isBossDie;
	bool m_refuseRevive;

	int m_bossNum;
	int m_bossDieNum;

	unsigned int m_my_team_alive;
	unsigned int m_op_team_alives;

	std::unordered_map<std::string, XExtString> m_exstring;

	std::map<UINT32, UINT32> m_killEnemyNum;

	UINT64 m_targetRoleID;

	XLevelState()
	{
		Reset();
	}

	void Reset();

	void SetExternalString(const std::string& str);

	bool QueryExternalString(std::string& str, bool decCount);

	void AddBossDieNum(int num);

	bool IsBossAllDie();

	void SetTargetRoleID(UINT64 roleID);
	
	UINT64 GetTargetRoleID();
};

struct XAllRoleExecutScriptInfo
{
	std::string m_funcName;
	std::set<UINT64> m_roleIDs;
};
typedef std::vector<XAllRoleExecutScriptInfo> XAllRoleExecutScript;

struct XAllRoleMissingExecutScriptInfo
{
	std::string m_funcName;
	std::set<Role*> m_role;
};
typedef std::vector<XAllRoleMissingExecutScriptInfo> XAllRoleMissingExecutScript;


class XLevelSpawner;
struct XLevelCallBack
{
	XLevelCallBack(XLevelSpawner* spawner);
	virtual void operator()() = 0;

	XLevelSpawner* m_pSpawner;
	UINT32 m_callBackNum;
};
struct XLevelScriptCallBack:XLevelCallBack
{
	friend class XLevelSpawner;
	friend class XLevelScript;
private:
	XLevelScriptCallBack(XLevelSpawner* pSpawner);
	virtual void operator()();
	void SetCallBackFunc(const std::string& callback);

	std::string m_callbackFunc;
};

class XLevelSpawner;
class XLevelScript
{
public:
	XLevelScript();
	~XLevelScript(){}
	void Init(XLevelSpawner* pSpawner);
	void Execute(std::string& func);
	void Execute(Role* role, std::string& func);
	void ReconnectExecute(Role* role, std::string& func);

	void ChangeWallState(const std::string& name, bool enable);

	const std::vector<XLevelInfo>* GetLevelScriptInfos(){return &m_levelScriptInfos;}
	void GetLevelScriptInfosWithNum(std::vector<XLevelInfo>& infos);
	const std::unordered_map<std::string, bool>& GetChangeDoorState(){return m_doorChangedState;}

	void GetRoleMissingScript(XAllRoleMissingExecutScript& infos);
	void GetRoleReconnectScript(XAllRoleMissingExecutScript& infos);

	void AddRoleExecutedScript(std::string& func);

protected:
	void NoticeClientExecuteScript(Scene* scene, std::string& func);
	void NoticeRoleExecuteScript(Role* role, std::string& func);

	//void _AddRoleExecutedScript(std::string& func);
	void _AddRoleExecutedScript(std::string& func, UINT64 roleID);

private:
	XLevelSpawner* m_pSpawner;
	UINT32 m_dwSceneTemplateID;

	std::map<std::string, XLevelFunc> m_funcScripts;
	std::vector<XLevelInfo> m_levelScriptInfos;
	XAllRoleExecutScript m_allRoleExecuteScript;

	std::unordered_map<std::string, bool> m_doorChangedState;
};

class Enemy;
class Scene;
class DummyRole;

class XLevelSpawner
{
public:
	static const int spawn_monster_per_frame = 2;

	UINT32 m_dwSceneTemplateID;
	std::map<int, XLevelWaveInfo> m_waveInfos;
	std::queue<XLevelBaseTask*> m_tasks;
	std::queue<XLevelBaseTask*> m_roleMissingTasks;
	XLevelState m_oLevelState;
		
public:
	XLevelSpawner(UINT32 dwSceneTemplateID, Scene* pScene): m_dwSceneTemplateID(dwSceneTemplateID), m_fTime(0), m_fRestTime(0), m_executeTimes(0), m_pScene(pScene), m_callbackNum(0) {}
	~XLevelSpawner() { UnInit(); }
	void Init();
	void UnInit();

	bool Update(float time);

	void ResetDynamicInfo();

	void OnMonsterDie(Enemy *pEnemy);
	void OnDummyRoleDie(DummyRole *pDummy);
	void OnDoodadFetch(int waveID);

	void OnEnterScene(Role* role);
	void AddRestTime(UINT32 time);
	void ResetRestTime();

	Scene* GetScene() { return m_pScene; }
	XLevelWaveInfo* GetLevelWaveInfo(int dwWaveID);
	XLevelScript* GetLevelScript(){return &m_levelScript;}

	UINT32 GetLevelCallBackNum();
	XLevelCallBack* CreateScriptCallBack();
	void OnCallBackFuncFinish(int callBackNum);
	void GenerateCallBackScriptTask(XLevelScriptCallBack* callBack);

	bool IsLevelWin();
	bool IsLevelLose();

	int GetTotalEnemyCount(){return m_oLevelState.m_total_monster;}

	void BossRevive(int waveID, UINT64 lastTempID, Enemy* enemy);

	void ReplaceRandmizedEnemyID(int waveID, int enemyID);

	//void AddRobotSummary(RoleSummary* summary);
	//void RandomRobotExString();
	//void GetRobotSummarys(std::vector<RoleSummary*>& summarys){ summarys = m_robotSummarys;}
	int GetWinConditionTime();
	int GetLoseConditionTime();
	bool HasWinConditionScore();

	void FillPreloadInfo(KKSG::SceneCfg& cfg);

	void PushReconnectRole(Role* role);
	const std::set<UINT64>& GetReconnectRole(){return m_reconnectRoles;}

protected:
	void SetupFinishCondition();
	void ProcessTaskQueue(float time);
	void GenerateTask(time_t curTime, float time);
	void GenerateNormalTask(XLevelWaveInfo *pWaveInfo);
	void GenerateRoundTask(XLevelWaveInfo *pWaveInfo);
	void GenerateScriptTask(XLevelWaveInfo *pWaveInfo);
	void GenerateRoleScriptTask(Role* role, std::string& func, bool isReconnect=false);
	void CheckMissingRoleScript();
	void CheckReconnectRoleScript();
	void SyncScriptState(Role* role);

private:
	bool _CheckRestTime(float time);
	void _ClearReconnectRoles(){m_reconnectRoles.clear();}

protected:
	float m_fTime;
	float m_fRestTime;
	UINT64 m_executeTimes;

	std::vector<XLevelWin> m_WinConditon;
	std::vector<XLevelLose> m_LoseCondition;

	std::set<UINT64> m_reconnectRoles;
	
	Scene* m_pScene;
	XLevelScript m_levelScript;

	UINT32 m_callbackNum;
	std::map<int, XLevelCallBack*> m_callBackList;

private:
	//std::vector<RoleSummary*> m_robotSummarys;//敌对机器人使用的summary
	//std::vector<std::string> m_robotExString;//出现的机器人exsring
};


#endif // __XLEVEL_WAVE_H__
