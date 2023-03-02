#ifndef _LEAGUE_TEAM_BATTLE_RECORD_H_
#define _LEAGUE_TEAM_BATTLE_RECORD_H_

#include "leagueteambase.h"

class LeagueTeamBattleRecord : public LeagueTeamBase
{
public:
	LeagueTeamBattleRecord(LeagueTeam* team);

	virtual bool LoadFromDB(const KKSG::LeagueTeamSaveData& data);
	virtual void SaveToDB(KKSG::LeagueTeamSaveData& data);
	virtual void OnDayPass();

	UINT32 GetTotalNum() { return m_totalNum; }
	UINT32 GetTotalWinNum() { return m_totalWinNum; }
	UINT32 GetTotalLoseNum() { return m_totalLoseNum; }
	UINT32 GetMaxContinueWin() { return m_maxContinueWin; }
	UINT32 GetMaxContinueLose() { return m_maxContinueLose; }
	float GetTotalWinRate();

	UINT32 GetCrossTotalNum() { return m_crossNum; }
	UINT32 GetCrossWinNum() { return m_crossWinNum; }
	UINT32 GetCrossLoseNum() { return m_crossLoseNum; }
	UINT32 GetCrossMaxContinueWin() { return m_crossMaxContinueWin; }
	UINT32 GetCrossMaxContinueLose() { return m_crossMaxContinueLose; }
	float GetCrossWinRate();

	void SyncCrossData(const KKSG::CrossLeagueTeamData& data);

	void AddBattleRecord(const KKSG::LeagueBattleOneRecord& data);

	std::list<KKSG::LeagueBattleOneRecord>& GetRecords() { return m_records; }

	void AddWinNum();
	void AddLoseNum();

private:
	UINT32 m_totalNum;
	UINT32 m_totalWinNum;
	UINT32 m_totalLoseNum;
	UINT32 m_maxContinueWin;
	UINT32 m_maxContinueLose;
	UINT32 m_continueWin;
	UINT32 m_continueLose;

	// 跨服比赛相关数据， 从CrossLeagueTeam同步过来
	UINT32 m_crossNum;
	UINT32 m_crossWinNum;
	UINT32 m_crossLoseNum;
	UINT32 m_crossMaxContinueWin;
	UINT32 m_crossMaxContinueLose;
	UINT32 m_crossContinueWin;
	UINT32 m_crossContinueLose;

	std::list<KKSG::LeagueBattleOneRecord> m_records;
};

#endif