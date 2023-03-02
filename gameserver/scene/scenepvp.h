#ifndef __SCENEPVP_H__
#define __SCENEPVP_H__

#include "utility/state.h"
#include "gamelogic/pvpbasedata.h"
#include "scenevsbase.h"

namespace KKSG
{
	class PVPResult;
	class PvpNowGameData;
	class SceneSwitchData;
	class PvpBattleInfo;
	class PvpOneRec;
}

class OneRec;
class Role;
class ScenePvp : public SceneVsBase
{
public:
	ScenePvp(Scene* scene);
	~ScenePvp();

	virtual void InitState();
	virtual bool Init(const KKSG::SceneSwitchData& roData);
	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	bool Init(const KKSG::PvpBattleInfo& pvpInfo);
	void InitBattleData();

	virtual UINT32 GetGroup(UINT64 roleid);

	virtual void Update();
	virtual bool OnEvent(const VsEvent& event);

	virtual VsStateType IsAllLoad();
	virtual UINT32 GetLoadWaitTime();
	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded);

	virtual void OnEnterP321();

	virtual void OnEnterFight();
	virtual VsStateType CheckFightEnd();

	virtual void OnEnterAfterFight();
	virtual VsStateType CheckAfterFightEnd();

	virtual void OnStateEnd();

	void BattleBeginNtf();
	void BattleEndNtf(PvpCampType win, int type);
	void FillNowGameData(KKSG::PvpNowGameData& data);

	void SetRoleBattleState(UINT64 roleID, PvpBattleState rolestate);

	bool IsFirstInToBattle();

	void AddRoleDie(UINT64 roleID); 
	void ReviveRole(UINT64 roleID);
	void CheckRoleRevive(bool force = false);

	UINT32 GetGroupByRoleID(UINT64 roleID);

	UINT64 GetRedLeaderID(){return m_redLeaderID;}
	UINT64 GetBlueLeaderID(){return m_blueLeaderID;}

	void OneGameEnd(PvpCampType win, int type);
	bool AllGameEnd();
	time_t GetOneGameLeftTime();

	PvpCampType CheckRoleOutWin();
	PvpCampType CheckLeaderQuitWin();
	PvpCampType CheckLeaderDieWin();
	PvpCampType CheckTimeOutWin();
	void FillScore(PvpCampType win);
	UINT32 GetRoleCount(PvpCampType type, PvpBattleState state);

	void MakeAccounts();
	PvpCampType GetFinalWiner();	
	void FillPvpResult(KKSG::PVPResult* result, Role* pRole);

	void FillPvpRec(KKSG::PvpOneRec& rec, PvpCampType type, UINT32 win, UINT32 lose, UINT32 draw, UINT64 mvp);
	void FillPvpRoleBrief(KKSG::PvpRoleBrief& data, const KKSG::RoleSmallInfo& info);

	void TestPrint();

	const std::unordered_map<UINT64, PvpBattleData>& GetRoleDatas(){return m_roledatas;}

	void GetSamePvpCampRoles(UINT64 roleID, std::vector<UINT64>& IDS);

	UINT64 GetMVP() { return m_mvpID; }
	UINT64 GetWinUId();

	void ResetOther();

	PvpCampType GetRoleCamp(UINT64 roleId);

private:

	void RandomLeader();
	void ClearBuff();
	void LeaderAddBuff();
	void ResetHpMpContiKill();

	bool InFightStage();

	void AddPvpRecord(Role* pRole, KKSG::PvpOneRec& rec);
	std::string GetRoleName(UINT64 roleID);

	PvpBattleData* GetBattleData(UINT64 roleID);

private:

	int m_redwincount;
	int m_bluewincount;
	int m_drawcount;
	UINT64 m_redLeaderID;
	UINT64 m_blueLeaderID;
	UINT64 m_mvpID;
	std::unordered_map<UINT64, PvpBattleData> m_roledatas;

	KKSG::PvpBattleInfo m_pvpData;
};

#endif