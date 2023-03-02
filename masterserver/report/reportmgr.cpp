#include "pch.h"
#include "reportmgr.h"
#include "role/rolesummarymgr.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "util.h"
#include "util/timespecificmgr.h"
#include "util/gametime.h"

ReportMgr::ReportMgr()
{
	m_handler[KKSG::SCENE_HEROBATTLE] = new ReportHeroBattleHandler;
	m_handler[KKSG::SCENE_MOBA] = new ReportMobaBattleHandler;
}

ReportMgr::~ReportMgr()
{
	for (auto it = m_handler.begin(); it != m_handler.end(); ++it)
	{
		delete it->second;
	}
}

bool ReportMgr::Init()
{
	if (!LoadFromDB())
	{
		return false;
	}
	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	
	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&ReportMgr::CheckWeekPass, this));
	return true;
}

void ReportMgr::Uninit()
{

}

void ReportMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	SaveToDB(10);
}

void ReportMgr::AddToDirtyList(UINT64 roleId)
{
	m_dirtyList.push_back(roleId);
}

bool ReportMgr::LoadFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::string sql("select _id, data from rolereport");
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(sql.c_str(), sql.size(), &poRes))
	{
		LogError("Read rolereport failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		// roleid
		UINT64 roleId;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			roleId = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		KKSG::RoleReportData data;
		std::string rawData;
		// data
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			rawData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'data' column failed");
			return false;
		}

		if (!data.ParseFromString(rawData))
		{
			LogError("parse role report failed, roleId[%llu]", roleId);
			return false;
		}
		RoleReportInfo& info = m_infos[roleId];
		info.SetRoleId(roleId);
		info.FromKKSG(data);
	}

	poRes->Release();
	LogInfo("Load rolereport num: %u", m_infos.size());

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all rolereport data from db use time: %llu ms", qwTimeUsed);
	return true;
}

void ReportMgr::SaveToDB(UINT32 num)
{
	UINT32 i = 0;
	while (1)
	{
		if (m_dirtyList.empty())
		{
			break;
		}
		if (num && i >= num)
		{
			break;
		}
		UINT64 roleId = m_dirtyList.pop_front();
		RoleReportInfo* pInfo = GetRoleReportInfo(roleId);
		if (pInfo == NULL)
		{
			continue;
		}	
		pInfo->UpdateToDB();
		i++;
	}
}

RoleReportInfo* ReportMgr::GetRoleReportInfo(UINT64 roleId, bool add)
{
	auto it = m_infos.find(roleId);
	if (it != m_infos.end())
	{
		return &it->second;
	}
	if (!add)
	{
		return NULL;
	}
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su == NULL)
	{
		return NULL;
	}
	RoleReportInfo info;
	info.SetRoleId(roleId);
	auto p = m_infos.insert(std::make_pair(roleId, info));
	return &p.first->second;
}

UINT32 ReportMgr::GetRoleBanTime(UINT64 roleId, UINT32 sceneType)
{
	RoleReportInfo* pInfo = GetRoleReportInfo(roleId);
	if (pInfo == NULL)
	{
		return 0;
	}
	return pInfo->GetBanTime(sceneType);
}

bool ReportMgr::CheckWeekPass()
{
	for (auto it = m_infos.begin(); it != m_infos.end(); ++it)
	{
		it->second.CheckWeekReset();
	}
	return true;
}

void ReportMgr::OnRoleLogin(UINT64 roleId)
{
	RoleReportInfo* pInfo = GetRoleReportInfo(roleId);
	if (pInfo == NULL)
	{
		return ;
	}
	pInfo->CheckWeekReset();
}

IReportHandler* ReportMgr::GetHandler(UINT32 sceneType)
{
	auto it = m_handler.find(sceneType);
	return it == m_handler.end() ? NULL : it->second;
}

UINT32 ReportMgr::GetTotalNum(UINT32 sceneType)
{
	auto it = m_totalBanNum.find(sceneType);
	if (it == m_totalBanNum.end())
	{
		return 0;
	}
	return it->second;
}

void ReportMgr::AddTotalNum(UINT32 sceneType)
{
	m_totalBanNum[sceneType]++;
}

void ReportMgr::DecTotalNum(UINT32 sceneType)
{
	UINT32& num = m_totalBanNum[sceneType];
	if (num > 0)
	{
		num--;
	}
}




