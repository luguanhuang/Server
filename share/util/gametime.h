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
	// ��ȡ��ǰʱ���(��ƫ��)
	time_t GetTime();
	void SetTimeOffset(INT32 offset);

	//Same Check
	bool IsInSameHour(time_t t1, time_t t2);
	bool IsInSameDay(time_t t1, time_t t2, bool offset);
	bool IsInSameWeek(time_t t1, time_t t2, bool offset);


	//�����ܼ� [1, 7]
	UINT32 GetWeekDay(bool offset=true);
	UINT32 GetWeekDay(time_t t,bool offset=true);


	//��ȡ�������
	UINT32 GetDiffDayCount(time_t t1, time_t t2);


	//��ȡday/monthΨһ���
	INT32 GetDayUnique(time_t t,bool offset=true);
	INT32 GetWeekUnique(time_t t,bool offset=true);
	INT32 GetMonthUnique(time_t t,bool offset=true);
	INT32 GetTodayUnique(bool offset=true);
	INT32 GetThisMonthUnique(bool offset=true);
	INT32 GetTodayUniqueTag();


	//��ȡһ��/һ�ܿ�ʼʱ��
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
