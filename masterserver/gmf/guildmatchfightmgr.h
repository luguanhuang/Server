#ifndef __GUILDMATCHFIGHTMGR_H__
#define __GUILDMATCHFIGHTMGR_H__

#include "timer.h"

enum GMFRoomType
{
	GMF_RT_GPR = 1,//积分赛
	GMF_RT_GMF = 2,//巅峰对决赛
};

namespace KKSG
{
	class GmfUpdateData;
	class GmfRoom;
}

class Scene;
class CGuildMFRoom
{
public:
	CGuildMFRoom(UINT64 guild11, UINT64 guild22, UINT32 warType, UINT32 battleId, GMFRoomType type);

	UINT64 GetUniqueID();
	time_t GetCreateTime() { return m_createTime; }

	void SaveData(KKSG::GmfRoom& data);

	inline GMFRoomType GetType() { return m_type; }

	inline void SetSceneID(UINT32 sceneid){ m_sceneID = sceneid; }
	inline UINT32 GetSceneID(){ return m_sceneID; }

	inline UINT64 GetGuildID11(){ return m_guildID11; }
	inline UINT64 GetGuildID22(){ return m_guildID22; }

	inline UINT32 GetOWarType(){ return m_oWarType; }
	inline UINT32 GetOBattleId(){ return m_oBattleId; }

	inline UINT32 GetWatcherID() { return m_watcherID; }
	inline void SetWatcherID(UINT32 watcherid) { m_watcherID = watcherid; }

	inline bool EnterStoped() { return m_enterStop; }
	inline void SetEnterStop(bool stop) { m_enterStop = stop; }

	void TestPrint();

private:
	UINT64 m_UID;

	UINT32 m_sceneID;
	time_t m_createTime;

	UINT64 m_guildID11;
	UINT64 m_guildID22;

	UINT32 m_oWarType;
	UINT32 m_oBattleId;

	UINT32 m_watcherID;
	bool m_enterStop;

	GMFRoomType  m_type;
};

struct GMFWResponse
{
	UINT64 waitRoleID;//key
	INT64 uniqueID;
	time_t wTime;
};

class CRole;
class GuildMatchFightMgr : public ITimer
{
	GuildMatchFightMgr();
	~GuildMatchFightMgr();
	DECLARE_SINGLETON(GuildMatchFightMgr);
public:
	bool Init();
	void Uninit();

	UINT64 AddOneGMFRoom(UINT64 guildid11, UINT64 guildid22, UINT32 warType, UINT32 battleId, GMFRoomType type = GMF_RT_GMF);

	bool CreateSceneBegin(UINT64 uniqueid);
	void CreateSceneEnd(UINT64 uniqueid, UINT32 sceneid);

	//void JoinBegin(UINT64 guildid);
	int JoinRequest(CRole* pRole);

	void HandlerGmfUpdate(const KKSG::GmfUpdateData& data);
	void WatchBegin(UINT64 uniqueid, UINT32 liveID);
	void FightEnd(UINT64 uniqueid, UINT64 winguildid, UINT64 loseguildid, UINT32 winscore, UINT32 losescore);
	void SetEnterStop(UINT64 uniqueid, bool enterstop);

	//void OnRoleEnterScene(CRole* pRole);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void TestMakeFight(CRole* pRole, int type, UINT64 guild11, UINT64 guild22);	
	void TestUpdate();

	void TestBegin(UINT32 gamenum);
	void TestEnd(UINT32 sceneid);

	void TestPrint();

private:

	//void JoinIcon(CRole* pRole);

	void DelOneGMFRoom(UINT64 uniqueID);
	void CheckErrorRoom();

	std::unordered_map<UINT64, CGuildMFRoom> m_allRooms;

	UINT32 m_gmfSceneTemplateID;
	UINT32 m_gprSceneTemplateID;
	HTIMER m_handler;
};

#endif