#ifndef __DRAGON_GUILD_TASK_H_
#define __DRAGON_GUILD_TASK_H_

#include "util/gametime.h"
#include "dragonguildbase.h"

class DragonGuildTask : public DragonGuildBase
{
public:
	DragonGuildTask(DragonGuild* dragonguild);

	virtual bool LoadFromDB(const KKSG::DragonGuildSaveData& data);
	virtual void SaveToDB(KKSG::DragonGuildSaveData& data);

	
	virtual void OnDayPass();
	//刷新到下一个任务刷新时间
	void UpdateTaskRefreshTime();
	std::string GetRefreshTimeStr();
	void OnTaskRefresh();
	
	void PushOneTask(UINT32 taskID);
	void PushOneAchive(UINT32 AchiveID);
	void DeleteOneAchive(UINT32 AchiveID);
	
	void RefreshDragonGuildTask();
	void CheckAndRefreshAchivement();
	
	void AddTaskFinishCount(KKSG::DragonGuildTaskType taskType, UINT32 taskID);

	void CostTaskChestCount(KKSG::DragonGuildTaskType taskType, UINT32 taskID);
	
	KKSG::ErrorCode GetTaskStatus(KKSG::DragonGuildTaskType taskType, UINT32 taskID);
	bool FillTaskFinishedList(std::vector<UINT32>& tasks, std::vector<UINT32>& achievements );

	void GetDragonGuildTaskIdList(KKSG::DragonGuildTaskType taskType, std::vector<KKSG::DragonGuildTaskInfo>& taskList);

private:		
	//小分队讨伐任务列表, 周期后重置
	std::map<UINT32, KKSG::DragonGuildTaskInfo>m_mapTasks;	
	//小分队成就任务列表
	std::map<UINT32, KKSG::DragonGuildTaskInfo>m_mapAchivements;	
	UINT32 taskRefreshTime;
};


#endif