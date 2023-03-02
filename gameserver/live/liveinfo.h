#ifndef __LIVEINFO_H__
#define __LIVEINFO_H__


class ITimer;
class Scene;
class Role;
class CProtocol;
namespace KKSG
{
	class GetWatchInfoByIDRes;

	class OneLiveRecordInfo;

	class EnterWatchBattleRes;
}

class LiveInfo
{
public:
	LiveInfo(int sceneID);

	virtual ~LiveInfo(){}

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual void SendLiveNotice();

	const std::list<Role*>* GetOnlineRoles();

	bool CheckVisible();

	bool operator < (const LiveInfo& info);

	bool OnEnterRole(Role* role);

	void LiveIconNtf(bool show);

	UINT32 GetSceneID(UINT32 liveID);


	void AddCommendNum(UINT32 num);

	void AddWatchNum(int num);
	
	UINT32 GetCommendNum(){return m_commendNum;}

	UINT32 GetWatchNum(){return m_watchNum;}

	UINT32 m_liveID;
	int m_liveType;
	int m_startTime;
	int m_watchNum;
	int m_commendNum;

	//int m_teamID;
	int m_sceneID;
	int m_sceneType;

	//给客户端显示的和存db的是不一样的
	std::map<UINT64, KKSG::RoleBriefInfo> m_roles;
};

template <class T>
class XSort
{
	virtual void Add(T* t);
protected:
	std::list<T*> m_list;
	int m_curNum;
	int m_maxNum;
};

class LiveInfoList// : XSort<LiveInfo>
{
public:
	LiveInfoList()
	{
		m_liveType = 0;
	}

	virtual ~LiveInfoList();

	LiveInfo* GetLiveInfo(UINT32 liveID);

	void Sort();

	void AddLiveInfo(LiveInfo* info);

	void BeforeRemove(LiveInfo* info);

	void RemoveLiveInfo(LiveInfo* info);

	std::list<LiveInfo*>* GetLiveList();
	
	UINT32 GetSceneID(UINT32 liveID);

	UINT32 FillInfo2Client(KKSG::GetWatchInfoByIDRes& roRes);

	void AddWatchNum(UINT32 liveID);

	void AddCommendNum(UINT32 liveID);

	UINT32 GetCommendNum(UINT32 liveID);

	UINT32 GetWatchNum(UINT32 liveID);

	void UpdateRoleLiveRecord(LiveInfo* info, Role* role);
protected:
	int m_liveType;
	std::list<LiveInfo*> m_liveInfo;
};

class TeamLiveInfo : public LiveInfo
{
public:

	TeamLiveInfo(int liveType, int sceneID, int expID, std::vector<UINT64>& roles);

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	//std::vector<UINT64> m_roles;
	int m_expID;
	UINT64 m_leaderID;
};


class NestLiveInfo : public TeamLiveInfo
{
public:
	NestLiveInfo(int sceneID, int expID, std::vector<UINT64>& roles);

	virtual void SendLiveNotice();
};

class NestLiveList : public LiveInfoList
{
public:
	NestLiveList();
};

class DragonLiveInfo : public TeamLiveInfo
{
public:
	DragonLiveInfo(int sceneID, int expID, std::vector<UINT64>& roles);

	virtual void SendLiveNotice();
};

class DragonLiveList : public LiveInfoList
{
public:
	DragonLiveList();
};

class RecommendLiveList : public LiveInfoList
{
public:
	RecommendLiveList();
};

class PVPLiveInfo : public LiveInfo
{
public:
	PVPLiveInfo(UINT32 sceneID, UINT64 role1ID, UINT64 role2ID);

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual void SendLiveNotice();

	UINT64 m_roleID1;
	UINT64 m_roleID2;
	UINT32 m_pkPoint;
};

class PVPLiveList : public LiveInfoList
{
public:
	PVPLiveList();
};

class PVP2LiveInfo : public LiveInfo
{
public:
	PVP2LiveInfo(UINT32 sceneID, UINT32 score);

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual void SendLiveNotice();

	UINT32 m_pkPoint;
};

class PVP2LiveList : public LiveInfoList
{
public:
	PVP2LiveList();
};

class ProtectCaptainLiveInfo : public LiveInfo
{
public:
	ProtectCaptainLiveInfo(UINT32 sceneID);

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual void SendLiveNotice();
};

class ProtectCaptainLiveList : public LiveInfoList
{
public:
	ProtectCaptainLiveList();
};

class GuildBattleLiveInfo : public LiveInfo
{
public:
	GuildBattleLiveInfo(UINT32 sceneID, UINT64 guildID1, UINT64 guildID2, UINT32 curLevel);

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual void SendLiveNotice();

private:
	bool _Convert2OneLiveRecord(KKSG::OneLiveRecordInfo* info);

public:
	UINT64 m_guildID1;
	UINT64 m_guildID2;
	UINT32 m_curLevel;
};

class GuildBattleLiveList : public LiveInfoList
{
public:
	GuildBattleLiveList();
};

class HeroBattleLiveInfo : public LiveInfo
{
public:
	HeroBattleLiveInfo(UINT32 sceneID);

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual void SendLiveNotice();
};

class HeroBattleLiveList : public LiveInfoList
{
public:
	HeroBattleLiveList();
};

class LeagueBattleLiveInfo : public LiveInfo
{
public:
	LeagueBattleLiveInfo(UINT32 sceneID);

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual void SendLiveNotice();
private:
	bool _Convert2OneLiveRecord(KKSG::OneLiveRecordInfo* info);

private:
	std::string m_leagueName1;
	std::string m_leagueName2;
};

class LeagueBattleLiveList : public LiveInfoList
{
public:
	LeagueBattleLiveList();
};

class CustomPkLiveInfo : public LiveInfo
{
public:
	CustomPkLiveInfo(UINT32 sceneID);

	virtual bool Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual bool Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info);

	virtual void SendLiveNotice();

	UINT32 m_pkPoint;
};

class CustomPkLiveList : public LiveInfoList
{
public:
	CustomPkLiveList();
};


#endif // 
