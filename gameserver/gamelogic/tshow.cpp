#include "pch.h"
#include "tshow.h"
#include "time.h"
#include "util/XCommon.h"
#include "foreach.h"

/*void TShow::Update()
{
	time_t nowweek = time(NULL);
	if (!XCommon::IsWeekSame(time_t(m_week), nowweek))
	{
		m_week = (UINT32)nowweek;
		m_rolecount.clear();
		m_ischanged = true;
	}
}

void TShow::DebugReset()
{
	time_t nowweek = time(NULL);
	m_week = (UINT32)nowweek;
	m_rolecount.clear();
	m_ischanged = true;
}

void TShow::RecordRoleCount(UINT64 besendid, UINT64 roleid, UINT32 count)
{
	Update();

	bool isin = false;
	for (auto i = m_rolecount.begin(); i != m_rolecount.end(); ++i)
	{
		if (i->roleid == roleid)
		{
			i->count += count;
			isin = true;
		}
	}
	if (!isin)
	{
		TShowRoleCountRecord record;
		record.roleid = roleid;
		record.count = count;
		m_rolecount.push_back(record);
	}

	m_updatetime = (UINT32)time(0);

	CRankListMgr::Instance()->OnChangeTShowVotedCount(besendid, GetVotedNum(), m_updatetime);

	m_ischanged = true;
}

bool TShow::IsLess(const TShowRoleCountRecord& a, const TShowRoleCountRecord& b)
{
	return a.count > b.count;
}

void TShow::Sort(bool isall)
{
	if (isall)
	{
		std::sort(m_rolecount.begin(), m_rolecount.end(), IsLess);
	}
	else
	{
		UINT32 count = (m_rolecount.size() >= 3) ? 3 : m_rolecount.size();
		std::partial_sort(m_rolecount.begin(), m_rolecount.begin() + count, m_rolecount.end(), IsLess);
	}	
}

UINT32 TShow::GetVotedNum()
{
	UINT32 num = 0;
	foreach(i in m_rolecount)
	{
		num += i->count;
	}
	return num;
}*/

INSTANCE_SINGLETON(TShowConfig)

TShowConfig::TShowConfig()
{

}

TShowConfig::~TShowConfig()
{

}

bool TShowConfig::Init()
{

	if (!m_oFashionRankTable.LoadFile("table/FashionRank.txt"))
	{
		LogWarn("Load FashionRank.txt failed!");
		return false;
	}

	return true;
}

void TShowConfig::Uninit()
{

}

bool TShowConfig::GetFashionRankReward(UINT32 rank, std::vector<ItemDesc>& vec)
{
	foreach(i in m_oFashionRankTable.Table)
	{
		if ((*i)->FashionRank[0] <= (int)rank && (int)rank <= (*i)->FashionRank[1])
		{
			foreach(j in (*i)->Reward)
			{
				ItemDesc temp((*j)[0], (*j)[1]);
				vec.push_back(temp);
			}
			return true;
		}
	}

	return false;
}
