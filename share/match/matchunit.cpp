#include "pch.h"
#include "matchunit.h"
#include "table/globalconfig.h"
#include "team/teamcommon.h"

#include <ctime>

MatchUnit::MatchUnit(UINT32 matchID, int type, const KKSG::KMatchUnit& kunit)
	:m_matchID(matchID), m_type(type), m_kunit(kunit)
{
	
	m_startTime = UINT32(time(NULL));
}

MatchUnit::~MatchUnit()
{

}

void MatchUnit::FillRoles(std::unordered_set<UINT64>& roleIDs)
{
	FillRoles(roleIDs, m_kunit);
}

void MatchUnit::FillRoles(std::unordered_set<UINT64>& roleIDs, const KKSG::KMatchUnit& kunit)
{
	for(int i = 0; i < kunit.roles_size(); ++i)
	{
		roleIDs.insert(kunit.roles(i).roleid());
	}
}

void MatchUnit::FillServers(std::unordered_set<UINT32>& serverIDs)
{
	FillServers(serverIDs, m_kunit);
}

void MatchUnit::FillServers(std::unordered_set<UINT32>& serverIDs, const KKSG::KMatchUnit& kunit)
{
	for(int i = 0; i < kunit.roles_size(); ++i)
	{
		serverIDs.insert(kunit.roles(i).serverid());
	}
}

bool MatchUnit::IsFromTs()
{
	return IsFromTs(m_kunit);
}
	
bool MatchUnit::IsFromTs(const KKSG::KMatchUnit& kunit)
{
	if(!kunit.serverid() && HTeam::IsTsTeam(kunit.teamid()))
	{
		return true;
	}
	return false;
}

void MatchUnit::InfoPrint()
{
	SSInfo << " matchid = " << GetMatchID() << " type = " << GetType() << " time = " << GetStartTime() << END;
	SSInfo << " server = " << GetServerID() << " role = " << GetRoleID() << " team = " << GetTeamID() << END;
	for(int i = 0; i < m_kunit.roles_size(); ++i)
	{
		SSInfo << " role = " << m_kunit.roles(i).roleid() << " server = " << m_kunit.roles(i).serverid() << END;
	}
}

void MatchUnit::FillMatchKey(KMatchKey& key)
{
	FillMatchKey(key, m_kunit);
}

void MatchUnit::FillMatchKey(KMatchKey& key, const KKSG::KMatchUnit& kunit)
{
	key.serverID = kunit.serverid();
	key.roleID = kunit.roleid();
	key.teamID = kunit.teamid();
}

bool MatchUnitPkTwo::MatchCross(UINT32 now)
{
	if(0 == GetGlobalConfig().PkTwoMatchToWorldTime)
	{
		return false;
	}
	return m_startTime + GetGlobalConfig().PkTwoMatchToWorldTime < now;
}

bool MatchUnitPkTwo::MatchRobot(UINT32 now)
{
	auto stage = GetStage(now);
	if(NULL == stage)
	{
		return false;
	}
	//概率只有0和100 不在判断时间
	if(stage->robotpercent() > 0 && stage->lookupid())
	{
		return true;
	}

	return false;
}

bool MatchUnitPkTwo::MatchOther(UINT32 now, MatchUnitPkTwo* other)
{
	if(NULL == other)
	{
		return false;
	}
	auto stage = GetStage(now);
	if(NULL == stage)
	{
		return false;
	}
	UINT32 myPoint = GetPoint();
	UINT32 otherPoint = other->GetPoint();
	UINT32 span = (myPoint > otherPoint)?(myPoint - otherPoint):(otherPoint - myPoint);
	if(stage->span() >= span)
	{
		return true;
	}
	return false;
}

UINT32 MatchUnitPkTwo::GetPoint()
{
	if(m_kunit.roles_size() <= 0)
	{
		return 0;
	}
	UINT32 allPoint = 0;
	for(int i = 0; i < m_kunit.roles_size(); ++i)
	{
		allPoint += (m_kunit.roles(i).pkrec().point());
	}
	return UINT32(allPoint/m_kunit.roles_size());
}

KKSG::PkMatchStage* MatchUnitPkTwo::GetStage(UINT32 now)
{
	if(m_kunit.roles_size() <= 0)
	{
		return NULL;
	}
	auto& data = *m_kunit.mutable_roles(0);
	if(data.pkmatchstage_size() <= 0)
	{
		return NULL;
	}
	for(int i = data.pkmatchstage_size() - 1; i >= 0; --i)
	{
		auto stage = data.mutable_pkmatchstage(i);
		if(stage->ctime() >= now)
		{
			return stage;
		}
	}
	return data.mutable_pkmatchstage(0);
}


PvpMatchUnit::PvpMatchUnit(KMatchID matchID, int type, const KKSG::KMatchUnit& kunit)
	:MatchUnit(matchID, type, kunit)
{
	m_matched = false;
}

PvpMatchUnit::~PvpMatchUnit()
{

}
