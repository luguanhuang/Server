#include "pch.h"
#include "pushhandler.h"
#include "pushmgr.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "levelseal/levelsealMgr.h"
#include "define/pushtypedef.h"
#include <time.h>

PushHandler::SpecificTimerItem::SpecificTimerItem(const TimeSpecific& time, PushMessageTable::RowData* data)
{
	tableData = *data;
	timerId = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&PushHandler::SpecificTimerItem::OnTimer, this));
}

PushHandler::SpecificTimerItem::~SpecificTimerItem()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(timerId);
}

bool PushHandler::SpecificTimerItem::OnTimer()
{
	PushMgr::Instance()->PushToAll(&tableData);
	return true;
}

PushHandler::~PushHandler()
{
	ClearTimerItem();
}

void PushHandler::ClearTimerItem()
{
	for (auto it = m_timers.begin(); it != m_timers.end(); ++it)
	{
		delete *it;
	}
	m_timers.clear();
}

void PushHandler::AddTimerItem(SpecificTimerItem* timer)
{
	m_timers.push_back(timer);
}

void PushHandler::Update(bool force)
{
	time_t now = time(NULL);
	if (XCommon::IsDaySame(now, m_lastUpdateTime) && !force)
	{
		return;
	}
	m_lastUpdateTime = now;
	Load();
}

void SpecificTimePushHandler::Load()
{
	ClearTimerItem();
	const std::vector<PushMessageTableData*>& data = PushMgr::Instance()->GetAllTableData();
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		PushMessageTableData* tableData = *it;
		// 是否定时推送
		if (!tableData->IsCommonGlobal)
		{
			continue;
		}
		// 时间格式
		if (tableData->Time.size() < 2)
		{
			SSWarn << "Time size error" << END;
			continue;
		}
		// 特定日期
		if (tableData->Date != 0)
		{
			UINT32 nowYear, nowMonth, nowDay;
			XCommon::GetDate(nowYear, nowMonth, nowDay);
			UINT32 year = tableData->Date / 1000;
			UINT32 mon = tableData->Date / 100 - year * 10000;
			UINT32 day = tableData->Date % 100;
			if (nowYear != year || nowMonth != mon || nowDay != day)
			{
				continue;
			}
		}
		// 每周几
		UINT32 weekDay = XCommon::GetWeekNum();
		bool isWeekDay = false;
		if (tableData->WeekDay.empty())
		{
			isWeekDay = true;
		}
		for (int i = 0; i < tableData->WeekDay.size(); i++)
		{
			if (tableData->WeekDay[i] == 0 || tableData->WeekDay[i] == weekDay)
			{
				isWeekDay = true;
				break;
			}
		}
		if (!isWeekDay)
		{
			continue;
		}

		TimeSpecific time(tableData->Time[0], tableData->Time[1], 0);
		SpecificTimerItem* item = new SpecificTimerItem(time, tableData);
		AddTimerItem(item);
	}
}

void LevelSealPushHandler::Load()
{
	ClearTimerItem();
	const stServerLevelSealInfo& levelSealInfo = CLevelSealMgr::Instance()->GetLevelSealInfo();
	time_t now = time(NULL);

	std::vector<PushMessageTableData*> data;
	PushMgr::Instance()->GetTableData(Push_LevelSeal, data);
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		PushMessageTableData* tableData = *it;
		if (tableData->TimeOffset.size() < 3)
		{
			SSWarn << "TimeOffset size error" << END;
			continue;
		}
		if ((levelSealInfo.m_uType - 1) != tableData->SubType)
		{
			continue;
		}
		UINT32 deltaDay = XCommon::GetDiffDayCount(levelSealInfo.m_uStartTime, now);
		if (deltaDay != tableData->TimeOffset[0])
		{
			continue;
		}
		TimeSpecific time(tableData->TimeOffset[1], tableData->TimeOffset[2], 0);
		SpecificTimerItem* item = new SpecificTimerItem(time, tableData);
		AddTimerItem(item);
	}
}
