#ifndef __TEAMINVITEMGR_H__
#define __TEAMINVITEMGR_H__

#include <time.h>
#include "timer.h"
#include "table/ExpeditionTable.h"
#include "pb/enum.pb.h"

class Role;

class InviteHistory
{
public:
	InviteHistory(UINT32 delTime);
	void Update();
	void AddOneInvHistory(UINT64 roleID);
	bool InInvHistory(UINT64 roleID);
	int GetHistoryCount(){ return (int)m_history.size(); }

	void TestPrint();

private:
	UINT32 m_delTime;
	std::unordered_map<UINT64, time_t> m_history;
};

namespace KKSG
{
	class TeamInviteRes;
	class TeamInvite;
	class InvHistoryRes;
	class TeamInvRoleInfo;
}

class TeamInviteMgr : public ITimer
{
	TeamInviteMgr();
	~TeamInviteMgr();
	DECLARE_SINGLETON(TeamInviteMgr)

public:

	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void FillInviteList(Role* pRole, int expid, KKSG::TeamInviteRes &roRes);
	void AddInvToHistory(UINT64 invFromID, UINT64 invToID);
	KKSG::TeamInvRoleState GetInviteRoleState(Role* pRole, Role* inRole, ExpeditionTable::RowData *pRow);

	void TestPrint();

private:

	bool IsInInvToHistory(UINT64 invFromID, UINT64 invToID);

	void FillInviteFriend(Role* pRole, ExpeditionTable::RowData *pRow, KKSG::TeamInviteRes &roRes, std::unordered_set<UINT64>& recRole);
	void FillInviteGuild(Role* pRole, ExpeditionTable::RowData *pRow, KKSG::TeamInviteRes &roRes, std::unordered_set<UINT64>& recRole);
	void FillInviteRec(Role* pRole, ExpeditionTable::RowData *pRow, KKSG::TeamInviteRes &roRes, std::unordered_set<UINT64>& recRole);

	HTIMER m_handler;
	std::unordered_map<UINT64, InviteHistory> m_invToHistory;//ÑûÇëÁËË­(Ä°ÉúÈË)

};

#endif // __TEAMINVITEMGR_H__