#ifndef _SKY_TEAM_RECORD_H_
#define _SKY_TEAM_RECORD_H_


#include "skyteambase.h"

class SkyTeamRecord : public SkyTeamBase
{
public:
	SkyTeamRecord(SkyTeam* pSTeam);

	virtual bool LoadFromDB(const KKSG::SkyTeamSaveData& data);
	virtual void SaveToDB(KKSG::SkyTeamSaveData& data);
	virtual void OnDayPass();

	float GetTotalWinRate();
	void AddWinNum();
	void AddLoseNum();

	inline UINT32 GetTotalNum() { return m_totalNum; }
	inline UINT32 GetTotalWinNum() { return m_totalWinNum; }
	inline UINT32 GetTotalLoseNum() { return m_totalLoseNum; }
	inline UINT32 GetMaxContinueWin() { return m_maxContinueWin; }
	inline UINT32 GetMaxContinueLose() { return m_maxContinueLose; }
	inline UINT32 GetTodayNum() { return m_todayNum; }

	void AddBattleRecord(const KKSG::SkyCraftBattleRecord& data);

	void FillRecord(KKSG::GetSkyCraftRecordRes& data);

private:
	UINT32 m_totalNum;
	UINT32 m_totalWinNum;
	UINT32 m_totalLoseNum;
	UINT32 m_maxContinueWin;
	UINT32 m_maxContinueLose;
	UINT32 m_continueWin;
	UINT32 m_continueLose;
	UINT32 m_todayNum;
	std::list<KKSG::SkyCraftBattleRecord> m_records;
};

#endif
