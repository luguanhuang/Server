#include "pch.h"
#include "leagueseasonbase.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "util/timespecificmgr.h"

LeagueSeasonBase::LeagueSeasonBase()
:m_seasonOpenTime(0)
,m_seasonEndTime(0)
,m_nextSeasonOpenTime(-1)
,m_lastCanOpenTime(0)
,m_lastApplyTime(0)
,m_lastRacePointDayZero(0)
,m_eleMapBuildTime(0)
,m_selfEndTime(-1)
,m_crossStartTime(-1)
,m_crossEleMapBuildTime(0)
,m_lastCrossRacePointDayZero(0)
,m_todayState(KKSG::LBTS_BeforeOpen)
,m_clearSelfServer(false)
,m_clearSelfTime(0)
,m_lastCheckOpenTime(0)
,m_selfEliShowStartTime(0)
,m_selfEliShowEndTime(0)
,m_crossEliShowStartTime(0)
,m_crossEliShowEndTime(0)
{

}

LeagueSeasonBase::~LeagueSeasonBase()
{

}

void LeagueSeasonBase::BuildTimeState()
{
	m_stateMap.clear();
	m_seasonOpenTime = GetPreStartTime();;
	m_seasonEndTime = 0;
	m_nextSeasonOpenTime = GetNextStartTime();

	m_lastCanOpenTime = 0;
	m_lastApplyTime = 0;
	m_lastRacePointDayZero = 0;
	m_eleMapBuildTime = 0;
	m_selfEndTime = -1;
	m_crossStartTime = -1;
	m_crossEleMapBuildTime = 0;
	m_lastCrossRacePointDayZero = 0;

	m_clearSelfServer = (GetGlobalConfig().LeagueClearOpen != 0) ? true : false;
	m_clearSelfTime = -1;

	// ������������
	if (m_seasonOpenTime == (UINT32)(-1))
	{
		return;
	}
	UINT32 startZeroTime = XCommon::GetDayBeginTime(m_seasonOpenTime, false);
	m_stateMap[m_seasonOpenTime] = KKSG::LBTS_BeforeOpen;

	// �����׶�
	UINT32 applyEndTime = startZeroTime + GetGlobalConfig().LeagueMatchSign * SECONDS_ONE_DAY;
	m_stateMap[applyEndTime] = KKSG::LBTS_Apply;

	// �����Ա�����ʱ��
	m_lastApplyTime = applyEndTime - SECONDS_ONE_DAY;

	// �����Կ�����ʱ��
	m_lastCanOpenTime = m_seasonOpenTime;
	//m_lastCanOpenTime = applyEndTime - SECONDS_ONE_DAY;

	UINT32 curTime = applyEndTime;
	// ����������(4-11)
	for (int i = 0; i < GetGlobalConfig().LeagueRacePointLastDay; ++i)
	{
		UINT32 weekDay = GameTime::GetWeekDay(curTime, false);	// ��һ��֮ǰ��ȡ�ܼ�
		auto& raceWeekDay = GetGlobalConfig().LeagueBattlePointRaceWeekDay;
		curTime += SECONDS_ONE_DAY;								// ����Ľ�����
		// ������
		if (std::find(raceWeekDay.begin(), raceWeekDay.end(), weekDay) != raceWeekDay.end())
		{
			m_stateMap[curTime] = KKSG::LBTS_PointRace;
			m_lastRacePointDayZero = curTime - SECONDS_ONE_DAY;
		}
		// ����
		else
		{
			m_stateMap[curTime] = KKSG::LBTS_Idle;
		}
	}

	// �������ɰ�ǿ����ͼ��ʱ��(���һ��������������һ��ʱ��)
	{
		UINT32 year, month, day;
		XCommon::GetDate(m_lastRacePointDayZero, year, month, day);
		UINT32 hour = GetGlobalConfig().LeagueBattlePointRaceTime[1] / 100;
		UINT32 min = GetGlobalConfig().LeagueBattlePointRaceTime[1] % 100;
		UINT32 endTime = XCommon::MakeTimeNew(year, month, day, hour, min);
		m_eleMapBuildTime = endTime + GetGlobalConfig().LeagueBattleEleMapTime;
	}

	// ��̭������
	for (int i = 0; i < GetGlobalConfig().LeagueEleminationAfterDay; ++i)
	{
		curTime += SECONDS_ONE_DAY;
		m_stateMap[curTime] = KKSG::LBTS_Idle;
	}
	m_stateMap[curTime] = KKSG::LBTS_Elimination;

	// ������̭��������������
	m_selfEndTime = curTime;

	m_selfEliShowStartTime = curTime - SECONDS_ONE_DAY;

	if (GetGlobalConfig().LeagueClearAfterCrossRacePointDay == 0)
	{
		m_clearSelfTime = curTime;
	}
	else
	{
		m_clearSelfTime = curTime + (GetGlobalConfig().LeagueClearAfterCrossRacePointDay - 1) * SECONDS_ONE_DAY;
	}

	LogInfo("Clear Self Time[%u]", m_clearSelfTime);

	// ������̭�������������֮����
	for (int i = 0; i < GetGlobalConfig().LeagueCrossRacePointAfterDay; ++i)
	{
		curTime += SECONDS_ONE_DAY;
		m_stateMap[curTime] = KKSG::LBTS_CrossIdle;
	}

	// �����ʼʱ��
	m_crossStartTime = curTime;
	m_selfEliShowEndTime = m_crossStartTime;

	// ���������
	for (int i = 0; i < GetGlobalConfig().LeagueCrossRacePointLastDay; ++i)
	{
		UINT32 weekDay = GameTime::GetWeekDay(curTime, false);	// ��һ��֮ǰ��ȡ�ܼ�
		auto& raceWeekDay = GetGlobalConfig().LeagueBattleCrossPointRaceWeekDay;
		curTime += SECONDS_ONE_DAY;
		// ������
		if (std::find(raceWeekDay.begin(), raceWeekDay.end(), weekDay) != raceWeekDay.end())
		{
			m_stateMap[curTime] = KKSG::LBTS_CrossPointRace;
			m_lastCrossRacePointDayZero = curTime - SECONDS_ONE_DAY;
		}
		// ����
		else
		{
			m_stateMap[curTime] = KKSG::LBTS_CrossIdle;
		}
	}

	// �������ɿ����ǿ����ͼ��ʱ��(���һ�����������������һ��ʱ��)
	{
		UINT32 year, month, day;
		XCommon::GetDate(m_lastCrossRacePointDayZero, year, month, day);
		UINT32 hour = GetGlobalConfig().LeagueBattlePointRaceTime[1] / 100;
		UINT32 min = GetGlobalConfig().LeagueBattlePointRaceTime[1] % 100;
		UINT32 endTime = XCommon::MakeTimeNew(year, month, day, hour, min);
		m_crossEleMapBuildTime = endTime + GetGlobalConfig().LeagueBattleEleMapTime;
	}

	// ����������������̭�����
	for (int i = 0; i < GetGlobalConfig().LeagueCrossEleminationAfterDay; ++i)
	{
		curTime += SECONDS_ONE_DAY;
		m_stateMap[curTime] = KKSG::LBTS_CrossIdle;
	}
	// �����̭������
	m_stateMap[curTime] = KKSG::LBTS_CrossElimination;

	m_crossEliShowStartTime = curTime - SECONDS_ONE_DAY;

	for (int i = 0; i < GetGlobalConfig().LeagueSeasonEndAfterDay; ++i)
	{
		curTime += SECONDS_ONE_DAY;
		m_stateMap[curTime] = KKSG::LBTS_SeasonEnd;
	}
	// ��������ʱ��
	m_seasonEndTime = curTime - SECONDS_ONE_DAY;
	m_crossEliShowEndTime = m_seasonEndTime;

	// ������������������ʼ��һ������
	m_nextSeasonOpenTime = GetNextStartTime();
	m_stateMap[m_nextSeasonOpenTime] = KKSG::LBTS_BeforeOpen;

	for (auto it = m_stateMap.begin(); it != m_stateMap.end(); ++it)
	{
		LogInfo("%s : state[%u]", TimeUtil::GetDateTimeStr(it->first).c_str(), it->second);
	}

	// �ϴμ�鿪��������ʱ������¸���������ʱ�䣬 ˵���Ĺ�ʱ�䣬����Ѽ�鿪��ʱ������
	// GM�Ĺ�ʱ��ļ������ 1.�ϴμ�鿪��ʱ����δ��  2.�ϴ�
	UINT32 now = GameTime::GetTime();
	if (m_lastCheckOpenTime > now)
	{
		m_lastCheckOpenTime = 0;
	}
}

UINT32 LeagueSeasonBase::GetPreStartTime()
{
	UINT32 year, month, day;
	UINT32 now = GameTime::GetTime();
	XCommon::GetDate(year, month, day);

	UINT32 maxPreOpenTime = 0;
	for (size_t i = 0; i < GetGlobalConfig().LeagueBattleOpenDate.size(); ++i)
	{
		month = GetGlobalConfig().LeagueBattleOpenDate[i][0];
		day = GetGlobalConfig().LeagueBattleOpenDate[i][1];
		UINT32 hour = GetGlobalConfig().LeagueBattleOpenDate[i][2];

		UINT32 openSec = XCommon::MakeTimeNew(year, month, day, hour);
		if (now > openSec)
		{
			if (openSec > maxPreOpenTime)
			{
				maxPreOpenTime = openSec;
			}
		}
	}

	// ����ӽ�����û���ҵ��ϸ���������ȥ����
	if (maxPreOpenTime == 0)
	{
		year--;
		for (size_t i = 0; i < GetGlobalConfig().LeagueBattleOpenDate.size(); ++i)
		{
			month = GetGlobalConfig().LeagueBattleOpenDate[i][0];
			day = GetGlobalConfig().LeagueBattleOpenDate[i][1];
			UINT32 hour = GetGlobalConfig().LeagueBattleOpenDate[i][2];

			UINT32 openSec = XCommon::MakeTimeNew(year, month, day, hour);
			if (now > openSec)
			{
				if (openSec > maxPreOpenTime)
				{
					maxPreOpenTime = openSec;
				}
			}
		}
	}

	// û���ܿ���������
	if (maxPreOpenTime == 0)
	{
		return (UINT32)(-1);
	}

	return maxPreOpenTime;
}

UINT32 LeagueSeasonBase::GetNextStartTime()
{
	UINT32 year, month, day;
	UINT32 now = GameTime::GetTime();
	XCommon::GetDate(year, month, day);

	UINT32 minNextOpenTime = -1;
	for (size_t i = 0; i < GetGlobalConfig().LeagueBattleOpenDate.size(); ++i)
	{
		month = GetGlobalConfig().LeagueBattleOpenDate[i][0];
		day = GetGlobalConfig().LeagueBattleOpenDate[i][1];
		UINT32 hour = GetGlobalConfig().LeagueBattleOpenDate[i][2];

		UINT32 openSec = XCommon::MakeTimeNew(year, month, day, hour);
		if (now < openSec)
		{
			if (openSec < minNextOpenTime)
			{
				minNextOpenTime = openSec;
			}
		}
	}

	// �ӽ���û�ҵ���һ��������ʱ�䣬��ȥ������
	if (minNextOpenTime == -1)
	{
		year++;
		for (size_t i = 0; i < GetGlobalConfig().LeagueBattleOpenDate.size(); ++i)
		{
			month = GetGlobalConfig().LeagueBattleOpenDate[i][0];
			day = GetGlobalConfig().LeagueBattleOpenDate[i][1];
			UINT32 hour = GetGlobalConfig().LeagueBattleOpenDate[i][2];

			UINT32 openSec = XCommon::MakeTimeNew(year, month, day, hour);
			if (now < openSec)
			{
				if (openSec < minNextOpenTime)
				{
					minNextOpenTime = openSec;
				}
			}
		}
	}

	return minNextOpenTime;
}

KKSG::LeagueBattleTimeState LeagueSeasonBase::GetCurStateByTime(UINT32 now)
{
	if (m_stateMap.empty())
	{
		return KKSG::LBTS_SeasonEnd;
	}
	auto it = m_stateMap.upper_bound(now);
	if (it == m_stateMap.end())
	{
		return m_stateMap.rbegin()->second;
	}
	return it->second;
}

bool LeagueSeasonBase::IsCrossPointRaceDay()
{
	return m_todayState == KKSG::LBTS_CrossPointRace;
}

bool LeagueSeasonBase::IsInCrossState()
{
	UINT32 now = GameTime::GetTime();
	return (now > m_selfEndTime) ? true : false;
}

bool LeagueSeasonBase::IsShowEli()
{
	UINT32 now = GameTime::GetTime();
	if (now >= m_selfEliShowStartTime && now < m_selfEliShowEndTime)
	{
		return true;
	}
	if (now >= m_crossEliShowStartTime && now < m_crossEliShowEndTime)
	{
		return true;
	}
	return false;
}

KKSG::LeagueEliType LeagueSeasonBase::GetEliType()
{
	UINT32 now = GameTime::GetTime();
	if (now >= m_selfEliShowStartTime && now < m_selfEliShowEndTime)
	{
		return KKSG::LeagueEliType_Self;
	}
	if (now >= m_crossEliShowStartTime && now < m_crossEliShowEndTime)
	{
		return KKSG::LeagueEliType_Cross;
	}
	return KKSG::LeagueEliType_None;
}

void LeagueSeasonBase::DumpSeason(std::string& output)
{
	if (m_stateMap.empty())
	{
		output = "season close";
		return;
	}
	std::ostringstream oss;
	oss << TimeUtil::GetDateTimeStr(m_seasonOpenTime) << ":" << "LBTS_Apply\n";
	for (UINT32 t = m_seasonOpenTime + SECONDS_ONE_DAY; t <= m_seasonEndTime + SECONDS_ONE_DAY; t = t + SECONDS_ONE_DAY)
	{
		UINT32 tt = XCommon::GetDayBeginTime(t, false);
		auto state = GetCurStateByTime(tt);
		oss << TimeUtil::GetDateTimeStr(tt) << ":" ;
		switch (state)
		{
		case KKSG::LBTS_Apply: oss << "LBTS_Apply\n"; break;
		case KKSG::LBTS_BeforeOpen: oss << "LBTS_BeforeOpen\n"; break;
		case KKSG::LBTS_Idle: oss << "LBTS_Idle\n"; break;
		case KKSG::LBTS_PointRace: oss << "LBTS_PointRace\n"; break;
		case KKSG::LBTS_Elimination: oss << "LBTS_Elimination\n"; break;
		case KKSG::LBTS_CrossIdle: oss << "LBTS_CrossIdle\n"; break;
		case KKSG::LBTS_CrossPointRace: oss << "LBTS_CrossPointRace\n"; break;
		case KKSG::LBTS_CrossElimination: oss << "LBTS_CrossElimination\n"; break;
		case KKSG::LBTS_SeasonEnd: oss << "LBTS_SeasonEnd\n"; break;
		}
	}
	output = oss.str();
}
