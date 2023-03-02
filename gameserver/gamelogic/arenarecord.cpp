#include "pch.h"
#include "arenarecord.h"
#include "globalconfig.h"
#include "time.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"

CArenaRecord::CArenaRecord()
{
	Reset();
}

void CArenaRecord::Reset()
{
	m_optimalrank = UINT32(-1);
	m_dayupdate = 0;
	m_point = 0;
}

bool CArenaRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_arenarecord())
	{
		const KKSG::ArenaRecord* pRecord = &poRoleAllInfo->arenarecord();
		m_optimalrank = pRecord->optimalrank();
		m_point = pRecord->point();
		m_dayupdate = pRecord->dayupdate();
		for (int i = 0; i < pRecord->pointreward_size(); ++i)
		{
			m_pointreward.insert(pRecord->pointreward(i));
		}
		for (int i = 0; i < pRecord->rankreward_size(); ++i)
		{
			m_rankreward.insert(pRecord->rankreward(i));
		}
		Update();
	}
	return true;
}

void CArenaRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_ismodify.TestAndReset())
	{
		KKSG::ArenaRecord* pRecord = poRoleAllInfo->mutable_arenarecord();
		pRecord->Clear();
		pRecord->set_optimalrank(m_optimalrank);
		pRecord->set_point(m_point);
		pRecord->set_dayupdate(m_dayupdate);
		for (auto i = m_pointreward.begin(); i != m_pointreward.end(); ++i)
		{
			pRecord->add_pointreward(*i);
		}
		for (auto i = m_rankreward.begin(); i != m_rankreward.end(); ++i)
		{
			pRecord->add_rankreward(*i);
		}

		roChanged.insert(pRecord);
	}
}

UINT32 CArenaRecord::GetOptimalRank()
{
	return m_optimalrank;
}

void CArenaRecord::SetOptimalRank(UINT32 dwOptimalRank)
{
	m_optimalrank = dwOptimalRank;
	m_ismodify.Set();
}

void CArenaRecord::Update()
{
	UINT32 nowday = XCommon::GetTodayUnique();
	if (nowday != m_dayupdate)
	{
		m_point = 0;
		m_pointreward.clear();
		m_dayupdate = nowday;
		m_ismodify.Set();
	}
	return;
}

UINT32 CArenaRecord::GetPoint()
{
	return m_point;
}

void CArenaRecord::AddPoint(UINT32 point)
{
	m_point += point;
	m_ismodify.Set();
}

void CArenaRecord::SetPoint(UINT32 point)
{
	m_point = point;
	m_ismodify.Set();
}

bool CArenaRecord::IsGetPointReward(UINT32 index)
{
	auto i = m_pointreward.find(index);
	if (i == m_pointreward.end())
	{
		return false;
	}
	return true;
}

void CArenaRecord::SetPointReward(UINT32 index)
{
	m_pointreward.insert(index);
	m_ismodify.Set();
}

bool CArenaRecord::IsGetRankReward(UINT32 index)
{
	auto i = m_rankreward.find(index);
	if (i == m_rankreward.end())
	{
		return false;
	}
	return true;
}

void CArenaRecord::SetRankReward(UINT32 index)
{
	m_rankreward.insert(index);
	m_ismodify.Set();
}

const std::set<UINT32>& CArenaRecord::GetPointReward()
{
	return m_pointreward;
}

const std::set<UINT32>& CArenaRecord::GetRankReward()
{
	return m_rankreward ;
}
