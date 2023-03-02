#include "pch.h"
#include "crossrolesummarymgr.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "util.h"
#include "role/ptcm2d_saverolesummarydata.h"
#include "network/dblink.h"
#include "network/worldlink.h"
#include "role/ptcm2d_deletecrossrolesummary.h"
#include "common/ptcm2k_getrolesummaryfromworld.h"
#include "leagueteam/crossleagueteammgr.h"

CrossRoleSummaryMgr::CrossRoleSummaryMgr()
{

}

CrossRoleSummaryMgr::~CrossRoleSummaryMgr()
{

}

bool CrossRoleSummaryMgr::Init()
{
	if (!LoadFromDB())
	{
		LogError("load cross role summary failed");
		return false;
	}

	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	return true;
}

void CrossRoleSummaryMgr::Uninit()
{
}

void CrossRoleSummaryMgr::SaveAll2DB()
{
	PtcM2D_SaveRoleSummaryData ntf;
	ntf.m_Data.set_is_cross_rolesummary(true);

	while (!m_dirtyList.empty())
	{
		UINT64 id = m_dirtyList.pop_front();
		auto it = m_roleSummarys.find(id);
		if (it == m_roleSummarys.end())
		{
			continue;
		}
		CRoleSummary& su = it->second;
		ntf.m_Data.add_data()->CopyFrom(su.GetData());
		if (ntf.m_Data.data_size() >= 100)
		{
			DBLink::Instance()->SendTo(ntf);		
			ntf.m_Data.clear_data();
		}
	}

	if (ntf.m_Data.data_size() != 0)
	{
		DBLink::Instance()->SendTo(ntf);		
	}
}

void CrossRoleSummaryMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	PtcM2D_SaveRoleSummaryData ntf;
	ntf.m_Data.set_is_cross_rolesummary(true);
	UINT32 i = 0;
	while (!m_dirtyList.empty() && i < 10)
	{
		UINT64 id = m_dirtyList.pop_front();
		auto it = m_roleSummarys.find(id);
		if (it == m_roleSummarys.end())
		{
			continue;
		}
		CRoleSummary& su = it->second;
		ntf.m_Data.add_data()->CopyFrom(su.GetData());
		i++;
	}

	if (ntf.m_Data.data_size() != 0)
	{
		DBLink::Instance()->SendTo(ntf);		
	}
}

bool CrossRoleSummaryMgr::LoadFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::string sql("select _id, data from cross_rolesummary");
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(sql.c_str(), sql.size(), &poRes))
	{
		LogError("Read cross_rolesummary failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		// id 
		UINT64 id;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			id = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		// data
		std::string data;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			data.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'data' column failed");
			return false;
		}

		KKSG::RoleSummaryStored rawData;
		if (!rawData.ParseFromString(data))
		{
			LogError("parse cross rolesummary data failed, id[%llu]", id);
			continue;
		}

		CRoleSummary& su = m_roleSummarys[id];
		su.Init(rawData);
	}

	poRes->Release();
	LogInfo("load cross role summary size: %u", m_roleSummarys.size());

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all cross role summary data from db use time: %llu ms", qwTimeUsed);

	return true;
}

void CrossRoleSummaryMgr::UpdateRoleSummary(const KKSG::RoleSummaryStored& data)
{
	UINT64 id = data.roleid();
	auto it = m_roleSummarys.find(id);
	if (it == m_roleSummarys.end())
	{
		CRoleSummary& su = m_roleSummarys[id];
		su.Init(data);
	}
	else
	{
		it->second.Init(data);
	}
	AddToDirtyList(id);
}

void CrossRoleSummaryMgr::AddToDirtyList(UINT64 id)
{
	m_dirtyList.push_back(id);
}

CRoleSummary* CrossRoleSummaryMgr::GetRoleSummary(UINT64 roleId)
{
	auto it = m_roleSummarys.find(roleId);
	if (it == m_roleSummarys.end())
	{
		UpdateFromWorld(roleId);
		return NULL;
	}
	return &it->second;
}

void CrossRoleSummaryMgr::UpdateFromWorld(UINT64 roleId)
{
	if (!CrossLeagueTeamMgr::Instance()->IsCrossLeagueMember(roleId))
	{
		return;
	}
	// 不存在的跨服RoleSummary去World请求
	PtcM2K_GetRoleSummaryFromWorld ntf;
	ntf.m_Data.add_roleids(roleId);
	WorldLink::Instance()->SendTo(ntf);
}

void CrossRoleSummaryMgr::OnSeasonEnd()
{
	m_roleSummarys.clear();
	m_dirtyList.clear();

	PtcM2D_DeleteCrossRoleSummary ntf;
	DBLink::Instance()->SendTo(ntf);
}



