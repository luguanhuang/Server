#ifndef __TASKMGR_H__
#define __TASKMGR_H__

#include "tablemgr/tablehelper.h"

#include "TaskTableNew.h"
#include "DailyTask.h"
#include "DailyTaskFormat.h"
#include "DailyTaskReward.h"
#include "table/DailyTaskLuck.h"

#include "define/taskdef.h"

struct TaskCondition
{
	TaskCondition():type(0), id(0), step(0), maxStep(0) {}

	UINT32 type;
	UINT32 id;
	UINT32 step;
	UINT32 maxStep;
};

struct TaskConfig
{
	TaskConfig()
	:id(0),type(0),startTime(0),endTime(0),needLevel(0),preTaskId(0),activityId(0) {}

	UINT32 id;							// 任务id
	UINT32 type;						// 类型(主线支线等)
	UINT32 startTime;					// 开始时间
	UINT32 endTime;						// 结束时间
	UINT32 needLevel;					// 等级
	UINT32 preTaskId;					// 前置任务Id
	UINT32 activityId;					// 需要的任务Id
	std::vector<UINT32> nextTaskids;	// 后续任务Id
	std::vector<UINT32> mailIds;		// 奖励邮件

	std::vector<Sequence<uint, 2>> rewards;	// 奖励
	std::vector<TaskCondition> conds;
};

class TaskMgr : public TableHelper<TaskMgr>
{
public:
	TaskMgr();
	~TaskMgr();

public:
	bool Init();
	void Uninit();

	bool DoLoadFile();
	void ClearFile();

	TaskConfig* GetTaskConfig(UINT32 taskId);
	void GetTaskByLevel(UINT32 level, std::vector<UINT32>& taskIds);
	void GetTaskByLevel(UINT32 preLevel, UINT32 nowLevel, std::vector<UINT32>& taskIds);
	const std::vector<TaskConfig*>& GetInitTask();
	const std::vector<UINT32>& GetAllTask();
	void GetMainTaskUntil(UINT32 taskId, std::list<UINT32>& taskIds);

	bool CheckTimeTaskStart();
	bool CheckTimeTaskEnd();
	bool IsTaskItem(UINT32 itemId);
	bool IsTaskMonster(UINT32 monsterId);

	void RandomDailyTask(UINT32 periodType, bool isbackflow, UINT32 level, std::vector<DailyTask::RowData*>& tasks);
	void RandomDailyTaskWithFormat(DailyTaskFormat::RowData* format, UINT32 periodType, bool isbackflow, UINT32 level, std::vector<DailyTask::RowData*>& tasks);
	DailyTask::RowData* RandomOneSubTask(UINT32 periodType, bool isbackflow, UINT32 level, UINT32 quality, std::vector<UINT32>& except);
	DailyTaskFormat::RowData* GetRandomFormat(UINT32 periodType, UINT32 level);
	DailyTaskFormat::RowData* GetFormat(UINT32 formatId);
	DailyTaskReward::RowData* GetDailyReward(UINT32 periodType, UINT32 level, UINT32 type, UINT32 num);
	DailyTask::RowData* GetDailyTask(UINT32 id);
	UINT32 GetMinTaskNum(UINT32 level);
	UINT32 GetMaxTaskNum(UINT32 level);
	UINT32 GetDailyTaskId() { return m_dailyTaskId; }
	UINT32 GetWeeklyTaskId() { return m_weeklyTaskId; }
	std::vector<UINT32>& GetTimeTaskIds() { return m_timeTaskIds; }
	std::vector<UINT32>* GetWeeklyBoxScores(UINT32 level);
	std::vector<Sequence<UINT32, 2>>* GetDailyTaskTotalReward(DailyTaskReward::RowData* pData, UINT32 score);
	std::vector<Sequence<UINT32, 2>>* GetDailyTaskExtraReward(DailyTaskReward::RowData* pData, UINT32 score);

	UINT32 RandomSubTask(UINT32 num, std::vector<DailyTask::RowData*>& src, std::vector<DailyTask::RowData*>& dest, std::set<UINT32>& TaskTypeSet);
	void GetActivityOpenTask(UINT32 activityId, std::vector<UINT32>& tasks);

	UINT32 GetTaskTypeUnique(DailyTask::RowData* pData);
	UINT32 RandomDailyTaskFormat(UINT32 luck);

private:
	void ParserCondition(TaskConfig& config, TaskTableNew::RowData* data);
	void ParseTime(TaskConfig& config, TaskTableNew::RowData* data);

	inline UINT32 GetSubTaskKey(UINT32 periodType, UINT32 level, UINT32 quality)
	{
		// 10-4-4
		return (periodType) | (quality << 4) | (level << 8);
	}
	inline UINT32 GetSubRewardKey(UINT32 periolType, UINT32 level, UINT32 type, UINT32 value)
	{
		// 14 - 10 - 4 - 4
		return (periolType) | (type << 4) | (level << 8) | (value << 18);
	}

private:
	std::map<UINT32, TaskConfig> m_tasks;			// 所有任务
	std::multimap<UINT32, UINT32> m_levelTask;		// 等级对应任务
	std::vector<TaskConfig*> m_initTask;			// 创角色初始任务
	std::set<UINT32> m_taskItem;					// 任务道具
	std::set<UINT32> m_taskMonster;					// 任务怪物
	std::vector<UINT32> m_allTask;					// 所有任务

	// 日常跑环任务相关
	DailyTask m_dailyTask;
	DailyTaskFormat m_dailyFormat;
	DailyTaskReward m_dailyReward;
	std::map<UINT32, std::vector<DailyTask::RowData*> > m_levelQualityMapBackFlow;	// key: , value: 任务id, 回流玩家的任务
	std::map<UINT32, std::vector<DailyTask::RowData*> > m_levelQualityMap;			// key: , value: 任务id
	std::map<UINT32, DailyTaskReward::RowData*> m_reweadMap;		// key: , value: reward 
	std::map<UINT32, UINT32> m_minTaskNum;					// 领奖的最少任务数量
	std::map<UINT32, UINT32> m_maxTaskNum;					// 领奖最多任务数量
	std::map<UINT32, std::vector<UINT32>> m_weeklyBoxed;	// 日常任务的箱子奖励
	UINT32 m_dailyTaskId;								// 日常任务id
	UINT32 m_weeklyTaskId;								// 周任务id
	std::vector<UINT32> m_timeTaskIds;					// 限时任务

	std::list<UINT32> m_mainTaskList;					// 主线任务链
	std::map<UINT32, std::vector<UINT32>> m_activityOpenTask;		// 活动开启的任务

	std::map<UINT32, std::map<UINT32, UINT32>> m_luckFormatProb;
};

#endif // __TASKMGR_H__