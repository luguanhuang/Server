#ifndef _CROSS_LEAGUE_TEAM_H_
#define _CROSS_LEAGUE_TEAM_H_

class CrossLeagueTeam
{
public:
	CrossLeagueTeam();
	~CrossLeagueTeam();

	void LoadFromData(const KKSG::CrossLeagueTeamData& data);
	void FillData(KKSG::CrossLeagueTeamData& data);

	void SetDirty(bool dirty = true);
	bool GetDirty() { return m_dirty; }

	UINT64 GetId() { return m_id; }

	void AddWinNum();
	void AddLoseNum();
	float GetWinRate();

public:
	bool m_dirty;
	UINT64 m_id;
	UINT32 m_serverId;
	std::string m_name;
	UINT32 m_score;
	std::vector<UINT64> m_memberIds;
	UINT32 m_totalNum;
	UINT32 m_winNum;
	UINT32 m_loseNum;
	UINT32 m_time;
	UINT32 m_continueWin;
	UINT32 m_continueLose;
	UINT32 m_maxContinueWin;
	UINT32 m_maxContinueLose;
};

#endif