#include "pch.h"
#include "crossleagueteam.h"
#include "timeutil.h"

CrossLeagueTeam::CrossLeagueTeam()
:m_dirty(false)
,m_id(0)
,m_serverId(0)
,m_score(0)
,m_totalNum(0)
,m_winNum(0)
,m_loseNum(0)
,m_time(0)
,m_continueWin(0)
,m_continueLose(0)
,m_maxContinueWin(0)
,m_maxContinueLose(0)
{

}

CrossLeagueTeam::~CrossLeagueTeam()
{

}

void CrossLeagueTeam::LoadFromData(const KKSG::CrossLeagueTeamData& data)
{
	m_id = data.id();
	m_serverId = data.serverid();
	m_name = data.name();
	m_score = data.score();
	
	m_memberIds.clear();
	for (int i = 0; i < data.memberids_size(); ++i)
	{
		m_memberIds.push_back(data.memberids(i));
	}
	m_totalNum = data.total_num();
	m_winNum = data.win_num();
	m_loseNum = data.lose_num();
	m_time = data.time();

	m_continueWin = data.continuewin();
	m_continueLose = data.continuelose();
	m_maxContinueWin = data.max_continuewin();
	m_maxContinueLose = data.max_continuelose();
}

void CrossLeagueTeam::FillData(KKSG::CrossLeagueTeamData& data)
{
	data.set_id(m_id);
	data.set_serverid(m_serverId);
	data.set_name(m_name);
	data.set_score(m_score);

	data.clear_memberids();
	for (int i = 0; i < m_memberIds.size(); ++i)
	{
		data.add_memberids(m_memberIds[i]);
	}

	data.set_total_num(m_totalNum);
	data.set_win_num(m_winNum);
	data.set_lose_num(m_loseNum);
	data.set_time(m_time);

	data.set_continuewin(m_continueWin);
	data.set_continuelose(m_continueLose);
	data.set_max_continuewin(m_maxContinueWin);
	data.set_max_continuelose(m_maxContinueLose);
}

void CrossLeagueTeam::SetDirty(bool dirty)
{
	m_dirty = dirty;
}

void CrossLeagueTeam::AddWinNum()
{
	m_totalNum++;
	m_winNum++;
	m_continueWin++;

	if (m_continueWin > m_maxContinueWin)
	{
		m_maxContinueWin = m_continueWin;
	}
	m_continueLose = 0;

	m_time = TimeUtil::GetTime();
}

void CrossLeagueTeam::AddLoseNum()
{
	m_totalNum++;
	m_loseNum++;
	m_continueLose++;

	if (m_continueLose > m_maxContinueLose)
	{
		m_maxContinueLose = m_continueLose;
	}
	m_continueWin = 0;

	m_time = TimeUtil::GetTime();
}

float CrossLeagueTeam::GetWinRate()
{
	if (m_totalNum == 0)
	{
		return 0.0f;
	}
	return m_winNum * 1.0f / m_totalNum;
}