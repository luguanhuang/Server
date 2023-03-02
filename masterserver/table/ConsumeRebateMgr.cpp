#include "pch.h"
#include "ConsumeRebateMgr.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "pay/payconsumemgr.h"

INSTANCE_SINGLETON(CConsumeRebateConfig)

CConsumeRebateConfig::CConsumeRebateConfig()
{

}

CConsumeRebateConfig::~CConsumeRebateConfig()
{

}

bool CConsumeRebateConfig::Init()
{
	return LoadFile(false);
}

void CConsumeRebateConfig::Uninit()
{

}

void CConsumeRebateConfig::ClearFile()
{
	m_oTable.Clear();
}

bool CConsumeRebateConfig::CheckFile(bool isReload)
{
	if (isReload)//如果有活动在开启则不允许reload;
	{
		for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); it ++)
		{
			if (CheckOpen((*it)))
			{
				LogWarn("already exist activity,can't reload.");
				return false;
			}
		}
	}

	ConsumeRebate tmpRebate;
	if (!tmpRebate.LoadFile("table/ConsumeRebate.txt"))
	{
		LogWarn("Load ConsumeRebate.txt failed!");
		return false;
	}

	UINT32 nowtime = UINT32(GameTime::GetTime());
	bool isOpen = false;
	for (auto it = tmpRebate.Table.begin(); it != tmpRebate.Table.end(); it ++)
	{
		if ((*it)->datetype < ConsumeRebateDateType1 || (*it)->datetype >= ConsumeRebateDateTypeMax)
		{
			LogWarn("invalid config,invalid datetype.");
			return false;
		}
		if ((*it)->datetype == ConsumeRebateDateType1 && CheckOpen((*it)))
		{
			if (isOpen)
			{
				LogWarn("invalid config,more than 1 dateype=1");
				return false;
			}
			isOpen = true;
		}
	}

	std::vector<ConsumeRebate::RowData*> vList;
	bool isopendate = false;
	for (auto it = tmpRebate.Table.begin(); it != tmpRebate.Table.end(); it ++)
	{
		if ((*it)->duration && (*it)->datetype == ConsumeRebateDateType2)//
		{
			UINT32 start = GetTimeFromStr((*it)->startdate);
			if (start == 0)
			{
				LogWarn("invalid startdate formart %u", (*it)->startdate);
				return false;
			}
			if (CheckOpen((*it)))//不能同时有多个指定日期的活动开启;
			{
				if (isopendate)
				{
					LogWarn("dateacitivty is exist");
					return false;
				}
				isopendate = true;
			}
			if (isOpen && (start > nowtime || isopendate))//开服前几天活动已经开启，有正在开启的活动或者后续还有待开启的指定日期的活动,禁止启动;
			{
				LogWarn("openserverdate is exist,has dateactivity or other dateactivity soon");
				return false;
			}

			for (auto i = vList.begin(); i != vList.end(); i ++)//判断是否有重叠的时间段;
			{
				if (CheckDaySame(*i, *it))
				{
					LogWarn("invalid config,exist same days");
					return false;
				}
			}
			vList.push_back(*it);
		}
	}
	return true;
}

bool CConsumeRebateConfig::LoadFile(bool isReload)
{
	if (!CheckFile(isReload))
	{
		return false;
	}

	ClearFile();
	m_oTable.LoadFile("table/ConsumeRebate.txt");

	if (isReload)
	{
		CPayConsumeMgr::Instance()->CheckAndRsync();
	}
	return true;
}

UINT32 CConsumeRebateConfig::GetTimeFromStr(UINT32 nDays)
{
	tm stTime;
	stTime.tm_year = nDays/10000 - 1900;
	stTime.tm_mon = nDays/100%100 - 1;
	stTime.tm_mday = nDays%100;
	stTime.tm_hour = HOUR_ONE_DAY_BEGIN;
	stTime.tm_min = 0;
	stTime.tm_sec = 0;
	if (stTime.tm_mon < 0 || stTime.tm_mon > 11 || stTime.tm_mday < 0 || stTime.tm_mday > 30)
	{
		LogError("invalid startdate format");
		return 0;
	}
	UINT32 StartDate = (UINT32)mktime(&stTime);
	return StartDate==UINT32(-1)?0:StartDate;
}

bool CConsumeRebateConfig::CheckOpen(ConsumeRebate::RowData* pRow)
{
	if (pRow->datetype == ConsumeRebateDateType1 && pRow->duration)
	{
		return true;
	}
	if (pRow->datetype == ConsumeRebateDateType2)
	{
		UINT32 nowtime = UINT32(GameTime::GetTime());
		UINT32 start = GetTimeFromStr(pRow->startdate);
		if (start &&  start <= nowtime && nowtime < (start + pRow->duration * XCommon::GetOneDayTime()))
		{
			return true;
		}
	}
	return false;
}

bool CConsumeRebateConfig::CheckDaySame(ConsumeRebate::RowData* pRowA, ConsumeRebate::RowData* pRowB)
{
	UINT32 startA = GetTimeFromStr(pRowA->startdate);
	UINT32 startB = GetTimeFromStr(pRowB->startdate);
	if(std::max(startA, startB) < std::min(startA + pRowA->duration * XCommon::GetOneDayTime(), startB +pRowB->duration * XCommon::GetOneDayTime()))
	{
		return true;
	}
	return false;
}

ConsumeRebate::RowData* CConsumeRebateConfig::GetOpenConfig()
{
	for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); it ++)
	{
		if (CheckOpen((*it)))
		{
			return (*it);
		}
	}
	return NULL;
}
