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
	int UsedCount;//��ǰ�������ط�ʹ��(Ŀǰ���Ϊ1)
	int TotalUsed;//һ����ʹ���˶��ٴ�
	UINT32 LastOpTime;//��һ�ζ��������ʱ��(ʹ�� �黹)
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
	//tlog���Ը�ʽʱʹ��;
	Role* CreateTestRole();

	HTIMER m_handler;
};

#endif // __ROBOTGENERATOR_H__