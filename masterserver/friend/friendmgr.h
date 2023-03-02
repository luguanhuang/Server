#ifndef __FRIENDMGR_H__
#define __FRIENDMGR_H__

#include "timer.h"

struct Friend;
class FriendMgr : public ITimer
{
	FriendMgr();
	~FriendMgr();
	DECLARE_SINGLETON(FriendMgr);
public:
	bool Init();
	void Uninit();

	Friend* GetFriendInfo(UINT64 uid);
	void AddFriendInfo(UINT64 uid, Friend* friend_);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void SendNoNotifyAddFriend(UINT64 uid);

	bool IsExpire(UINT64 uid);

private:
	HTIMER m_timehandle;
	std::unordered_map<UINT64, Friend*> m_id2friend;
};

#endif