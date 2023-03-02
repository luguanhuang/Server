#include "pch.h"
#include "matchbase.h"


bool KMatchHalfRoom::Add(KMatchID matchID, UINT32 count)
{
	if(m_curCount + count > m_fullCount)//to much
	{
		return false;
	}
	m_units.push_back(matchID);
	m_curCount += count;
    return true;
}

bool KMatchHalfRoom::IsFull()
{
	return m_curCount >= m_fullCount;
}

bool KMatchRoom::Add(KMatchID matchID, UINT32 count)
{
	if(m_room1.GetCurCount() <= m_room2.GetCurCount())
	{
		return m_room1.Add(matchID, count);
	}
	else
	{
		return m_room2.Add(matchID, count);
	}
	return false;
}

bool KMatchRoom::IsFull()
{
	return m_room1.IsFull() && m_room2.IsFull();
}

void KMatchRoom::FillUnits(std::list<KMatchID>& units)
{
	units.insert(units.end(), m_room1.Units().begin(), m_room1.Units().end());
	units.insert(units.end(), m_room2.Units().begin(), m_room2.Units().end());
}
