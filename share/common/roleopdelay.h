#ifndef __ROLEOPDELAY_H__
#define __ROLEOPDELAY_H__

class RoleOpDelay
{
public:

	bool HaveRole(UINT64 roleID);
	void AddRole(UINT64 roleID, UINT32 delayEndTime);
	void DelRole(UINT64 roleID);
	//���ü������ ��������(û�м�ʱɾ��delay���ڵ����)
	void Update(UINT32 now);

private:
	std::unordered_map<UINT64, UINT32> m_roles;
};

#endif