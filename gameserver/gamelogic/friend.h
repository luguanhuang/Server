//#ifndef __FRIEND_H__
//#define __FRIEND_H__
//
//#include "utility/XCommon.h"
//#include "gamelogic/globalconfig.h"
//
//#define MAX_FRIEND_COUNT		(GlobalConfig::FriendMaxCount)
//#define MAX_BLACK_COUNT			(GlobalConfig::BlackMaxCount)
//#define MAX_APPLYFRIEND_COUNT	(GlobalConfig::ApplyMaxCount)
//#define MAX_DEGREE		(GlobalConfig::FriendMaxDegree)
//#define MAX_GIFTDEGREE	(GlobalConfig::GiftSendMinDegree)
//#define MAX_GIFTRECEIVE	(GlobalConfig::GiftReceiveMaxTimes)
//#define MAX_GIFTSEND	(GlobalConfig::GiftReceiveMaxTimes)
//
//namespace KKSG
//{
//	class Friend2Client;
//	class SocialInfo2Db;
//}
//enum FriendErr
//{
//	FriendSuccess,
//
//	FriendMax,
//	FriendRepeated,
//	FriendNotExist,
//
//	BlackSuccess,
//	
//	InSelfBlackList,
//	InOtherBlackList,
//	BlackNotExist,
//	BlackMax,
//};
//
//struct FriendInfo
//{
//	UINT32 m_degree;
//	UINT32 m_daydegree;
//	int m_updatetime;
//	std::map<UINT32, UINT32> m_degreetimes;
//
//	UINT32 m_receiveall;
//	UINT32 m_receive;
//	UINT32 m_receivetime;
//	UINT32 m_send;
//
//	bool Update()
//	{
//		int today = XCommon::GetTodayUnique();
//		if (m_updatetime != today)	
//		{
//			m_degreetimes.clear();
//			m_daydegree = 0;
//			m_updatetime = today;
//			m_receive = 0;
//			m_receivetime = 0;
//			m_send = 0;
//			return true;
//		}
//		return false;
//	}
//	
//	bool Limit(UINT32 type, UINT32 count)
//	{
//		auto i = m_degreetimes.find(type);
//		if (i == m_degreetimes.end())
//		{
//			m_degreetimes[type] = 1;
//			return false;
//		}
//		if (0 != count && count <= i->second)
//		{
//			return true;
//		}
//		++i->second;
//		return false;
//	}
//
//	UINT32 Add(UINT32 degree)
//	{
//		m_daydegree += degree;
//		if (m_degree + degree >= MAX_DEGREE)
//		{
//			degree = MAX_DEGREE > m_degree ? MAX_DEGREE - m_degree : 0;
//		}
//		m_degree += degree;
//		return degree;
//	}
//
//	FriendInfo()
//	{
//		m_degree = 0;
//		m_updatetime = 0;
//		m_receive = 0;
//		m_send = 0;
//		m_receiveall = 0;
//	}
//};
//
//struct FriendApplyInfo
//{
//	UINT64 roleid;
//	UINT32 timestamp;
//
//	FriendApplyInfo()
//	{
//		roleid = 0;
//		timestamp = 0;
//	}
//};
//
//struct Friend
//{
//	bool m_ischanged;
//	std::map<UINT64, FriendInfo> m_friends;
//	// friends deleted 
//	//std::deque<UINT64> m_friendsdeleted;	
//	std::deque<FriendApplyInfo> m_applys;
//	std::deque<UINT64> m_blacklist;
//
//
//	Friend()
//	{
//		m_ischanged = false;
//	}
//
//	bool SaveToDb(KKSG::SocialInfo2Db* socialinfo2db);
//	bool LoadFromDb(const KKSG::SocialInfo2Db* socialinfo2db);
//
//	bool InApply(UINT64 otherid);
//	bool AddApply(UINT64 otherid, UINT64& deleted);
//	bool DeleteApply(UINT64 otherid);
//
//	UINT32 CanAddFriend(UINT64 friendroleid);
//	UINT32 AddFriend(UINT64 friendroleid);
//	UINT32 RemoveFriend(UINT64 friendroleid);
//	inline UINT32 GetFriendCount() { return (UINT32)m_friends.size(); }
//	inline bool IsFriend(UINT64 friendroleid) { return m_friends.end() != m_friends.find(friendroleid); }
//
//	UINT32 GetDegree(UINT64 friendid);
//	UINT32 GetDayDegree(UINT64 friendid);
//	UINT32 AddDegree(UINT64 friendid, UINT32 degree, UINT32 type, UINT32 countlimit, bool islimit = true);
//
//	UINT32 GetReceiveAll(UINT64 otherid);
//	UINT32 GetReceiveTime(UINT64 otherid);
//	UINT32 GetReceive(UINT64 otherid);
//	UINT32 GetSend(UINT64 otherid);
//	bool SetReceive(UINT64 otherid, UINT32 state);
//	bool SetSend(UINT64 otherid, UINT32 state);
//
//	bool IsBlack(UINT64 otherroleid);
//	UINT32 AddBlackList(UINT64 otherroleid);
//	UINT32 RemoveBlackList(UINT64 otherroleid);
//
//	void ToFriendList(std::vector<UINT64>& ids);
//	void ToApplyList(std::vector<UINT64>& ids);
//
//	void ShowBlackList( std::string& str );
//
//	static bool Friend2Client(UINT64 selfid, UINT64 otherid, KKSG::Friend2Client* friend2client);
//};
//
//#endif