#include "pch.h"
#include "payconsumemgr.h"
#include "pb/project.pb.h"
#include "config.h"
#include "servermgr/servercrossdatamgr.h"
#include "util/gametime.h"
#include "timeutil.h"
#include "payv2Record.h"


INSTANCE_SINGLETON(CPayConsumeMgr)

CPayConsumeMgr::CPayConsumeMgr()
{

}

CPayConsumeMgr::~CPayConsumeMgr()
{

}

bool CPayConsumeMgr::Init()
{
	return true;
}

void CPayConsumeMgr::Uninit()
{

}

void CPayConsumeMgr::SetData(const KKSG::ConsumeRebateNtfData& roInfo)
{
	m_StartDate = roInfo.startdate();
	m_EndDate = roInfo.enddate();
	m_Rate = roInfo.rebate();
}

UINT32 CPayConsumeMgr::GetRate(UINT64 roleid)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		KKSG::ServerCrossData* pCrossData = ServerCrossDataMgr::Instance()->GetDataByRoleID(roleid);
		if (pCrossData)
		{
			return GetRate(pCrossData->rebate().startdate(), pCrossData->rebate().enddate(), pCrossData->rebate().rebate());
		}
		return 0;
	}
	return GetRate(m_StartDate, m_EndDate, m_Rate);
}

UINT32 CPayConsumeMgr::GetRate(UINT32 start, UINT32 end, UINT32 rate)
{
	UINT32 nowtime = UINT32(GameTime::GetTime());
	if (start <= nowtime && nowtime < end && rate)
	{
		return rate;
	}
	return 0;
}

void CPayConsumeMgr::GetRebatInfo(Role *pRole, std::string& out)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		KKSG::ServerCrossData* pCrossData = ServerCrossDataMgr::Instance()->GetDataByRoleID(pRole->GetID());
		if (pCrossData)
		{
			out = "cross server,start=" + TimeUtil::GetDateTimeStr(pCrossData->rebate().startdate()) + ",end=" + TimeUtil::GetDateTimeStr(pCrossData->rebate().enddate()) + ",rate=" + ToString(pCrossData->rebate().rebate());
		}
	}
	else
	{
		out = "start=" + TimeUtil::GetDateTimeStr(m_StartDate) + ",end=" + TimeUtil::GetDateTimeStr(m_EndDate) + ",rate=" + ToString(m_Rate);
	}
	out += ",consume=" + ToString(pRole->Get<CPayV2Record>()->GetTodayConsume());
}
