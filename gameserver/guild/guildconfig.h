#ifndef __GUILDCONFIG_H__
#define __GUILDCONFIG_H__

#include "guild/guilddef.h"
#include "table/GuildConfigTable.h"
#include "table/GuildBonusTable.h"
#include "table/GuildGoblinTable.h"
#include "table/GuildSystemTable.h"
#include "table/GuildChallegeTable.h"
#include "table/GuildCamp.h"
#include "table/GuildCampRank.h"

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

class RoleSummary;
struct ItemDesc;
class GuildConfig
{
	GuildConfig();
	~GuildConfig();
	DECLARE_SINGLETON(GuildConfig)

public:
	bool Init();
	void Uninit();

	void ClearFile();
	bool LoadFile();
	bool CheckFile();

	bool HasPermission(GuildPosition pos, GuildPermission pem);
	bool IsOpen(UINT32 level, UINT32 type);

	UINT32 GetConfig(UINT32 level, UINT32 type);
	UINT32 CalculateLevel(UINT32 exp);
	UINT32 GetMaxLevel();
	
	GuildConf *GetGuildConf(int GuildLevel);
	GuildChallegeConf* GetChallegeConf(int GuildLvl, int RewardPoint);

	int getGuildGoblinLevelByCount(int num);
	const GuildGoblinTable::RowData* getGoblinInfo(int level);

	void NoticeGuildSystemOpen(RoleSummary* summary, UINT64 guildid, UINT32 level);
	bool ExistDareReward(UINT32 guildLvl, UINT32 rewardType);
	UINT32 GetCanDareRewardTimes(UINT32 guildLvl, UINT32 rewardType);

	UINT32 GetDareGuildExp(UINT32 guildLvl);

	void GetStageGuildExp(UINT32 guildlevel, UINT32 scenetype, std::map<UINT32, UINT32>& guildreward);
	ItemDesc GetStageRoleContribute(UINT32 guildlevel, UINT32 scenetype);
	UINT32 GetContributeNum(UINT32 guildlevel, UINT32 scenetype);

	UINT32 GetBonusTemplateID(UINT32 bonusType, UINT32 var);
	std::map<UINT32, UINT32>* GetBonusContent(UINT32 bonusType);
	const GuildBonusTable::RowData* GetBonusTemplateInfo(UINT32 bonusType, UINT32 bonusVar);
	const GuildBonusTable::RowData* GetBonusTemplateInfo(UINT32 bonusTemplateID);

	const GuildCamp::RowData* GetGuildCampData(UINT32 guildCampID);
	
private:
	bool m_PERMMATRIX[GPEM_MAX][GPOS_MAX];

	std::map<UINT32, std::vector<UINT32>> m_level2systemid;
	GuildConfigTable   m_oTable;
	GuildSystemTable   m_desctable;
	GuildBonusTable    m_oGuildBonusTable;
	GuildGoblinTable   m_oGuildGoblinTable;
	GuildChallegeTable m_oGuildChallegeTable;
	GuildCamp m_oGuildCampTable;
	GuildCampRank m_oGuildCampRankTable;

	std::map<int, std::vector<UINT32> > m_guildChallegeMap;  // 存放工会挑战人次奖励配置

	std::map<int, std::map<UINT32, UINT32>> m_guildBonusMap;
};

#endif // __GUILDCONFIG_H__
