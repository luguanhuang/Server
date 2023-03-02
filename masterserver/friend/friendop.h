#ifndef __FRIENDOP_H__
#define __FRINEDOP_H__

#include "friend.h"
#include "friend/ptcm2c_friendopntfnew.h"
#include "friend/ptcm2g_updatefriendongs.h"

namespace KKSG
{
	class FriendList;
}
struct FriendNotify
{
	UINT64 roleid;
	KKSG::FriendOpType type;
	PtcM2C_FriendOpNtfNew ntf;

	bool isnotifygs;
	PtcM2G_UpdateFriendOnGS gsntf;

	FriendNotify(UINT64 _roleid)
	:roleid(_roleid)
	,isnotifygs(false)
	{
	}
	void PackCount(UINT32 send, UINT32 left);
	void Pack(UINT32 type, UINT64 id);
	void Pack(UINT32 type, const std::vector<UINT64>& ids);
	void Send();
};

class CRole;
struct Friend;
struct FriendInfo;
class FriendOp
{
public:
	FriendOp(UINT64 roleid);
	virtual ~FriendOp();

	void SetOther(UINT64 otherid);

	CFriendRecord* GetRecord() { return &m_self->m_record; }

	void NotifyFriend();
	void NotifyApply();
	void NotifyGiftCount();
	void NotifyGS(CRole* role);

	bool IsInvalid() { return m_invalid; }

	// friend
	bool IsFriendMax();
	bool IsFriend(UINT64 otherid);
	UINT32 AddApply();
	bool IsInApply();
	bool DeleteApply(UINT64 otherid);
	void DeleteApplyBoth();
	UINT32 CanAddFriend();
	UINT32 AddFriend();
	UINT32 RemoveFriend();
	UINT32 GetFriendCount() const;
	void FillFriendList(KKSG::FriendList* list);
	const std::map<UINT64, FriendInfo>* GetFriendList();
	void GetFriendListSorted(std::set<FriendInfo>& friendlist);

	// degree
	UINT32 GetDegree(UINT64 otherid);
	UINT32 GetDayDegree(UINT64 otherid);
	UINT32 AddDegree(UINT32 type, UINT32 degree = 0, UINT32 param = 1);
    void GoalAwards(UINT64 xroleid, Friend* xfriend);
	void CleanDegree();

	// gift
	UINT32 GetReceiveAll(UINT64 otherid);
	UINT32 GetReceiveTime(UINT64 otherid);
	UINT32 GetReceive(UINT64 otherid);
	UINT32 GetSend(UINT64 otherid);
	bool SetReceive(UINT64 otherid, UINT32 state);
	bool SetSend(UINT64 otherid, UINT32 state);

	// blacklist
	bool IsInBlackList(UINT64 otherid);
	bool IsInOtherBlackList(UINT64 roleid);
	UINT32 AddBlackList();
	UINT32 RemoveBlackList(UINT64 otherid);
	void SendBlackList(CRole* role);
	void DoTxLog(UINT64 roleid, UINT64 otherid, INT32 Op);

private:
	bool m_invalid;
	UINT64 m_roleid;
	UINT64 m_otherid;
	Friend* m_self;
	Friend* m_other;
};

#endif
