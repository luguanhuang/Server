#include "pch.h"
#include "payconsumemgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "table/ConsumeRebateMgr.h"
#include "pay/ptcm2g_consumerebatentf.h"
#include "network/gslink.h"
#include "other/servercrossdatamgr.h"
#include "network/dblink.h"

INSTANCE_SINGLETON(CPayConsumeMgr)

CPayConsumeMgr::CPayConsumeMgr()
{
	m_DateType = 0;
	m_StartDate = 0;
	m_EndDate = 0;
	m_Rate = 0;
	m_DurationDays = 0;
	m_IsNeedSave = false;
	m_LastUpdateTime = GameTime::GetTime();
}

CPayConsumeMgr::~CPayConsumeMgr()
{

}

bool CPayConsumeMgr::Init()
{
	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 500, -1, __FILE__, __LINE__);
	if (!LoadFromDB())//还有开服活动未结束时,则配置无效;
	{
		CheckConfig();
	}

	return true;
}

void CPayConsumeMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_TimeHandler);
}

void CPayConsumeMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (!XCommon::IsDaySame(GameTime::GetTime(), m_LastUpdateTime))
	{
		m_LastUpdateTime = GameTime::GetTime();
		CheckAndRsync();
	}
	SaveData();
}

bool CPayConsumeMgr::LoadFromDB()
{
	std::string strConsumeRebate = CGlobalConfigDBBuild::Instance()->GetConfigValue(eConsumeRebate);
	if (strConsumeRebate.empty())
	{
		return false;
	}
	KKSG::ConsumeRebateBrief roInfo;
	if (roInfo.ParseFromString(strConsumeRebate) && roInfo.days())
	{
		UINT32 opentime = MSConfig::Instance()->GetGameServerOpenTimeStamp();
		UINT32 nowtime = UINT32(GameTime::GetTime());
		if (opentime <= nowtime && (opentime + roInfo.days() * XCommon::GetOneDayTime()) > nowtime)
		{
			m_StartDate = opentime;
			m_EndDate = opentime + roInfo.days() * XCommon::GetOneDayTime();
			m_Rate = roInfo.rate();
			m_DurationDays = roInfo.days();
			m_DateType = ConsumeRebateDateType1;
			LogInfo("rebate from db start=%s,end=%s,datetype=%u,rate=%u", TimeUtil::GetDateTimeStr(m_StartDate).c_str(), TimeUtil::GetDateTimeStr(m_EndDate).c_str(), m_DateType, m_Rate);
			return true;
		}
	}
	return false;
}

void CPayConsumeMgr::CheckConfig()
{
	auto it = CConsumeRebateConfig::Instance()->GetOpenConfig();
	if (it == NULL)
	{
		return;
	}
	if (it->datetype == ConsumeRebateDateType1)
	{
		m_StartDate = MSConfig::Instance()->GetGameServerOpenTimeStamp();
	}
	else if (it->datetype == ConsumeRebateDateType2)
	{
		m_StartDate = CConsumeRebateConfig::Instance()->GetTimeFromStr(it->startdate);
	}
	else
	{
		return ;
	}
	m_EndDate = m_StartDate + it->duration * XCommon::GetOneDayTime();
	m_Rate = it->rebaterate;
	m_DateType = it->datetype;
	m_DurationDays = it->duration;
	m_IsNeedSave = true;

	LogInfo("rebate from config start=%s,end=%s,datetype=%u,rate=%u", TimeUtil::GetDateTimeStr(m_StartDate).c_str(), TimeUtil::GetDateTimeStr(m_EndDate).c_str(), m_DateType, m_Rate);
}

void CPayConsumeMgr::BuildData(KKSG::ConsumeRebateNtfData* pData)
{
	pData->set_startdate(m_StartDate);
	pData->set_enddate(m_EndDate);
	pData->set_rebate(m_Rate);
}

void CPayConsumeMgr::SendToGs(UINT32 line)
{
	PtcM2G_ConsumeRebateNtf ptc;
	BuildData(&ptc.m_Data);
	if (line)
	{
		GSLink::Instance()->SendToLine(line, ptc);
	}
	else
	{
		GSLink::Instance()->SendToAllLine(ptc);
	}
}

void CPayConsumeMgr::SaveData()
{
	if (m_DateType != ConsumeRebateDateType1 || !DBLink::Instance()->IsConnected() || !m_IsNeedSave)
	{
		return;
	}
	KKSG::ConsumeRebateBrief roInfo;
	roInfo.set_days(m_DurationDays);
	roInfo.set_rate(m_Rate);
	m_IsNeedSave = false;
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eConsumeRebate, roInfo.SerializeAsString());
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eConsumeRebate);
}

void CPayConsumeMgr::CheckAndRsync()
{
	if(!LoadFromDB())
	{
		CheckConfig();
	}
	SendToGs();
	ServerCrossDataSyn::SynDataToWorld();
}
