#ifndef _TASK_DEFINE_H_
#define _TASK_DEFINE_H_

enum TaskTpye
{
	TaskType_Master = 1,
	TaskType_Branch = 2,
	TaskType_Pratice = 3,
	TaskType_Daily = 4,
	TaskType_Weekly = 7,
	TaskType_Activity = 8,
};

enum DailyTaskRewardType
{
	DailyTaskRewardType_Sub = 1,		// 每个小任务奖励
	DailyTaskRewardType_Total = 2,		// 总任务奖励
};

enum TaskDonateOper
{
	TaskDonateOper_Add = 1,
	TaskDonateOper_Remove = 2,
	TaskDonateOper_Update = 3,
	TaskDonateOper_RemoveAllWeek = 4,
};


enum DailyTaskFormatQuality
{
	DTFQ_D = 0,
	DTFQ_C = 1,
	DTFQ_B = 2,
	DTFQ_A = 3,
	DTFQ_S = 4,
};

enum TaskRefreshType
{
	TaskRefreshType_CantRefresh = 1,		
	TaskRefreshType_CanRefresh = 2,
	TaskRefreshType_UpdateScore = 3,
	TaskRefreshType_UpdateLuck = 4,
	TaskRefreshType_BuyRefreshCount = 5,
	TaskRefreshType_NewAccept = 6,

};

#endif