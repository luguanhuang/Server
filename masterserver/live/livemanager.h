#ifndef __LIVEMANAGER_H__
#define __LIVEMANAGER_H__

#define CROSS_LIVEID_GSLINE_OFFSET 100

class ITimer;
class Scene;
class CRole;
namespace KKSG
{
	class OneLiveRecordInfo;

	class GetWatchInfoByTypeRes;
}

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

	virtual void Sort();

	KKSG::OneLiveRecordInfo* GetLiveInfo(UINT32 liveID);

	void AddLiveInfo(KKSG::OneLiveRecordInfo* info);

	void RemoveLiveInfo(KKSG::OneLiveRecordInfo* info);

	std::list<KKSG::OneLiveRecordInfo*>* GetLiveList();
	
	UINT32 GetSceneID(UINT32 liveID);

	virtual UINT32 FillInfo2Client(CRole* role, KKSG::GetWatchInfoByTypeRes& roRes);

	bool HasFriend(CRole* role, KKSG::OneLiveRecordInfo& info);

	bool HasGuild(CRole* role, KKSG::OneLiveRecordInfo& info);

protected:
	int m_liveType;
	std::list<KKSG::OneLiveRecordInfo*> m_liveInfo;
};

class NestLiveList : public LiveInfoList
{
public:
	NestLiveList();

	virtual void Sort();
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

class FriendLiveList : public LiveInfoList
{
public:
	FriendLiveList();

	virtual UINT32 FillInfo2Client(CRole* role, KKSG::GetWatchInfoByTypeRes& roRes);
};

class GuildLiveList : public LiveInfoList
{
public:
	GuildLiveList();

	virtual UINT32 FillInfo2Client(CRole* role, KKSG::GetWatchInfoByTypeRes& roRes);
};

class FriendAndGuildLiveList : public LiveInfoList
{
public:
	FriendAndGuildLiveList();

	virtual UINT32 FillInfo2Client(CRole* role, KKSG::GetWatchInfoByTypeRes& roRes);
};

class PVPLiveList : public LiveInfoList
{
public:
	PVPLiveList();
};

class PVP2LiveList : public LiveInfoList
{
public:
	PVP2LiveList();
};

class ProtectCaptainLiveList : public LiveInfoList
{
public:
	ProtectCaptainLiveList();
};

class GuildBattleLiveList : public LiveInfoList
{
public:
	GuildBattleLiveList();
};

class HeroBattleLiveList : public LiveInfoList
{
public:
	HeroBattleLiveList();
};

class LeagueBattleLiveList : public LiveInfoList
{
public:
	LeagueBattleLiveList();
};

class CustomPkLiveList : public LiveInfoList
{
public:
	CustomPkLiveList();
};


class LiveManager : ITimer
{
	LiveManager();
	~LiveManager();
	DECLARE_SINGLETON(LiveManager)

public:
	bool Init();
	void Uninit();

	bool IsCrossInfo(UINT32 liveID, UINT32* gsLine=NULL);
	void OnCrossGSClose(UINT32 gsLine, bool isAll = false);

	void OnTimer(UINT32 dwID, UINT32 dwCount);

	KKSG::OneLiveRecordInfo* GetLiveInfo(int liveType, int sceneID, int teamID, int guildID, UINT64 roleID);

	KKSG::OneLiveRecordInfo* GetLiveInfo(int liveType, UINT32 liveID);

	LiveInfoList* GetLiveInfoList(int type);

	KKSG::OneLiveRecordInfo* CreateLiveInfo(KKSG::OneLiveRecordInfo* info);

	void UpdateRecommendList(KKSG::OneLiveRecordInfo* info, bool isAdd);

	bool RemoveLiveInfo(int liveType, int sceneID, int teamID, int guildID, UINT64 roleID);

	UINT32 FillLiveInfo(CRole* role, int type, KKSG::GetWatchInfoByTypeRes& roRes);

	UINT32 EnterWatchBattle(/*CRole* role, */int type, UINT32 liveID, UINT32& sceneID, UINT32& sceneTemplateID);
	
	//void OnEnterWatch(CRole* role, int liveID);

	//void LeaveWatchBattle(CRole* role, int liveID);

	//void NotifyWatchInfoChange(Scene* scene, KKSG::OneLiveRecordInfo* info);

	void SyncInfoFromGS(const KKSG::OneLiveRecordInfo* info, UINT32 deleteID, UINT32 sceneID, bool needSort);

	void AllSyncSort();

protected:

	void AddLive(const KKSG::OneLiveRecordInfo* info, UINT32 sceneID, bool needSort);

	void RemoveLive(UINT32 liveID);

	void _SyncNumToGs();
	void _SyncNumToGs(UINT32 num);
private:

	std::map<int, LiveInfoList*> m_liveList;
	HTIMER m_timer;
};

#endif // 
