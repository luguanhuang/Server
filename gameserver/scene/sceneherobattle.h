#ifndef __SCENEHEROBATTLE_H__
#define __SCENEHEROBATTLE_H__

#include "scene.h"
#include "scenevsbase.h"
#include "vsstate.h"
#include "vshandler.h"
#include "define/noticedef.h"
#include "utility/tlogger.h"
#include "gamelogic/herobattlemgr.h"

enum HeroBattleState
{
	HeroBattleState_Normal,
	HeroBattleState_Loot,
	HeroBattleState_InFight,
};

enum HeroBattleTipsEnum
{
	HeroBattleTips_StartLoot			= 0,
	HeroBattleTips_Enemy_StartLoot		= 1,
	HeroBattleTips_StartInFight			= 2,
	HeroBattleTips_StartOverTime		= 3,
};

struct HeroBattleRoleData
{
	HeroBattleRoleData(){}
	HeroBattleRoleData(const KKSG::PvpRoleInfo& _info)
	{
		roleid = _info.roleid();
		info = _info;
		enterGameTime = 0;
		nextReviveTime = 0;
		isInCircle = false;
		heroid = 0;
		canChooseHero = false;
		state = 0;

		lastKillNum = 0;
		lastDamage = 0;
		lastHeroReviveTime = 0;

		ancientPower = 0;

		heroOrder = 1;
		preHero = -1;
		isChangeHero = false;
		kda = 0.0f;

		firstSendSignalTime = 0;
		signalCount = 0;
	}

	UINT64 roleid;	
	KKSG::PvpRoleInfo info;

	UINT64 enterGameTime;
	UINT64 nextReviveTime;

	bool isInCircle;

	UINT32 heroid;	
	bool canChooseHero;

	std::vector<UINT32> freeWeekHero;
	std::map<UINT32, UINT32> experienceHero;
	UINT32 state; ///> 0->初始选英雄阶段，1->游戏阶段

	UINT32 lastKillNum;				///> 选择该英雄前的总人头
	UINT32 lastDamage;				///> 选择该英雄前的总伤害
	UINT32 lastHeroReviveTime;		///> 当前英雄复活时间

	double ancientPower;			///> 上古力量值

	UINT32 heroOrder;				///> 当前英雄顺序
	UINT32 preHero;					///> 前一个选择的英雄
	bool isChangeHero;				///> 当前英雄跟上次比是否改变
	float kda;
	std::set<UINT64> reportIds;		///> 举报过我的玩家ID

	UINT64 firstSendSignalTime;		///> 发信号（每x秒不能超过y次）
	UINT32 signalCount;			
};

struct HeroBattleFightTeam
{
	HeroBattleFightTeam(UINT32 _teamid)
	{
		teamid = _teamid;
		point = 0;
	}
	std::vector<UINT64> teamMember;
	UINT32 teamid;
	UINT32 point;
	double eloPoint;
};

class SceneHeroBattle : public SceneVsBase
{
public:
	SceneHeroBattle(Scene* pScene);
	~SceneHeroBattle();
	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual void InitState();

	virtual void OnEnterBeforeNtf(Role* pRole);
	virtual void OnRoleReconnectBeforeNtf(Role* role) {};

	virtual UINT32 GetGroup(UINT64 roleid);

	virtual bool OnEvent(const VsEvent& event);
	virtual void Update();

	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();

	virtual VsStateType CheckFightEnd();

	virtual void OnStateEnd();

	virtual void OnRoleReconnect(Role* pRole);

	virtual void OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value);

	inline UINT32 GetTeam1() { return 1; }
	inline UINT32 GetTeam2() { return 2; }

	KKSG::ErrorCode SetHero(UINT64 roleid, UINT32 heroid);

	void FillResultAndReward(KKSG::HeroBattleResult* result, UINT64 roleId);
	UINT32 GetWinTeam();
	UINT64 GetMvpID();	

	const std::unordered_map<UINT64, HeroBattleRoleData>& GetRoleDatas(){return m_roleData;}

	void ResetHeroVar(HeroBattleRoleData& roledata);
	void PrintHeroTLog(HeroBattleRoleData& roledata, Role* pRole, bool isEnd);

	bool IsWinRole(UINT64 roleid);

	void GMEnd(Role* pRole);

	UINT32 GetRoleCamp(UINT64 roleid);

	void AddExperienceHero(UINT64 roleid, UINT32 heroid, UINT32 endTime);
	const CombatStatistics* GetCombatStatistics(UINT64 roleId);

	HeroBattleRoleData* GetRoleData(UINT64 roleId);

	virtual KKSG::ErrorCode CheckReportPlayer(UINT64 roleId, UINT64 badRoleId, int& num);

	KKSG::ErrorCode SelectHeroAncientPower(Role* pRole, UINT32 selectPower);

	void SendToTeamRole(UINT32 teamid, const CProtocol& oPtc);

	KKSG::ErrorCode SendSignal(Role* pRole, UINT32 type);

private:
	virtual UINT32 GetUpInterval() { return 0; }

	void _OnTickNormalState();
	void _OnTickLootState();
	void _OnTickInFightState();

	void _SetHeroBattleState(HeroBattleState state);

	void _OnEnterNormalState();
	void _OnEnterLootState();
	void _OnEnterInFightState();

	void _AddRoleDie(UINT64 roleid);
	void _CheckRoleRevive(UINT64 nowTime);
	void _RoleRevive(HeroBattleRoleData& data, bool isBeRevive);
	void _CheckChooseHero(UINT64 nowTime);

	void _SendGameNotice(Role* pRole, CNoticeType type);
	void _SendGameNoticeToAll(CNoticeType type);

	UINT32 _GetSameTeamInCircle();	///> 4种返回值，2种teamid + 0->圈内没人 + (UINT32)-1->圈内不同玩家
	UINT32 _GetTeamRoleNumInCircle(UINT32 teamid);

	bool _UpdateRoleInCircle();
	bool _IsInCircle(UINT64 roleid);	

	void _RecalLootRate();			///> 重算抢夺速率

	bool _CheckLastPointCondition(UINT32 teamid);
	bool _CheckEnd();

	void _SendCanUseHeroToClient(Role* pRole);
	void _SendTeamRoleToClient(Role* pRole = NULL);		///> 同步两队队伍人员信息（所选英雄）
	void _SendTeamDataToClient(Role* pRole = NULL);		///> 同步人头数，分数
	void _SendSyncDataToClient(Role* pRole = NULL);		///> 同步抢夺进度，是否争夺中数据
	void _SendInCircleToClient(Role* pRole = NULL);		///> 同步在圈内的人
	void _SendAncientPowerToClient(Role* pRole = NULL);	///> 同步上古力量值

	void _SyncAllDataToClient(Role* pRole);				///> 同步所有数据

	void _AddOnePoint(bool force = false);

	void _ChangeOverTimeState(bool isStart);

	void _ClearLootData();

	void _CalcuKDA();
	void _MakeEndGameData();		///> 结束游戏，记录数据

	UINT64 _GetMVPID(UINT32 teamid = 0);	

	void _SendTipsToTeam(HeroBattleTipsEnum en, UINT32 teamid = (UINT32)-1);

	void _UpdateAncientPowerPerSecond();
	void _AddAncientPower(HeroBattleRoleData& data, UINT32 type, double percent = 0);

private:
	VsRoleDataMgr m_datamgr;
	VsLoadMgr m_loadmgr;

	HeroBattleMapCenterConf m_centerConf;

	HeroBattleFightTeam m_fightTeam1;
	HeroBattleFightTeam m_fightTeam2;

	std::unordered_map<UINT64, HeroBattleRoleData> m_roleData;  ///> 应该进入的role

	UINT64 m_lastUpdateTime;			///> 上一次update时间

	UINT32 m_occupant;					///> 当前占领队伍

	UINT32 m_lootTeam;					///> 抢夺队伍
	float m_lootProgress;				///> 抢夺进度
	float m_lootRate;					///> 抢夺速率

	UINT64 m_lastAddPointTime;			///> 最后一次加分数时间(ms)	

	UINT64 m_startOverTime;				///> 开始加时的时间(ms)
	UINT32 m_overTimeCount;				///> 加时次数

	UINT64 m_lastAddAncientPowerTime;	///> 最后一次加上古力量时间

	HeroBattleState m_state;

	UINT64 m_mvpid;
	UINT64 m_loseMvpid;

	time_t m_noOneTime;					///> 场景没人的开始时间(为了保险起见，场景无人时间过长就销毁）

	bool m_isSendNotice;

	std::string m_battleUniqueTag;		///> tlog 一场唯一标识 
};



#endif