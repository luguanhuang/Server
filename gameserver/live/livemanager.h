#ifndef __LIVEMANAGER_H__
#define __LIVEMANAGER_H__


class ITimer;
class Scene;
class Role;
class CProtocol;
class LiveInfo;
class LiveInfoList;

namespace KKSG
{
	class GetWatchInfoByIDRes;

	class OneLiveRecordInfo;

	class EnterWatchBattleRes;
}

class LiveManager : ITimer
{
	LiveManager();
	~LiveManager();
	DECLARE_SINGLETON(LiveManager)

public:
	bool Init();
	void Uninit();

	void OnTimer(UINT32 dwID, UINT32 dwCount);

	void Update(float deltaTime);

	LiveInfo* GetLiveInfo(int liveType, int sceneID, int teamID, int guildID, UINT64 roleID);

	LiveInfo* GetLiveInfo(int liveType, UINT32 liveID);

	LiveInfoList* GetLiveInfoList(int type);

	UINT32 Commend(Role* role);

	//create not need roles in scene
	LiveInfo* CreateLiveInfo(int liveType, int sceneID, std::vector<int>* teamID, std::vector<UINT64>* guildID, std::vector<UINT64>* roleIDs);

	void UpdateRecommendList(LiveInfo* info, bool isAdd);

	bool RemoveLiveInfo(int liveType, int sceneID, int teamID, int guildID, UINT64 roleID);

	UINT32 FillLiveInfo(int type, KKSG::GetWatchInfoByIDRes& roRes);

	//UINT32 EnterWatchBattle(Role* role, int type, int liveID);
	
	void OnEnterWatch(Role* role, UINT32 liveID);

	void LeaveWatchBattle(Role* role, UINT32 liveID);

	void NotifyWatchInfoChange(Scene* scene, LiveInfo* info);

	void EnterSceneNotifyWatchData(Role* role, Scene* scene);

	void SyncWatchDataToWatcher(Scene* scene, LiveInfo* info);

	void SyncInfoToMs(UINT32 liveID, bool isDelete, bool needSort = false);
	void SyncAllInfoToMs(UINT32 serverID = 0);

	void OnChange(UINT32 liveID);

	void OnLeave(Role* role, UINT32 liveID);

	void OnEnter(Role* role, UINT32 liveID);
	//void AddUpdateList(UINT32 liveID){}

	void SetMainHallLiveNum(UINT32 num){m_liveNumInMainHall = num;}
	UINT32 GetMainHallLiveNum(){return m_liveNumInMainHall;}
	void OnFirstEnterNotifyIconNum(Role* role);
protected:

	void AddLive(LiveInfo* info);
	void _SyncToMs(CProtocol& protocl, UINT32 serverID = 0);
	bool _CheckVisible(int liveType, int sceneID, std::vector<int>* teamID, std::vector<UINT64>* guildID, std::vector<UINT64>* roleIDs);

private:
	std::map<int, LiveInfoList*> m_liveList;
	HTIMER m_timer;

	std::set<UINT32> m_updateList;
	std::set<UINT32> m_deleteList;
	//temp process
	UINT32 m_liveNumInMainHall;
};

#endif // 
