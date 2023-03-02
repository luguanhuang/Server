#ifndef __GUILD_ARENA_H__
#define __GUILD_ARENA_H_

#include "timer.h"
#include "table/globalconfig.h"
#include "table/GuildArenaReward.h"
#include "table/GuildArenaPointReward.h"
#include "pb/enum.pb.h"
//#include "gamelogic/item.h"
#include "guild/ptcm2c_synguildarenadisplacenew.h"
#include "define/itemdef.h"
#include "role/switch.h"
#include "guild/guild.h"

/*
struct fightUnit
{
	enum FIGHT_UNIT_STATE
	{
		READY     = 1, // 准备
		FIGHTING  = 2, // 战斗中
		DIE       = 3, // 阵亡
		WAIVER    = 4, // 
	};

	fightUnit()
	{
		pos = 0;
		roleId = 0;
		state = READY;
	}

	UINT32 pos;              // 阵位(1-5)
	UINT64 roleId;           // 玩家id 
	FIGHT_UNIT_STATE state;  // 状态

};
*/

/*
// 参赛队伍信息  以公会为单位
class CGuildArenaTeam
{
public:
	CGuildArenaTeam();
	CGuildArenaTeam(UINT64 guildId);
	~CGuildArenaTeam();

public:
	inline UINT64 GetGuildId() { return m_guildId; }
	inline std::map<UINT32, fightUnit>& GetFigthRoleMap(){ return m_figthRoleMap; }

public:
	void SimpleDeploy();   // 一键部署 
	void AutoDeploy();     // 自动部署
	void RenewUnitState(); // 恢复
	void LastRenewState(); // 冠军状态恢复
	void SetState(UINT32 pos, fightUnit::FIGHT_UNIT_STATE state); // 设置状态
	void GetFightRole(std::vector<UINT64>& roleVec);
	UINT32 GetRoleInMap();
	UINT32 Deploy(UINT64 roleId, UINT64 operId, UINT32 pos);      // 部署

private:
	void _InitPos();
	void _ClearPos();
	bool _IsFightRoleMapFull();
	bool _IsRoleInFightMap(UINT64 roleId);
	bool _IsRoleFree(UINT64 roleId);
	bool _IsPosFree(UINT32 pos);
	bool _IsTeamFree();
	UINT32 _GetRolePos(UINT64 roleId);

private:
	UINT64 m_guildId;
	std::map<UINT32, fightUnit> m_figthRoleMap;
};
*/

// 战斗单位  2只队伍pk
class CGuildArenaBattle
{

public:
	enum BATTLE_STATE
	{
		BATTLE_READY    = 1,
		BATTLE_FIGHTING = 2,
		BATTLE_FINISH   = 3,
	};

public:
	CGuildArenaBattle();
	CGuildArenaBattle(UINT32 battleId);
	CGuildArenaBattle(UINT32 battleId, UINT32 pos, UINT64 guildOne);
	CGuildArenaBattle(UINT32 battleId, UINT32 posOne, UINT64 guildOne, UINT32 posTwo, UINT64 guildTwo);
	~CGuildArenaBattle();

public:
	void AddTeam(UINT32 pos, UINT64 guildId);
	//void AddTeam(UINT32 pos, CGuildArenaTeam& team);
	//void RenewUnitState();
	//bool AutoEndBattle();

	bool HasGuild(UINT64 guildId);
	UINT64 GetLoserId();
	UINT32 GetFightState(); // 返回值 1 入场 2 观战 3结束
	bool CheckEnd();
	//CGuildArenaTeam* GetWiner();

public:
	inline UINT64 GetWinId() { return m_winGuildId; }
	inline BATTLE_STATE GetState() { return m_state; } 
	inline UINT32 GetWatchId() { return m_watchId; }
	//inline std::map<UINT32, CGuildArenaTeam>& GetTeamMap() { return m_teamMap; } 
	inline UINT32 GetTeamNum() { return m_teamMap.size(); }
	inline void SetWinId(UINT64 winId) { m_winGuildId = winId; }
	inline void SetState(BATTLE_STATE state) { m_state = state; }
	inline bool IsFinish() { return m_state == BATTLE_FINISH; }
	inline void clearWathId() { m_watchId = 0; }
	inline UINT32 GetBattleId() { return m_battleId; }
	inline void SetWatchId(UINT32 watchId) { m_watchId = watchId; }
	inline std::map<UINT32, UINT64>& GetTeamMap() { return m_teamMap; } 

private:
	BATTLE_STATE m_state; // 状态
	UINT32 m_battleId;    // 对战编号(1-7)
	UINT64 m_winGuildId;  // 战胜公会id
	UINT32 m_watchId;     // 观看id
	//std::map<UINT32, CGuildArenaTeam> m_teamMap;  // 一场战斗包括两个公会
	std::map<UINT32, UINT64> m_teamMap;  // 一场战斗包括两个公会
};

// 擂台战类  8只队伍淘汰赛 共7场比赛
class CGuildArenaWar
{
public:
	enum REWARD_TYPE
	{
		WIN          = 1,
		LOSE         = 2,
		CHA_WIN      = 3,
		CHA_LOSE     = 4,
		CHA_LEA_WIN  = 5,
		CHA_LEA_LOSE = 6,
	};

public:
	const static UINT32 BATTLE_MAX = 8;  // 8支队伍组成一个组

public:
	CGuildArenaWar();
	CGuildArenaWar(UINT32 warType);
	~CGuildArenaWar();

public:
	void Init(std::map<UINT32, UINT64> guildMap);
	void StartNewTurn();
	bool IsWarEnd();
	inline std::map<UINT32, CGuildArenaBattle>& GetBattleMap() { return m_battleMap; }
	inline std::set<UINT32>& GetCurTurnSet() { return m_curTurnSet; } 
	void GiveReward(REWARD_TYPE rewardType, UINT64 guildId);          // 
	void GetAllGuildId(std::set<UINT64>& allGuildSet);

private:
	bool _CreateNextTurn();      // 生成新一轮战斗匹配
	void _StartNewtTurnBattle(); // 调用战斗函数
	void _RenewUnitState();      // 刷新战斗单元状态
	//bool _CheckAutoEnd(CGuildArenaBattle* pBattle, UINT32 battleId); //自动结束战斗检查

private:
	UINT32 m_warType;
	std::set<UINT32> m_curTurnSet; // 当前轮次比赛id
	std::map<UINT32, CGuildArenaBattle> m_battleMap; 

};

//  擂台战管理类   前八名参赛
class CGuildWarMgr : public ITimer
{
	enum WAR_STATE
	{
		TRUCE           = 1,  // 休战
		READY           = 2,  // 开启
		NOTICE_ONE      = 3,  // 预备公告1
		NOTICE_TWO      = 4,  // 预备公告2
		NOTICE_THREE    = 5,  // 预备公告3 
		LAMP_ONE        = 6,  // 跑马灯1
		LAMP_TWO        = 7,  // 跑马灯2 
		LAMP_THREE      = 8,  // 跑马灯3
		WAR_ONE_START   = 9,  // 初赛
		WAR_TWO_START   = 10,  // 复赛
		WAR_FINAL_START = 11,  // 决赛
		WAR_END         = 12,  // 结束
	};

	const static UINT32 WAR_MAX = 3; // 三个组比赛


private:
	CGuildWarMgr();
	~CGuildWarMgr();
	DECLARE_SINGLETON(CGuildWarMgr);

public:
	bool Init(); 
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount); 

public:
	inline std::map<UINT32, CGuildArenaWar>& GetWarMap() { return m_arenaMap; } 
	inline std::set<UINT64>& GetGuildSet() { return m_allGuildSet; } 
	inline void AddShowRole(UINT64 roleId){ m_showRoleSet.insert(roleId); }
	inline void DelShowRole(UINT64 roleId){ m_showRoleSet.erase(roleId); }
	inline UINT32 GetState(){ return m_state; }

public:
	bool CheckEnd(); // 是否结束
	//void FightUnitBegin(UINT64 guildId, UINT32 pos);  // 单元开始战斗同步
	//void FightUnitLose(UINT64 guildId, UINT32 pos);   // 单元战斗失败同步
	void BattleFinish(UINT32 warType, UINT32 battleId, UINT64 winId);  // 处理战斗结束返回
	void SynRoleOnline(UINT64 roleId, bool online);
	//void SynDisplace(UINT32 warType, UINT32 battleId, UINT64 guildId, std::vector<KKSG::GuildDarenaUnit> units);

	//CGuildArenaTeam* GetTeam(UINT64 guildId);
	void CheckPlayerOnline(CRole* pRole); // 上线检查强制icon
	//void Deploy(UINT64 guildId, UINT64 roleId, UINT64 optId, UINT32 pos);  // 阵位部署
	void GetReward(UINT32 warType, UINT32 rewardType, std::vector<ItemDesc>& itemVec, std::map<UINT32, UINT32>& guildReward);
	//void SynUnitFightState(UINT64 guildId, UINT32 pos, UINT32 state);
	void SynBattleInfo(UINT32 warType, UINT32 battleId);  // 同步战斗信息
	KKSG::GuildArenaState GetTimeState();
	UINT32 GetWarType(UINT64 guildId);
	void SetBattleWatchId(UINT32 warType, UINT32 battleId, UINT32 watchId);
	CGuildArenaBattle* GetBattleByBattleId(UINT32 warType, UINT32 battleId);
	CGuildArenaBattle* GetBattleByGuildId(UINT64 guildId);
	UINT32 GetFightTime(UINT32 type);
	void CheckNextNotice(UINT32 type);
	void GetAllGuildId(std::set<UINT64>& allGuildSet);
	void CreateBattle(std::vector<UINT64>& guildVec);
	bool IsOnTime();
	void ClearData();
	bool IsGuildWaring(UINT64 guildId);

private:
	void _InitArenaWar();
	bool _IsTime(WAR_STATE stateType);
	void _SetBattleWiner(UINT32 warType, UINT32 battleId, UINT64 winId);
	time_t _GetTime(WAR_STATE state);

	void _NoticeInit();
	void _NoticeReady();
	//void _NoticeStart();
	void _DoNewBattleTurn();

	void _NoticeWarBegin();
	void _NoticeWarEnd();
	void _TimeOut();
	void _BattleEndNotice(UINT32 warType, UINT32 battleId);

	void _BattleBeforeNotice(WAR_STATE state);
	void _EnterSceneNotice(WAR_STATE state);
	void _NoticeNextState(KKSG::GuildArenaState type);
	void _GiveAuctReward(UINT64 first, UINT64 second);


private:
	UINT32 m_noticeTimes;
	WAR_STATE m_state;	
	std::set<UINT64> m_showRoleSet;
	std::set<UINT64> m_allGuildSet;
	std::map<UINT32, CGuildArenaWar> m_arenaMap;
	HTIMER m_timehandler;
	GuildArenaReward m_reward;
};

/////////////////////////////////////////////////////////////
// 擂台战积分赛

struct IntegralUnitData
{
	IntegralUnitData()
	{
		guildId = 0;
		guildScore = 0;
	}

	UINT64 guildId;     // id
	UINT32 guildScore;  // 积分

	bool operator()(const IntegralUnitData& x, const IntegralUnitData& y);
	bool operator< (const IntegralUnitData& x)const;
};

struct IntegralBattleData
{
	IntegralBattleData()
	{
		isDo = false;
		state = KKSG::integralready;
		guildOne = 0;
		guildTwo = 0;
		guildOneScore = 0;
		guildTwoScore = 0;
		iconOne = 0;
		iconTwo = 0;
		warTime = 0;
	}

	bool isDo; // 是否已经开打
	KKSG::IntegralState state;
	UINT64 guildOne;
	UINT64 guildTwo;
	UINT32 guildOneScore;
	UINT32 guildTwoScore;
	UINT32 iconOne;
	UINT32 iconTwo;
	UINT32 warTime;
	std::string nameOne;
	std::string nameTwo;
};

struct HistoryRecordData
{
	std::string first;
	std::string second;
};

class CGuildArenaIntegralMgr : public ITimer
{

private:
	CGuildArenaIntegralMgr();
	~CGuildArenaIntegralMgr();
	DECLARE_SINGLETON(CGuildArenaIntegralMgr);
	const static UINT32 UPDATECD = 10;

public:
	bool Init(); 
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount); 
	
	bool Load();
	std::string ToBinary();
	void Update();
public:
	bool IsChange();
	bool IsApplye(UINT64 guildId);
	void SetChange();
	void Apply(UINT64 guildId);  // 报名
	void GuildWin(UINT32 type, UINT64 guildId, UINT32 winTimes); 
	void AddHistoryRecord(std::string first, std::string second);
	void CheckNewTurn(); // 新一轮之前的清理工作
	void CreateFinalBattle(); // 取前八名参加最后的巅峰擂台战
	void CheckFinalBattle();  // 
	void GetAllStartTime(std::set<UINT32>& allStartTime);
	KKSG::GuildArenaType GetTimeType();
	UINT32 GetApplyEndTime();
	UINT32 GetWeekNum(UINT32 nowTime = 0); // 0未开启 1单周 2双周
	bool IsApplyTime(UINT32 nowTime = 0);    // 是否在报名时间
	void SetBattleState(UINT64 guildOne, UINT64 guildTwo, KKSG::IntegralState state);
	void WriteBattleResoult(UINT64 guildOne, UINT32 winOne, UINT64 guildTwo, UINT32 winTwo);
	void DoOnlineCheck(UINT64 roleId);
	void ClearALLData();
	UINT32 GetCurTurnNum();
	bool IsGuildWaring(UINT64 guildId);
	void ClearScoreByErrorReason(UINT32 turnNum);

public:
	inline void SetIntegralOneTime(UINT32 time) { m_IntegralOneTime = time; SetChange(); }
	inline void SetIntegralTwoTime(UINT32 time) { m_IntegralTwoTime = time; SetChange(); }
	inline void SetIntegralThreeTime(UINT32 time) { m_IntegralThreeTime = time; SetChange(); }
	inline void SetIntegralFourTime(UINT32 time) { m_IntegralFourTime = time;  SetChange(); }
	inline void SetIntegralOneEnd(UINT32 time) { m_IntegralOneEnd = time; SetChange(); }
	inline void SetIntegralTwoEnd(UINT32 time) { m_IntegralTwoEnd = time; SetChange(); }
	inline void SetIntegralThreeEnd(UINT32 time) { m_IntegralThreeEnd = time; SetChange(); }
	inline void SetIntegralFourEnd(UINT32 time) { m_IntegralFourEnd = time; SetChange(); }
	inline void SetcreateTime(UINT32 time) { m_createTime = time;  SetChange(); }
	inline void SetbeginTime(UINT32 time) { m_beginTime = time;  SetChange(); }
	inline std::map<UINT32, std::vector<IntegralBattleData> >& GetTrunData() { return m_turnData; }
	inline std::set<IntegralUnitData>& GetApplyList() { return m_applyGuild; }
	inline const std::map<UINT32, HistoryRecordData>& GetHistory() { return m_historyRecord; }

private:
	bool _IsOnWarTime(UINT32 type);           // 是否在战斗时间内
	void _CreateIntegralWar(UINT32 type);     // 创建积分赛对阵列表
	void _DoIntegralWar(UINT32 turnNum);      // 开启一轮积分赛
	void _CheckGuildDelete();                 // 清理掉
	void _CreateByeGuild(UINT32 type, std::set<IntegralUnitData>& applyGuild); // 生成轮空公会
	void _DoCreateIntegral(UINT32 type, std::map<UINT32, std::vector<IntegralUnitData> >& guildGroup);
	void _CreateGroup(const std::set<IntegralUnitData>& applyGuild, std::map<UINT32, std::vector<IntegralUnitData> >& guildGroup);
	void _BattleBeforeNotice(UINT32 beforeTime);
	void _AddWinTime(UINT64 guildId, UINT32 winTimes);
	void _SendBattleState(UINT64 guildId, KKSG::IntegralState state);
	void _DoBattleTimeEnd();
	void _NoticeWarBegin(UINT64 guildId);
	void _NoticeWarEnd(UINT64 guildId);
	void _SendApplyEndMail();
	UINT64 _CreateByeGuildId();
	void _GiveReward(UINT64 guildId, bool isWin);



private:
	HTIMER m_timehandler;
	Switch m_switch;            // 数据变更标记
	UINT32 m_beginTime;         // 本轮开始时间
	UINT32 m_createTime;        // 报名完后的对阵列表生成时间记录
	UINT32 m_IntegralOneTime;   // 第一轮积分赛开启记录
	UINT32 m_IntegralOneEnd;    // 第一轮积分赛结束记录
	UINT32 m_IntegralTwoTime;   // 第二轮积分赛开启记录
	UINT32 m_IntegralTwoEnd;    // 第二轮积分赛结束记录
	UINT32 m_IntegralThreeTime; // 第三轮积分赛开启记录
	UINT32 m_IntegralThreeEnd;  // 第三轮积分赛结束记录
	UINT32 m_IntegralFourTime;  // 第四轮积分赛开启记录
	UINT32 m_IntegralFourEnd;   // 第四轮积分赛结束记录
	UINT32 m_FinalCreateTime;   // 巅峰擂台战开启时间

	UINT32 m_UpdateCD;
	std::set<UINT64> m_byeGuild;                          // 已经轮空过的公会 
	std::set<IntegralUnitData> m_applyGuild;             // 报名公会
	std::map<UINT32, HistoryRecordData> m_historyRecord; // 历届冠亚军记录
	std::map<UINT32/*轮次1-4*/, std::vector<IntegralBattleData> > m_turnData; // 对战数据
	GuildArenaPointReward m_rewardTable; // 公会擂台战配置
};



#endif