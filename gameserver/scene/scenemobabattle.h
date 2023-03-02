#ifndef __SCENEMOBABATTLE_H__
#define __SCENEMOBABATTLE_H__

#include "scene/scenevsbase.h"
#include "scene/scenemobainfo.h"
#include "vshandler.h"
#include "gamelogic/mobabattlemgr.h"
#include "protocol.h"

struct SceneMobaHintTag
{
	bool hint10;
	bool hint20;

	SceneMobaHintTag()
	{
		hint10 = false;
		hint20 = false;
	}
};

class Scene;
class SceneMobaBattle : public SceneVsBase
{
public:
	SceneMobaBattle(Scene* scene);
	~SceneMobaBattle();

	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual void InitState();

	virtual void OnEnterBeforeNtf(Role* pRole);
	virtual void OnRoleReconnectBeforeNtf(Role* role) {}

	virtual UINT32 GetGroup(UINT64 roleid);
	
	virtual bool OnEvent(const VsEvent& event);
	virtual void OnNonRoleEnterScene(Unit* pUnit);

	virtual void Update();

	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();

	virtual void OnEnterFight();
	virtual void OnStateEnd();

	virtual void OnRoleReconnect(Role* pRole);

	virtual void OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value);

	virtual void OnEnemyDead(Enemy* pEnemy);

	inline UINT32 GetTeam1() { return MOBA_GROUP_1; }
	inline UINT32 GetTeam2() { return MOBA_GROUP_2; }

	KKSG::ErrorCode SetHero(UINT64 roleid, UINT32 heroid);
	void AddExperienceHero(UINT64 roleid, UINT32 heroid, UINT32 endTime);

	KKSG::ErrorCode AddGroupLevel(UINT32 group, UINT32 addLevel);
	UINT32 GetGroupLevel(UINT32 group);

	MobaRole* GetMobaRole(UINT64 roleid);

	MobaTeamInfo* GetMobaTeamInfo(UINT32 teamid);

	void GetEnemy(UINT32 group, UINT32 tag, MobaEnemySet& enemySet);

	KKSG::ErrorCode Upgrade(UINT64 roleid, UINT32 param, UINT32& nowParam);

	std::unordered_map<UINT64, MobaRole>& GetRoleDatas() { return m_roleData; }

	void FillResult(KKSG::MobaBattleResult* result);
	void FillRoleResultAndReward(KKSG::MobaBattleRoleResult* result, UINT64 roleid);

	bool IsWinRole(UINT64 roleid);

	KKSG::ErrorCode SendSignal(Role* pRole, UINT32 type);

	virtual void GetSameTeamRole(UINT64 roleid, std::vector<Role*>& sameGroup);

	virtual void FillChatSource(KKSG::ChatSource* source);

	void SendToTeamRole(UINT32 teamid, const CProtocol& oPtc);
	
	virtual KKSG::ErrorCode CheckReportPlayer(UINT64 roleId, UINT64 badRoleId, int& num);

	void GMEnd(UINT64 roleid);
	void GMAddExp(UINT64 roleid, UINT32 addExp);

private:
	virtual UINT32 GetUpInterval() { return 0; }

	void _UpdateMiniMap();

	void _OnRoleDie(Role* pRole, MobaRole& deadData);
	void _SetReviveTime(MobaRole& deadData);
	void _OnEnemyDie(Enemy* pEnemy);
	void _EraseEnemy(Enemy* pEnemy);

	void _SendRoleKillRoleNtf(MobaRole& killerData, MobaRole& deadData);
	void _SendEnemyKillRoleNtf(Enemy* pKiller, MobaRole& deadData);
	void _SendRoleKillEnemyNtf(MobaRole& killerData, Enemy* pDead);
	void _SendEnemyKillEnemyNtf(Enemy* pKiller, Enemy* pDead);

	double _GetExpDiff(UINT32 teamid);
	double _GetAddExp(UINT32 level, double exp, double expDiff, const Sequence<double, 2>& param);	

	void _CheckRoleRevive(UINT64 nowTime);
	void _CheckChooseHero(UINT64 nowTime);

	void _CheckTowerChangeTarget(Unit* pTarget, Unit* pCaster);

	void _SyncAllDataToClient(Role* pRole);									///> 同步所有数据
	void _SendCanUseHeroToClient(Role* pRole);								///> 能选英雄
	void _SendTeamRoleToClient(Role* pRole = NULL);							///> 同步两队队伍人员信息（所选英雄,战绩等）	
	void _SendMobaTeamMsg(Role* pRole = NULL);								///> 同步两队队伍比分，兵线等级
	void _SendMobaMiniMapToClient(UINT32 teamid, Role* pRole = NULL);		///> 同步小地图图标显示

	void _SendMobaRoleChangeToClient(MobaRole& data, UINT32 field = 0);		///> 同步单人信息改变	
	void _SendAllMobaRoleChangeToClient(UINT32 field = 0);					///> 同步所有人信息改变	

	void _MakeEndGameData();

private:
	std::unordered_map<UINT32, MobaTeamInfo> m_team2info;
	std::unordered_map<UINT64, MobaRole> m_roleData;

	VsLoadMgr m_loadmgr;

	std::map<std::pair<UINT32, UINT32>, std::unordered_set<UINT64>> m_grouptag2Enemy;
	std::unordered_set<UINT64> m_belongTeamEnemy;

	UINT64 m_lastUpdateTime;		///> 上一次update时间
	UINT64 m_lastAddExpTime;		///> 上一次加经验时间
	UINT64 m_lastUpdateMiniMapTime;	///> 上一次更新小地图标识时间

	time_t m_startGameTime;			///> 开始游戏时间

	std::unordered_map<UINT32, UINT32> m_posIndex2KillNum;

	UINT32 m_winTeam;				///> 胜利队伍
	UINT64 m_mvpid;
	UINT64 m_loseMvpid;
	UINT64 m_damageMaxid;
	UINT64 m_beHitDamageMaxid;

	bool m_isSpecialEnd;			///> 是否因为没人时间过长结束
	time_t m_noOneTime;				///> 场景没人的开始时间(为了保险起见，场景无人时间过长，就销毁)

	SceneMobaHintTag m_hinttag;
};

#endif