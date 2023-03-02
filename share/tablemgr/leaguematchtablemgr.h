#ifndef _LEAGUE_MATCH_TABLE_MGR_H_
#define _LEAGUE_MATCH_TABLE_MGR_H_

#include "util/utilsingleton.h"

#include "table/LeagueMatchTable.h"
#include "table/LeagueRankReward.h"
#include "table/LeagueBattleReward.h"

enum LBRSevType
{
	LBR_SEV_SELF = 1,//本服奖励类型
	LBR_SEV_CROSS = 2,//跨服奖励类型
};

enum LBRRoundType
{
	LBR_RT_NONE = 0,
	LBR_8_WIN = 1,
	LBR_8_LOSE = 2,
	LBR_4_WIN = 3,
	LBR_4_LOSE = 4,
	LBR_2_WIN = 5,
	LBR_2_LOSE= 6,
};

class LeagueMatchTableMgr : public Singleton<LeagueMatchTableMgr>
{
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetSpan(UINT32 point, UINT32 time);
	UINT32 GetMaxTime(UINT32 point, UINT32& maxScore);
	void GetRankReward(UINT32 rank, std::vector<Sequence<UINT32, 2>>& rewards);

	// 八强奖励
	LeagueBattleReward::RowData* GetBattleRewardConf(LBRSevType sevType, LBRRoundType roundType);

private:
	LeagueMatchTable m_table;
	LeagueRankReward m_rankReward;
	LeagueBattleReward m_lbrTable;	

	std::map<UINT32, std::map<UINT32, UINT32> > m_timeSpan;			// key1: 基础分  key2: 匹配时间 value: 最大分差
	std::map<UINT32, UINT32>	m_maxTime;							// key: 分 value: 最大匹配时间
	std::map<UINT32, std::vector<Sequence<UINT32, 2>>> m_rank2Reward;	// key: 名次 value: 奖励
};

#endif