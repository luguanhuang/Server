#ifndef _LEAGUE_BATTLE_BASE_H_
#define _LEAGUE_BATTLE_BASE_H_

enum LeagueGroupType
{
	LeagueGroup_None = 0,
	LeagueGroup_Red = 1,
	LeagueGroup_Blue = 2,
};

struct LeagueBattleRole
{
	UINT64 m_roleId;
	KKSG::LeagueBattleRoleState m_state;
	UINT32 m_killNum;
};

class SceneLeague;

struct LeagueBattleTeam
{
	LeagueBattleTeam();

	UINT32 m_serverId;
	std::string m_serverName;
	UINT64 m_leagueTeamId;
	std::string m_name;
	UINT32 m_oldScore;
	UINT32 m_newScore;
	int m_deltaScore;
	UINT32 m_oldRank;
	UINT32 m_newRank;
	int  m_deltaRank;
	UINT32 m_totalNum;
	UINT32 m_winNum;
	std::vector<UINT64> m_allmembers;				// 所有成员id，无论是否在线，是否上阵
	std::vector<LeagueBattleRole> m_battleRoles;	// 上阵成员

	void LoadFromKKSG(const KKSG::LeagueBattleTeamData& data);
	void FillTeamBrief(KKSG::LeagueBattleTeamBrief& data);
	void FillTeamData(KKSG::LeagueBattleTeamData& data);
	void FillBattleEndDataToMS(KKSG::LeagueBattleUpdateDataG2M& data, UINT64 winLeagueId, LeagueBattleTeam& other);
	

	inline UINT32 GetMemberSize() { return (UINT32)m_allmembers.size(); }
	bool HasMember(UINT64 roleId);
	inline UINT32 GetBattleRoleSize() { return (UINT32)m_battleRoles.size(); }
	bool HasBattleRole(UINT64 roleId);
	void AddBattleRole(UINT64 roleId);
	void RemoveBattleRole(UINT64 roleId);
	LeagueBattleRole* GetBattleRole(UINT64 roleId);
	KKSG::LeagueBattleRoleState GetBattleRoleState(UINT64 roleId);
	UINT64 GetNextBattleRoleId();

	void UpdateScore(bool isWin, UINT32 otherScore);
};

class LeagueBattleRoom
{
public:
	LeagueBattleRoom(): m_lastCheckTime(0), m_allRank(0) {}
	void LoadFromKKSG(const KKSG::LeagueBattleInfo& info);
	UINT32 GetGroup(UINT64 roleId);
	bool IsRaceMatch()
	{
		return m_battleType == KKSG::LeagueBattleType_RacePoint || m_battleType == KKSG::LeagueBattleType_CrossRacePoint;
	}
	bool IsEliMatch()
	{
		return m_battleType == KKSG::LeagueBattleType_CrossRacePoint || m_battleType == KKSG::LeagueBattleType_CrossEliminate;
	}



	bool IsRoleLoad(UINT64 roleId);
	void AddRoleLoad(UINT64 roleId);
	void RemoveRoleLoad(UINT64 roleId);

	inline KKSG::LeagueBattleType GetBattleType() { return m_battleType; }
	LeagueBattleTeam* GetBattleTeam(UINT64 roleId);
	LeagueBattleTeam* GetBattleTeamById(UINT64 leagueTeamId);
	LeagueBattleTeam& GetRedTeam() { return m_teamRed; }
	LeagueBattleTeam& GetBlueTeam() { return m_teamBlue; }

	LeagueBattleRole* GetBattleRole(UINT64 roleId);
	void SetRoleState(UINT64 roleId, KKSG::LeagueBattleRoleState state);

	void FillTeamBrief(UINT64 roleId, KKSG::LeagueBattleTeamBrief& team);
	void FillRoleBrief(UINT64 roleId, KKSG::LeagueBattleRoleBrief& info);

	void FillTeamData(UINT64 roleId, KKSG::LeagueBattleTeamData& data);
	void FillTeamData(LeagueBattleTeam* team, KKSG::LeagueBattleTeamData& data);
	void FillRedTeamData(KKSG::LeagueBattleTeamData& data);
	void FillBlueTeamData(KKSG::LeagueBattleTeamData& data);

	void FillTeamUpData(LeagueBattleTeam* team, KKSG::LeagueBattleOneTeam& data);
	void FillRedTeamUpData(KKSG::LeagueBattleOneTeam& data);
	void FillBlueTeamUpData(KKSG::LeagueBattleOneTeam& data);

	void FillTeamResultDataById(UINT64 leagueId, KKSG::LeagueBattleResultTeam& data);
	void FillTeamResultData(LeagueBattleTeam* team, KKSG::LeagueBattleResultTeam& data);

	void CheckSwitchWatcher(SceneLeague* battle);
	void UpdateScore(UINT64 winId);

	void NotifyReadyEndToMs(UINT32 sceneId);
	void NotifyEndToMs(UINT32 sceneId, UINT64 winTeamId);
	void NotifyLiveInfoToMsOrWorld(UINT32 sceneId, UINT32 liveId);

	bool IsChangingWatch(UINT64 roleId) { return m_changeingWatcher.find(roleId) != m_changeingWatcher.end(); }
	void AddChangingWatch(UINT64 roleId) { m_changeingWatcher.insert(roleId); }
	void RemoveChangingWatch(UINT64 roleId) { m_changeingWatcher.erase(roleId); }

public:
	UINT32 m_roomId;
	KKSG::LeagueBattleType m_battleType;
	LeagueBattleTeam m_teamRed;
	LeagueBattleTeam m_teamBlue;
	std::map<UINT64, KKSG::LeagueBattleRoleBrief> m_briefs;
	std::vector<UINT64> m_loadedRole;
	std::set<UINT64> m_changeingWatcher;
	UINT32 m_lastCheckTime;
	UINT32 m_allRank;
};

#endif
