#ifndef __TEAMINVITEMGR_H__
#define __TEAMINVITEMGR_H__

#include <time.h>
#include "timer.h"
#include "table/ExpeditionTable.h"
#include "pb/enum.pb.h"
#include "pb/project.pb.h"

class CRole;

class TeamInviteRec
{
public:
	TeamInviteRec(const KKSG::TeamInvite& data);
	~TeamInviteRec();

	UINT32 GetInviteID() { return m_data.inviteid(); }
	UINT32 GetPassTime(UINT32 now);

	int GetTeamID() { return m_data.teambrief().teamid(); }

	UINT64 GetInvFromID() { return m_data.invfromroleid(); }
	UINT64 GetInvToID() { return m_data.invtoroleid(); }

	const KKSG::TeamInvite& Data() const { return m_data; }

private:
	KKSG::TeamInvite m_data;//信息全在这里面(尽量减少里面的信息)
};

class TeamInviteMgr : public ITimer
{
	TeamInviteMgr();
	~TeamInviteMgr();
	DECLARE_SINGLETON(TeamInviteMgr)

public:

	bool Init();
	void Uninit();

	void MakeInvite(KKSG::TeamInvite& data);
	void OnAck(CRole* pRole, UINT32 inviteID, bool accept);
	bool IsInInviting(UINT64 fromID, UINT64 toID);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void AddUnfRefuseForNow(UINT64 roleID);

	void DelInvToMe(UINT64 invToID);
	void FillAllInvToMe(UINT64 invToID, KKSG::InvHistoryRes& roRes);

	void DelInvByTeam(int teamID);

	void RefuseNtf(UINT64 roleID, const std::string& name);

	void TestPrint();
	void ClearAll();

private:
	UINT32 NewInviteID();
	bool IsInRefuseForNow(UINT64 roleID);
	void FillInviteData(KKSG::TeamInvite& data, TeamInviteRec& rec);
	void DelInvNtf(int inviteID, UINT64 invToID);

	std::unordered_map<UINT32, TeamInviteRec> m_invs;
	std::unordered_map<UINT64, time_t> m_refuseForNow;//暂不接受 陌生邀请

	HTIMER m_handler;
};

#endif // __TEAMINVITEMGR_H__