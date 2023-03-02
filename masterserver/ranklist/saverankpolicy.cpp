#include "pch.h"
#include <time.h>
#include "saverankpolicy.h"

bool SaveToDBEveryPeriod::IsTimeToSave()
{
	if (m_tSaveInterval <= 0)
	{
		return false;
	}

	time_t now = time(NULL);
	if (now > m_tLastSaveTime + m_tSaveInterval)
	{
		m_tLastSaveTime = now;
		return true;
	}

	return false;
}

void SaveToDBEveryPeriod::SetSaveInterval(int time)
{
	m_tSaveInterval = time;
}

SaveToDBEveryPeriod::SaveToDBEveryPeriod()
{
	m_tSaveInterval = 0;
	m_tLastSaveTime = 0;
}

//////////////////////////////////////////////////////////////////////////
bool SaveToDBAtSpecialTime::IsTimeToSave()
{
	time_t dwTime = time(NULL);
	tm* pTime = localtime(&dwTime);
	tm temptime = *pTime;

	temptime.tm_hour = m_hour;
	temptime.tm_min = m_minute;
	time_t dwDesTime = mktime(&temptime);

	if (dwDesTime <= dwTime && (dwTime - dwDesTime) <= 30)
	{
		if (!m_IsTodayUpdate)
		{
			m_IsTodayUpdate = true;
			return true;
		}
	}
	else
	{
		m_IsTodayUpdate = false;
	}
	return false;
}

void SaveToDBAtSpecialTime::SetSaveTime(int hour, int min)
{
	m_hour = hour;
	m_minute = min;
}

SaveToDBAtSpecialTime::SaveToDBAtSpecialTime()
{
	m_IsTodayUpdate = false;
	m_hour = 0;
	m_minute = 0;
}
