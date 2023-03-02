#include "pch.h"
#include "roletaskmgr.h"
#include "event/eventmgr.h"
#include "util/gametime.h"
#include "util/XRandom.h"
#include "role/role.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "util/timespecificmgr.h"
#include "global/GlobalConfigDBBuild.h"

RoleTaskMgr::RoleTaskMgr()
:m_lastPassDayTime(0)
{

}

RoleTaskMgr::~RoleTaskMgr()
{

}

bool RoleTaskMgr::Init()
{
	if (!DoLoadFile())
	{
		return false;
	}
	if (!LoadFromDB())
	{
		LogError("load from db failed");
		return false;
	}

	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	
	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&RoleTaskMgr::OnDayPass, this));
	return true;
}

void RoleTaskMgr::Uninit()
{

}

bool RoleTaskMgr::OnDayPass()
{
	for (auto it = m_roleTask.begin(); it != m_roleTask.end(); ++it)
	{
		it->second.OnDayPass();
	}

	m_lastPassDayTime = GameTime::GetTime();
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eRoleTaskResetTime, ToString(m_lastPassDayTime));	
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eRoleTaskResetTime);

	return true;
}

void RoleTaskMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	SaveToDB(20);
}

void RoleTaskMgr::SaveToDB(UINT32 num)
{
	UINT32 i = 0;
	while (!m_dirtyList.empty() && i < num)
	{
		UINT64 id = m_dirtyList.pop_front();
		CRoleTask* pTask = GetRoleTask(id);
		if (pTask == NULL)
		{
			continue;
		}
		pTask->UpdateToDB();
		i++;
	}
}

bool RoleTaskMgr::LoadFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::string sql("select _id, data from roletask");
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(sql.c_str(), sql.size(), &poRes))
	{
		LogError("Read table roletask failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

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

		std::string bData;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			bData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'data' column failed");
			return false;
		}
		KKSG::RoleTaskSaveData data;
		data.ParseFromString(bData);

		CRoleTask& roleTask = m_roleTask[roleId];
		roleTask.m_roldId = roleId;
		roleTask.FromKKSG(data);
	}

	poRes->Release();
	SSInfo << "Load roletask num: " << m_roleTask.size() << END;

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all roletask data from db use time: %llu ms", qwTimeUsed);
	return true;
}

bool RoleTaskMgr::DoLoadFile()
{
	if (!m_dailyFormat.LoadFile("table/DailyTaskFormat.txt"))
	{
		LogError("load table/DailyTaskFormat.txt failed");
		return false;
	}
	DailyTaskLuck luckTable;
	if (!luckTable.LoadFile("table/DailyTaskLuck.txt"))
	{
		LogError("load table/DailyTaskLuck.txt failed");
		return false;
	}

	m_luckFormatProb.clear();
	m_luckProb.clear();
	m_backFlowLuckProb.clear();

	UINT32 luckSumSate = 0;
	UINT32 backflowSumRate = 0;
	for (auto it = luckTable.Table.begin(); it != luckTable.Table.end(); ++it)
	{
		DailyTaskLuck::RowData* pData = *it;
		UINT32 luck = pData->id;
		UINT32 sumRate = 0;
		for (auto it2 = pData->prob.begin(); it2 != pData->prob.end(); ++it2)
		{
			auto& seq = *it2;
			if (seq[1] == 0)
			{
				continue;
			}
			sumRate += seq[1];
			auto pFormat = GetDailyFormat(seq[0]);
			if (pFormat == NULL)
			{
				LogError("no format [%u]", seq[0]);
				return false;
			}
			m_luckFormatProb[luck][sumRate] = pFormat;
		}
		if (pData->getProb)
		{
			luckSumSate += pData->getProb;
			m_luckProb[luckSumSate] = luck;
		}
		if (pData->backflowProb)
		{
			backflowSumRate += pData->backflowProb;
			m_backFlowLuckProb[backflowSumRate] = luck;
		}
	}
	return true;
}

void RoleTaskMgr::ClearFile()
{
	m_dailyFormat.Clear();
	m_luckFormatProb.clear();
	m_luckProb.clear();
	m_backFlowLuckProb.clear();
}

void RoleTaskMgr::UpdateTask(UINT64 roleId, KKSG::TaskOper oper, UINT32 type, UINT32 id, UINT32 step)
{
	UINT32 eventID = TASK_EVENT_OFFSET;
	switch (oper)
	{
	case KKSG::TaskOper_Set: 
		eventID += TASK_EVENT_SET;
		break;
	case KKSG::TaskOper_Add:
		eventID += TASK_EVENT_ADD;
		break;
	case KKSG::TaskOper_Del:
		eventID += TASK_EVENT_DEL;
		break;
	}
	EventMgr::Instance()->AddEvent(roleId, eventID, type, id, step);
}

void RoleTaskMgr::AddWeeklyTaskStepById(UINT64 roleId, UINT32 taskId, UINT32 step)
{
	EventMgr::Instance()->AddEvent(roleId, TASK_EVENT_OFFSET + TASK_EVENT_ADD_TASKID_STEP, taskId, step);
}

void RoleTaskMgr::AddWeeklyTaskHelpInfo(UINT64 roleId, UINT64 helperId, const std::string& helperName, UINT32 itemId, UINT32 itemCount)
{
	UINT32 now = GameTime::GetTime();
	EventMgr::Instance()->AddEvent(roleId, TASK_EVENT_OFFSET + TASK_EVENT_HELP_RECORD, itemId, itemCount, now, helperId, 0, helperName);
}

void RoleTaskMgr::AddDailyTaskRefresh(UINT64 roleId, UINT32 formatId)
{
	EventMgr::Instance()->AddEvent(roleId, TASK_EVENT_OFFSET + TASK_EVENT_DAILY_TASK_REFRESH, formatId);
}

CRoleTask* RoleTaskMgr::GetRoleTask(UINT64 roleId)
{
	auto it = m_roleTask.find(roleId);
	if (it != m_roleTask.end())
	{
		return &it->second;
	}
	CRoleTask task;
	task.m_roldId = roleId;
	task.OnDayPass();
	auto insertPair = m_roleTask.insert(std::make_pair(roleId, task));
	return &insertPair.first->second;
}

DailyTaskFormat::RowData* RoleTaskMgr::GetDailyFormat(UINT32 id)
{
	for (auto it = m_dailyFormat.Table.begin(); it != m_dailyFormat.Table.end(); ++it)
	{
		DailyTaskFormat::RowData* pData = *it;
		if (pData->id == id)
		{
			return pData;
		}
	}
	return NULL;
}

UINT32 RoleTaskMgr::RandomLuck(bool isBack)
{
	auto& pMap = isBack ? m_backFlowLuckProb : m_luckProb;
	if (pMap.empty())
	{
		return 1;
	}
	return pMap.upper_bound(XRandom::randInt(0, pMap.rbegin()->first))->second;
}

DailyTaskFormat::RowData* RoleTaskMgr::RandomDailyTaskFormat(UINT32 luck)
{
	auto it1 = m_luckFormatProb.find(luck);
	if (it1 == m_luckFormatProb.end())
	{
		return NULL;
	}
	auto& rateMap = it1->second;
	if (rateMap.empty())
	{
		return NULL;
	}
	auto it2 = rateMap.upper_bound(XRandom::randInt(0, rateMap.rbegin()->first));
	return it2->second;
}

void RoleTaskMgr::OnRoleLogin(CRole* pRole)
{
	CRoleTask* pRoleTask = GetRoleTask(pRole->GetID());
	if (pRoleTask)
	{
		pRoleTask->OnRoleLogin(pRole);
	}
}

void RoleTaskMgr::AddToDirty(UINT64 roleId)
{
	m_dirtyList.push_back(roleId);
}

void RoleTaskMgr::CheckDayPass()
{
	const std::string temp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eRoleTaskResetTime);
	m_lastPassDayTime = convert<UINT32>(temp);
	if (!GameTime::IsInSameDay(GameTime::GetTime(), m_lastPassDayTime, true))
	{
		OnDayPass();
	}
}


