#ifndef __GUILDCASTLEMGR_H__
#define __GUILDCASTLEMGR_H__

#include "timer.h"
#include "guildcastlebase.h"
#include "table/TerritoryBattleReward.h"
#include "table/TerritoryRewd.h"

class GCOneFight;

class GuildCastleMgr : public ITimer
{
	GuildCastleMgr();
	~GuildCastleMgr();
	DECLARE_SINGLETON(GuildCastleMgr)

public:
	bool Init();
	void Uninit();
	bool CheckFile();
	void ClearFile();
	bool LoadFile();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void AddOneFight(UINT32 territoryID, UINT64 defentGuild, const std::set<UINT64>& attackGuild);
	bool MakeOneFight(const std::vector<UINT64>& guilds, UINT32 territoryID);

	GCOneFight* GetOneFight(UINT64 uid);
	GCOneFight* GetTerritoryFight(UINT32 territoryID);
	GCOneFight* GetSceneFight(UINT32 sceneID);
	void GiveGuildFeatsReward();
	void GiveGulidReward();
	void FightResultReport(UINT32 terrID, UINT64 winGuildID);

	void GetGuildJoinRole(UINT64 guildID, std::set<UINT64>& roles);
	void AddGuildJoinRole(UINT64 roleID);
	void SetGuildResult(UINT64 guildID, UINT32 territoryID, bool win, UINT64 winGuildID);

	void ClearFightData();

	TerritoryBattleReward::RowData* GetJoinRewardConf(UINT32 terrLevel, int type);

	void TestMakeFight(UINT32 territoryID);
	void TestPrint();

	std::unordered_map<UINT64, GCGuildJoin> GetGuildJoin()
	{
		std::unordered_map<UINT64, GCGuildJoin> mapGuildInfo;
		mapGuildInfo = m_guildJoins;
		return mapGuildInfo;
	}


private:
	void DoTxLog(UINT64 guildID, UINT32 territoryID, bool win, bool iscastle);
	UINT32 GetLevelSeal();
	UINT64 NewUID();
	void Add(GCOneFight* oneFight);
	void Del(UINT64 UID);

	bool CheckFightStage();

	std::unordered_map<UINT64, GCOneFight*> m_allFights;//所有战场
	std::unordered_map<UINT64, GCGuildJoin> m_guildJoins;//公会参与
	GCActStateMgr m_stateMgr;//状态

	TerritoryBattleReward m_joinRewardTable;

	TerritoryRewd m_territoryRewdTable;

	HTIMER m_handler;	



};

#endif