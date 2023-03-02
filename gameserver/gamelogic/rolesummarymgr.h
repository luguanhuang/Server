#ifndef __ROLESUMMARYMGR_H__
#define __ROLESUMMARYMGR_H__

#include "rolesummary.h"

class ITimer;
class RoleSummaryMgr
{
	RoleSummaryMgr();
	~RoleSummaryMgr();
	DECLARE_SINGLETON(RoleSummaryMgr)

public:
	bool Init();
	void Uninit();

	///> 本地变化通知MS
	void SendChangesToMS();

	///> 只支持查到当前GS上的角色
	RoleSummary* GetRoleSummary(UINT64 qwRoleID, bool isnew = false);

	void UpdateRoleSummary(Role *pRole, int mask, void* arg = NULL);

	void SendSingleRoleSummary(UINT64 roleid);
	void MarkToDelete(UINT64 roleid, bool isdelete);
	void RemoveRoleSummary(UINT64 roleid);

	UINT32 GetCount() { return m_oRoleSummarys.size(); }
	UINT32 GetQueueCount() { return m_queuechanges.size(); }

	///> rolesummary存db
	void SummaryToDBStored(RoleSummary* pSummary, KKSG::RoleSummaryStored& oDBSummary);
	///> db构造rolesummary
	void DBStoredToSummary(const KKSG::RoleSummaryStored* pData, RoleSummary* pSummary);

	inline std::unordered_map<UINT64, RoleSummary*>::iterator Begin() { return m_oRoleSummarys.begin(); }
	inline std::unordered_map<UINT64, RoleSummary*>::iterator End() { return m_oRoleSummarys.end(); }

private:
	void AddRoleSummary(RoleSummary* rolesummary);
	void MarkChanged(UINT64 roleid, UINT32 mask);

private:
	ITimer *m_pTimerHandler;

	std::queue<UINT64> m_queuechanges;
	std::unordered_map<UINT64, UINT32> m_ChangedRoleSummarys;
	std::unordered_map<UINT64, RoleSummary*> m_oRoleSummarys;
};

#endif // __ROLESUMMARYMGR_H__