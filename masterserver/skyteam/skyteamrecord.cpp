#include "pch.h"
#include "skyteamrecord.h"

#include "table/globalconfig.h"

SkyTeamRecord::SkyTeamRecord(SkyTeam* pSTeam)
:SkyTeamBase(pSTeam)
,m_totalNum(0)
,m_totalWinNum(0)
,m_totalLoseNum(0)
,m_maxContinueWin(0)
,m_maxContinueLose(0)
,m_continueWin(0)
,m_continueLose(0)
,m_todayNum(0)
{
}

bool SkyTeamRecord::LoadFromDB(const KKSG::SkyTeamSaveData& data)
{
	KKSG::SkyTeamRecordData record;
	if (!record.ParseFromString(data.record()))
	{
		LogError("parse record error");
		return false;
	}
	m_totalNum = record.total_num();
	m_totalWinNum = record.total_winnum();
	m_totalLoseNum = record.total_losenum();
	m_maxContinueWin = record.max_continuewin();
	m_maxContinueLose = record.max_continuelose();
	m_continueWin = record.continuewin();
	m_continueLose = record.continuelose();
	m_todayNum = record.today_num();

	m_records.clear();
	for (int i = 0; i < record.records_size(); i++)
	{
		m_records.push_back(record.records(i));
	}
	return true;
}

void SkyTeamRecord::SaveToDB(KKSG::SkyTeamSaveData& data)
{
	KKSG::SkyTeamRecordData record;
	record.set_total_num(m_totalNum);
	record.set_total_winnum(m_totalWinNum);
	record.set_total_losenum(m_totalLoseNum);
	record.set_max_continuewin(m_maxContinueWin);
	record.set_max_continuelose(m_maxContinueLose);
	record.set_continuewin(m_continueWin);
	record.set_continuelose(m_continueLose);
	record.set_today_num(m_todayNum);

	record.clear_records();
	for (auto it = m_records.begin(); it != m_records.end(); ++it)
	{
		record.add_records()->CopyFrom(*it);
	}

	record.SerializeToString(data.mutable_record());
	SetDirty(false);
}

void SkyTeamRecord::OnDayPass()
{
	m_todayNum = 0;
	SetDirty();
}

float SkyTeamRecord::GetTotalWinRate()
{
	if (m_totalNum == 0)
	{
		return 0.0f;
	}
	return m_totalWinNum * 1.0f / m_totalNum;
}

void SkyTeamRecord::AddWinNum()
{
	m_totalWinNum++;
	m_totalNum++;
	m_continueWin++;
	m_todayNum++;

	if (m_continueWin > m_maxContinueWin)
	{
		m_maxContinueWin = m_continueWin;
	}

	m_continueLose = 0;
	SetDirty();
}

void SkyTeamRecord::AddLoseNum()
{
	m_totalLoseNum++;
	m_totalNum++;
	m_continueLose++;
	m_todayNum++;

	if (m_continueLose > m_maxContinueLose)
	{
		m_maxContinueLose = m_continueLose;
	}

	m_continueWin = 0;
	SetDirty();
}

void SkyTeamRecord::AddBattleRecord(const KKSG::SkyCraftBattleRecord& data)
{
	// 积分赛才记录对战记录，淘汰赛不记录
	if (data.type() != KKSG::SCT_RacePoint)
	{
		return;
	}
	m_records.push_back(data);
	if (m_records.size() > GetGlobalConfig().SkyCraftRecordNum)
	{
		m_records.pop_front();
	}
	SetDirty();
}

void SkyTeamRecord::FillRecord(KKSG::GetSkyCraftRecordRes& data)
{
	data.set_winnum(m_totalWinNum);
	data.set_losenum(m_totalLoseNum);
	data.set_winrate(GetTotalWinRate());
	data.set_max_continuewin(m_maxContinueWin);
	data.set_max_continuelose(m_maxContinueLose);

	data.clear_records();
	for (auto it = m_records.begin(); it != m_records.end(); ++it)
	{
		data.add_records()->CopyFrom(*it);
	}
}