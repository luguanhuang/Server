#ifndef __SELECTROLESTATEMGR_H__
#define __SELECTROLESTATEMGR_H__

enum SessionState
{
	SESSIONSTATE_NOT_EXIST,
	SESSIONSTATE_FREE,
	SESSIONSTATE_CREATE_ROLE,
	SESSIONSTATE_SELECT_ROLE,
	SESSIONSTATE_INGAME,
};

// һ���ͻ���Session���������󣬷�ֹ����ظ�����CreateRole��SelectRole��Ϣ
class SelectRoleStateMgr
{
	SelectRoleStateMgr();
	~SelectRoleStateMgr();
	DECLARE_SINGLETON(SelectRoleStateMgr)

public:

	bool Init();
	void Uninit();

	SessionState FindSessionState(UINT64 session);
	bool TryChangeState(UINT64 session, SessionState state);
	void SetSessionState(UINT64 session, SessionState state);
	void RemoveSessionState(UINT64 session);

	void Dump();

private:

	std::unordered_map<UINT64, SessionState> m_SessionStates;
};

#endif // __SELECTROLESTATEMGR_H__