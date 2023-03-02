#include "pch.h"
#include "dragonguildtask.h"
#include "dragonguild.h"
#include "util/gametime.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "table/globalconfig.h"
#include "table/ItemConfigMgr.h"
#include "define/itemdef.h"
#include "ranklist/ranklistmgr.h"
#include "dragonguild/ptcm2g_notifydragonguildredpoint.h"
#include "global/GlobalConfigDBBuild.h"
#include "define/systemiddef.h"
#include "util/XCommon.h"
#include "timeutil.h"

DragonGuildTask::DragonGuildTask(DragonGuild* dragonguild):DragonGuildBase(dragonguild),taskRefreshTime(0)
{
	m_mapAchivements.clear();
	CheckAndRefreshAchivement();
	m_mapTasks.clear();
	RefreshDragonGuildTask();
	
	UpdateTaskRefreshTime();
}

bool DragonGuildTask::LoadFromDB(const KKSG::DragonGuildSaveData& data)
{
	KKSG::DragonGuildTaskData taskData;
	if (!taskData.ParseFromString(data.task()))
	{
		LogError("parse DragonGuildTaskData.task error");
		return false;
	}
	taskRefreshTime = taskData.refreshtime();
	m_mapTasks.clear();
	for (int i = 0; i < taskData.tasks_size(); ++i)
	{
		m_mapTasks[taskData.tasks(i).taskid()] = taskData.tasks(i);
	}
	m_mapAchivements.clear();
	for (int i = 0; i < taskData.achivements_size(); ++i)
	{
		m_mapAchivements[taskData.achivements(i).taskid()] = taskData.achivements(i);
	}
	OnDayPass();
	return true;
}

void DragonGuildTask::SaveToDB(KKSG::DragonGuildSaveData& data)
{
	KKSG::DragonGuildTaskData taskData;
	taskData.clear_tasks();
	for (auto it = m_mapTasks.begin(); it != m_mapTasks.end(); ++it)
	{
		taskData.add_tasks()->CopyFrom(it->second);
	}
	taskData.clear_achivements();
	for (auto it = m_mapAchivements.begin(); it != m_mapAchivements.end(); ++it)
	{
		taskData.add_achivements()->CopyFrom(it->second);
	}
	taskData.set_refreshtime(taskRefreshTime);
	taskData.SerializeToString(data.mutable_task());
}

void DragonGuildTask::UpdateTaskRefreshTime()
{
	Sequence<UINT32, 3>date = GetGlobalConfig().DragonGuildTaskRefreshStartDate;
	UINT32 year = date[0], month = date[1], day = date[2];
	UINT32 openSec = XCommon::MakeTimeNew(year, month, day, 5);
	UINT32 timeNow = GameTime::GetTime();
	UINT32 cycleTime = GetGlobalConfig().DragonGuildTaskRefreshCycleTime;
	UINT32 d = (timeNow - openSec)/(cycleTime * 3600 * 24);
	taskRefreshTime = openSec + (d + 1) * cycleTime * 3600*24;
	SetDirty();
}

std::string DragonGuildTask::GetRefreshTimeStr()
{
	return TimeUtil::GetDateTimeStr(taskRefreshTime);
}

void DragonGuildTask::OnDayPass()
{
	CheckAndRefreshAchivement();
	UINT32 timeNow = GameTime::GetTime();
	//到达下一任务周期
	if( timeNow >= taskRefreshTime )
	{
		UpdateTaskRefreshTime();
		OnTaskRefresh();
	}
	//现在时间加一个周期还没达到刷新时间，说明改过时间，纠正回来...
	UINT32 cycleTime = GetGlobalConfig().DragonGuildTaskRefreshCycleTime;
	if( (timeNow + (cycleTime * 3600 * 24)) < taskRefreshTime ) 
	{
		UpdateTaskRefreshTime();
		OnTaskRefresh();
	}
	SetDirty();
}

void DragonGuildTask::OnTaskRefresh()
{
	m_mapTasks.clear();
	RefreshDragonGuildTask();
	SetDirty();
}

void DragonGuildTask::RefreshDragonGuildTask()
{
	std::vector<UINT32>taskList;
	taskList.clear();
	UINT32 worldLevelNow = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel));
	if(worldLevelNow <= 0 || worldLevelNow > 200)
		worldLevelNow = 40;
	DragonGuildTableMgr::Instance()->RandomTask(worldLevelNow, taskList);
	for(UINT32 i = 0; i < taskList.size(); ++i)
	{
		PushOneTask(taskList[i]);
	}
}

void DragonGuildTask::PushOneTask(UINT32 taskID)
{
	if (m_mapTasks.find(taskID) != m_mapTasks.end())
	{
		return;
	}
	m_mapTasks[taskID].set_taskid(taskID);
	m_mapTasks[taskID].set_finishcount(0);
	m_mapTasks[taskID].set_receivecount(0);
	SetDirty();
}

void DragonGuildTask::CheckAndRefreshAchivement()
{
	std::map<UINT32,bool>new_achieveMap;
	std::vector<UINT32>achiveList;
	achiveList.clear();
	DragonGuildTableMgr::Instance()->GetAchiveList(achiveList);
	for(size_t i = 0; i < achiveList.size(); ++i)
	{
		UINT32 achiveID = achiveList[i];
		new_achieveMap[achiveID] = true;
		PushOneAchive(achiveList[i]);
	}
	std::map<UINT32, KKSG::DragonGuildTaskInfo>m_mapTemp(m_mapAchivements);
	for(auto it = m_mapTemp.begin(); it != m_mapTemp.end(); ++it)
	{
		UINT32 achiveID = it->first;
		if(new_achieveMap.find(achiveID) == new_achieveMap.end())
		{
			DeleteOneAchive(achiveID);
		}
	}
}

void DragonGuildTask::PushOneAchive(UINT32 achiveID)
{
	if (m_mapAchivements.find(achiveID) != m_mapAchivements.end())
	{
		return;
	}
	m_mapAchivements[achiveID].set_taskid(achiveID);
	m_mapAchivements[achiveID].set_finishcount(0);
	m_mapAchivements[achiveID].set_receivecount(0);
	SetDirty();
}

void DragonGuildTask::DeleteOneAchive(UINT32 achiveID)
{
	auto it = m_mapAchivements.find(achiveID);
	if (it == m_mapAchivements.end())
	{
		return;
	}
	m_mapAchivements.erase(it);
	SetDirty();
}

KKSG::ErrorCode  DragonGuildTask::GetTaskStatus(KKSG::DragonGuildTaskType taskType, UINT32 taskID)
{
	if(taskType == KKSG::TASK_NORMAL)
	{
		DragonGuildTaskTable::RowData* data = DragonGuildTableMgr::Instance()->GetTaskRow(taskID);
		if(data == NULL || m_mapTasks.find(taskID) == m_mapTasks.end())
			return KKSG::ERR_FAILED;
		if(m_mapTasks[taskID].finishcount() < data->count)
			return KKSG::ERR_REWARD_LIMIT;
		return KKSG::ERR_SUCCESS;
	}
	else if(taskType == KKSG::TASK_ACHIVEMENT)
	{
		DragonGuildAchieveTable::RowData* data = DragonGuildTableMgr::Instance()->GetAchiveRow(taskID);
		if(data == NULL || m_mapAchivements.find(taskID) == m_mapAchivements.end())
			return KKSG::ERR_FAILED;
		if(m_mapAchivements[taskID].finishcount() < data->count)
			return KKSG::ERR_REWARD_LIMIT;
		if(m_mapAchivements[taskID].receivecount() >= data->chestCount)
			return KKSG::ERR_TEAM_TOWER_DAYCOUNT;
		return KKSG::ERR_SUCCESS;
	}
	return KKSG::ERR_FAILED;
}

bool DragonGuildTask::FillTaskFinishedList(std::vector<UINT32>& tasks, std::vector<UINT32>& achievements)
{
	bool haveTaskFinished = false;
	for(auto it = m_mapTasks.begin(); it != m_mapTasks.end(); ++it)
	{
		UINT32 taskID = it->first;
		DragonGuildTaskTable::RowData* data = DragonGuildTableMgr::Instance()->GetTaskRow(taskID);
		
		if( (it->second).finishcount() >= data->count)
		{
			haveTaskFinished = true;
			tasks.push_back(taskID);
		}
	}
	for(auto it = m_mapAchivements.begin(); it != m_mapAchivements.end(); ++it)
	{
		UINT32 taskID = it->first;
		DragonGuildAchieveTable::RowData* data = DragonGuildTableMgr::Instance()->GetAchiveRow(taskID);
		if( (it->second).finishcount() >= data->count && (it->second).receivecount() < data->chestCount)
		{
			haveTaskFinished = true;
			achievements.push_back(taskID);
		}
	}
	return haveTaskFinished ;
}

void DragonGuildTask::AddTaskFinishCount(KKSG::DragonGuildTaskType taskType, UINT32 taskID)
{
	bool notify = false;
	UINT32 addExp = 0;
	if(taskType == KKSG::TASK_NORMAL)
	{
		DragonGuildTaskTable::RowData* data = DragonGuildTableMgr::Instance()->GetTaskRow(taskID);
		auto it = m_mapTasks.find(taskID);
		if(it != m_mapTasks.end())
		{
			KKSG::DragonGuildTaskInfo& info = it->second;
			if(data->count > info.finishcount())
			{
				info.set_finishcount(info.finishcount() + 1);
				SetDirty();
				if(data->count == info.finishcount())
				{
					notify = true;
					addExp = data->guildExp;
				}
			}
		}
	}
	else if(taskType == KKSG::TASK_ACHIVEMENT)
	{
		DragonGuildAchieveTable::RowData* data = DragonGuildTableMgr::Instance()->GetAchiveRow(taskID);
		auto it = m_mapAchivements.find(taskID);
		if(it != m_mapAchivements.end())
		{
			KKSG::DragonGuildTaskInfo& info = it->second;
			if(data->count > info.finishcount())
			{
				info.set_finishcount(info.finishcount() + 1);
				SetDirty();
				if(data->count == info.finishcount())
				{
					notify = true;
					addExp = data->guildExp;
				}
			}
		}
	}
	if(notify)
	{
		PtcM2G_NotifyDragonGuildRedPoint ntf;
		ntf.m_Data.set_systemid(SYS_DRAGONGUILD_TASK);
		ntf.m_Data.set_tasktype(taskType);
		ntf.m_Data.set_taskid(taskID);
		m_owner->BroadCastToGS(ntf);

		m_owner->AddExp(addExp);
	}
}

void DragonGuildTask::CostTaskChestCount(KKSG::DragonGuildTaskType taskType, UINT32 taskID)
{
	/*
	if(taskType == KKSG::TASK_NORMAL)
	{
		auto it = m_mapTasks.find(taskID);
		if(it != m_mapTasks.end())
		{
			KKSG::DragonGuildTaskInfo& info = it->second;
			if(info.receivecount() >  0)
			{
				info.set_receivecount(info.receivecount() - 1);
				SetDirty();
			}
		}
	}
	else 
	*/
	if(taskType == KKSG::TASK_ACHIVEMENT)
	{
		DragonGuildAchieveTable::RowData* data = DragonGuildTableMgr::Instance()->GetAchiveRow(taskID);
		auto it = m_mapAchivements.find(taskID);
		if(it != m_mapAchivements.end()  && data != NULL)
		{
			KKSG::DragonGuildTaskInfo& info = it->second;
			if(info.receivecount() <  data->chestCount)
			{
				info.set_receivecount(info.receivecount() + 1);
				SetDirty();
			}
		}
	}
}

void DragonGuildTask::GetDragonGuildTaskIdList(KKSG::DragonGuildTaskType taskType, std::vector<KKSG::DragonGuildTaskInfo>& taskList)
{
	if(taskType == KKSG::TASK_NORMAL)
	{
		for(auto it = m_mapTasks.begin(); it != m_mapTasks.end(); ++it)
		{
			taskList.push_back(it->second);
		}
	}
	else if(taskType == KKSG::TASK_ACHIVEMENT)
	{
		for(auto it = m_mapAchivements.begin(); it != m_mapAchivements.end(); ++it)
		{
			taskList.push_back(it->second);
		}
	}
}