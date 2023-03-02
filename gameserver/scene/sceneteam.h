#ifndef __SCENETEAM_H__
#define __SCENETEAM_H__

#include "table/ExpeditionTable.h"
#include "math/XVector3.h"
#include "scene/scene.h"
#include "gamelogic/towerrecord.h"
#include "buff/XBuffTemplateManager.h"
#include "pb/project.pb.h"

namespace KKSG
{
	class SceneSwitchData;
	class SceneSwitchTeamData;
	class TeamSynAll;
}

class Scene;
class RoleSummary;

struct CtCsInfo
{
	CtCsInfo()
	{
		leaderID = 0;
	}
	UINT64 leaderID;
	std::unordered_set<UINT64> beHelper;
	std::unordered_set<UINT64> notHelper;
};

struct MemberInfo
{
	MemberInfo()
	{
	}
	std::map<int, int> m_mapTowerFetchReward;
};

struct ItemDesc;
class SceneTeam
{
public:
	SceneTeam(Scene* scene);
	virtual ~SceneTeam();

	void Init(const KKSG::SceneSwitchTeamData& data);
	void InitRoleData(const KKSG::SceneSwitchData& roData, Role* pRole);

	inline UINT64 GetLeaderID() { return m_KKSGData.teamsyndata().leaderid(); }
	inline int GetTeamID() { return m_KKSGData.teamsyndata().teamid(); }
	inline int GetExpID() { return m_KKSGData.teamsyndata().expid();}

	void OnClientLoaded(Role* pRole);
	void Update();
	bool IsAllCross();

	void ChangeScene();
	bool HasNextScene() { return (GetNextExpID() != 0); }
	void OnSceneCompleteForDragonStatistics(bool win);
    void OnSceneComplete(bool win);
	bool HandleChangeScene(std::vector<Role*>& roles, UINT32 nextSceneID, const KKSG::SceneSwitchData& oData);

	bool FindMember(UINT64 roleID);

	int GetMemberCountWithoutRobot();

	std::vector<UINT64>& GetAllMemberIDs(){return m_allMemberIDs;}

	std::vector<RoleSummary*>& GetRobots() { return m_robotSummarys; }

	void GetTeammates(UINT64 unitID, std::vector<UINT64>& IDS);

	bool IsHelper(UINT64 roleID);
	int UseTicket(UINT64 roleID);

	void InitCostTeam(bool iswin, const std::vector<Role*>& roles, std::map<UINT64, ItemDesc>& reward);

	//爬塔
	void UpdateTowerInfo(int time);
	void AddSyncSceneFetchReward(Role* role, int itemID, int itemCount);
	std::map<int, int>* GetSyncSceneFetchReward(Role* role);
	RoleTowerInfo& GetTowerInfo(){return m_towerInfo;}

	void InitDroppableInfo();
	bool IsCostTeam();
	bool IsCostCount();

	int GotoNextScene(CtCsInfo& info);

	bool IsGoddessOrEndlessA();//女神试练或者无尽深渊
    bool IsDragonExp();//是龙本
    bool IsCompeteDragon();//是竞技龙本
    bool InAirShip();//在飞船场景中

	const KKSG::TeamSynMember* GetSynMember(UINT64 roleID);

	void InfoPrint();
	///>
private:
	//live
	void CreateLiveInfo();
	void RemoveLiveInfo();

	//change scene
	//void TeamChangeScene(UINT32 sceneID);
	//void ChangeSceneRoleFilter(UINT32 sceneID, const std::vector<Role*>& input, std::vector<Role*>& rightRoles, std::map<Role*, int>& errorRoles);
	UINT32 GetNextExpID();
	UINT32 GetNextSceneID(UINT32 nextExpID);
	void ChangeSceneRoleFilter(UINT32 expID, const std::vector<Role*>& input, std::map<Role*, int>& roleRet, CtCsInfo& info);
	int ChangeSceneCheck(std::vector<Role*>& roles, const std::map<Role*, int>& roleRet, const CtCsInfo& info);
	void ChangeSceneDataCh(UINT32 expID, KKSG::SceneSwitchTeamData& data, const std::map<Role*, int>& roleRet, const CtCsInfo& info);
	void ChangeSceneFailNtf(const std::map<Role*, int>& roleRet);
	void ChagneSceneDeathRole(const std::vector<Role*>& roles, KKSG::SceneSwitchData& data);
	void ChangeSceneTotalCostTime(KKSG::SceneSwitchData& data);

	//robot
	void AddRobot(const Vector3& pos, float face);
	void SetRobotAttrPrecent(DummyRole* pDummy, ExpeditionTable::RowData *pRow, bool rolecopy);

	//buff
	//void InitRoleBuffs();
	void AddRoleBuffs(Role* pRole);

	//other
	UINT32 GetRoleCountWithoutHelper();
	void WinAddTeamDayCount();

	void ResultNtf(bool iswin);

	//const KKSG::TeamSynMember* GetSynMember(UINT64 roleID);

	bool IsTsTeam();

	void CheckSetCanDestroy();

	void CheckKingBackReward();

	Scene* m_pScene;
	ExpeditionTable::RowData* m_pConf;

	//role(no robot)
	std::vector<UINT64> m_allMemberIDs;
	// robot
	std::vector<RoleSummary*> m_robotSummarys;

	//std::unordered_map<UINT64, std::vector<BuffDesc> > m_roleBuffs;

	KKSG::SceneSwitchTeamData m_KKSGData;
	bool m_addRobotFlag;

	time_t m_createTime;

	//爬塔
	std::map<UINT64, MemberInfo> m_memberInfos;
	RoleTowerInfo m_towerInfo;

	std::set<UINT64> m_switchDeadRoles;
};

#endif // 
