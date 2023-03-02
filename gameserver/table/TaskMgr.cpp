#include "pch.h"
#include "TaskMgr.h"
#include <time.h>
#include "pb/enum.pb.h"
#include "util/timespecificmgr.h"
#include "timeutil.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"
#include "util/XRandom.h"

TaskMgr::TaskMgr()
:m_dailyTaskId(0)
,m_weeklyTaskId(0)
{

}

TaskMgr::~TaskMgr()
{

}

bool TaskMgr::Init()
{
	return LoadFile();
}

void TaskMgr::Uninit()
{
}

bool TaskMgr::DoLoadFile()
{
	TaskTableNew newTask;
	if (!newTask.LoadFile("table/TaskListNew.txt"))
	{
		LogWarn("Load table table/TaskListNew.txt failed!");
		return false;
	}

	UINT32 now = TimeUtil::GetTime();
	UINT32 headMainTaskId = 0;
	for (size_t i = 0; i < newTask.Table.size(); ++i)
	{
		TaskTableNew::RowData* data = newTask.Table[i];
		TaskConfig& config = m_tasks[data->TaskID];
		config.id = data->TaskID;
		config.type = data->TaskType;
		config.needLevel = data->RequiredLevel;
		config.preTaskId = data->PreTaskID;
		config.activityId = data->ActivityID;
		config.rewards = data->RewardItem;
		config.mailIds = data->MailID;
		ParseTime(config, data);
		ParserCondition(config, data);

		if (config.startTime > now)
		{
			TimeSpecific spec(config.startTime);
			CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&TaskMgr::CheckTimeTaskStart, this));
		}

		if (config.endTime > now)
		{
			TimeSpecific spec(config.endTime);
			CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&TaskMgr::CheckTimeTaskEnd, this));
		}

		if (config.startTime && config.endTime)
		{
			m_timeTaskIds.push_back(config.id);
		}

		// 前置任务设置后续任务
		if (config.preTaskId)
		{
			TaskConfig& parent = m_tasks[config.preTaskId];
			parent.nextTaskids.push_back(config.id);
		}

		if (config.type == TaskType_Master && config.preTaskId == 0)
		{
			headMainTaskId = config.id;
			LogInfo("First MainTask TaskId: %u", headMainTaskId);
		}

		// 等级开放
		m_levelTask.insert(std::make_pair(config.needLevel, config.id));

		// 所有任务集合
		m_allTask.push_back(config.id);
		if (config.type == TaskType_Daily)
		{
			m_dailyTaskId = config.id;
		}
		else if (config.type == TaskType_Weekly)
		{
			m_weeklyTaskId = config.id;
		}
		if (config.activityId)
		{
			m_activityOpenTask[config.activityId].push_back(config.id);
		}
	}

	// 初始主线任务
	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		TaskConfig* config = &it->second;
		if (config->type == TaskType_Master && config->preTaskId == 0
			&& config->needLevel <= 1)
		{
			m_initTask.push_back(config);
		}
	}

	// 主线任务链
	m_mainTaskList.clear();
	TaskConfig* pConfig = GetTaskConfig(headMainTaskId);	
	while (pConfig)
	{
		if (pConfig->nextTaskids.empty())
		{
			break;
		}
		if (pConfig->nextTaskids.size() > 1)
		{
			LogError("Task[%u] NextTask not only one", pConfig->id);
			return false;
		}
		m_mainTaskList.push_back(pConfig->id);

		UINT32 nextId = pConfig->nextTaskids[0];
		auto it = std::find(m_mainTaskList.begin(), m_mainTaskList.end(), nextId);
		if (it != m_mainTaskList.end())
		{
			LogError("MainTask[%u] Apper twice", nextId);
			return false;
		}
		pConfig = GetTaskConfig(nextId);
	}

	for (auto it = m_mainTaskList.begin(); it != m_mainTaskList.end(); ++it)
	{
		LogInfo("MainTask List[%u]", *it);
	}
	// 日常任务
	m_dailyTask.Clear();	
	if (!m_dailyTask.LoadFile("table/DailyTask.txt"))
	{
		LogWarn("Load table table/DailyTask.txt failed!");
		return false;
	}

	m_dailyFormat.Clear();
	if (!m_dailyFormat.LoadFile("table/DailyTaskFormat.txt"))
	{
		LogWarn("Load table table/DailyTaskFormat.txt failed!");
		return false;
	}

	m_dailyReward.Clear();
	if (!m_dailyReward.LoadFile("table/DailyTaskReward.txt"))
	{
		LogWarn("Load table table/DailyTaskReward.txt failed!");
		return false;
	}

	for (size_t i = 0; i < m_dailyTask.Table.size(); ++i)
	{
		DailyTask::RowData* data = m_dailyTask.Table[i];
		// TODO 种类多了重新考虑一下
		for (UINT32 j = 0; j < data->conditionId.size(); ++j)
		{
			// 任务道具
			if (data->tasktype == KKSG::TaskConn_ItemID)
			{
				m_taskItem.insert(data->conditionId[j]);
			}
		}

		for (UINT32 j = data->level[0]; j <= data->level[1]; ++j)
		{
			UINT32 key = GetSubTaskKey(data->category, j, data->taskquality);
			switch (data->Option)
			{
			case 0:
				{
					m_levelQualityMap[key].push_back(data);
					m_levelQualityMapBackFlow[key].push_back(data);
					break;
				}
			case 1:
				{
					m_levelQualityMap[key].push_back(data);
					break;
				}
			case 2:
				{
					m_levelQualityMapBackFlow[key].push_back(data);
					break;
				}
			default:
				break;
			}
		}
	}

	for (size_t i = 0; i < m_dailyReward.Table.size(); ++i)
	{
		DailyTaskReward::RowData* data = m_dailyReward.Table[i];
		for (UINT32 j = data->level[0]; j <= data->level[1]; ++j)
		{
			UINT32 key = GetSubRewardKey(data->category, j, data->tasktype, data->taskquality);
			m_reweadMap[key] = data;

			// 领取总奖励的最少完成任务数
			if (data->tasktype == DailyTaskRewardType_Total && data->category == KKSG::PeriodTaskType_Daily)
			{
				UINT32 old = m_minTaskNum[j];
				if (old == 0)
				{
					m_minTaskNum[j] = data->taskquality;
					m_maxTaskNum[j] = data->taskquality;
				}
				else
				{
					if (data->taskquality < old)
					{
						m_minTaskNum[j] = data->taskquality;
					}
					if (data->taskquality > old)
					{
						m_maxTaskNum[j] = data->taskquality;
					}
				}
			}
			if (data->tasktype == DailyTaskRewardType_Total && data->category == KKSG::PeriodTaskType_Weekly)
			{
				m_weeklyBoxed[j].push_back(data->taskquality);
			}
		}
	}

	DailyTaskLuck luckTable;
	if (!luckTable.LoadFile("table/DailyTaskLuck.txt"))
	{
		LogError("load table/DailyTaskLuck.txt failed");
		return false;
	}
	m_luckFormatProb.clear();
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
			auto pFormat = GetFormat(seq[0]);
			if (pFormat == NULL)
			{
				LogError("no format [%u]", seq[0]);
				return false;
			}
			m_luckFormatProb[luck][sumRate] = pFormat->id;
		}
	}

	return true;
}

void TaskMgr::ClearFile()
{
	m_tasks.clear();
	m_levelTask.clear();
	m_initTask.clear();
	m_taskItem.clear();
	m_taskMonster.clear();
	m_allTask.clear();
	m_weeklyBoxed.clear();

	m_levelQualityMap.clear();
	m_levelQualityMapBackFlow.clear();
	m_reweadMap.clear();
	m_minTaskNum.clear();
	m_maxTaskNum.clear();
	m_timeTaskIds.clear();
	m_activityOpenTask.clear();
	m_luckFormatProb.clear();
}

TaskConfig* TaskMgr::GetTaskConfig(UINT32 taskId)
{
	auto it = m_tasks.find(taskId);
	if (it == m_tasks.end())
	{
		return NULL;
	}
	return &it->second;
}

void TaskMgr::GetTaskByLevel(UINT32 level, std::vector<UINT32>& tasks)
{
	auto itFirst = m_levelTask.lower_bound(0);
	auto itSecond = m_levelTask.upper_bound(level);

	for (auto it = itFirst; it != itSecond; ++it)
	{
		tasks.push_back(it->second);
	}
}

void TaskMgr::GetTaskByLevel(UINT32 preLevel, UINT32 nowLevel, std::vector<UINT32>& tasks)
{
	if (preLevel > nowLevel)
	{
		std::swap(preLevel, nowLevel);
	}
	auto itFirst = m_levelTask.upper_bound(preLevel);
	auto itSecond = m_levelTask.upper_bound(nowLevel);

	for (auto it = itFirst; it != itSecond; ++it)
	{
		tasks.push_back(it->second);
	}
}

void TaskMgr::ParserCondition(TaskConfig& config, TaskTableNew::RowData* data)
{
	// 关卡
	for (auto it = data->PassScene.begin(); it != data->PassScene.end(); ++it)
	{
		TaskCondition cond;
		cond.type = KKSG::TaskConn_StageID;
		cond.id = (*it)[0];
		cond.maxStep = (*it)[1];
		config.conds.push_back(cond);
	}
	// 关卡
	for (auto it = data->TaskScene.begin(); it != data->TaskScene.end(); ++it)
	{
		TaskCondition cond;
		cond.type = KKSG::TaskConn_StageID;
		cond.id = (*it)[0];
		cond.maxStep = (*it)[1];
		config.conds.push_back(cond);
	}
	// 任务道具
	for (auto it = data->TaskItem.begin(); it != data->TaskItem.end(); ++it)
	{
		TaskCondition cond;
		cond.type = KKSG::TaskConn_ItemID;
		cond.id = (*it)[0];
		cond.maxStep = (*it)[1];
		config.conds.push_back(cond);
		m_taskItem.insert(cond.id);
	}
	// 杀怪条件
	for (auto it = data->TaskMonster.begin(); it != data->TaskMonster.end(); ++it)
	{
		TaskCondition cond;
		cond.type = KKSG::TaskConn_MonsterID;
		cond.id = (*it)[0];
		cond.maxStep = (*it)[1];
		config.conds.push_back(cond);
		m_taskMonster.insert(cond.id);
	}
}

static UINT32 GetTime(std::vector<UINT32>& datas)
{
	if (datas.size() < 6)
	{
		return 0;
	}

	tm when;
	when.tm_year = datas[0] - 1900;
	when.tm_mon = datas[1] - 1;
	when.tm_mday = datas[2];
	when.tm_hour = datas[3];
	when.tm_min = datas[4];
	when.tm_sec = datas[5];
	time_t t = mktime(&when);
	return t;
}

void TaskMgr::ParseTime(TaskConfig& config, TaskTableNew::RowData* data)
{
	if (data->TaskTime.size() != 6 || data->EndTime.size() != 6)
	{
		return;
	}
	config.startTime = GetTime(data->TaskTime);
	config.endTime = GetTime(data->EndTime);
}

const std::vector<TaskConfig*>& TaskMgr::GetInitTask()
{
	return m_initTask;
}

const std::vector<UINT32>& TaskMgr::GetAllTask()
{
	return m_allTask;
}

bool TaskMgr::CheckTimeTaskStart()
{
	for (auto it = RoleManager::Instance()->Begin(); it != RoleManager::Instance()->End(); ++it)
	{
		Role* role = *it;
		role->Get<TaskRecord>()->CheckTimeTaskStart();
	}
	return true;
}

bool TaskMgr::CheckTimeTaskEnd()
{
	for (auto it = RoleManager::Instance()->Begin(); it != RoleManager::Instance()->End(); ++it)
	{
		Role* role = *it;
		role->Get<TaskRecord>()->CheckTimeTaskEnd();
	}
	return true;
}

bool TaskMgr::IsTaskItem(UINT32 itemId)
{
	return m_taskItem.find(itemId) != m_taskItem.end();
}

bool TaskMgr::IsTaskMonster(UINT32 monsterId)
{
	return m_taskMonster.find(monsterId) != m_taskMonster.end();
}

DailyTaskFormat::RowData* TaskMgr::GetFormat(UINT32 formatId)
{
	for (auto it = m_dailyFormat.Table.begin(); it != m_dailyFormat.Table.end(); ++it)
	{
		if ((*it)->id == formatId)
		{
			return *it;
		}
	}
	return NULL;
}

/** 跑圈任务相关接口 ***/
DailyTaskFormat::RowData* TaskMgr::GetRandomFormat(UINT32 periodType, UINT32 level)
{
	UINT32 sumRate = 0;
	std::vector<DailyTaskFormat::RowData*> data;
	for (size_t i = 0; i < m_dailyFormat.Table.size(); ++i)
	{
		DailyTaskFormat::RowData* format = m_dailyFormat.Table[i];
		if (format->category != periodType)
		{
			continue;
		}
		if (level >= format->level[0] && level <= format->level[1])
		{
			sumRate += format->weight;
			data.push_back(format);
		}
	}

	if (data.empty())
	{
		return NULL;
	}
	if (data.size() == 1)
	{
		return data[0];
	}
	UINT32 num = XRandom::randInt(0, sumRate);
	for (size_t i = 0; i < data.size(); ++i)
	{
		DailyTaskFormat::RowData* format = data[i];
		if (num < format->weight)
		{
			return format;
		}
		num -= format->weight;
	}
	// impossible
	return data[data.size() - 1];
}

void TaskMgr::RandomDailyTask(UINT32 periodType, bool isbackflow, UINT32 level, std::vector<DailyTask::RowData*>& tasks)
{
	// 随机一个模版
	DailyTaskFormat::RowData* format = GetRandomFormat(periodType, level);
	if (format == NULL)
	{
		LogWarn("random format failed, level[%u] periodType[%u]", level, periodType);
		return;
	}
	RandomDailyTaskWithFormat(format, periodType, isbackflow, level, tasks);
}

void TaskMgr::RandomDailyTaskWithFormat(DailyTaskFormat::RowData* format, UINT32 periodType, bool isbackflow, UINT32 level, std::vector<DailyTask::RowData*>& tasks)
{
	std::set<UINT32> TaskTypeSet;
	UINT32 remain = 0;
	for (size_t i = 0; i < format->format.size(); ++i)
	{
		UINT32 quality = format->format[i][0];
		UINT32 num = format->format[i][1] + remain;
		UINT32 key = GetSubTaskKey(periodType, level, quality);
		std::map<UINT32, std::vector<DailyTask::RowData*>>* levelquality2task = isbackflow ? &m_levelQualityMapBackFlow : &m_levelQualityMap;
		auto it = levelquality2task->find(key);
		if (it == levelquality2task->end())
		{
			LogWarn("no daily task, level[%u], quality[%u] periodType[%u]", level, quality, periodType);
			remain = num;
			continue;
		}
		UINT32 getNum = RandomSubTask(num, it->second, tasks, TaskTypeSet);
		remain = num - getNum;
	}
}

UINT32 TaskMgr::RandomSubTask(UINT32 num, std::vector<DailyTask::RowData*>& src, std::vector<DailyTask::RowData*>& dest, std::set<UINT32>& TaskTypeSet)
{
	std::map<UINT32, std::vector<DailyTask::RowData*>> srcMap;
	for (auto it = src.begin(); it != src.end(); ++it)
	{
		DailyTask::RowData* pData = *it;
		UINT32 taskType = GetTaskTypeUnique(pData);
		if (TaskTypeSet.find(taskType) != TaskTypeSet.end())
		{
			continue;
		}
		srcMap[taskType].push_back(pData);
	}

	if (srcMap.size() <= num)
	{
		for (auto it = srcMap.begin(); it != srcMap.end(); ++it)
		{
			DailyTask::RowData* pData = it->second[XRandom::randInt(0, it->second.size())];
			dest.push_back(pData);
			TaskTypeSet.insert(it->first);	
		}
		return srcMap.size();
	}

	for (int i = 0; i < num; ++i)
	{
		UINT32 randNum = XRandom::randInt(0, srcMap.size());
		auto it = srcMap.begin();
		std::advance(it, randNum);
		DailyTask::RowData* pData = it->second[XRandom::randInt(0, it->second.size())];
		dest.push_back(pData);
		TaskTypeSet.insert(it->first);	
		srcMap.erase(it);
	}
	return num;
}

DailyTask::RowData* TaskMgr::RandomOneSubTask(UINT32 periodType, bool isbackflow, UINT32 level, UINT32 quality, std::vector<UINT32>& except)
{
	UINT32 key = GetSubTaskKey(periodType, level, quality);
	std::map<UINT32, std::vector<DailyTask::RowData*>>* levelquality2task = isbackflow ? &m_levelQualityMapBackFlow : &m_levelQualityMap;
	auto it = levelquality2task->find(key);
	if (it == levelquality2task->end())
	{
		LogWarn("no daily task, level[%u], quality[%u] periodType[%u]", level, quality, periodType);
		return NULL;
	}
	std::set<UINT32> TaskTypeSet;
	for (auto it = except.begin(); it != except.end(); ++it)
	{
		DailyTask::RowData* pData = GetDailyTask(*it);
		if (pData == NULL)
		{
			continue;
		}
		UINT32 taskType = GetTaskTypeUnique(pData);
		TaskTypeSet.insert(taskType);
	}

	std::vector<DailyTask::RowData*> tasks;
	for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
	{
		DailyTask::RowData* pData = *it2;
		UINT32 taskType = GetTaskTypeUnique(pData);
		if (TaskTypeSet.find(taskType) == TaskTypeSet.end())
		{
			tasks.push_back(pData);
		}
	}
	if (tasks.empty())
	{
		LogWarn("no daily task, level[%u], quality[%u] periodType[%u] can random", level, quality, periodType);
		// 品质降一级，继续找
		return RandomOneSubTask(periodType, isbackflow, level, quality - 1, except);
	}
	return tasks[XRandom::randInt(0, tasks.size())];
}

DailyTaskReward::RowData* TaskMgr::GetDailyReward(UINT32 periodType, UINT32 level, UINT32 type, UINT32 num)
{
	UINT32 key = GetSubRewardKey(periodType, level, type, num);
	auto it = m_reweadMap.find(key);
	if (it != m_reweadMap.end())
	{
		return it->second;
	}
	return NULL;
}

DailyTask::RowData* TaskMgr::GetDailyTask(UINT32 id)
{
	return m_dailyTask.GetBytaskID(id);
}

UINT32 TaskMgr::GetMinTaskNum(UINT32 level)
{
	auto it = m_minTaskNum.find(level);
	if (it != m_minTaskNum.end())
	{
		return it->second;
	}
	return 0;
}

UINT32 TaskMgr::GetMaxTaskNum(UINT32 level)
{
	auto it = m_maxTaskNum.find(level);
	if (it != m_maxTaskNum.end())
	{
		return it->second;
	}
	return 0;
}

void TaskMgr::GetMainTaskUntil(UINT32 taskId, std::list<UINT32>& taskIds)
{
	taskIds.clear();
	bool found = false;
	for (auto it = m_mainTaskList.begin(); it != m_mainTaskList.end(); ++it)
	{
		taskIds.push_back(*it);
		if (*it == taskId)
		{
			found = true;
			break;
		}
	}
	if (found == false)
	{
		taskIds.clear();
	}
}

std::vector<UINT32>* TaskMgr::GetWeeklyBoxScores(UINT32 level)
{
	auto it = m_weeklyBoxed.find(level);
	if (it == m_weeklyBoxed.end())
	{
		return NULL;
	}
	return &it->second;
}

void TaskMgr::GetActivityOpenTask(UINT32 activityId, std::vector<UINT32>& tasks)
{
	auto it = m_activityOpenTask.find(activityId);
	if (it == m_activityOpenTask.end())
	{
		return;
	}
	tasks = it->second;
}

UINT32 TaskMgr::GetTaskTypeUnique(DailyTask::RowData* pData)
{
	if (pData->tasktype == KKSG::TaskConn_WorldBossRank)
	{
		return pData->tasktype;
	}
	return pData->conditionId[0] * 100 + pData->tasktype;
}

std::vector<Sequence<UINT32, 2>>* TaskMgr::GetDailyTaskTotalReward(DailyTaskReward::RowData* pData, UINT32 score)
{
	switch (score)
	{
	case DTFQ_S: return &pData->reward_s;
	case DTFQ_A: return &pData->reward_a;
	case DTFQ_B: return &pData->reward_b;
	case DTFQ_C: return &pData->reward_c;
	case DTFQ_D: return &pData->reward_d;
	default:	 return &pData->reward_d;
	}
}

std::vector<Sequence<UINT32, 2>>* TaskMgr::GetDailyTaskExtraReward(DailyTaskReward::RowData* pData, UINT32 score)
{
	switch (score)
	{
	case DTFQ_S: return &pData->extrareward_s;
	case DTFQ_A: return &pData->extrareward_a;
	case DTFQ_B: return &pData->extrareward_b;
	case DTFQ_C: return &pData->extrareward_c;
	case DTFQ_D: return &pData->extrareward_d;
	default:	 return &pData->extrareward_d;
	}
}

UINT32 TaskMgr::RandomDailyTaskFormat(UINT32 luck)
{
	auto it1 = m_luckFormatProb.find(luck);
	if (it1 == m_luckFormatProb.end())
	{
		return 0;
	}
	auto& rateMap = it1->second;
	if (rateMap.empty())
	{
		return 0;
	}
	auto it2 = rateMap.upper_bound(XRandom::randInt(0, rateMap.rbegin()->first));
	return it2->second;
}










