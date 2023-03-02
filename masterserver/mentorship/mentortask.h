#ifndef __MENTORTASK_H__
#define __MENTORTASK_H__

class MentorShipOneSide;

namespace KKSG
{
	class OneMentorRelationInfo2Client;
	class MentorSelfInfo;
}

struct TaskTypeVar
{
	TaskTypeVar()
	{
		m_taskType = 0;
		m_var = 0;
	}
	bool operator == (TaskTypeVar& b)
	{
		if (m_taskType == b.m_taskType && m_var == b.m_var)
		{
			return true;
		}

		return false;
	}

	bool operator < (const TaskTypeVar& b) const
	{
		if (m_taskType == b.m_taskType && m_var == b.m_var)
		{
			return false;
		}
		if (m_taskType < b.m_taskType)
		{
			return true;
		}
		if (m_taskType == b.m_taskType)
		{
			if (m_var < b.m_var)
			{
				return true;
			}
			return false;
		}

		return false;
	}
	UINT32 m_taskType;
	UINT64 m_var;
};

struct OneMentorTask
{
	OneMentorTask()
	{
		m_taskID = 0;
		m_completeProgress = 0;
		m_completeTime = 0;
	}
	bool IsTaskComplete();
	bool CanReport(MentorShipOneSide* self, UINT64 roleID);
	bool Report(MentorShipOneSide* self, UINT64 roleID);
	bool Confirm(UINT64 roleID);
	bool IsCompleteBeforeMentor(MentorShipOneSide* self, UINT64 roleID);
	bool CheckComplete(bool useMax);

	void Convert2KKSGOneMentorTaskInfo(MentorShipOneSide* self, KKSG::OneMentorTaskInfo& info, UINT64 masterID = 0);

	int m_taskID;
	int m_completeProgress;
	int m_completeTime;
	//UINT32 m_taskType;
	//UINT64 m_var;
	TaskTypeVar m_var;
	std::map<UINT64, int> m_reportStatus;
};

class MentorTask
{
public:
	MentorTask(MentorShipOneSide* pOneSide);
	~MentorTask();

public:
	bool Init();
	void Uninit();
	bool Load(const KKSG::MentorAllData2Db& data);
	void Save(KKSG::MentorAllData2Db& data);

	void DoOpen();
	void Refresh();
	void AddTaskValue(UINT32 taskType, UINT64 taskVar, UINT32 add, UINT32 time=0);

	bool ConfirmTask(int taskID, UINT64 roleID);
	bool ReportTask(int taskID, UINT64 roleID);
	bool ReportAllTask(UINT64 roleID);
	void RemoveReportTask(UINT64 roleID);
	void CompleteAutoReport(UINT64 roleID);

	int GetTaskCompleteNum();
	const std::map<int, OneMentorTask>& GetTaskList(){return m_selfTaskList;}

	bool CanReportTask();
	//show to master
	void Convert2OneRelationClient(KKSG::OneMentorRelationInfo2Client& relation, UINT64 masterID);
	//show to student
	void Convert2SelfClient(KKSG::MentorSelfInfo& selfInfo);

	void DebugClear();
	void DoTxLog(int taskID, UINT64 roleID);
private:
	bool _IsTypeOpenClear(int type);
	void _GiveReward(UINT64 selfID, UINT64 roleID, int taskID);
	bool _UpdateTaskStatus(int taskID, UINT64 roleID, KKSG::EMentorTaskStatus status);
	bool _GetCanReportList(UINT64 roleID, std::vector<int>& taskList);
	bool _CheckFinishTask(/*UINT32 taskType, UINT64 taskVar, */UINT32 taskID, UINT32 progress);
	void _AddDailyActiveTaskValue(UINT32 taskType, UINT64 taskVar, UINT32 add, UINT32 time);
	bool _IsTaskTypeUseMaxValue(UINT32 taskType);
	bool _IsTaskTypeUseMinValue(UINT32 taskType);
	void _NotifyTaskHint();

private:
	MentorShipOneSide* m_pMentorOneSide;
	std::map<int, OneMentorTask> m_selfTaskList;
	std::map<TaskTypeVar, UINT32> m_taskTypeValue;
	std::map<TaskTypeVar, UINT32> m_taskTypeValueChangeTime;
};

#endif // 
