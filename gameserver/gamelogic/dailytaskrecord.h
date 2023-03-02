#ifndef _ADVENTURE_TASK_H_
#define _ADVENTURE_TASK_H_

#include "taskaux.h"

/**
 * �ճ�����(�ܻ�����)
 */
class Role;
class TaskRecord;

struct DailyTaskOne : public SubTask
{
	DailyTaskOne(PeriodTask* pOwner): SubTask(pOwner) {}
};

class DailyTaskRecord : public PeriodTask
{
public:
	DailyTaskRecord();
	~DailyTaskRecord();

	virtual bool Load(const KKSG::RoleTask& data); 
	virtual void Save(KKSG::RoleTask& data);
	virtual void OnDayPass();

	virtual void OnSubTaskAccept(SubTask* pSubTask);
	virtual void OnSubTaskFinish(SubTask* pSubTask);
	virtual void OnTaskOneReward(SubTask* pSubTask, DailyTask::RowData* pConfig);
	virtual void OnFirstEnterScene();

	bool CanAccept();

	// ˢ��ð���������һ������
	void Refresh();
	void RefreshWithCheck(UINT32 formatId);
	void Refresh(DailyTaskFormat::RowData* format);
	// ��ȡ�ܵĽ���
	KKSG::ErrorCode TakeReward();
	KKSG::ErrorCode GiveUp();

	void FillDailyTaskInfo(KKSG::GetDailyTaskInfoRes& res);
	void CheckRedPoint();
	UINT32 GetRemainDailyCount();
	UINT32 GetScore() { return m_score; }
	bool CanRefresh();
	void SetLuck(UINT32 luck);

	void UpdateScoreToMs();
	void UpdateCanRefreshToMs();
	void UpdateNewAcceptToMs();
	void CheckScore();

	void GMFinish();
	void GMReset();
	void GMResetHelpNum();
	void DoTxLog(UINT32 id, INT32 state);

private:
	UINT32 m_score;
	UINT32 m_luck;
};

#endif