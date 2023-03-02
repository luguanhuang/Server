#ifndef __ROLEOPDELAY_H__
#define __ROLEOPDELAY_H__

class RoleOpDelay
{
public:

	bool HaveRole(UINT64 roleID);
	void AddRole(UINT64 roleID, UINT32 delayEndTime);
	void DelRole(UINT64 roleID);
	//调用间隔过长 会产生误差(没有及时删除delay到期的玩家)
	void Update(UINT32 now);

private:
	std::unordered_map<UINT64, UINT32> m_roles;
};

#endif