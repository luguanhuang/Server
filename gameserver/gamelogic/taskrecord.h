#ifndef _TASK_RECORD_H_
#define _TASK_RECORD_H_

#include "unit/roleserilizeinterface.h"
#include "table/TaskMgr.h"
#include "dailytaskrecord.h"
#include "weeklytaskrecord.h"
#include "pb/enum.pb.h"
#include "tablemgr/backflowtablemgr.h"

class Role;
struct TaskOneRecord
{
	UINT32 id;
	UINT32 type;	// �������ݿ�
	KKSG::TaskStatus status;
	std::vector<TaskCondition> conds;
	void ToKKSG(KKSG::TaskInfo& taskinfo);
	void FromKKSG(const KKSG::TaskInfo& taskinfo);
};

class TaskRecord : public RoleSerializeInterface
{
public:

	TaskRecord(Role *pRole);
	~TaskRecord();
	
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	// ����ģ������(������)�������
	void UpdateTask(KKSG::TaskOper oper, UINT32 type, UINT32 id = 0, UINT32 step = 1);
	// �ͻ�����ȡ����(����ȡ����npc�Ի�����)
	KKSG::ErrorCode TakeTask(UINT32 taskId);
	// �ͻ�����ȡ������(����ȡ����npc�Ի�����)
	KKSG::ErrorCode TakeReward(UINT32 taskId);
	void FinishTaskWithReward(UINT32 taskId);
	void FinishBackFlowTask(BackFlowServerConfig::RowData* pData);

	void OnActivityOpen(UINT32 activityId);
	void OnActivityClose(UINT32 activityId);

	// �����Ƿ��Ѿ�����(��ȡ�������)
	bool IsTaskFinish(UINT32 taskId);
	// �����Ƿ�������(���ڵ�����ȡ״̬)
	bool IsTaskDoing(UINT32 taskId);

	void OnEnterScene();
	void OnFirstTimeEnterScene();

	void OnDayPass();
	void OnLevelUp(UINT32 oldLevel, UINT32 newLevel);

	void CheckTimeTaskStart();
	void CheckTimeTaskEnd();

	void SetDirty() { m_dirty.Set(); }
	void FinishDailyTask();
	void FinishWeeklyTask(bool finish);
	DailyTaskRecord& GetDailyRecord() { return m_dailyTask; }
	WeeklyTaskRecord& GetWeeklyRecord() { return m_weeklyTask; }
	UINT32 GetRemainDailyCount();

	// GM ����
	// ���ָ������
	void GMFinishTask(UINT32 taskId);
	// �������������������
	void GMFinishDoingTask();
	// �������񵽳�ʼ״̬
	void GMResetTask();
	// ��ȡ����(���ж���ȡ������ֱ����ȡ)
	void GMTakeTask(UINT32 taskId);
	// �������
	void GMFinishAll();

	void PushOverState(UINT32 taskId);
	void AcceptTask(UINT32 taskId, bool push = true, UINT32 activityId = 0);

private:
	bool FilterCondition(UINT32 type, UINT32 id);
	void CheckAndUpdateStep(TaskOneRecord& task);
	void UpdateStep(KKSG::TaskOper oper, TaskCondition& cond, UINT32 step);
	void CheckFinish(TaskOneRecord& task);

	void RemoveDoingTask(UINT32 taskId);
	void DealTaskFinish(TaskConfig* pConfig);
	void OnTaskFinish(TaskConfig* config);

	TaskOneRecord* GetTask(UINT32 taskId);

	void PushTaskState(TaskOneRecord& task);

	void CheckTaskTableChange();
	void GiveTaskReward(TaskConfig* config, bool noExp = false);

	void DoLog(UINT32 taskId, UINT32 taskType, UINT32 taskStatus, UINT32 score=0);

private:
	Switch m_dirty;
	UINT32 m_lastUpdateTime;
	Role *m_role;
	std::vector<TaskOneRecord> m_doingTasks;	// ��ǰ������������
	std::set<UINT32> m_finishTasks;				// �Ѿ���ɵ�����

	// ÿ����������ĵط��� û�����״̬����ɺ󲻴�finish������������Accept
	DailyTaskRecord m_dailyTask;
	WeeklyTaskRecord m_weeklyTask;
};

#endif