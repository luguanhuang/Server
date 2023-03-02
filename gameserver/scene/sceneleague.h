#ifndef _SCENE_LEAGUE_H_
#define _SCENE_LEAGUE_H_

#include "scenevsbase.h"
#include "leaguebattlebase.h"
#include "timer.h"


class Scene;
class Role;
class SceneLeague : public SceneVsBase, public ITimer
{
public:
	SceneLeague(Scene* scene);
	~SceneLeague();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual void InitState();

	virtual UINT32 GetGroup(UINT64 roleid);
	virtual UINT64 GetFightGroupUId(UINT64 roleid);

	virtual void Update();

	// load信息推送 
	virtual void OnEnterBeforeNtf(Role* role);
	virtual void OnRoleReconnect(Role* role);

	// 流程控制
	// WaitLoad
	virtual void OnEnterWaitLoad();
	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();

	// Ready阶段
	void OnReadyStart();
	VsStateType CheckReadyEnd();
	void OnReadyEnd();

	// Prepare321
	virtual void OnEnterP321();

	// Fight
	virtual void OnEnterFight();
	virtual VsStateType CheckFightEnd();

	// AfterFight
	virtual void OnEnterAfterFight();
	virtual VsStateType CheckAfterFightEnd();

	// End
	virtual void OnStateEnd();

	// 事件
	virtual bool OnEvent(const VsEvent& event);
	void OnRoleEnter(Role* pRole);		// 玩家进入场景，包括战斗和观战
	void OnRoleLoad(Role* pRole);
	void OnRoleQuit(Role* pRole);
	void OnRoleDie(Role* pRole);

	// 上下阵
	int SetRoleUp(Role* pRole);
	int SetRoleDown(Role* pRole);

	bool IsFightRole(UINT64 roleId);
	void OnBeforeChangeWather(Role* pRole);

	LeagueBattleRoom& GetLeagueRoomInfo(){return m_room;}
	bool BeginRankUpdate();
	void EndRankUpdate(const KKSG::LeagueRankUpdateG2MRes* res);

	void GMSetReady();

private:
	UINT32 GetReadyLeftTime();
	UINT32 GetFightLeftTime();
	UINT32 GetAfterFightLeftTime();
	KKSG::LeagueBattleFightState GetBattleState(UINT32& leftTime);

	bool SetFightRole();

	bool CheckRoleFailed(UINT64 roleId);
	UINT64 GetHpPrecentLessID();
	void UpdateHp();
	void UpdateWinLeague();
	void ClearFailInfo();
	void RemoveFailRole();

	void NotifyLoadInfo(Role* pRole);
	void NotifyOneBattleResult();
	void NotifyBattleResult();
	void NotifyBattleState(Role* pRole = NULL);
	void NotifyBaseData(Role* pRole = NULL);
	void DoTxLog();

private:
	LeagueBattleRoom m_room;		// 战斗相关数据
	bool m_initLoaded;				// 初始加载，第一个玩家doenterscene设置成true
	bool m_readyEndBefore;			// 准备阶段结束之前几秒，用来通知MS后面进来就是观战了
	bool m_readyEnd;				// 准备阶段结束标记

	UINT64 m_roleId1;				// 正在战斗的roleid1
	UINT64 m_roleId2;				// 正在战斗的roleid2

	// 每轮战斗的胜负信息
	UINT64 m_winId;
	UINT64 m_failId;
	float m_winHpPercent;
	float m_failHpPercent;

	// 最终结果
	UINT64 m_winLeagueId;
	UINT64 m_loseLeagueId;

	bool m_updateRankDelay;
	HTIMER m_timer;
};

#endif