#include "pch.h"
#include "leaguematchbase.h"
#include "timeutil.h"
#include "tablemgr/leaguematchtablemgr.h"

LeagueMatchBase::LeagueMatchBase()
:m_uid(0)
{

}

LeagueMatchBase::~LeagueMatchBase()
{

}

UINT32 LeagueMatchBase::NewUid()
{
	return ++m_uid;
}

int LeagueMatchBase::Start(LeagueMatchUnit& unit)
{
	UINT64 ltId = unit.leagueTeamId;
	if (IsInMatch(ltId))
	{
		return KKSG::ERR_LEAGUE_TEAM_IN_MATCH;
	}
	if (IsInDelay(ltId))
	{
		return KKSG::ERR_LEAGUE_TEAM_IN_MATCH;
	}

	unit.startTime = TimeUtil::GetTime();

	m_allUnits.insert(std::make_pair(ltId, unit));
	m_sortuUnits[std::make_pair(unit.score, unit.leagueTeamId)] = unit.startTime;

	return KKSG::ERR_SUCCESS;
}

void LeagueMatchBase::Cancel(UINT64 ltId)
{
	auto it = m_allUnits.find(ltId);
	if (it == m_allUnits.end())
	{
		return ;
	}
	LeagueMatchUnit& unit = it->second;
	m_sortuUnits.erase(std::make_pair(unit.score, unit.leagueTeamId));
	m_allUnits.erase(it);
}

bool LeagueMatchBase::IsInMatch(UINT64 ltId)
{
	return m_allUnits.find(ltId) != m_allUnits.end();
}

LeagueMatchUnit* LeagueMatchBase::GetMatchUnit(UINT64 ltId)
{
	auto it = m_allUnits.find(ltId);
	if (it == m_allUnits.end())
	{
		return NULL;
	}
	return &it->second;
}

void LeagueMatchBase::GetMatchLeagueIds(std::vector<UINT64>& leagueIds)
{
	leagueIds.clear();
	for (auto it = m_allUnits.begin(); it != m_allUnits.end(); ++it)
	{
		leagueIds.push_back(it->first);
	}
}

void LeagueMatchBase::DoMatch()
{
	if (m_sortuUnits.size() < 2)
	{
		return;
	}
	auto it = m_sortuUnits.begin();
	auto itNext = it;
	auto itTemp = it;
	for (; it != m_sortuUnits.end(); )
	{
		itTemp = it;
		itNext = ++it;
		it = itTemp;
		if (itNext == m_sortuUnits.end())
		{
			break;
		}
		if (!IsFit(it->first.first, it->second, itNext->first.first, itNext->second))
		{
			it++;
			continue;
		}
		auto itUnit1 = m_allUnits.find(it->first.second);
		auto itUnit2 = m_allUnits.find(itNext->first.second);
		if (itUnit1 == m_allUnits.end() || itUnit2 == m_allUnits.end())
		{
			it++;
			LogError("impossible");
			continue;
		}
		OnMatchSuccess(itUnit1->second, itUnit2->second);
		AddToDelay(itUnit1->second.leagueTeamId);
		AddToDelay(itUnit2->second.leagueTeamId);

		m_allUnits.erase(itUnit1);
		m_allUnits.erase(itUnit2);
		m_sortuUnits.erase(it++);
		m_sortuUnits.erase(it++);
	}
}

void LeagueMatchBase::DoTimeOut()
{
	UINT32 now = TimeUtil::GetTime();
	UINT32 maxScore = 0;
	UINT32 maxTime = 0;
	for (auto it = m_sortuUnits.begin(); it != m_sortuUnits.end(); )
	{
		UINT32 score = it->first.first;
		UINT32 startTime = it->second;
		UINT32 delta = (now > startTime) ? (now - startTime) : 0;
		if (score > maxScore)
		{
			maxTime = LeagueMatchTableMgr::Instance()->GetMaxTime(score, maxScore);
		}

		if (delta < maxTime)
		{
			it++;
			continue;
		}

		auto it2 = m_allUnits.find(it->first.second);
		if (it2 == m_allUnits.end())
		{
			m_sortuUnits.erase(it++);
			LogError("impossible");
			continue;
		}
		OnMatchTimeOut(it2->second);
		m_allUnits.erase(it2);
		m_sortuUnits.erase(it++);
	}
}

bool LeagueMatchBase::IsFit(UINT32 score1, UINT32 time1, UINT32 score2, UINT32 time2)
{
	UINT32 delta = (score1 < score2) ? (score2 - score1) : (score1 - score2);
	if (delta == 0)
	{
		return true;
	}

	UINT32 now = TimeUtil::GetTime();
	UINT32 deltaTime1 = (now > time1) ? (now - time1) : 0;
	UINT32 deltaTime2 = (now > time2) ? (now - time2) : 0;
	UINT32 span1 = LeagueMatchTableMgr::Instance()->GetSpan(score1, deltaTime1);
	if (delta <= span1)
	{
		LogDebug("Match Success, score1: %u time1: %u, score2: %u time2: %u", score1, deltaTime1, score2, deltaTime2);
		return true;
	}

	UINT32 span2 = LeagueMatchTableMgr::Instance()->GetSpan(score2, deltaTime2);
	if (delta <= span2)
	{
		LogDebug("Match Success, score1: %u time1: %u, score2: %u time2: %u", score1, deltaTime1, score2, deltaTime2);
		return true;
	}

	return false;
}

void LeagueMatchBase::DoCheckDelay()
{
	UINT32 now = TimeUtil::GetTime();
	for (auto it = m_delayUnits.begin(); it != m_delayUnits.end();)
	{
		if (now >= it->second)
		{
			LogDebug("Erase Delay[%llu]", it->first);
			m_delayUnits.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void LeagueMatchBase::AddToDelay(UINT64 ltId)
{
	UINT32 now = TimeUtil::GetTime();
	m_delayUnits[ltId] = now + 10;
}

void LeagueMatchBase::DeleteFromDelay(UINT64 ltId)
{
	m_delayUnits.erase(ltId);
}

bool LeagueMatchBase::IsInDelay(UINT64 ltId)
{
	return m_delayUnits.find(ltId) == m_delayUnits.end() ? false : true;
}
