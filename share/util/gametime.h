#ifndef _H_Game_Time_H__
#define _H_Game_Time_H__

#include <time.h>


#define HOUR_ONE_DAY_BEGIN	5
#define SECONDS_ONE_DAY     (3600 * 24)
#define SECONDS_ONE_WEEK    (3600 * 24 * 7)
#define SECONDS_TIME_ZONE	(3600 * 8)
#define SECONDS_OFFSET		(3600 * HOUR_ONE_DAY_BEGIN)

#define BASE_WEEK_DAY          4              /* 1970-01-01: Thursday */


namespace GameTime
{
	// 获取当前时间戳(带偏移)
	time_t GetTime();
	void SetTimeOffset(INT32 offset);

	//Same Check
	bool IsInSameHour(time_t t1, time_t t2);
	bool IsInSameDay(time_t t1, time_t t2, bool offset);
	bool IsInSameWeek(time_t t1, time_t t2, bool offset);


	//返回周几 [1, 7]
	UINT32 GetWeekDay(bool offset=true);
	UINT32 GetWeekDay(time_t t,bool offset=true);


	//获取相差天数
	UINT32 GetDiffDayCount(time_t t1, time_t t2);


	//获取day/month唯一编号
	INT32 GetDayUnique(time_t t,bool offset=true);
	INT32 GetWeekUnique(time_t t,bool offset=true);
	INT32 GetMonthUnique(time_t t,bool offset=true);
	INT32 GetTodayUnique(bool offset=true);
	INT32 GetThisMonthUnique(bool offset=true);
	INT32 GetTodayUniqueTag();


	//获取一天/一周开始时间
	time_t GetDayBeginTime(time_t t,bool offset=true);
	time_t GetWeekBeginTime(time_t t,bool offset=true);
	time_t GetTodayBeginTime(bool offset=true);
	time_t GetThisWeekBeginTime(bool offset=true);


	time_t MakeTime(time_t dwNowTime, int nHour, int nMin, int nSec);
	time_t MakeTime(UINT32 year, UINT32 month, UINT32 day);
	bool IsWhithinTime(int nBeginHour, int nBeginMin, int nBeginSec, int nEndHour, int nEndMin, int nEndSec);

	std::string GetDateStr(time_t t);
};

#endif
