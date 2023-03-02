#ifndef __SCENERESWAR_H__
#define __SCENERESWAR_H__

#include "utility/state.h"
#include "scenevsbase.h"
#include "vshandler.h"
#include "pb/project.pb.h"

class Enemy;
class Scene;
class Role;

class SceneResWar : public SceneVsBase
{
public:
	SceneResWar(Scene* pScene);
	~SceneResWar(){}
	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	UINT32 GetRolePPT(UINT64 nRoleID);
	UINT32 GetAvgPPT();
	virtual void InitState();

	void OnEnterScene(Role* role);
	virtual UINT32 GetGroup(UINT64 roleid);
	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded);
	std::string GetRoleName(UINT64 roleID);
	UINT64 GetGuildID(UINT64 roleid);
	UINT64 GetGuildBuffTime(UINT64 nGuildID);
	UINT32 GetTeamID(UINT64 roleid);
	virtual bool OnEvent(const VsEvent& event);
	virtual void Update();

	virtual VsStateType IsAllLoad();

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
	void DoTxLog(UINT64 roleid, UINT32 TeamID);
	void FillNowData(KKSG::ResWarRes& roRes);
	void FillInitData(KKSG::ResWarAllTeamBaseInfo& data);
	void FillAllInfo(KKSG::ResWarAllInfo& data);

	UINT32 GetTeamScore(UINT32 teamid);
	void ResetPosition(bool rest = false);
	bool OneGroupAllDeadOrQuit();
	bool DeadOrQuit(UINT64 unitID);
	void BeforeDoEnterScene(Role* pRole);
	void GetSameGroupRoles(Role* pRole, std::vector<Role*>& roles);

	inline UINT32 GetFightCount(){ return m_fightCount; }
	void AddFightCount(){ ++m_fightCount; }

	KKSG::ResWarTimeType GetTimeType();
	UINT32 GetStateLeftTime();

	UINT32 GetWinTeam();

	UINT32 GetTeam11();
	UINT32 GetTeam22();
	UINT32 GetGroup11();
	UINT32 GetGroup22();

	inline UINT32 GetSkyGroup() { return m_oResWar.groupid(); }
	inline UINT32 GetEndTime() { return m_oResWar.endtime(); }


private:

	KKSG::ResWarSceneInfo m_oResWar;

	VsRoleDataMgr m_datamgr;
	VsLoadMgr m_loadmgr;

	std::vector<UINT64> m_unitids;//为了遍历方便

	std::map<UINT64,UINT32> m_mapPPT;
	UINT64 m_nAvgPPT;

	time_t m_lastSynTime;
	UINT32 m_saveTime;//战斗提前结束 节约的时间
	int m_fightCount;
	UINT32 m_nSelfBuffTime;
	UINT32 m_nTargetBuffTime;
};

#endif // 
