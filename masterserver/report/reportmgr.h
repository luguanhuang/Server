#ifndef _REPORT_MGR_H_
#define _REPORT_MGR_H_

#include "util/utilsingleton.h"
#include "util/maplist.h"
#include "timer.h"

#include "rolereport.h"
#include "rolereporthandler.h"

class ReportMgr : public ITimer, public Singleton<ReportMgr>
{
public:
	ReportMgr();
	~ReportMgr();

	bool Init();
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	RoleReportInfo* GetRoleReportInfo(UINT64 roleId, bool add = false);
	UINT32 GetRoleBanTime(UINT64 roleId, UINT32 sceneType);

	void AddToDirtyList(UINT64 roleId);

	bool LoadFromDB();
	void SaveToDB(UINT32 num = 0);

	bool CheckWeekPass();
	void OnRoleLogin(UINT64 roleId);

	IReportHandler* GetHandler(UINT32 sceneType);

	UINT32 GetTotalNum(UINT32 sceneType);
	void AddTotalNum(UINT32 sceneType);
	void DecTotalNum(UINT32 sceneType);

private:
	std::unordered_map<UINT64, RoleReportInfo> m_infos;
	MapList<UINT64> m_dirtyList;
	std::map<UINT32, IReportHandler*> m_handler;
	std::unordered_map<UINT32, UINT32> m_totalBanNum;
};

#endif