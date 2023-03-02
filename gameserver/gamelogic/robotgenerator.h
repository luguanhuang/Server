#ifndef __ROBOTGENERATOR_H__
#define __ROBOTGENERATOR_H__

#include "robotconfig.h"
#include "robotcreatestep.h"
#include "timer.h"
#include <iosfwd>

class Role;
class RoleSummary;

struct RobotUsedInfo
{
	int UsedCount;//当前被几个地方使用(目前最多为1)
	int TotalUsed;//一共被使用了多少次
	UINT32 LastOpTime;//上一次对其操作的时间(使用 归还)
};

class RobotGenerator : public ITimer
{
	RobotGenerator();
	~RobotGenerator();
	DECLARE_SINGLETON(RobotGenerator)

public:

	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	RoleSummary *CreateRobotSummary(RobotConf *pConf);
	void DebugTransforRobotConfToRole(Role *pRole, RobotConf *pConf);
	void FreeRoleSummary(RoleSummary *pSummary);
	void UseRobotSummary(RoleSummary *pSummary);

	inline std::unordered_map<RoleSummary *, RobotUsedInfo>::iterator Begin() { return m_RobotLibrary.begin(); }
	inline std::unordered_map<RoleSummary *, RobotUsedInfo>::iterator End() { return m_RobotLibrary.end(); }


	void DebugDumpLibrary(std::ostream &oss);

	friend class TLogMgr;
private:

	std::vector<IRobotGenStep*> m_CreateRobotStep;
	std::unordered_map<RoleSummary *, RobotUsedInfo> m_RobotLibrary;
	//tlog测试格式时使用;
	Role* CreateTestRole();

	HTIMER m_handler;
};

#endif // __ROBOTGENERATOR_H__