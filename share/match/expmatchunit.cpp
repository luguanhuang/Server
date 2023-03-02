#include "pch.h"
#include "expmatchunit.h"
#include "table/globalconfig.h"

void MatchUnitExp::SetWait(int roomID, UINT32 now)
{
	m_waitRoomID = roomID;
	m_time = now;
}

void MatchUnitExp::ClearWait()
{
	m_waitRoomID = 0;
	m_time = 0;
}

int MatchUnitExp::GetExpID()
{
	auto pData = GetExpData();
	if(pData)
	{
		return pData->expid();
	}
	return 0;
}

UINT32 MatchUnitExp::GetMatchCount()
{
	auto pData = GetExpData();
	if(pData)
	{
		return pData->rolecount();
	}
	return 0;
}

const KKSG::KMatchExp* MatchUnitExp::GetExpData()
{
	if(m_kunit.roles_size() > 0)
	{
		return (&m_kunit.roles(0).expdata());
	}
	return NULL;
}

std::string MatchUnitExp::GetRoleName()
{
	auto pData = GetExpData();
	if(pData)
	{
		return pData->name();
	}
	return "null";
}

bool MatchUnitExp::IsWaitTimeEnd(UINT32 now)
{
	return (m_time + GetGlobalConfig().TeamFastMatchConfirmT <= now);
}
