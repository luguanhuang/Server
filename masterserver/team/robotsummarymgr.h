#ifndef __ROBOTSUMMARYMGR_H__
#define __ROBOTSUMMARYMGR_H__

#include "timer.h"

namespace KKSG
{
	class RoleSummaryStored;
	class MSRobotSumamryRes;
}

class CRoleSummary;
class CRole;
class RobotSummary
{
public:
	RobotSummary(UINT64 roleID, int lookupid, UINT64 robotid, const KKSG::RoleSummaryStored& data);
	~RobotSummary();
	
	UINT64 GetRoleID(){ return m_roleID; }
	int GetLookUpID(){ return m_lookupid; }
	UINT64 GetRobotID(){ return m_robotID; }
	const KKSG::RoleSummaryStored& GetData() { return m_data; }

	time_t GetCreateTime(){ return m_createTime; }

private:
	UINT64 m_roleID;
	time_t m_createTime;
	int m_lookupid;
	UINT64 m_robotID;
	KKSG::RoleSummaryStored m_data;
};

class RobotSummaryMgr : public ITimer
{
	RobotSummaryMgr();
	~RobotSummaryMgr();
	DECLARE_SINGLETON(RobotSummaryMgr)

public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool HaveCanUseRobotSummary(UINT64 roleID, int lookupid);
	void RegisterRobotSummary(UINT64 roleID, int lookupid);

	void AddRobotSummary(UINT64 roleID, int lookupid, const KKSG::MSRobotSumamryRes& data);
	RobotSummary* GetRobotSummary(UINT64 roleID, int lookupid);
	void FreeRobotSummary(UINT64 roleID);

private:

	std::unordered_map<UINT64, RobotSummary*> m_robotSummarys;
	std::unordered_map<UINT64, time_t> m_registerRoles;

	HTIMER m_handler;
};

#endif