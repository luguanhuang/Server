#ifndef _CROSS_ROLE_SUMMARY_MGR_H_
#define _CROSS_ROLE_SUMMARY_MGR_H_

#include "util/utilsingleton.h"
#include "timer.h"
#include "rolesummary.h"
#include "util/maplist.h"

class CrossRoleSummaryMgr : public ITimer, public Singleton<CrossRoleSummaryMgr>
{
public:
	CrossRoleSummaryMgr();
	~CrossRoleSummaryMgr();

	bool Init();
	void Uninit();
	void SaveAll2DB();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool LoadFromDB();

	void UpdateRoleSummary(const KKSG::RoleSummaryStored& data);

	void AddToDirtyList(UINT64 id);

	CRoleSummary* GetRoleSummary(UINT64 roleId);

	void UpdateFromWorld(UINT64 roleId);

	void OnSeasonEnd();

private:
	std::unordered_map<UINT64, CRoleSummary> m_roleSummarys;
	MapList<UINT64> m_dirtyList;
};

#endif