#include "pch.h"
#include "timespecificmgr.h"
#include "time.h"
#include "util/gametime.h"

#define TIMESPAN 30 
#define TIMEUPDATE 1000

INSTANCE_SINGLETON(CTimeSpecificMgr);

CTimeSpecificMgr::CTimeSpecificMgr()
:m_TimerHandle(INVALID_HTIMER)
,m_dwCount(0)
,m_isOnTimer(false)
{

}

CTimeSpecificMgr::~CTimeSpecificMgr()
{
	for (auto i = m_mapEvent.begin(); i != m_mapEvent.end(); ++i)
	{
		if (NULL == i->second)
		{
			continue;
		}
		delete i->second;
		i->second = NULL;
	}
	m_mapEvent.clear();
}

bool CTimeSpecificMgr::Init()
{
	m_TimerHandle = CTimerMgr::Instance()->SetTimer(this, 0, TIMEUPDATE, -1, __FILE__, __LINE__);
	if (INVALID_HTIMER == m_TimerHandle)
	{
		SSWarn<<"m_TimeHandle is NULL"<<END;
		return false;
	}
	return true;
}

void CTimeSpecificMgr::Uninit()
{
	if (INVALID_HTIMER != m_TimerHandle)
	{	
		CTimerMgr::Instance()->KillTimer(m_TimerHandle);
	}
}

/*void CTimeSpecificMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (m_mapEvent.empty())
	{
		return;
	}
	auto i = m_mapEvent.begin();
	while(i != m_mapEvent.end())
	{
		if (NULL == i->second)
		{
			SSWarn<<"time specific event is NULL, id:"<<i->first<<END;
			m_mapEvent.erase(i++);
			continue;
		}
		if (IsInSpecificTime(i->second->stTime))
		{
			if (!i->second->isdone)
			{
				i->second->isdone = i->second->pFuncion();
				if (i->second->isonce)
				{
					delete i->second;
					i->second = NULL;
					m_mapEvent.erase(i++);
					continue;
				}
			}
		}
		else
		{
			if (i->second->isdone)
			{
				i->second->isdone = false;
			}
		}
		++i;
	}
}*/

void CTimeSpecificMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	std::vector<std::pair<time_t, UINT32>> insertSet;

	time_t nowTime = GameTime::GetTime();

	m_isOnTimer = true;

	auto i = m_timeSet.begin();
	while(i != m_timeSet.end())
	{	
		const time_t& preTime = i->first;
		if (preTime > nowTime)
		{
			break;
		}
		UINT32 countID = i->second;
		if(m_mapEvent.find(countID) == m_mapEvent.end())
		{
			LogError("can't find this id:%u in mapEvent", countID);
			m_timeSet.erase(i++);
			assert(false);
			continue;
		}
		TimeSpecificEvent* pEvent = m_mapEvent[countID];
		if(NULL == pEvent)
		{
			LogError("time specific event is NULL, id:%u", countID);
			m_mapEvent.erase(countID);
			m_timeSet.erase(i++);
			assert(false);
			continue;
		}

		//check
		if(std::find(m_delEvent.begin(), m_delEvent.end(), countID) != m_delEvent.end())
		{
			++i;
			continue;
		}

		bool isdone = pEvent->pFuncion();
		if(pEvent->isonce) // only trigger once
		{
			delete pEvent;
			m_mapEvent.erase(countID);
			m_timeSet.erase(i++);
			continue;
		}
		
		if(isdone) //trigger event done
		{
			time_t nextTime = GetNextTime(pEvent->stTime);
			insertSet.push_back(std::make_pair(nextTime, countID));
			pEvent->triggerTime = nextTime;

			m_timeSet.erase(i++);
			continue;
		}
		//not done, trigger next time
		i++;
	}	

	for(auto i = insertSet.begin(); i != insertSet.end(); i++)
	{
		m_timeSet.insert(*i);
	}

	m_isOnTimer = false;

	for(auto i = m_addEvent.begin(); i != m_addEvent.end(); ++i)
	{
		m_timeSet.insert(*i);
	}
	m_addEvent.clear();

	for (auto i = m_delEvent.begin(); i != m_delEvent.end(); ++i)
	{
		UnRegisterEvent(*i);
	}
	m_delEvent.clear();
}

UINT32 CTimeSpecificMgr::RegisterEvent(const TimeSpecific& stTime, TimeSpecificCallBack pCallBack)
{
	TimeSpecificEvent* pEvent = new TimeSpecificEvent(stTime);
	pEvent->pFuncion = pCallBack;

	++m_dwCount;
	if (m_dwCount == 0) m_dwCount = 1;

	m_mapEvent[m_dwCount] = pEvent;
	if (0 != stTime.time)
	{
		pEvent->triggerTime = stTime.time;
	}
	else
	{
		time_t nextTime = GetNextTime(stTime);
		pEvent->triggerTime = nextTime;
	}

	if (m_isOnTimer)
	{
		m_addEvent.push_back(std::make_pair(pEvent->triggerTime, m_dwCount));
	}
	else
	{
		m_timeSet.insert(std::make_pair(pEvent->triggerTime, m_dwCount));
	}
	return m_dwCount;
}

void CTimeSpecificMgr::UnRegisterEvent(UINT32 dwHandle)
{
	if(m_isOnTimer)
	{
		m_delEvent.push_back(dwHandle);
		return;
	}

	auto i = m_mapEvent.find(dwHandle);
	if (i == m_mapEvent.end())
	{
		return;
	}	

	if (NULL != i->second)
	{
		m_timeSet.erase(std::make_pair(i->second->triggerTime, i->first));
		delete i->second;
		i->second = NULL;
	}
	m_mapEvent.erase(i);
	return;
}

bool CTimeSpecificMgr::IsInSpecificTime(const TimeSpecific& stTime)
{
	time_t dwTime = GameTime::GetTime();

	if (0 != stTime.time)
	{
		return dwTime >= stTime.time;
	}

	tm* pTime = localtime(&dwTime);
	if (UINT32(-1) != stTime.hour)
	{
		pTime->tm_hour = stTime.hour;
	}
	if (UINT32(-1) != stTime.min)
	{
		pTime->tm_min = stTime.min;
	}
	pTime->tm_sec = stTime.sec;
	time_t dwDesTime = mktime(pTime);
	if (dwDesTime <= dwTime && dwTime - dwDesTime <= TIMESPAN)
	{
		return true;
	}
	return false;
}

time_t CTimeSpecificMgr::GetNextTime(const TimeSpecific& stTime)
{
	time_t dwTime = GameTime::GetTime();
	tm* pTime = localtime(&dwTime);
	if (UINT32(-1) != stTime.hour)
	{
		pTime->tm_hour = stTime.hour;
	}
	if (UINT32(-1) != stTime.min)
	{
		pTime->tm_min = stTime.min;
	}
	pTime->tm_sec = stTime.sec;
	time_t dwDesTime = mktime(pTime);
	if(dwDesTime > dwTime)
	{
		return dwDesTime;
	}
	if (UINT32(-1) != stTime.hour)
	{
		return dwDesTime + 24 * 3600;
	}
	if (UINT32(-1) != stTime.min)
	{
		return dwDesTime + 3600;
	}
	return dwDesTime + 60;
}