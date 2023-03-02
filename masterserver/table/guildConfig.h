#ifndef __GUILDCONFIG_H_
#define __GUILDCONFIG_H_


#include "guild/guilddef.h"
#include "table/GuildConfigTable.h"
#include "table/GuildBonusTable.h"
#include "table/GuildGoblinTable.h"
#include "table/GuildSystemTable.h"
#include "table/GuildChallegeTable.h"
#include "table/GuildRankRewardTable.h"
#include "table/GuildSalaryTable.h"
#include "table/GuildCamp.h"
#include "table/GuildCampRank.h"
#include "../share/define/itemdef.h"

typedef GuildConfigTable::RowData GuildConf;
typedef GuildChallegeTable::RowData GuildChallegeConf;

enum
{
	GuildExp = 0,
	GuildNumber = 1,
	PokerTimes = 2,
	PokerReplaceTimes = 3,
	GuildSign = 4,
	GuildWelfare = 5,
	GuildStore = 6,
	GuildSkill = 7,
	GuildStage = 8,
	GuildPokerTimes = 9,
	AllGuildSign = 10,
	AllGuildStage = 11,
	GuildDragon = 12,
	GuildWar = 13,
	GuildStudySkillTime = 14,

	GuildDummy,
};

class CGuildConfig
{
	CGuildConfig();
	~CGuildConfig();
	DECLARE_SINGLETON(CGuildConfig);

public:
	bool Init();
	void Uninit();

	void ClearFile();
	bool LoadFile();
	bool CheckFile();

public:
	bool HasPermission(GuildPosition pos, GuildPermission pem) { return m_PERMMATRIX[pem][pos]; }
	UINT32 GetConfig(UINT32 level, UINT32 type);
	GuildConf *GetGuildConf(int GuildLevel);
	UINT32 GetCanDareRewardTimes(UINT32 guildLvl, UINT32 rewardType);
	GuildChallegeConf* GetChallegeConf(int GuildLvl, int RewardPoint);
	inline UINT32 GetMaxLevel() { return m_oTable.Table.size(); }
	void NoticeGuildSystemOpen(UINT64 guildid, UINT32 level);

	int getGuildGoblinLevelByCount(int num);
	const GuildGoblinTable::RowData* getGoblinInfo(int level);

	const GuildBonusTable::RowData* GetBonusTemplateInfo(UINT32 bonusType, UINT32 bonusVar);
	const GuildBonusTable::RowData* GetBonusTemplateInfo(UINT32 bonusTemplateID);
	const GuildBonusTable* GetGuildBonusTable();

	bool IsOpen(UINT32 level, UINT32 type);
	void GetGuildRankReward(UINT32 rank, UINT32 rewardType, std::vector<ItemDesc>& rewardVec, std::map<UINT32, UINT32>& guildReward);

	const GuildSalaryTable::RowData* GetGuildSalaryInfo(UINT32 guildLvl);
	const GuildCamp::RowData* GetGuildCampTableData(UINT32 guildCampID);
	bool GetGuildCampRankReward(UINT32 rank ,std::map<UINT32, UINT32>& rewards);

private:
	bool m_PERMMATRIX[GPEM_MAX][GPOS_MAX];
	std::map<UINT32, std::vector<UINT32>> m_level2systemid;
	GuildConfigTable   m_oTable;
	GuildSystemTable   m_desctable;
	GuildBonusTable    m_oGuildBonusTable;
	GuildGoblinTable   m_oGuildGoblinTable;
	GuildChallegeTable m_oGuildChallegeTable;
	GuildRankRewardTable m_oRankRewardTable;
	GuildSalaryTable m_oGuildSalaryTable;
	GuildCamp m_oGuildCampTable;
	GuildCampRank m_oGuildCampRankTable;

	std::map<int, std::vector<UINT32> > m_guildChallegeMap;  // 存放工会挑战人次奖励配置
};

#endif