#include "pch.h"
#include "gametime.h"
#include <stdio.h>

static INT32 g_timeOffSet = 0;

namespace GameTime
{
	time_t GetTime()
	{
		return time(NULL) + g_timeOffSet;
	}
	
	void SetTimeOffset(INT32 offset)
	{
		g_timeOffSet = offset;
	}

	bool IsInSameHour(time_t t1, time_t t2)
	{
		return t1 / 3600 == t2 / 3600;
	}

	bool IsInSameDay(time_t t1, time_t t2, bool offset)
	{
		INT64 time1 = (INT64)t1 + SECONDS_TIME_ZONE;
		INT64 time2 = (INT64)t2 + SECONDS_TIME_ZONE;
		if(offset)
		{
			time1 -= SECONDS_OFFSET;
			time2 -= SECONDS_OFFSET;
		}
		return time1 / SECONDS_ONE_DAY == time2 / SECONDS_ONE_DAY;
	}

	bool IsInSameWeek(time_t t1, time_t t2, bool offset)
	{
		INT64 time1 = (INT64)t1 + SECONDS_TIME_ZONE;
		INT64 time2 = (INT64)t2 + SECONDS_TIME_ZONE;
		if(offset)
		{
			time1 -= SECONDS_OFFSET;
			time2 -= SECONDS_OFFSET;
		}
		INT32 week1 = (INT32)(time1 / SECONDS_ONE_DAY + BASE_WEEK_DAY - 1) / 7;
		INT32 week2 = (INT32)(time2 / SECONDS_ONE_DAY + BASE_WEEK_DAY - 1) / 7;
		return week1 == week2;
	}

	UINT32 GetWeekDay(bool offset)
	{
		time_t now = GetTime();
		return GetWeekDay(now,offset);
	}

	UINT32 GetWeekDay(time_t t,bool offset)
	{
		/*INT64 time = (INT64)t + SECONDS_TIME_ZONE - SECONDS_OFFSET;
		INT32 wday = (time / SECONDS_ONE_DAY + BASE_WEEK_DAY) % 7;
		return wday == 0 ? 7 : wday;*/

		if(t < SECONDS_OFFSET) return 0;
		if (offset)
		{
			t -= SECONDS_OFFSET;
		}
		tm* pstTime = localtime(&t);
		return pstTime->tm_wday == 0 ? 7 : pstTime->tm_wday;
	}

	UINT32 GetDiffDayCount(time_t t1, time_t t2)
	{
		INT64 time1 = (INT64)t1 + SECONDS_TIME_ZONE - SECONDS_OFFSET;
		INT64 time2 = (INT64)t2 + SECONDS_TIME_ZONE - SECONDS_OFFSET;
		INT32 nNum1 = (INT32)(time1 / SECONDS_ONE_DAY);
		INT32 nNum2 = (INT32)(time2 / SECONDS_ONE_DAY);
		return (UINT32)abs(nNum1 - nNum2);
	}

	INT32 GetDayUnique(time_t t,bool offset)
	{
		INT64 time = 0;
		if (offset)
		{
			time =  (INT64)t + SECONDS_TIME_ZONE - SECONDS_OFFSET;
		}else
		{
			time =  (INT64)t + SECONDS_TIME_ZONE;
		}
		return (INT32)(time / SECONDS_ONE_DAY);
	}

	INT32 GetWeekUnique(time_t t,bool offset)
	{
		INT64 time = 0;
		if (offset)
		{
			time =  (INT64)t + SECONDS_TIME_ZONE + (SECONDS_ONE_DAY * 3) - SECONDS_OFFSET;
		}else
		{
			time =  (INT64)t + SECONDS_TIME_ZONE + (SECONDS_ONE_DAY * 3);
		}
		return (INT32)(time / SECONDS_ONE_WEEK);
	}

	INT32 GetMonthUnique(time_t t,bool offset)
	{
		if(t < SECONDS_OFFSET) return 0;
		if (offset)
		{
			t -= SECONDS_OFFSET;
		}
		struct tm *timeinfo = localtime(&t);
		return timeinfo->tm_year * 1000 +  timeinfo->tm_mon;
	}

	INT32 GetTodayUnique(bool offset)
	{
		time_t now = GetTime();
		return GetDayUnique(now,offset);
	}

	INT32 GetThisMonthUnique(bool offset)
	{
		time_t now = GetTime();
		return GetMonthUnique(now);
	}

	time_t GetDayBeginTime(time_t t,bool offset)
	{
		if(t < SECONDS_OFFSET) return 0;
		UINT32 value = 0;
		if (offset)
		{
			value = SECONDS_OFFSET;
		}
		t -= value;
		struct tm* pstTime = localtime(&t);
		return t - (pstTime->tm_hour * 3600 + pstTime->tm_min * 60 + pstTime->tm_sec) + value;
	}

	time_t GetWeekBeginTime(time_t t,bool offset)
	{
		if(t < SECONDS_OFFSET) return 0;
		UINT32 value = 0;
		if (offset)
		{
			value = SECONDS_OFFSET;
		}
		t -= value;
		struct tm* pstTime = localtime(&t);
		UINT32 week = (pstTime->tm_wday ? pstTime->tm_wday : 7);
		return t - (pstTime->tm_hour * 3600 + pstTime->tm_min * 60 + pstTime->tm_sec + (week - 1) * 86400) + value;
	}

	time_t GetTodayBeginTime(bool offset)
	{
		time_t now = GetTime();
		return GetDayBeginTime(now,offset);
	}

	time_t GetThisWeekBeginTime(bool offset)
	{
		time_t now = GetTime();
		return GetWeekBeginTime(now,offset);
	}


	time_t MakeTime(time_t dwNowTime, int nHour, int nMin, int nSec)
	{
		tm* stTime = localtime(&dwNowTime);
		stTime->tm_hour = nHour;
		stTime->tm_min = nMin;
		stTime->tm_sec = nSec;
		return mktime(stTime);
	}

	time_t MakeTime(UINT32 year, UINT32 month, UINT32 day)
	{
		tm when;
		when.tm_year = year - 1900;
		when.tm_mon = month - 1;
		when.tm_mday = day;
		when.tm_hour = 0;
		when.tm_min = 0;
		when.tm_sec = 0;
		return mktime(&when);
	}

	bool IsWhithinTime(int nBeginHour, int nBeginMin, int nBeginSec, int nEndHour, int nEndMin, int nEndSec)
	{
		time_t dwNowTime = GetTime();
		tm* stTime = localtime(&dwNowTime);
		stTime->tm_hour = nBeginHour;
		stTime->tm_min = nBeginMin;
		stTime->tm_sec = nBeginSec;
		time_t dwBeginTime = mktime(stTime);

		stTime->tm_hour = nEndHour;
		stTime->tm_min = nEndMin;
		stTime->tm_sec = nEndSec;
		time_t dwEndTime = mktime(stTime);

		if (dwNowTime >= dwBeginTime && dwNowTime <= dwEndTime)
		{
			return true;
		}
		return false;
	}

	
	INT32 GetTodayUniqueTag()
	{
		time_t now = GetTime();
		struct tm *timeinfo = localtime(&now);
		return timeinfo->tm_year * 1000 +  timeinfo->tm_yday;
	}

#ifdef WIN32
	#define snprintf _snprintf
#endif
	std::string GetDateStr(time_t t)
	{
		struct tm *timeinfo = localtime(&t);
		UINT32 year = timeinfo->tm_year + 1900;
		UINT32 month = timeinfo->tm_mon + 1;
		UINT32 day = timeinfo->tm_mday;
		char buff[128] = {'\0'};
		snprintf(buff, sizeof(buff), "%d-%02d-%02d", year, month, day);
		return std::string(buff);
	}
};







