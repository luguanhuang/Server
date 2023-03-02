#ifndef _LEAGUE_TEAM_BASE_H_
#define _LEAGUE_TEAM_BASE_H_

class LeagueTeam;
class LeagueTeamBase
{
public:
	LeagueTeamBase(LeagueTeam* team): m_owner(team), m_dirty(false) {}
	virtual ~LeagueTeamBase() {}

	virtual bool LoadFromDB(const KKSG::LeagueTeamSaveData& data) = 0;
	virtual void SaveToDB(KKSG::LeagueTeamSaveData& data) = 0;
	virtual void OnDayPass() = 0;

	void SetDirty(bool dirty = true);
	inline bool IsDirty() { return m_dirty; }

protected:
	LeagueTeam* m_owner;
	bool m_dirty;
};

#endif