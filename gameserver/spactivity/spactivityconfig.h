#ifndef __SPACTIVITYCONFIG_H__
#define __SPACTIVITYCONFIG_H__

#include "table/SuperActivity.h"
#include "table/SuperActivityTime.h"
#include "table/SuperActivityTask.h"
#include "table/BackflowActivity.h"
#include "table/AncientTimesTable.h"


typedef SuperActivityTime::RowData SpActivityTimeConf;
typedef SuperActivityTask::RowData SpActivityTaskConf;
typedef BackflowActivity::RowData BackflowActivityConf;

enum
{
	SpActivityTask_Normal = 0,
	SpActivityTask_Father = 1,
	SpActivityTask_Son    = 2,
};

struct SpActGetTaskConfArg
{
	SpActGetTaskConfArg(UINT32 _baseTask, UINT32 _num1 = -1)
	{
		baseTask = _baseTask;
		num1 = _num1;
	}
	UINT32 baseTask;
	UINT32 num1;
};

class Role;

class SpActivityConfig
{
	SpActivityConfig(){}
	~SpActivityConfig(){}
	DECLARE_SINGLETON(SpActivityConfig)

public:
	bool Init();
	void Uninit();

	bool CheckFile(bool isReload);
	bool LoadFile(bool isReload = true);
	void ClearFile();

	SpActivityTimeConf* GetTimeConf(UINT32 actid){ return m_oSpActivityTime.GetByactid(actid); }

	SpActivityTaskConf* GetTaskConf(UINT32 taskid){ return m_oSpActivityTask.GetBytaskid(taskid); }

	void GetRoleHaveTaskConf(Role* pRole, const SpActGetTaskConfArg& arg, std::vector<SpActivityTaskConf*>& outTask, UINT32 actid = 0);
	void GetRoleHaveTaskConf(Role* pRole, const std::vector<SpActGetTaskConfArg>& vecArgs, std::vector<SpActivityTaskConf*>& outTask, UINT32 actid = 0);

	bool IsTaskConfSuitArg(SpActivityTaskConf* conf, const SpActGetTaskConfArg& arg);

	void GetAllTaskByActID(UINT32 actid, std::vector<SpActivityTaskConf*>& outTask);

	const SuperActivityTime& GetSpAcitivityTimeTable() { return m_oSpActivityTime; }

	void GetTaskConfsByTaskIDs(const std::vector<UINT32>& taskids, std::vector<SpActivityTaskConf*>& confs);

	const BackflowActivityConf* GetBackflowActivityConf(UINT32 taskid);

	const BackflowActivity& GetBackflowActivityTable() { return m_oBackFlowActivity; }

private:
	bool _CheckReloadTimeTable(SuperActivityTime& newTime);

private:
	SuperActivityTime m_oSpActivityTime;
	SuperActivityTask m_oSpActivityTask;
	
	std::unordered_map<UINT32, std::vector<SpActivityTaskConf*>> m_mapActTask; // key : actid, contain father task

	BackflowActivity m_oBackFlowActivity;
};


#endif