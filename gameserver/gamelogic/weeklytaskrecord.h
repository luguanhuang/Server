#ifndef _WEEKLY_TASK_RECORD_H_
#define _WEEKLY_TASK_RECORD_H_

#include "taskaux.h"

/**
 * 周任务
 */
class Role;
class TaskRecord;
struct WeeklyTaskOne : public SubTask
{
	WeeklyTaskOne(PeriodTask* pOwner): SubTask(pOwner), refreshCount(0) {}

	void FromKKSG(const KKSG::WeeklyTaskInfo& info, DailyTask::RowData* pTask);
	void ToKKSG(KKSG::WeeklyTaskInfo& info);

	UINT32 refreshCount;
};

class WeeklyTaskRecord : public PeriodTask
{
public:
	WeeklyTaskRecord();
	~WeeklyTaskRecord();

	virtual bool Load(const KKSG::RoleTask& data); 
	virtual void Save(KKSG::RoleTask& data);

	virtual void OnSubTaskAccept(SubTask* pSubTask);
	virtual void OnSubTaskFinish(SubTask* pSubTask);
	virtual void OnTaskOneReward(SubTask* pSubTask, DailyTask::RowData* pConfig);
	virtual void OnFirstEnterScene();

	void OnDayPass();
	void OnWeekPass();
	bool CanAccept();

	// 刷新冒险任务，随机一批任务
	void Refresh();
	// 领取悬赏箱子奖励
	KKSG::ErrorCode TakeBoxReward(UINT32 score);
	KKSG::ErrorCode RefreshOne(UINT32 index);

	void CheckRedPoint();
	void CheckFinish();

	void FillTaskInfo(KKSG::GetWeeklyTaskInfoRes& res);
	void AddHelperInfo(KKSG::TaskHelpInfo& info);
	void ClearHelperInfo();

	void GMFinish();
	void GMReset();
	void GMResetHelpNum();
	void DoTxLog(UINT32 index, UINT32 op, UINT32 preid=0);

	bool IsBoxRewarded(UINT32 score);
	void SetBoxReward(UINT32 score);
	bool IsAllBoxReward();
	UINT32 GetRemainCount();
	UINT32 GetRemainFreeRefreshCount();


	UINT32 GetScore() { return m_score; }
	const std::vector<UINT32>& GetRewardedBox() { return m_rewardedBox; }

private:
	UINT32 m_score;
	std::vector<UINT32> m_rewardedBox;
	google::protobuf::RepeatedPtrField<KKSG::TaskHelpInfo> m_helperInfo;
	UINT32 m_freeRefreshCount;
};

#endif