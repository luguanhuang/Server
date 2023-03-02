#ifndef __TIMESPECIFICMGR_H__
#define __TIMESPECIFICMGR_H__

#include "timer.h"
#include <map>

typedef std::function<bool(void)> TimeSpecificCallBack;

#define INVALID_TIMESPECIFIC 0 

struct TimeSpecific
{
	UINT32 hour;	///> Сʱ��Ϊ-1����ʾÿСʱ�����㣬����0~23֮��
	UINT32 min;		///> ������Ϊ-1����ʾÿ���Ӷ����㣬����0~58֮��
	UINT32 sec;		///> ������0~58֮��

	time_t time;	///> ��ʱ��ִ��

	TimeSpecific(UINT32 _hour, UINT32 _min, UINT32 _sec)
	{
		Reset();
		hour = _hour;
		min = _min;
		sec = _sec;
		if((hour != (UINT32)(-1) && hour > 23) || (min != (UINT32)(-1) && min > 59) || sec > 59)
		{
			assert(false);
		}
	}

	TimeSpecific(time_t temptime)
	{
		Reset();
		time = temptime;
	}

	TimeSpecific()
	{
		Reset();
	}

	void Reset()
	{
		hour = 0;
		min = 0;
		sec = 0;
		time = 0;
	}
};

struct TimeSpecificEvent
{
	TimeSpecific stTime;
	TimeSpecificCallBack pFuncion;
	bool isonce;
	time_t triggerTime;

	TimeSpecificEvent(const TimeSpecific& time)
	:stTime(time)
	{
		isonce = (0 != stTime.time);
	}
};

struct TimeSpecificSaveEvent
{
	bool isAdd;
	UINT32 handler;
	TimeSpecific stTime;
	TimeSpecificCallBack callBack;
};

class CTimeSpecificMgr : ITimer
{
	CTimeSpecificMgr();
	virtual ~CTimeSpecificMgr();
	DECLARE_SINGLETON(CTimeSpecificMgr);
public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	UINT32 RegisterEvent(const TimeSpecific& stTime, TimeSpecificCallBack pCallBack);
	void UnRegisterEvent(UINT32 dwHandle);

private:
	bool IsInSpecificTime(const TimeSpecific& stTime);
	time_t GetNextTime(const TimeSpecific& stTime);

private:
	HTIMER m_TimerHandle;
	UINT32 m_dwCount;
	std::set<std::pair<time_t, UINT32>> m_timeSet;
	std::unordered_map<UINT32, TimeSpecificEvent*> m_mapEvent;

	bool m_isOnTimer;
	std::vector<std::pair<time_t, UINT32>> m_addEvent;
	std::vector<UINT32> m_delEvent;

};

#endif