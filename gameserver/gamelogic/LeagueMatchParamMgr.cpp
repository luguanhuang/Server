#include "pch.h"
#include "LeagueMatchParamMgr.h"

bool LeagueMatchParamMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void LeagueMatchParamMgr::Uninit()
{
	ClearFile();
}

bool LeagueMatchParamMgr::CheckFile()
{
	LeagueMatchParamTable tmp;
	if (!tmp.LoadFile("table/LeagueMatchesParam.txt"))
	{
		LogError("load table/LeagueMatchesParam.txt");
		return false;
	}
	return true;
}

bool LeagueMatchParamMgr::LoadFile()
{
	if (!CheckFile())	
	{
		return false;
	}

	m_table.LoadFile("table/LeagueMatchesParam.txt");

	for (auto it = m_table.Table.begin(); it != m_table.Table.end(); ++it)
	{
		LeagueMatchParamTable::RowData* data = *it;
		m_scoreMap[data->max] = data;
	}

	return true;
}

void LeagueMatchParamMgr::ClearFile()
{
	m_table.Clear();
	m_scoreMap.clear();
}

UINT32 LeagueMatchParamMgr::GetK(UINT32 score, bool isWin)
{
	LeagueMatchParamTable::RowData* data = NULL;
	auto it = m_scoreMap.lower_bound(score);
	if (it == m_scoreMap.end())
	{
		data = m_scoreMap.rbegin()->second;
	}
	else
	{
		data = it->second;
	}

	return isWin ? data->winK : data->loseK;
}
