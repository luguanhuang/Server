#ifndef __RESWARPVEMGR_H__
#define __RESWARPVEMGR_H__

#include "table/GuildMineralBattle.h"
#include "table/GuildArenaBattlePvpReward.h"
#include "table/GuildBuffTable.h"
#include "table/GuildMineralBufflist.h"
#include "table/Rite.h"
#include "table/GuildMineralBattleReward.h"


class ResWarAwardMgr
{
	ResWarAwardMgr();
	~ResWarAwardMgr();
	DECLARE_SINGLETON(ResWarAwardMgr);
public:
	bool Init();
	void Uninit();
	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	GuildArenaBattlePvpReward::RowData* GetPVPAward(UINT32 nLevel);
	GuildMineralBattleReward::RowData* GetGuildMineralBattleReward(UINT32 nRank,UINT32 nSealType);
	bool IsItemBuff(UINT32 nItemID);
	GuildMineralBufflist::RowData* GetGuildMineralBufflist(UINT32 i);
	GuildMineralBufflist::RowData* GetGuildMineralBufflistMine(UINT32 i);
	GuildMineralBattle::RowData*  GetPVEAward(UINT32 nID);
	UINT32  GetMineralLevelSeal(UINT32 nLevelSeal,std::map<UINT32,UINT32> &mapMineral);
	UINT32 GetLevelSeal(UINT32 nSealType);
	UINT32 GetWorstRank();
	UINT32 GetRiteTime(UINT32 nDurationTime);
	GuildBuffTable::RowData * GetGuildBuffID(UINT32 nID);
	GuildBuffTable::RowData * GetGuildBuffItemID(UINT32 nItemID);
	GuildMineralBufflist::RowData* GetRandBuffList();
private:
	GuildMineralBattle m_guildMineralBattleTable;
	GuildArenaBattlePvpReward m_guildArenaBattlePvpReward;
	GuildBuffTable m_guildBuffTable;
	GuildMineralBufflist m_guildMineralBufflist;
	Rite m_riteTable;
	GuildMineralBattleReward m_mineralBattleReward;
	std::map<UINT32, std::vector<GuildMineralBufflist::RowData*> > m_mapGuildMineralBuff;
};

#endif