#include "pch.h"
#include "leaguematchtablemgr.h"

bool LeagueMatchTableMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void LeagueMatchTableMgr::Uninit()
{
	ClearFile();
}

bool LeagueMatchTableMgr::CheckFile()
{
	LeagueMatchTable tmp;
	if (!tmp.LoadFile("table/LeagueMatches.txt"))
	{
		LogError("load table/LeagueMatches.txt");
		return false;
	}
	LeagueRankReward tmp2;
	if (!tmp2.LoadFile("table/LeagueRankReward.txt"))
	{
		LogError("load table/LeagueRankReward.txt");
		return false;
	}
	LeagueBattleReward tmp3;
	if (!tmp3.LoadFile("table/LeagueBattleReward.txt"))
	{
		LogError(" load table/LeagueBattleReward.txt failed");
		return false;
	}
	return true;
}

bool LeagueMatchTableMgr::LoadFile()
{
	if (!CheckFile())	
	{
		return false;
	}

	m_table.LoadFile("table/LeagueMatches.txt");
	for (auto it = m_table.Table.begin(); it != m_table.Table.end(); ++it)
	{
		LeagueMatchTable::RowData* data = *it;
		UINT32 point = data->point[1];
		m_timeSpan[point][data->time] = data->span;
	}

	for (auto it = m_timeSpan.begin(); it != m_timeSpan.end(); ++it)
	{
		m_maxTime[it->first] = it->second.rbegin()->first;
	}

	m_lbrTable.LoadFile("table/LeagueBattleReward.txt");
	m_rankReward.LoadFile("table/LeagueRankReward.txt");

	for (auto it = m_rankReward.Table.begin(); it != m_rankReward.Table.end(); ++it)
	{
		auto conf = *it;
		m_rank2Reward[conf->rank[1]] = conf->reward;
	}

	return true;
}

void LeagueMatchTableMgr::ClearFile()
{
	m_table.Clear();
	m_timeSpan.clear();
	m_maxTime.clear();
	m_lbrTable.Clear();
}

UINT32 LeagueMatchTableMgr::GetSpan(UINT32 point, UINT32 time)
{
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

UINT32 LeagueMatchTableMgr::GetMaxTime(UINT32 point, UINT32& maxScore)
{
	auto it = m_maxTime.lower_bound(point);
	if (it == m_maxTime.end())
	{
		maxScore = m_maxTime.rbegin()->first;
		return m_maxTime.rbegin()->second;
	}
	maxScore = it->first;
	return it->second;
}

void LeagueMatchTableMgr::GetRankReward(UINT32 rank, std::vector<Sequence<UINT32, 2>>& rewards)
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

LeagueBattleReward::RowData* LeagueMatchTableMgr::GetBattleRewardConf(LBRSevType sevType, LBRRoundType roundType)
{
	for(size_t i = 0; i < m_lbrTable.Table.size(); ++i)
	{
		const auto pConf = m_lbrTable.Table[i];
		if(pConf->ID == sevType && pConf->type == roundType)
		{
			return pConf;
		}
	}
	return NULL;
}