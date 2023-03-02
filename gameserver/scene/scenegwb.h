#ifndef __SCENEGWB_H__ 
#define __SCENEGWB_H__ 

#include "guild/gmfbase.h"
#include "scenevsbase.h"
#include "vshandler.h"
#include "vsscenerecord.h"

namespace KKSG
{
	class GmfRoom;
	class GmfScenePara;
}

class Scene;
class SceneGWB : public SceneVsBase
{
public:
	SceneGWB(Scene* scene);
	~SceneGWB();

	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	void Init(const KKSG::GmfScenePara& para);

	virtual void InitState();

	virtual UINT32 GetGroup(UINT64 roleid);
	virtual UINT64 GetFightGroupUId(UINT64 roleid);

	inline bool EnterStoped(){ return m_enterStop; }
	void OnEnterStop();
	inline UINT32 EnterLessTime(){ return 5; }

	virtual void OnJoinStart() = 0;
	virtual VsStateType OnJoinStop() = 0;
	inline bool JoinStoped(){ return m_joinStop; }
	UINT32 JoinLeftTime();

	virtual void OnEnterP321();

	virtual void OnEnterFight();
	UINT32 GetFightLeftTime();

	UINT32 GetAfterFightLeftTime();

	void ResetFightGroup();
	void ResetPosition();

	void FightLog();

	virtual UINT32 GetJoinMaxCount() { return 5; }
	virtual int HandlerJoinRequest(Role* pRole, const KKSG::GmfReadyArg& oArg, KKSG::GmfReadyRes& oRes);

	void ResetLine();

	virtual void RoomStateToClient(Role* pRole = NULL);
	void BattleBaseDataToClient(Role* pRole);
	void BattleStateToClient(Role* pRole);
	virtual void BattleCombatToClient(Role* pRole) {}
	void AddLiveToMs(UINT32 liveID);
	void AllEndToMs(UINT64 winGuildID, UINT64 loseGuildID, UINT32 winscore = 0, UINT32 losescore = 0);
	void EnterStopToMs(bool stop = false);
	void FillGmfRoleBrief(KKSG::GmfRoleBrief& data, UINT64 roleid);
	void FillGmfGuildBrief(KKSG::GmfGuildBrief& data, UINT64 gid);

	int GetBattleState(UINT32& lefttime);

	bool IsFightRole(UINT64 roleID);
	bool IsWatchEnd();

	void OnRoleLoad(UINT64 roleid);
	virtual void OnRoleQuit(UINT64 roleid);
	virtual void OnRoleDie(UINT64 roleid) {}
	virtual void OnRoleRevive(UINT64 roleid) {}

	void OnBeforeChangeWather(Role* pRole);

	virtual void SubUpdate() {}
	void CheckStateSyn();

	//鼓舞
	UINT64 GetInspireID(UINT64 roleid, std::vector<UINT64>& addAttrIDs);
	void TLogInspireID(Role* pRole, UINT64 guildID);

	//观战
	UINT32 AddLive();
	void RemoveLive();

	void SetRoleBrief(Role* pRole);

	void JoinMakeRecord(Role* pRole);

	inline UINT64 GetGuildID11(){ return m_room.GetGuildID(GMF_HRT_ONE); }
	inline UINT64 GetGuildID22(){ return m_room.GetGuildID(GMF_HRT_TWO); }

	virtual void Update();
	virtual bool OnEvent(const VsEvent& event);

protected:

	GuildMFRoom m_room;
	bool m_joinStop;
	bool m_enterStop;
	VsLoadMgr m_loadMgr;
	GmfKickRecord m_kickMgr;

	int m_guildwartimestate;
	time_t m_lastStateSynTime;

	std::vector<UINT64> m_fightRoles;//战斗中

private:
	std::unordered_map<UINT64, KKSG::GmfRoleBrief> m_roleBrief;
};

#endif