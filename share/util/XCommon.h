#ifndef __XCOMMON_H__
#define __XCOMMON_H__

#include "pch.h"
#include "XRandom.h"
#include "timeutil.h"
#include "math.h"
#include <time.h>

#define SecondsToMillionSec(x) ((UINT32)((x) * 1000))
#define MillionSecToSeconds(x) ((x) / 1000.0f)

class Vector3;

struct TimeMark
{
	std::string log;
	int param;
	int step;
	UINT64 qwEnterMark;
	
	TimeMark(const std::string strLog, int nParam = 0)
	{
		log = strLog;
		param = nParam;
		step = 0;
		qwEnterMark = TimeUtil::GetMicroSecond();
	}
	void Mark(bool bIsMS = true)
	{
		UINT64 qwLeaveMark = TimeUtil::GetMicroSecond();
		int nSpan = bIsMS ? (int)(qwLeaveMark - qwEnterMark)/1000 : (int)(qwLeaveMark - qwEnterMark);
		if (nSpan > 0)
		{
			LogWarn("%s step[%d] param[%d] timespan:[%d]%s", log.c_str(), ++step, param, nSpan, (bIsMS ? "ms":"mics"));
		}
	}

	~TimeMark()
	{
		UINT64 qwLeaveMark = TimeUtil::GetMicroSecond();
		int nSpan = (int)(qwLeaveMark - qwEnterMark)/1000;
		if (nSpan > 0)
		{
			LogWarn("%s step[%d] param:[%d] timespan:[%d]ms", log.c_str(), ++step, param, nSpan);
		}
	}
};

class XCommon
{
public:
	static bool IsPickedInRange(int n, int d);

	static UINT32 XHash(const char*);

	static inline HTIMER SetTimer(
		float delayInSeconds, 
		const XDelegate& callback, 
		const char *fileName, 
		int lineno) 
	{ 
		return SetTimer(delayInSeconds, callback, NULL, NULL, fileName, lineno); 
	}
	static inline HTIMER SetTimer(
		float delayInSeconds, 
		const XDelegate& callback, 
		void* param, 
		const char *fileName, 
		int lineno) 
	{ 
		return SetTimer(delayInSeconds, callback, NULL, param, fileName, lineno); 
	}
	static inline HTIMER SetTimer(
		float delayInSeconds, 
		const XDelegate& callback, 
		IArgs* args, 
		const char *fileName, 
		int lineno) 
	{ 
		return SetTimer(delayInSeconds, callback, args, NULL, fileName, lineno); 
	}
	static inline HTIMER SetTimer(
		float delayInSeconds, 
		const XDelegate& callback, 
		IArgs* args, 
		void* param, 
		const char *fileName, 
		int lineno) 
	{ 
		if(delayInSeconds < 0) delayInSeconds = 0; 
		return SetTimer(SecondsToMillionSec(delayInSeconds), callback, args, param, fileName, lineno); 
	}

	// utf8串中的字符数量
	static int CountUTF8Char(const std::string &s);

	static float GetSmoothFactor(float distance, float timespan, float nearenough);

	static time_t GetTodayBeginTime(bool offset=true);
	static time_t GetTodayPassSec();

	static time_t GetWeekBeginTime(bool offset=true);

	//1-7
	static UINT32 GetWeekNum(time_t dwTime = 0,bool offset=true);

	static time_t GetDayBeginTime(time_t dwTime,bool offset=true);

	static time_t GetOneDayTime();

	static UINT32 GetDiffDayCount(time_t begin, time_t end);

	static time_t MakeTime(time_t dwNowTime, int nHour, int nMin, int nSec);

	static bool IsWhithinTime(int nBeginHour, int nBeginMin, int nBeginSec, int nEndHour, int nEndMin, int nEndSec);
	
	static int GetTodayUnique(bool offset=true);

	static int GetMonthUnique(bool offset=true);

	static bool IsWeekSame(time_t begin, time_t end, bool offset = true);

	static bool IsDaySame(time_t begin, time_t end, bool offset = true);

	static bool IsHourSame(time_t t1, time_t t2);

	// 是否同年同月
	static bool IsMonthSame(time_t t1, time_t t2, bool offset = true);

	static int GetOneCount(int n);

	static bool IsNumber(const std::string& value);

	static int ValidNameCheck(const std::string &name);

	static inline int GetTodayTime()
	{
		return GetTodayUnique();
	}

	static std::string ReplaceStr(const std::string& str, const std::string& oldval, const std::string& newval);

	static void ReplaceOneStr(std::string& str, const std::string& oldval, const std::string& newval);

	static void GetDate(time_t time, UINT32& year, UINT32& month, UINT32& day);
	static void GetDate(UINT32& year, UINT32& month, UINT32& day);
	static time_t GetNextMonthStart(time_t time, bool offset = true);
	static time_t GetPreMonthStart(time_t time, bool offset = true);

	static time_t MakeTimeNew(UINT32 year, UINT32 month, UINT32 day, UINT32 hour = 0, UINT32 min = 0, UINT32 sec = 0);

	static time_t GetTime(struct tm& temp);

	//parse time from string in format: XXXX-XX-XX XX:XX:XX
	static time_t ParseTime(const std::string& timeStr);

	//parse time from string in format: XXXX/XX/XX XX:XX:XX
	static time_t ParseTimeBackSlash(const std::string& timeStr);

private:
	static HTIMER SetTimer(UINT32 delay, const XDelegate& callback, IArgs* args, void* param, const char *fileName, int lineno);

	static time_t _ParseTime(const std::string& timeStr, char seperator);

};

#endif	//__XCOMMON_H__