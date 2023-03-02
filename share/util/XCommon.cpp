#include "pch.h"
#include "XCommon.h"
#include "timeutil.h"
#include <time.h>
#include <float.h>
#include "pb/enum.pb.h"
#include "gametime.h"
#include "util.h"

UINT32 XCommon::XHash(const char* pChar)
{
	if(pChar == NULL) return 0;

	UINT32 hash = 0;
	UINT32 len = strlen(pChar);
	for (unsigned int i = 0; i < len; ++i)
	{
		hash = (hash << 5) + hash + pChar[i];
	}

	return hash;
}

bool XCommon::IsPickedInRange(int n, int d)
{
	if(n >= d) return true;

	int i = XRandom::randInt(0, d);
	return i < n;
}

HTIMER XCommon::SetTimer(UINT32 delay, const XDelegate& callback, IArgs* args, void* param, const char *fileName, int lineno)
{
	if(delay == 0)
	{
		callback(args, param);

		if(args != NULL)
		{
			delete args;
			args = NULL;
		}

		return INVALID_HTIMER;
	}
	else
	{
		/*
		 * IArgs will be deleted after on timer or killed in CTimerMgr.
		 */
		return CTimerMgr::Instance()->SetTimer(callback, args, param, delay, 1, fileName, lineno);
	}
}

float XCommon::GetSmoothFactor(float distance, float timespan, float nearenough)
{
	float _hitFactor = 0;

	if (distance > 0)
	{
		float deltaT = TimeUtil::GetDeltaTime();
		float div = nearenough / distance;
		float frame = timespan / deltaT;

		if (frame > 1)
		{
			float q = pow(div, 1.0f / frame);
			_hitFactor = (1 - q) / deltaT;
		}
		else
		{
			_hitFactor = FLT_MAX;
		}
	}

	return _hitFactor;
}

time_t XCommon::GetTodayBeginTime(bool offset)
{
	/*time_t now;
	time(&now);
	struct tm* pstTime = localtime(&now);
	return now - (pstTime->tm_hour * 3600 + pstTime->tm_min * 60 + pstTime->tm_sec);*/
	return GameTime::GetTodayBeginTime(offset);
}

time_t XCommon::GetTodayPassSec()
{
	return GameTime::GetTime() - GetTodayBeginTime(false);
}

time_t XCommon::GetWeekBeginTime(bool offset)
{
	/*time_t now;
	time(&now);
	struct tm* pstTime = localtime(&now);
	UINT32 week = (pstTime->tm_wday ? pstTime->tm_wday : 7);
	return now - (pstTime->tm_hour * 3600 + pstTime->tm_min * 60 + pstTime->tm_sec + (week - 1) * 86400);*/
	return GameTime::GetThisWeekBeginTime(offset);
}

UINT32 XCommon::GetWeekNum(time_t dwTime,bool offset)
{
	/*time_t t = dwTime;
	time(&t);
	struct tm* pstTime = localtime(&t);
	UINT32 week = (pstTime->tm_wday ? pstTime->tm_wday : 7);
	return week;	*/
	return 0 == dwTime ? GameTime::GetWeekDay(offset): GameTime::GetWeekDay(dwTime,offset);
}

time_t XCommon::GetDayBeginTime(time_t dwTime,bool offset)
{
	/*struct tm* pstTime = localtime(&dwTime);
	return dwTime - (pstTime->tm_hour * 3600 + pstTime->tm_min * 60 + pstTime->tm_sec);*/
	return GameTime::GetDayBeginTime(dwTime,offset);
}


time_t XCommon::MakeTime(time_t dwNowTime, int nHour, int nMin, int nSec)
{
	/*tm* stTime = localtime(&dwNowTime);
	stTime->tm_hour = nHour;
	stTime->tm_min = nMin;
	stTime->tm_sec = nSec;
	return mktime(stTime);*/
	return GameTime::MakeTime(dwNowTime, nHour, nMin, nSec);
}

bool XCommon::IsWhithinTime(int nBeginHour, int nBeginMin, int nBeginSec, int nEndHour, int nEndMin, int nEndSec)
{
	/*time_t dwNowTime = time(NULL);
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
	return false;*/
	return GameTime::IsWhithinTime(nBeginHour, nBeginMin, nBeginSec, nEndHour, nEndMin, nEndSec);
}

int XCommon::GetTodayUnique(bool offset)
{
	/*time_t now = time(NULL);
	struct tm *timeinfo = localtime(&now);
	return timeinfo->tm_year * 1000 +  timeinfo->tm_yday;*/
	return GameTime::GetTodayUnique(offset);
}

bool XCommon::IsWeekSame(time_t begin, time_t end, bool offset/* = true*/)
{
	/*if (end - begin < 0)
	{
		return true;
	}
	int span = (int)(end - begin);
	if (span >= 7*24*3600)
	{
		return false;
	}
	struct tm* begininfo = localtime(&begin);
	int beginweek = (0 == begininfo->tm_wday) ? 7: begininfo->tm_wday;
	struct tm* endinfo = localtime(&end);
	int endweek = (0 == endinfo->tm_wday) ? 7: endinfo->tm_wday;
	if (endweek > beginweek)
	{
		return true;
	}
	else if (endweek < beginweek)
	{
		return false;
	}
	else
	{
		return span <= 24*3600;	
	}*/
	return GameTime::IsInSameWeek(begin, end, offset);
}


bool XCommon::IsDaySame(time_t begin, time_t end, bool offset/* = true*/)
{
	/*int span = (int)(end - begin);
	if (span >= 24*3600)
	{
		return false;
	}

	struct tm* begininfo = localtime(&begin);
	int	begin_mday = begininfo->tm_mday;
	struct tm* endinfo = localtime(&end);
	int end_mday = endinfo->tm_mday;

	if (begin_mday == end_mday)
	{
		return true;
	}

	return false;*/
	return GameTime::IsInSameDay(begin, end, offset);
}

bool XCommon::IsHourSame(time_t timeOne, time_t timeTwo)
{
	/*time_t t = timeOne > timeTwo ? (timeOne - timeTwo) : (timeTwo - timeOne);
	if (t >= 60 * 60)
	{
		return false;
	}

	struct tm* tOne = localtime(&timeOne);
	int hourOne = tOne->tm_hour; 
	struct tm* tTwo = localtime(&timeTwo);
	int hourTwo = tTwo->tm_hour; 
	if (tOne != NULL && tTwo != NULL)
	{
		return hourOne == hourTwo; 
	}

	return false;*/
	return GameTime::IsInSameHour(timeOne, timeTwo);
}

bool XCommon::IsMonthSame(time_t t1, time_t t2, bool offset)
{
	if (offset)
	{
		t1 = (t1 > SECONDS_OFFSET) ? (t1 - SECONDS_OFFSET) : t1;
		t2 = (t2 > SECONDS_OFFSET) ? (t2 - SECONDS_OFFSET) : t2;
	}
	UINT32 year1, month1, day1;
	UINT32 year2, month2, day2;
	GetDate(t1, year1, month1, day1);
	GetDate(t2, year2, month2, day2);

	return (year1 == year2 && month1 == month2);
}

int XCommon::GetMonthUnique(bool offset)
{
	/*time_t now = time(NULL);
	struct tm *timeinfo = localtime(&now);
	return timeinfo->tm_year * 1000 +  timeinfo->tm_mon;*/
	return GameTime::GetThisMonthUnique(offset);
}

int XCommon::CountUTF8Char(const std::string &s)
{
	const int mask = 0;
	int Count = 0;
	for (size_t i = 0; i < s.size(); ++i)
	{
		unsigned char c = (unsigned char)s[i];
		if (c <= 0x7f || c >= 0xc0)
		{
			++Count;
		}
	}

	return Count;
}

int XCommon::GetOneCount(int n)
{
	int i = 0;
	while(0 != n)
	{
		++i;
		n = n & (n -1);
	}
	return i;
}

time_t XCommon::GetOneDayTime()
{
	return (time_t)(24 * 60 * 60);
}

int XCommon::ValidNameCheck(const std::string &name)
{
	int ch = XCommon::CountUTF8Char(name);
	if (ch < 4)
	{
		return KKSG::ERR_GUILD_NAME_TOO_SHORT;
	}

	if (ch > 16)
	{
		return KKSG::ERR_GUILD_NAME_TOO_LONG;
	}

	for (size_t i = 0; i < name.size(); ++i)
	{
		if (name[i] <= 32 && name[i] > 0)
		{
			return KKSG::ERR_NAME_HAS_INVALID_CHAR;
		}
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 XCommon::GetDiffDayCount( time_t begin, time_t end )
{
	/*if(begin > end)
	{
		std::swap(begin, end);
	}
	time_t diff = GetDayBeginTime(end) - GetDayBeginTime(begin);
	return (UINT32)(diff/GetOneDayTime());*/
	return GameTime::GetDiffDayCount(begin, end);
}

bool XCommon::IsNumber(const std::string& value)
{
	const char* str = value.c_str();
	while ((*str++ != 0) && (*str >='0') && (*str <='9'));
	return *str == 0;
}


std::string XCommon::ReplaceStr(const std::string& str, const std::string& oldval, const std::string& newval)
{
	if (oldval.empty())
	{
		return str;
	}
	std::string r;
	const char* begin = str.data();
	std::string::size_type p = 0;
	std::string::size_type q = str.find(oldval);
	while (q != std::string::npos)
	{
		r.append(begin + p, begin + q);
		r.append(newval);
		p = q + oldval.size();
		q = str.find(oldval, p);
	}
	r.append(begin + p, begin + str.size());
	return r;
}

void XCommon::ReplaceOneStr(std::string& str, const std::string& oldval, const std::string& newval)
{
	std::string::size_type pos(0);
	pos = str.find(oldval);
	if( pos != std::string::npos )
	{
		str.replace(pos, oldval.length(), newval);
	}
}

void XCommon::GetDate(time_t t, UINT32& year, UINT32& month, UINT32& day)
{
	struct tm st;
#ifdef WIN32
	localtime_s(&st, &t);
#else
	localtime_r(&t, &st);
#endif

	year = st.tm_year + 1900;
	month = st.tm_mon + 1;
	day = st.tm_mday;
}

void XCommon::GetDate(UINT32& year, UINT32& month, UINT32& day)
{
	time_t t = GameTime::GetTime();
	XCommon::GetDate(t, year, month, day);
}

time_t XCommon::GetNextMonthStart(time_t time, bool offset)
{
	UINT32 year, month, day;
	GetDate(time, year, month, day);
	if (month == 12)
	{
		year += 1;
		month = 1;
	}
	else
	{
		month++;
	}
	return MakeTimeNew(year, month, 1, offset ? HOUR_ONE_DAY_BEGIN : 0);
}

time_t XCommon::GetPreMonthStart(time_t time, bool offset)
{
	UINT32 year, month, day;
	GetDate(time, year, month, day);
	if (month == 1)
	{
		year -= 1;
		month = 12;
	}
	else
	{
		month--;
	}
	return MakeTimeNew(year, month, 1, offset ? HOUR_ONE_DAY_BEGIN : 0);
}

time_t XCommon::MakeTimeNew(UINT32 year, UINT32 month, UINT32 day, UINT32 hour, UINT32 min, UINT32 sec)
{
	struct tm st;
	st.tm_year = year - 1900;
	st.tm_mon = month - 1;
	st.tm_mday = day;
	st.tm_hour = hour;
	st.tm_min = min;
	st.tm_sec = sec;
	return mktime(&st);
}

time_t XCommon::GetTime(struct tm& temp)
{
	time_t clock;
	clock = mktime(&temp);
	return clock;
}

time_t XCommon::ParseTime(const std::string& timeStr)
{
	return _ParseTime(timeStr, '-');
}

time_t XCommon::ParseTimeBackSlash(const std::string& timeStr)
{
	return _ParseTime(timeStr, '/');
}

time_t XCommon::_ParseTime(const std::string& timeStr, char seperator)
{
	std::vector<std::string> vec;
	vec = Split(timeStr, ' ');

	if (vec.size() != 2)
	{
		return 0;
	}

	std::vector<std::string> date;
	date = Split(vec[0], seperator);
	if (date.size() != 3)
	{
		return 0;
	}
	
	std::vector<std::string> time;
	time = Split(vec[1], ':');
	if (time.size() < 2)
	{
		return 0;
	}

	struct tm temp;
	temp.tm_year = convert<int>(date[0]) - 1900;
	temp.tm_mon= convert<int>(date[1]) - 1;
	temp.tm_mday = convert<int>(date[2]);

	temp.tm_hour = convert<int>(time[0]);
	temp.tm_min = convert<int>(time[1]);
	if (time.size() == 3)
	{
		temp.tm_sec = convert<int>(time[2]);
	}
	else
		temp.tm_sec = 0;

	temp.tm_isdst = -1;

	time_t timestamp = mktime(&temp);

	return timestamp;

}

