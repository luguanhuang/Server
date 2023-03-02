#ifndef __FRIEND_H__
#define __FRIEND_H__

#include "util/XCommon.h"
#include "table/globalconfig.h"
#include "friendrecord.h"

namespace KKSG
{
	class Friend2Client;
	class Friend2Db;
}

struct FriendInfo
{
	UINT64 m_roleid;
	UINT32 m_degree;
	UINT32 m_daydegree;
	int m_updatetime;
	std::map<UINT32, UINT32> m_degreetimes;

	UINT32 m_receiveall;
	UINT32 m_receive;
	UINT32 m_receivetime;
	UINT32 m_send;

	bool Update();
	bool Limit(UINT32 type, UINT32 count);
	UINT32 Add(UINT32 degree);

	void Reset()
	{
		m_roleid = 0;
		m_degree = 0;
		m_updatetime = 0;
		m_receive = 0;
		m_send = 0;
		m_receiveall = 0;
	}

	FriendInfo()
	{
		Reset();
	}
	FriendInfo(UINT64 _roleid)
	{
		Reset();
		m_roleid = _roleid;
	}

	bool operator < (const FriendInfo& info) const
	{
		if (m_degree > info.m_degree)
		{
			return true;
		}
		else if (m_degree == info.m_degree)
		{
			return m_roleid < info.m_roleid;
		}
		else
		{
			return false;
		}
	}
};

struct FriendApplyInfo
{
	UINT64 roleid;
	UINT32 timestamp;

	FriendApplyInfo()
	{
		roleid = 0;
		timestamp = 0;
	}
};

struct Friend
{
	bool m_ischanged;
	
	CFriendRecord m_record;	
	std::map<UINT64, FriendInfo> m_friends;
	std::deque<FriendApplyInfo> m_applys;

	std::deque<UINT64> m_blacklist;

	std::vector<UINT64> m_noNotifyAddFriend;

	Friend()
	{
		m_ischanged = false;
	}

	void Changed() { m_ischanged = true; }
	bool IsChanged() { return m_ischanged; }

	bool SaveToDb(KKSG::Friend2Db& friend2db);
	bool LoadFromDb(const KKSG::Friend2Db& friend2db);

	bool InApply(UINT64 otherid);
	bool AddApply(UINT64 otherid, UINT64& deleted);
	bool DeleteApply(UINT64 otherid);

	UINT32 CanAddFriend(UINT64 friendroleid);
	UINT32 AddFriend(UINT64 friendroleid);
	UINT32 RemoveFriend(UINT64 friendroleid);
	inline UINT32 GetFriendCount() { return (UINT32)m_friends.size(); }
	inline bool IsFriend(UINT64 friendroleid) { return m_friends.end() != m_friends.find(friendroleid); }

	UINT32 GetDegree(UINT64 friendid);
	UINT32 GetDayDegree(UINT64 friendid);
	UINT32 AddDegree(UINT64 selfid, UINT64 friendid, UINT32 degree, UINT32 type, UINT32 countlimit, bool islimit = true);
	void CleanDegree(UINT64 friendid);

	UINT32 GetReceiveAll(UINT64 otherid);
	UINT32 GetReceiveTime(UINT64 otherid);
	UINT32 GetReceive(UINT64 otherid);
	UINT32 GetSend(UINT64 otherid);
	bool SetReceive(UINT64 otherid, UINT32 state);
	bool SetSend(UINT64 otherid, UINT32 state);

	bool IsBlack(UINT64 otherroleid);
	UINT32 AddBlackList(UINT64 otherroleid);
	UINT32 RemoveBlackList(UINT64 otherroleid);

	void ToFriendList(std::vector<UINT64>& ids);
	void ToApplyList(std::vector<UINT64>& ids);

	void AddNoNotifyAddFriend(UINT64 otherid);
	void ClearNoNotifyAddFriend();

	static bool Friend2Client(UINT64 selfid, UINT64 otherid, KKSG::Friend2Client* friend2client);

	void OnAddDegree(UINT64 selfID, UINT64 friendID, UINT32 degree);
};

#endif