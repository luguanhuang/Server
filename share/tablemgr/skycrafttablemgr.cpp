#include "pch.h"
#include "skycrafttablemgr.h"
#include <math.h>

bool SkyCraftTableMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void SkyCraftTableMgr::Uninit()
{
	ClearFile();
}

bool SkyCraftTableMgr::CheckFile()
{
	SkyCraftMatchTable tmp;
	if (!tmp.LoadFile("table/SkyCraftMatch.txt"))
	{
		LogError("load table/SkyCraftMatch.txt");
		return false;
	}

	SkyCraftMatchParamTable tmp2;
	if (!tmp2.LoadFile("table/SkyCraftMatchParam.txt"))
	{
		LogError("load table/SkyCraftMatchParam.txt");
		return false;
	}

	SkyCraftRankRewardTable tmp3;
	if (!tmp3.LoadFile("table/SkyCraftRankReward.txt"))
	{
		LogError("load table/SkyCraftRankReward.txt");
		return false;
	}

	SkyCraftRewardTable tmp4;
	if (!tmp4.LoadFile("table/SkyCraftReward.txt"))
	{
		LogError("load table/SkyCraftReward.txt");
		return false;
	}

	return true;
}

bool SkyCraftTableMgr::LoadFile()
{
	if (!CheckFile())	
	{
		return false;
	}
	ClearFile();

	m_match.LoadFile("table/SkyCraftMatch.txt");
	m_matchParam.LoadFile("table/SkyCraftMatchParam.txt");
	m_rankReward.LoadFile("table/SkyCraftRankReward.txt");
	m_reward.LoadFile("table/SkyCraftReward.txt");

	for (auto it = m_match.Table.begin(); it != m_match.Table.end(); ++it)
	{
		SkyCraftMatchTable::RowData* data = *it;
		UINT32 point = data->point[1];
		m_timeSpan[point][data->time] = data->span;
	}
	for (auto it = m_timeSpan.begin(); it != m_timeSpan.end(); ++it)
	{
		m_maxTime[it->first] = it->second.rbegin()->first;
	}

	for (auto it = m_matchParam.Table.begin(); it != m_matchParam.Table.end(); ++it)
	{
		SkyCraftMatchParamTable::RowData* data = *it;
		m_paramMap[data->max] = data;
	}

	for (auto it = m_rankReward.Table.begin(); it != m_rankReward.Table.end(); ++it)
	{
		auto conf = *it;
		m_rank2Reward[conf->rank[1]] = conf->reward;
	}

	return true;
}

void SkyCraftTableMgr::ClearFile()
{
	m_match.Clear();
	m_matchParam.Clear();
	m_rankReward.Clear();
	m_reward.Clear();

	m_timeSpan.clear();
	m_maxTime.clear();
	m_paramMap.clear();
	m_rank2Reward.clear();
}

UINT32 SkyCraftTableMgr::GetSpan(UINT32 point, UINT32 time)
{
	if (m_timeSpan.empty())
	{
		return 0;
	}
	std::map<UINT32, UINT32>* timeMap = NULL;
	auto it = m_timeSpan.lower_bound(point);
	if (it == m_timeSpan.end())
	{
		timeMap = &m_timeSpan.rbegin()->second;
	}
	else
	{
		timeMap = &it->second;
	}

	auto it2 = timeMap->lower_bound(time);
	if (it2 == timeMap->end())
	{
		return timeMap->rbegin()->second;
	}
	return it2->second;
}

UINT32 SkyCraftTableMgr::GetMaxTime(UINT32 point, UINT32& maxScore)
{
	if (m_maxTime.empty())
	{
		maxScore = 0;
		return 0;
	}
	auto it = m_maxTime.lower_bound(point);
	if (it == m_maxTime.end())
	{
		maxScore = m_maxTime.rbegin()->first;
		return m_maxTime.rbegin()->second;
	}
	maxScore = it->first;
	return it->second;
}

void SkyCraftTableMgr::GetRankReward(UINT32 rank, std::vector<Sequence<UINT32, 2>>& rewards)
{
	if (m_rank2Reward.empty())
	{
		return;
	}
	auto it = m_rank2Reward.lower_bound(rank);
	if (it == m_rank2Reward.end())
	{
		rewards = m_rank2Reward.rbegin()->second;
		return;
	}
	rewards = it->second;

}

SkyCraftRewardTable::RowData* SkyCraftTableMgr::GetBattleRewardConf(SKRRoundType roundType)
{
	for(size_t i = 0; i < m_reward.Table.size(); ++i)
	{
		const auto pConf = m_reward.Table[i];
		if( pConf->type == roundType)
		{
			return pConf;
		}
	}
	return NULL;
}

UINT32 SkyCraftTableMgr::GetK(UINT32 score, bool isWin)
{
	if (m_paramMap.empty())
	{
		return 0;
	}
	SkyCraftMatchParamTable::RowData* data = NULL;
	auto it = m_paramMap.lower_bound(score);
	if (it == m_paramMap.end())
	{
		data = m_paramMap.rbegin()->second;
	}
	else
	{
		data = it->second;
	}

	return isWin ? data->winK : data->loseK;
}

int SkyCraftTableMgr::GetDeltaScore(UINT32 selfScore, UINT32 otherScore, bool isWin)
{
	int delta = otherScore - selfScore;
	float expect = 1/(1 + pow(10, ((float)delta/(float)400)));
	float W = isWin ? 1.0f : 0.0f;
	float K = (float)GetK(selfScore, isWin);

	return (int)(K * (W - expect));
}

