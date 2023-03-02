
#ifndef __GUILDTERRITORYBATTLE_H__
#define __GUILDTERRITORYBATTLE_H__

#include "role/switch.h"
#include "table/TerritoryBattle.h"
#include "guild/guild.h"

struct TerritoryGuild 
{
	TerritoryGuild()
	{
		m_guildId = 0;
		m_allianceId = 0;
		m_roleId = 0;
		m_reqAllianceSet.clear();
	}

	TerritoryGuild(UINT64 guildId)
	{
		m_guildId = guildId;
		m_allianceId = 0;
		m_roleId = 0;
		m_reqAllianceSet.clear();
	}

	UINT64 m_guildId;
	UINT64 m_allianceId;               // 盟友id
	UINT64 m_roleId;                   // 宣战玩家id
	std::map<UINT64, UINT32/*时间*/> m_reqAllianceSet; // 被邀请盟友的id
	bool operator< (const TerritoryGuild &x)const;
};

struct CityUnit
{
	UINT64 m_guildId;   // 公会id
	UINT32 m_time;      // 占领时间
};

class CGuildTerritoryBattle : public ITimer
{

private:
	const static UINT32 UPDATECD = 10;  // 10秒更新存储一次数据
	CGuildTerritoryBattle();
	~CGuildTerritoryBattle();
	DECLARE_SINGLETON(CGuildTerritoryBattle);

public:
	bool Init(); 
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool Load();
	void Update();
	std::string ToBinary();

public:
	inline void SetChange() { m_switch.Set(); }
	inline bool IsChange() { return m_switch.TestAndReset(); }

public:
	bool DeclareWar(UINT64 roleId, UINT32 id);    // 宣战
	bool CanDeclareWar(UINT64 roleId, UINT32 id); // 宣战条件检查
	UINT32 ReqAlliance(UINT64 roleId, UINT64 guildId, UINT32 id); // 邀请盟友 
	UINT32 RcvAlliance(UINT64 roleId, UINT64 guildId);            // 接受盟友
	void SetBattleResult(UINT32 id, UINT64 guildId);              // 设置战斗结果
	UINT32 GetDeclareCityId(UINT64 guildId);                      // 查询宣战的领地id
	UINT32 GetTimeType();                          // 获取时间阶段
	UINT32 GetGameType(UINT32 id,bool haveTerritory,Guild* guild); // 获取地图状态 
	UINT32 ClearAlliance(UINT64 guildId);          // 清理盟友邀请列表
	void DoOnlineCheck(UINT64 roleId);             // 上线icon检查
	void GetAllCityId(std::set<UINT32>& idSet);    // 获取所有地图id
	UINT64 GetAllianceId(UINT64 guildId);          // 获取盟友id 
	const TerritoryGuild* GetUnit(UINT64 guildId); 
	void ClearData(bool all = false);
	UINT32 GetCDTime();
	bool IsInWaring(UINT64 guildId); 
	void ReSort(UINT32 id);
	bool IsDeclareTime();
	void DoGuildDismiss(UINT64 guildId); // 公会解散处理
	//bool IsGuildWaring(UINT64 guildId);  // 公会是否在战斗中
	void OnDesignation(UINT64 nGuildID,UINT32 nTerritoryID);

	inline std::map<UINT32, CityUnit>& GetCityMap() { return m_cityMap; } 
	inline std::map<UINT32, std::set<TerritoryGuild> >& GetChallList() { return m_challList; }
	inline TerritoryBattle::RowData* GetTerrConf(UINT32 id) { return m_territoryTable.GetByID(id); }

	
private:
	void _DoStart();        // 开战
	void _StartAucReward(); // 开始拍卖行奖励
	void _RemoveAlliance(UINT32 id, UINT64 guildId); // 清楚盟友关系
	void _ClearDeclareWar(UINT64 guildId);           // 清理宣战数据
	void _SetAllianceId(UINT64 guilId, UINT64 allianId); // 设置盟友id
	UINT32 _GetCityType(UINT32 id);                  // 查询该id领地类型 
	UINT32 _GetGuildTerrId(UINT64 guildId);          // 获取公会目前占有的id
	UINT32 _GetGuildTerrType(UINT64 guildId);        // 获取公会所占领地的类型
	UINT32 _GetAlliacnceNum(UINT64 guildId);         // 获取盟友列表个数
	bool _IsDeclare(UINT32 id, UINT64 guildId);      // 是否已经对领地宣战
	bool _IsWarTime();       
	bool _IsWaitTime();
	bool _IsDeclareHasGuild(UINT32 id, UINT64 guildId);
	//bool _IsWarTime();
	bool _HasAlliance(UINT64 guildId);                // 是否已经有了盟友
	bool _IsCityBeDeclare(UINT32 id);
	void _DoDeclareTimeEnd();
	void _ReturnDrogn(UINT64 roleid);                 // 落选后返回玩家龙币
	void _DoWarTimeEnd();
	void _NoticeWarIcon(UINT64 guildId, bool isBegin);   // 发送战斗icon
	void _NoticeAllIcon(UINT64 guildId);   // 发送盟友icon
	void _CheckAllIcon(UINT64 roleId);
	void _CheckWarIcon(UINT64 roleId);
	void _NoticeBeginWarning();
	void _GetAllDeclare(UINT32 id, std::set<UINT64>& declareSet);
	void _NoticeCityMaster();
	void _NoticeLoseDeclare(UINT32 id, UINT64 guildId);
	void _GetWarGuildSet(std::set<UINT64>& declareSet);
	void _ClearAllianceList(UINT64 guildId);
	void _NoticeCanDeclare(UINT32 id, UINT64 guildId);
	void _SendReward();

	bool _IsGuildWaring(UINT64 guildId);
	void _SendFailReward();
	UINT32 _GetWeekNum();
	void _DoClearJobe();
	void _DelGigIcon();



private:
	UINT32 m_battleBeginTime;         // 活动开启时间记录
	UINT32 m_aucBeginTime;            // 拍卖奖励开启时间记录
	UINT32 m_declareEndTime;          // 宣战时间结束
	UINT32 m_warEndTime;              // 战斗结束时间
	UINT32 m_rewardStartTime;         // 拍卖行奖励开始时间

	HTIMER m_timehandler;
	UINT32 m_UpDataTime;              // 数据存储记录时间
	Switch m_switch;                  // 数据变更标记
	TerritoryBattle m_territoryTable; // 配置表
	std::set<UINT32> m_tempWaringId;  // 战斗进行中的领地id
	std::map<UINT32, CityUnit> m_cityMap;  // 领地信息
	std::map<UINT32, std::set<TerritoryGuild> > m_challList; // 宣战列表
	std::set<UINT64> m_noticeRec;     // 推送icon记录
};


#endif