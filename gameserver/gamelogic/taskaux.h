#ifndef _TASK_AUX_H_
#define _TASK_AUX_H_

#include "pb/project.pb.h"
#include "table/TaskMgr.h"
#include "gamelogic/bagtransition.h"

class Role;
class TaskRecord;
class PeriodTask;

struct SubTask
{
	SubTask(PeriodTask* pOwner): m_pOwner(pOwner), id(0), type(0), step(0), maxStep(0), rewarded(false), askHelp(false), index(0) {}
	virtual ~SubTask() {}

	void FromKKSG(const KKSG::DailyTaskInfo& info, DailyTask::RowData* pTask);
	void ToKKSG(KKSG::DailyTaskInfo& info);
	bool UpdateTask(UINT32 oper, UINT32 tasktype, UINT32 conndid, UINT32 operstep);
	void SpecialHandle(UINT32& oper, UINT32 tasktype, UINT32& conndid, UINT32& operstep);
	void AddTaskStep(UINT32 operstep);
	bool IsItemTypeTask();
	bool IsTaskItem(ItemConf* pItemConf);

	PeriodTask* m_pOwner;
	UINT32 id;
	UINT32 type;
	UINT32 step;
	std::vector<UINT32> condIds;		// 条件物品id或者条件关卡id
	UINT32 maxStep;
	bool rewarded;
	bool askHelp;
	UINT32 index;
};

class PeriodTask
{
public:
	PeriodTask(KKSG::PeriodTaskType type);
	virtual ~PeriodTask();

	void SetHandler(Role* pRole, TaskRecord* pTask) { m_role = pRole; m_record = pTask; }
	void UpdateTask(UINT32 oper, UINT32 type, UINT32 id = 0, UINT32 step = 1);

	virtual bool Load(const KKSG::RoleTask& data) = 0; 
	virtual void Save(KKSG::RoleTask& data) = 0;

	virtual void OnSubTaskAccept(SubTask* pSubTask) = 0;
	virtual void OnSubTaskFinish(SubTask* pSubTask) = 0;
	virtual void OnTaskOneReward(SubTask* pSubTask, DailyTask::RowData* pConfig) = 0;
	virtual void OnFirstEnterScene() = 0;

	KKSG::ErrorCode TakeOneReward(UINT32 index);
	KKSG::ErrorCode TakeOneRewardById(UINT32 id);
	KKSG::ErrorCode TakeOneReward(SubTask* task);

	std::vector<SubTask*>& GetAllSubTasks() { return m_tasks; }

	UINT32 GetAskHelpNum() { return m_askHelpNum; }
	void AddAskHelpNum(UINT32 num = 1);
	void AddDonateCount(UINT32 value);
	UINT32 GetTodayDonateCount() { return m_DonateNum; }
	UINT32 GetTotalDonateCount() { return m_totalDonateNum; }
	UINT32 GetAcceptLevel() { return m_acceptLevel; }
	bool IsAllSubTaskRewarded();
	void SetDirty();
	TaskRecord* GetTaskRecord() { return m_record; }

	UINT32 TaskCondIdConvert(UINT32 connId);

	template <typename T>
	T* GetSubTask(UINT32 id)
	{
		for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)	
		{
			SubTask* pSubTask = *it;
			if (pSubTask->id == id)
			{
				return static_cast<T*>(pSubTask);
			}
		}
		return NULL;
	}
	template <typename T>
	T* GetSubTaskByIndex(UINT32 index)
	{
		if (index >= m_tasks.size())
		{
			return NULL;
		}
		return static_cast<T*>(m_tasks[index]);
	}

protected:
	void CheckAndUpdate(SubTask* task);
	void ClearSubTasks();

protected:
	KKSG::PeriodTaskType m_type;
	Role* m_role;
	TaskRecord* m_record;

	bool m_accepted;
	std::vector<SubTask*> m_tasks;
	UINT32 m_acceptLevel;
	UINT32 m_completeNum;
	UINT32 m_askHelpNum;
	bool m_rewarded;
	bool m_redPoint;

	UINT32 m_DonateNum;
	UINT32 m_totalDonateNum;

	UINT32 m_periodTaskNum;	// 任务领取过的次数
};

#endif