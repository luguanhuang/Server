#ifndef _LEAGUE_TEAM_MGR_H_
#define _LEAGUE_TEAM_MGR_H_

#include "util/utilsingleton.h"
#include "util/maplist.h"
#include "timer.h"

#include "leagueteam.h"

class CRole;
class Team;
class LeagueTeamMgr : public ITimer, public Singleton<LeagueTeamMgr>
{
public:
	LeagueTeamMgr();
	~LeagueTeamMgr();

	bool Init();
	void Uninit();
	void SaveAll2DB();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool LoadFromDB();

	// 临时放在这里
	void OnRoleLogin(CRole* pRole);
	void UpdateSeasonInfo(CRole* pRole);

	bool HasLeagueTeam(UINT64 roleId);
	bool IsInSameLeagueTeam(UINT64 roleId1, UINT64 roleId2);

	LeagueTeam* GetLeagueTeamByRoleId(UINT64 roleId);
	LeagueTeam* GetLeagueTeamById(UINT64 teamId);
	UINT64 GetLeagueTeamIdByRoleId(UINT64 roleId);

	void AddRoleLeagueTeam(UINT64 roleId, UINT64 teamId);
	void RemoveRoleLeagueTeam(UINT64 roleId);

	void AddToUpdateList(UINT64 leagueTeamId);

	void RemoveLeagueTeam(UINT64 leagueTeamId);

	KKSG::ErrorCode GMLeaveLeagueTeam(UINT64 roleId);

	// 组队相关
	int CheckJoinTeam(Team* pTeam, UINT64 roleId);
	int CheckStartTeamVote(Team* pTeam, const KKSG::TeamOPArg& roArg, UINT64& errRoleId);
	int CheckCreateTeam(Team* pTeam, const std::string& name, UINT64& errRoleId);
	int TeamStartBattle(Team* pTeam);

	int CheckNameValid(const std::string& name);

	bool IsNameUsed(const std::string& name);
	void SetNameUsed(const std::string& name);
	void RemoveNameUsed(const std::string& name);

	void OnSeasonEnd();

	bool GMCreateLeagueTeam(const std::vector<UINT64>& roleIds, const std::string& name, std::string& output);
	void GMSetRoleNum(UINT32 num) { m_gmRoleNum = num; }

	UINT32 GetTeamRoleNum();

	void SendAllRankReward();
	void SendRankReward(UINT64 ltId, UINT32 rank, bool isCross = false);
	void SendEliRoundReward(UINT32 round, UINT64 ltId, bool isWin, bool isCross = false);

	void TestGiveRoundReward(UINT32 round, UINT64 leagueId, bool isWin, bool isCross = false);

private:
	UINT64 NewId();
	LeagueTeam* CreateLeagueTeam(const std::vector<UINT64>& roleIds, UINT64 creatorId, const std::string& name);

private:
	UINT32 m_id;
	std::unordered_map<UINT64, LeagueTeam> m_teams;
	std::unordered_map<UINT64, UINT64> m_roleId2leagueTeamId;
	std::unordered_set<std::string> m_usedName;
	MapList<UINT64> m_dirtyList;
	UINT32 m_gmRoleNum;
};

#endif