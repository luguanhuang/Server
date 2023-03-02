#include "pch.h"
#include "leagueteambattlerecord.h"
#include "table/globalconfig.h"

LeagueTeamBattleRecord::LeagueTeamBattleRecord(LeagueTeam* team)
:LeagueTeamBase(team)
,m_totalNum(0)
,m_totalWinNum(0)
,m_totalLoseNum(0)
,m_maxContinueWin(0)
,m_maxContinueLose(0)
,m_continueWin(0)
,m_continueLose(0)
,m_crossNum(0)
,m_crossWinNum(0)
,m_crossLoseNum(0)
,m_crossMaxContinueWin(0)
,m_crossMaxContinueLose(0)
,m_crossContinueWin(0)
,m_crossContinueLose(0)
{

}

bool LeagueTeamBattleRecord::LoadFromDB(const KKSG::LeagueTeamSaveData& data)
{
	KKSG::LeagueBattleRecordData record;
	if (!record.ParseFromString(data.battlerecord()))
	{
		LogError("parse battle record error");
		return false;
	}

	m_totalNum = record.total_num();
	m_totalWinNum = record.total_winnum();
	m_totalLoseNum = record.total_losenum();
	m_maxContinueWin = record.max_continuwin();
	m_maxContinueLose = record.max_continuelose();
	m_continueWin = record.continuewin();
	m_continueLose = record.continuelose();

	m_crossNum = record.cross_num();
	m_crossWinNum = record.cross_winnum();
	m_crossLoseNum = record.cross_losenum();
	m_crossContinueWin = record.cross_conwin();
	m_crossContinueLose = record.cross_conlose();
	m_crossMaxContinueWin = record.cross_maxconwin();
	m_crossMaxContinueLose = record.cross_maxconlose();

	for (int i = 0; i < record.records_size(); ++i)
	{
		m_records.push_back(record.records(i));
	}

	return true;
}

void LeagueTeamBattleRecord::SaveToDB(KKSG::LeagueTeamSaveData& data)
{
	KKSG::LeagueBattleRecordData record;
	record.set_total_num(m_totalNum);
	record.set_total_winnum(m_totalWinNum);
	record.set_total_losenum(m_totalLoseNum);
	record.set_max_continuwin(m_maxContinueWin);
	record.set_max_continuelose(m_maxContinueLose);
	record.set_continuewin(m_continueWin);
	record.set_continuelose(m_continueLose);

	record.set_cross_num(m_crossNum);
	record.set_cross_winnum(m_crossWinNum);
	record.set_cross_losenum(m_crossLoseNum);
	record.set_cross_conwin(m_crossContinueWin);
	record.set_cross_conlose(m_crossContinueLose);
	record.set_cross_maxconwin(m_crossMaxContinueWin);
	record.set_cross_maxconlose(m_crossMaxContinueLose);

	for (auto it = m_records.begin(); it != m_records.end(); ++it)
	{
		record.add_records()->CopyFrom(*it);
	}

	record.SerializeToString(data.mutable_battlerecord());
	m_dirty = false;
}

void LeagueTeamBattleRecord::OnDayPass()
{

}

float LeagueTeamBattleRecord::GetTotalWinRate()
{
	if (m_totalNum == 0)
	{
		return 0.0f;
	}
	return m_totalWinNum * 1.0f / m_totalNum;
}

float LeagueTeamBattleRecord::GetCrossWinRate()
{
	if (m_crossNum == 0)
	{
		return 0.0f;
	}
	return m_crossWinNum * 1.0f / m_crossNum;
}


void LeagueTeamBattleRecord::AddWinNum()
{
	m_totalWinNum++;
	m_totalNum++;
	m_continueWin++;

	if (m_continueWin > m_maxContinueWin)
	{
		m_maxContinueWin = m_continueWin;
	}

	m_continueLose = 0;
	SetDirty();
}

void LeagueTeamBattleRecord::AddLoseNum()
{
	m_totalLoseNum++;
	m_totalNum++;
	m_continueLose++;

	if (m_continueLose > m_maxContinueLose)
	{
		m_maxContinueLose = m_continueLose;
	}

	m_continueWin = 0;
	SetDirty();
}

void LeagueTeamBattleRecord::AddBattleRecord(const KKSG::LeagueBattleOneRecord& data)
{
	// 积分赛才记录对战记录，淘汰赛不记录
	if (data.type() != KKSG::LeagueBattleType_RacePoint && data.type() != KKSG::LeagueBattleType_CrossRacePoint)
	{
		return;
	}

	m_records.push_back(data);
	if (m_records.size() > GetGlobalConfig().LeagueBattleRecordNum)
	{
		m_records.pop_front();
	}

	SetDirty();
}

void LeagueTeamBattleRecord::SyncCrossData(const KKSG::CrossLeagueTeamData& data)
{
	m_crossNum = data.total_num();
	m_crossWinNum = data.win_num();
	m_crossLoseNum = data.lose_num();
	m_crossContinueWin = data.continuewin();
	m_crossContinueLose = data.continuelose();
	m_crossMaxContinueWin = data.max_continuewin();
	m_crossMaxContinueLose = data.max_continuelose();

	SetDirty();
}
