#ifndef __PLATFORMFRIENDMANAGER_H__
#define __PLATFORMFRIENDMANAGER_H__

#include "platforminfofunctor.h"
#include "lructrl.h"

class ITimer;
class Scene;
class CRole;

namespace KKSG
{
	class ReqPlatFriendRankListRes;
	class SyncPlatFriend2MSData;
	class M2SReqPlatUserInfoRes; 
	class S2MSendGift2PlatFriendArg;
	class PlatFriendsInfo2DB;
}

#define REQCS_PLATFRIENDINFO_TIMEUNITCOUNT 1
#define REQCS_PLATFRIENDINFO_TIMEUNIT 120
#define REQCS_PLATFRIENDINFO_MAXSIZE 200
#define SYNC_PLATFRIENDLIST_FROMCLIENT_TIME 120
#define SYNC_PLATSELFINFO_TOCS_TIME 1
#define ONEDAY_SENDPLATFRIEND_MAXSIZE 500

struct SPlatUserDetailInfo
{
	SPlatUserDetailInfo()
	{
		maxAbility = 0;
		isOnline = false;
		viplevel = 0;
		level = 0;
		updatetime = 0;
		sendGiftTime = 0;
		startType = KKSG::StartUp_Normal;
		profession = KKSG::Role_Warrior;
	}
	UINT32 maxAbility;
	bool isOnline;
	UINT32 viplevel;
	UINT32 level;
	UINT32 updatetime;
	UINT32 sendGiftTime;
	int startType;
	int profession;
};

struct PlatUserInfo
{
	PlatUserInfo()
	{
		platformID = 0;
		serverID = 0;
		roleID = 0;
		detail = NULL;
	}
	PlatUserInfo(const PlatUserInfo& a);
	PlatUserInfo& operator = (const PlatUserInfo& a);
	~PlatUserInfo();
	SPlatUserDetailInfo* CreateDetail();
	SPlatUserDetailInfo* GetDetail()const{return detail;}
	void RemoveDetail();

	bool operator < (const PlatUserInfo& b);

	void InitFromRole(CRole* role);

	bool ConvertFromCSKKSGPlatUserAllInfo(const KKSG::PlatUserAllInfo& allInfo);
	bool Convert2CSKKSGPlatUserAllInfo(KKSG::PlatUserAllInfo& allInfo);
	bool ConvertFromClientKKSGPlatFriend(const KKSG::PlatFriend& platFriendInfo);
	bool Convert2KKSGClient(KKSG::PlatFriendRankInfo2Client& info);

	bool Load(const KKSG::PlatUserAllInfo2DB& allInfo);
	void Save(KKSG::PlatUserAllInfo2DB& allInfo);
	
	UINT32 platformID;
	std::string openid;
	std::string nickname;
	std::string bigpic;
	std::string roleName;

	UINT32 serverID;
	UINT64 roleID;

private:
	SPlatUserDetailInfo* detail;
};

struct PlatFriendList
{
	PlatFriendList()
	{
		m_clientReqTime = 0;
		m_updateTimeFromClient = 0;
		m_needSyncSelfInfo = true;
		m_needSyncFriendInfo = true;
		m_syncSelfInfo2CSTime = 0;
		m_todaySendGiftCount = 0;
		m_lastlogtime = 0;
		//m_isOnlyTimeChange = false;
	}

	bool LoadFromDb(const std::string& info);
	void Save2Db(KKSG::PlatFriendsInfo2DB* info);

	bool SyncPlatSelfInfoFromClient(CRole* role, const KKSG::SyncPlatFriend2MSData& data);
	bool SyncPlatSelfInfo2CS(CRole* role);

	bool SyncPlatFriendListFromClient(const KKSG::SyncPlatFriend2MSData& data);
	bool SyncPlatFriendListFromCS(const KKSG::M2SReqPlatUserInfoRes& data);

	void OnLoginAndOutSyncFromCS(const KKSG::M2SLoginAndoutSyncRes& roRes);
	void OnLogin(CRole* role);
	void OnLogout(CRole* role);

	void DelDetailInfoFromCache();

	bool IsFirstSync();
	bool CheckSyncTimeFromClient(int now);

	UINT32 GetPlatFriendRankList2Client(CRole* role,KKSG::ReqPlatFriendRankListRes& roRes);

	UINT32 GiveGift2PlatFriend(CRole* role, const std::string& destOpenID);
	bool GetPlatFriendUserAllInfo(const std::string& openid, KKSG::PlatUserAllInfo& allInfo);

	void InitSelfInfo(CRole* role);
	void SetOpenID(const std::string& openid);

	void GetPlatFriends(std::vector<std::string>& friends);
	const PlatUserInfo& GetSelfInfo();
	const PlatUserInfo* GetPlatFriendUserInfo(const std::string& openid);

	void DebugSetClientReqTime(int time);
	void DoLog();
private:
	UINT32 m_clientReqTime;
	UINT32 m_updateTimeFromClient;
	
	UINT32 m_todaySendGiftCount;

	bool m_needSyncSelfInfo;
	bool m_needSyncFriendInfo;
	UINT32 m_syncSelfInfo2CSTime;
	time_t m_lastlogtime;//最后一次记录tlog的时间;
	PlatUserInfo m_selfInfo;
	std::unordered_map<std::string, PlatUserInfo> m_platFriendList;
	//std::unordered_map<std::string, PlatUserInfo> m_platFriendAddList;
};

class CPlatformFriendMgr : public ITimer
{
	CPlatformFriendMgr();
	~CPlatformFriendMgr();
	DECLARE_SINGLETON(CPlatformFriendMgr)

public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool LoadFromDB();
	void Save2DB();
	void AddChangedInfo(const std::string& openid);

	PlatFriendList* GetPlatFriendList(CRole* role);
	PlatFriendList* GetPlatFriendListByOpenID(const std::string& openid);
	const PlatUserInfo* GetPlatSelfInfo(const std::string& openid);

	void SyncPlatFriendListFromClient(CRole* role, KKSG::SyncPlatFriend2MSData& data);
	void SyncPlatFriendListFromCSAndFillRank(CRole* role, KKSG::M2SReqPlatUserInfoRes& data, KKSG::ReqPlatFriendRankListRes &roRes);
	void LoginAndOutSyncFromCS(CRole* role, const KKSG::M2SLoginAndoutSyncRes& roRes);
	void OnLoginReceiveDetailPlatInfoFromCs(const std::string& openid);
	//需要调用
	void OnLoginOrOut(CRole* role, bool isLogin);

	void AddClientReq2CS(const std::string& openid);
	void RemoveClientReq2CS(const std::string& openid);
	bool IsClientReq2CSFull();

	UINT32 GetPlatFriendRankList2Client(CRole* role, KKSG::ReqPlatFriendRankListRes& roRes);
	UINT32 GiveGift2PlatFriend(CRole* role, const std::string& destOpenID);
	void ReceivePlatFriendGift(const KKSG::S2MSendGift2PlatFriendArg& roArg);

	bool IsPlatFriend(const std::string& srcOpenID, const std::string destOpenID);

	void DebugSyncPlatSelf(CRole* role);

	void QueryDetailInfoFromCS(CRole* role);

	void DelRolePlatDetailInfo(std::string& account);

	typedef CLruControl<std::string, CPlatFormInfoAddFunctor, CPlatFormInfoDelFunctor>	CRolePlatInfoCacheCtrl;

private:
	void _SyncUserStatus2CS(CRole* role, bool isLogin);

private:
	std::unordered_map<std::string, PlatFriendList> m_platFriendInfo;
	std::unordered_set<std::string> m_changedlist;

	std::unordered_map<std::string, UINT32> m_reqCSList;

	CRolePlatInfoCacheCtrl m_cachedDetailPlatInfo;
};

#endif //
