#include "pch.h"
#include "pushsubscribemgr.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "httptask/pushsubscribe.h"
#include "httptask/httptaskmgr.h"
#include "role/rolesummarymgr.h"
#include "pushconfig.h"

INSTANCE_SINGLETON(CPushSubscribeMgr)

CPushSubscribeMgr::CPushSubscribeMgr()
{
	m_lastUpdateTime = 0;
}

CPushSubscribeMgr::~CPushSubscribeMgr()
{

}

bool CPushSubscribeMgr::Init()
{
	if (!LoadConfig())
	{
		return false;
	}
	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	
	return true;
}

void CPushSubscribeMgr::Uninit()
{	
	CTimerMgr::Instance()->KillTimer(m_TimeHandler);
	ClearTimer();
}

bool CPushSubscribeMgr::LoadConfig()
{
	if (!m_oTable.LoadFile("table/PushSubscribe.txt"))
	{
		SSError << "load table/PushSubscribe.txt failed" << END;
		return false;
	}
	return true;
}

void CPushSubscribeMgr::LoadTimer()
{
	ClearTimer();
	for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); it ++)
	{
		PushSubscribeTable::RowData* pRow = *it;
		if (NULL == pRow)
		{
			return;
		}
		bool isToday = false;
		if (pRow->WeekDay.empty())
		{
			isToday = true;
		}
		else
		{
			auto i = std::find_if(pRow->WeekDay.begin(), pRow->WeekDay.end(), EqualPushWeekDays(UINT32(XCommon::GetWeekNum())));
			if (i != pRow->WeekDay.end())
			{
				isToday = true;
			}
		}
		if (!isToday)
		{
			continue;
		}
		for (UINT32 j = 0; j < pRow->Time.size(); ++ j)
		{
			UINT32 min = pRow->Time[j] % 100;
			UINT32 hour = pRow->Time[j] / 100;
			if (min > 59 || hour > 23)
			{
				SSError << "error config,invalid time " << pRow->Time[j] << END;
				continue;
			}
			TimeSpecific time(hour, min, 0);
			SpecificPushTimer* pTimer = new SpecificPushTimer(time, pRow);
			m_timers.push_back(pTimer);
		}
	}
}

void CPushSubscribeMgr::ClearTimer()
{
	for(auto it = m_timers.begin(); it != m_timers.end(); ++ it)
	{
		delete *it;
	}
	m_timers.clear();
}

void CPushSubscribeMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (MSConfig::Instance()->GetAppType()!=KKSG::GAME_APP_WECHAT)
	{
		return;
	}
	if(!CPushConfigMgr::Instance()->GetIsWxPush())
	{
		return;
	}
	if (!XCommon::IsDaySame(GameTime::GetTime(), m_lastUpdateTime))
	{
		LoadTimer();
		m_lastUpdateTime = GameTime::GetTime();
	}
}

UINT32 CPushSubscribeMgr::GetTxMsgId(UINT32 msgid)
{
	auto it = m_oTable.GetByMsgId(msgid);
	if (it != NULL)
	{
		return it->TxMsgId;
	}
	return UINT32(-1);
}

UINT32 CPushSubscribeMgr::GetMsgIdByTxMsgId(UINT32 id)
{
	for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); it ++)
	{
		if ((*it)->TxMsgId == id)
		{
			return (*it)->MsgId;
		}
	}
	return UINT32(-1);
}

CPushSubscribeMgr::SpecificPushTimer::SpecificPushTimer(const TimeSpecific& time, PushSubscribeTable::RowData* data)
{
	table = *data;
	timerId = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CPushSubscribeMgr::SpecificPushTimer::OnTimer, this));
}

CPushSubscribeMgr::SpecificPushTimer::~SpecificPushTimer()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(timerId);
}
bool CPushSubscribeMgr::SpecificPushTimer::OnTimer()
{
	SSInfo << "msgid=" <<  table.MsgId << "name=" << table.Name << END;
	//todo
	CPushSubscribeTask* poTask = new CPushSubscribeTask();
	poTask->BuildPostData(table.TxMsgId);
	HttpTaskMgr::Instance()->AddTask(poTask);
	//CPushSubscribeMgr::Instance()->Push(&table);
	return true;
}
