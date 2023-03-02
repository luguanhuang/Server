#ifndef _ROLE_TASK_MGR_H_
#define _ROLE_TASK_MGR_H_

#include "util/utilsingleton.h"
#include "roletask.h"
#include "tablemgr/tablehelper.h"
#include "table/DailyTaskFormat.h"
#include "table/DailyTaskLuck.h"
#include "timer.h"
#include "util/maplist.h"


class CRole;
class RoleTaskMgr : public ITimer, public TableHelper<RoleTaskMgr>
{
public:
	RoleTaskMgr();
	~RoleTaskMgr();
	bool Init();
	void Uninit();
	bool LoadFromDB();
	void SaveToDB(UINT32 num);

	virtual bool DoLoadFile();
	virtual void ClearFile();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void OnRoleLogin(CRole* pRole);
	bool OnDayPass();
	void CheckDayPass();
	void AddToDirty(UINT64 roleId);

	void UpdateTask(UINT64 roleId, KKSG::TaskOper oper, UINT32 type, UINT32 id = 0, UINT32 step = 1);
	void AddWeeklyTaskStepById(UINT64 roleId, UINT32 taskId, UINT32 step);
	void AddWeeklyTaskHelpInfo(UINT64 roleId, UINT64 helperId, const std::string& helperName, UINT32 itemId, UINT32 itemCount);
	void AddDailyTaskRefresh(UINT64 roleId, UINT32 formatId);

	CRoleTask* GetRoleTask(UINT64 roleId);

	UINT32 RandomLuck(bool isBack);
	DailyTaskFormat::RowData* RandomDailyTaskFormat(UINT32 luck);
	DailyTaskFormat::RowData* GetDailyFormat(UINT32 id);

private:
	UINT32 m_lastPassDayTime;
	std::unordered_map<UINT64, CRoleTask> m_roleTask;
	MapList<UINT64> m_dirtyList;

	DailyTaskFormat m_dailyFormat;
	DailyTaskLuck m_luckTable;
	std::map<UINT32, std::map<UINT32, DailyTaskFormat::RowData*> > m_luckFormatProb;
	std::map<UINT32, UINT32> m_luckProb;
	std::map<UINT32, UINT32> m_backFlowLuckProb;
};

#endif