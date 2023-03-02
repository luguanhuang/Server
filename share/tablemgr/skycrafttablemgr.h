#ifndef _SKYCRAFT_TABLE_MGR_H_
#define _SKYCRAFT_TABLE_MGR_H_

#include "util/utilsingleton.h"

#include "table/SkyCraftMatchTable.h"
#include "table/SkyCraftMatchParamTable.h"
#include "table/SkyCraftRankRewardTable.h"
#include "table/SkyCraftRewardTable.h"


enum SKRRoundType
{
	SKR_RT_NONE = 0,
	SKR_8_WIN = 1,
	SKR_8_LOSE = 2,
	SKR_4_WIN = 3,
	SKR_4_LOSE = 4,
	SKR_2_WIN = 5,
	SKR_2_LOSE= 6,
};

class SkyCraftTableMgr : public Singleton<SkyCraftTableMgr>
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
	SkyCraftRewardTable::RowData* GetBattleRewardConf(SKRRoundType roundType);
	UINT32 GetK(UINT32 score, bool isWin);
	int GetDeltaScore(UINT32 selfScore, UINT32 otherScore, bool isWin);

private:
	SkyCraftMatchTable m_match;
	SkyCraftMatchParamTable m_matchParam;
	SkyCraftRankRewardTable m_rankReward;
	SkyCraftRewardTable m_reward;

	std::map<UINT32, std::map<UINT32, UINT32> > m_timeSpan;			// key1: 基础分  key2: 匹配时间 value: 最大分差
	std::map<UINT32, UINT32>	m_maxTime;							// key: 分 value: 最大匹配时间
	std::map<UINT32, SkyCraftMatchParamTable::RowData*> m_paramMap;
	std::map<UINT32, std::vector<Sequence<UINT32, 2>>> m_rank2Reward;	// key: 名次 value: 奖励
};

#endif