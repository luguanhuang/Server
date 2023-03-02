#ifndef __SCENESKYCITY_H__
#define __SCENESKYCITY_H__

#include "utility/state.h"
#include "scenevsbase.h"
#include "vshandler.h"
#include "pb/project.pb.h"

class Enemy;
class Scene;
class Role;

class SceneSkyCity : public SceneVsBase
{
public:
	SceneSkyCity(Scene* pScene);
	~SceneSkyCity(){}
	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual void InitState();
	void CheckInitRobot();
	inline UINT32 GetRobotTeam(){ return (UINT32)(-1); }

	virtual UINT32 GetGroup(UINT64 roleid);

	virtual bool OnEvent(const VsEvent& event);
	virtual void Update();

	virtual void PreDoEnterScene(Role* pRole);

	virtual VsStateType IsAllLoad();
	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded);

	virtual void OnEnterP321();

	virtual void OnEnterFight();
	virtual VsStateType CheckFightEnd();

	virtual void OnEnterAfterFight();
	virtual VsStateType CheckAfterFightEnd();

	void OnStateEnd();
	
	void InitDataToClient(Role* pRole = NULL);
	void AllInfoToClient(Role* pRole = NULL);
	void GameResultToClient();
	void GameResultToMS();
	void DoTxLog(UINT64 roleid, UINT32 TeamID, std::string& trans);
	void FillNowData(KKSG::SkyCityRes& roRes);
	void FillInitData(KKSG::SkyCityAllTeamBaseInfo& data);
	void FillAllInfo(KKSG::SkyCityAllInfo& data);
	void FillLastResult(Role* pRole, KKSG::SkyCityFinalInfo& data);

	UINT32 GetTeamScore(UINT32 teamid);

	inline void SetCheckReviveFlag(bool flag) { m_checkReviveFlag = flag; }
	inline bool CheckReviveFlag(){ return m_checkReviveFlag; }
	void CheckRevive();
	void ResetPosition(bool rest = false);

	bool OneGroupAllDeadOrQuit();
	bool DeadOrQuit(UINT64 unitID);

	void MakeAllStatistics();

	void GetSameGroupRoles(Role* pRole, std::vector<Role*>& roles);

	inline UINT32 GetFightCount(){ return m_fightCount; }
	void AddFightCount(){ ++m_fightCount; }

	KKSG::SkyCityTimeType GetTimeType();
	UINT32 GetStateLeftTime();

	UINT32 GetWinTeam();

	UINT32 GetTeam11();
	UINT32 GetTeam22();
	UINT32 GetGroup11();
	UINT32 GetGroup22();

	inline UINT32 GetFloor() { return m_oSkyCity.nfloor(); }
	inline UINT32 GetSkyGroup() { return m_oSkyCity.ngroupid(); }
	inline UINT32 GetGameNum() { return m_oSkyCity.ngames(); }
	inline UINT32 GetEndTime() { return m_oSkyCity.endtime(); }

	bool IsLastGame();

private:
	std::string GetRoleName(UINT64 roleID);

private:

	KKSG::SkyCitySceneInfo m_oSkyCity;

	VsRoleDataMgr m_datamgr;
	VsLoadMgr m_loadmgr;

	std::vector<UINT64> m_unitids;//为了遍历方便

	time_t m_lastSynTime;
	UINT32 m_saveTime;//战斗提前结束 节约的时间
	int m_fightCount;

	bool m_checkReviveFlag;//
};

#endif // 
