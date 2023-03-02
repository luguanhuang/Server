#include "pch.h"
#include "reportdatarecord.h"
#include "pb/project.pb.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "reportdatamgr.h"
#include "reportdata/reportdata_def.h"

CReportDataRecord::CReportDataRecord(Role* role)
{
	m_poRole = role;
	m_WeekloginDays = 0;
	m_With20minsDays = 0;
	m_LastDayOnlineTime = 0;
	m_LastDayUpdate = 0;
	m_LastWeekUpdate = 0;
	m_WeekNestFastTime = 0;
	m_WeekNestDayTimes = 0;
	m_LastRecommondTime = 0;
	m_AbyssDayCount = 0;
	m_SuperRiskTodayCount = 0;
	m_BuyIbShopCount = 0;
	m_PokerCount = 0;
	m_LastPokerTime = 0;
	m_HorseMacthCount = 0;
	m_vWeekData.clear();
}

bool CReportDataRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_reportdata())
	{
		const KKSG::ReportDataRecord& oData = poRoleAllInfo->reportdata();
		m_WeekloginDays = oData.weekactivedays();
		m_With20minsDays = oData.within20minsdays();
		m_LastDayOnlineTime = oData.lastdayonlinetime();
		m_LastDayUpdate = oData.lastdayupdate();
		m_LastWeekUpdate = oData.lastweekupdate();
		m_WeekNestFastTime = oData.weeknestfasttime();
		m_WeekNestDayTimes = oData.weeknestdaytimes();
		for (INT32 i = 0; i < oData.wxdata_size(); i ++)
		{
			m_vWeekData.insert(std::make_pair(oData.wxdata(i).type(), oData.wxdata(i)));
		}
		m_LastRecommondTime = oData.lastrecommondtime();
		m_AbyssDayCount = oData.abyssdaycount();
		m_SuperRiskTodayCount = oData.superrisktodaycount();
		m_BuyIbShopCount = oData.buyibshopcount();
		m_PokerCount = oData.pokercount();
		m_LastPokerTime = oData.lastpokertime();
		m_HorseMacthCount = oData.horsemacthcount();
	}
	return true;
}

void CReportDataRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_ismodify.TestAndReset())
	{
		KKSG::ReportDataRecord* pData = poRoleAllInfo->mutable_reportdata();
		pData->Clear();
		pData->set_weekactivedays(m_WeekloginDays);
		pData->set_within20minsdays(m_With20minsDays);
		pData->set_lastdayonlinetime(m_LastDayOnlineTime);
		pData->set_lastdayupdate(m_LastDayUpdate);
		pData->set_lastweekupdate(m_LastWeekUpdate);
		pData->set_weeknestfasttime(m_WeekNestFastTime);
		pData->set_weeknestdaytimes(m_WeekNestDayTimes);

		for (auto it = m_vWeekData.begin(); it != m_vWeekData.end(); it ++)
		{
			KKSG::WeekReportData* pWeekData =  pData->add_wxdata();
			pWeekData->CopyFrom(it->second);
		}

		pData->set_lastrecommondtime(m_LastRecommondTime);
		pData->set_abyssdaycount(m_AbyssDayCount);
		pData->set_superrisktodaycount(m_SuperRiskTodayCount);
		pData->set_buyibshopcount(m_BuyIbShopCount);
		pData->set_pokercount(m_PokerCount);
		pData->set_lastpokertime(m_LastPokerTime);
		pData->set_horsemacthcount(m_HorseMacthCount);
		roChanged.insert(pData);
	}
}

void CReportDataRecord::Update()
{
	if (!XCommon::IsWeekSame(GameTime::GetTime(), m_LastWeekUpdate))
	{
		WeekReset();
		m_LastWeekUpdate = (UINT32)GameTime::GetTime();
	}

	if (!XCommon::IsDaySame(GameTime::GetTime(), m_LastDayUpdate))
	{
		DayReset();
		m_LastDayUpdate = (UINT32)GameTime::GetTime();
	}
}

void CReportDataRecord::WeekReset()
{
	if (m_WeekloginDays)
	{
		m_WeekloginDays = 0;
	}
	m_ismodify.Set();
}

void CReportDataRecord::UpdateWeekData(KKSG::WeekReportDataType type)
{
	UINT32 now = (UINT32)GameTime::GetTime();
	auto it = m_vWeekData.find(type);
	if (it == m_vWeekData.end())
	{
		KKSG::WeekReportData oData;
		oData.set_type(type);
		oData.set_joincount(1);
		oData.set_lastjointime(now);
		m_vWeekData.insert(std::make_pair(type, oData));
	}
	else
	{
		if (XCommon::IsDaySame(now, it->second.lastjointime()))//一天只能参与一次;
		{
			return;
		}
		if (!XCommon::IsWeekSame(now, it->second.lastjointime()))//跨周;
		{
			it->second.set_joincount(0);
		}
		it->second.set_joincount(it->second.joincount()+1);
		it->second.set_lastjointime(now);
	}
	m_ismodify.Set();
}

INT32 CReportDataRecord::GetWeekDataByType(KKSG::WeekReportDataType type)
{
	auto it = m_vWeekData.find(type);
	if (it == m_vWeekData.end())
	{
		return 0;
	}
	if (!XCommon::IsWeekSame(GameTime::GetTime(), it->second.lastjointime()))
	{
		return 0;
	}
	return it->second.joincount();
}

void CReportDataRecord::DayReset()
{
	m_WeekloginDays ++;
	m_WeekNestDayTimes = 0;
	m_AbyssDayCount = 0;
	m_SuperRiskTodayCount = 0;
	m_BuyIbShopCount = 0;
	if (m_With20minsDays == 2)
	{
		ReportDataMgr::Instance()->Within20Mins(m_poRole);
		m_With20minsDays = 0;
	}
	if (!XCommon::IsDaySame(m_LastDayUpdate, GameTime::GetTime() - XCommon::GetOneDayTime()))
	{
		//最后登录，如果不是昨天的话，说明连续条件已经中断，清除数据;
		m_With20minsDays = 0;
	}
	else
	{
		if (m_LastDayOnlineTime && m_LastDayOnlineTime < 1200)//登录小于20分钟;
		{
			m_With20minsDays ++;
			//SSDebug << "With20minsDays=" << m_With20minsDays << END;
		}
		else
		{
			m_With20minsDays = 0;
		}
		m_LastDayOnlineTime = 0;
	}
	m_ismodify.Set();
}

void CReportDataRecord::AddOnlineTime(UINT32 onlinetime)
{
	Update();
	m_LastDayOnlineTime += onlinetime;
	m_ismodify.Set();
}

void CReportDataRecord::UpdataWeekLoginDays()
{
	Update();
	ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_WeekActiveDays, ToString(m_WeekloginDays));
}

void CReportDataRecord::UpdateWeekNestFastTime(UINT32 time)
{
	Update();
	UINT32 iTime = 0;
	if (time && (m_WeekNestFastTime == 0 || m_WeekNestFastTime > time))
	{
		m_WeekNestFastTime = time;
		iTime = time;
	}
	m_WeekNestDayTimes ++;
	m_ismodify.Set();
	ReportDataMgr::Instance()->WeekNest(m_poRole, iTime, m_WeekNestDayTimes);
}

bool CReportDataRecord::CheckRecommondTime()
{
	//每周第一次上报;
	if (XCommon::IsWeekSame(GameTime::GetTime(), m_LastRecommondTime))
	{
		return false;
	}
	m_LastRecommondTime = (UINT32)GameTime::GetTime();
	return true;
}

void CReportDataRecord::UpdateAbyssCount()
{
	Update();
	m_AbyssDayCount ++;
	ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_AbyssParty, ToString(m_AbyssDayCount));
}

void CReportDataRecord::UpdateSuperRiskCount()
{
	Update();
	m_SuperRiskTodayCount ++;
	ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_SuperRiskDayCount, ToString(m_SuperRiskTodayCount));
}

void CReportDataRecord::UpdateBuyIbShopCount()
{
	Update();
	m_BuyIbShopCount ++;
	ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_BuyIbShopCount, ToString(m_BuyIbShopCount));
}

void CReportDataRecord::UpdatePokerCount()
{
	UINT32 now = GameTime::GetTime();
	if (XCommon::IsDaySame(now, m_LastPokerTime))//一天只能参与一次;
	{
		return;
	}
	m_LastPokerTime = now;
	m_PokerCount ++;
	ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_PokerCount, ToString(m_PokerCount));
}

void CReportDataRecord::UpdateHorseMatchCount()
{
	m_HorseMacthCount ++;
	ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_HorseMatchCount, ToString(m_HorseMacthCount));
}
