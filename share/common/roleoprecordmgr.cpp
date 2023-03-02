#include "pch.h"
#include "roleoprecordmgr.h"
#include "util/gametime.h"

void RolePairRecK::InfoPrint() const
{
	SSInfo << " type = " << recType << " role1 = " << roleID1 << " role2 = " << roleID2 << " stab = " << tstab << END;
}

void RoleOpRecMgr::Update(time_t now)
{
	UINT32 dayStab = GameTime::GetDayUnique(now);
	UINT32 weekStab = GameTime::GetWeekUnique(now);

	//每天只需要update一次
	if(m_lastUpDayStab == dayStab)
	{
		return;
	}
	m_lastUpDayStab = dayStab;

	for(auto it = m_rolePair2Count.begin(); it != m_rolePair2Count.end(); )
	{
		const auto& recK = (it->first);
		if(recK.recType == RPRT_MATCH_DAY && recK.tstab != dayStab)
		{
			m_rolePair2Count.erase(it++);
			continue;
		}
		if(recK.recType == RPRT_MATCH_WEEK && recK.tstab != weekStab)
		{
			m_rolePair2Count.erase(it++);
			continue;
		}
		it++;
	}
}

void RoleOpRecMgr::AddPairRec(UINT64 roleID1, UINT64 roleID2, time_t now)
{
	if(roleID1 > roleID2)
	{
		std::swap(roleID1, roleID2);
	}
	UINT32 dayStab = GameTime::GetDayUnique(now);
	AddPairRec(RolePairRecK(roleID1, roleID2, dayStab, RPRT_MATCH_DAY));

	UINT32 weekStab = GameTime::GetWeekUnique(now);
	AddPairRec(RolePairRecK(roleID1, roleID2, weekStab, RPRT_MATCH_WEEK));
}

void RoleOpRecMgr::AddPairRec(const RolePairRecK& reck)
{
	const static UINT32 CountLimit = 1000000;
	auto it = m_rolePair2Count.find(reck);
	if(it == m_rolePair2Count.end() && m_rolePair2Count.size() >= CountLimit)
	{
		SSWarn << " rec count to much " << END;
		return;
	}
	m_rolePair2Count[reck] ++;
}

bool RoleOpRecMgr::IsPairLimit(UINT64 roleID1, UINT64 roleID2, time_t now, UINT32 dayLimit, UINT32 weekLimit)
{
	if(roleID1 > roleID2)
	{
		std::swap(roleID1, roleID2);
	}
	UINT32 dayStab = GameTime::GetDayUnique(now);
	UINT32 weekStab = GameTime::GetWeekUnique(now);

	return IsPairLimit(RolePairRecK(roleID1, roleID2, dayStab, RPRT_MATCH_DAY), dayLimit) ||
		IsPairLimit(RolePairRecK(roleID1, roleID2, weekStab, RPRT_MATCH_WEEK), weekLimit);
}

bool RoleOpRecMgr::IsPairLimit(const RolePairRecK& reck, UINT32 limit)
{
	return GetPairRecCount(reck) >= limit;
}

UINT32 RoleOpRecMgr::GetPairRecCount(const RolePairRecK& reck)
{
	auto it = m_rolePair2Count.find(reck);
	if(it != m_rolePair2Count.end())
	{
		return it->second;
	}
	return 0;
}

void RoleOpRecMgr::InfoPrint()
{
	for(auto it = m_rolePair2Count.begin(); it != m_rolePair2Count.end(); ++it)
	{
		(it->first).InfoPrint();
		SSInfo << " count = " << it->second << END;
	}
}

void RoleOpRecMgr::Clear()
{
	m_rolePair2Count.clear();
}

RoleOpRecMgr::RoleOpRecMgr()
{
	m_lastUpDayStab = 0;
}
