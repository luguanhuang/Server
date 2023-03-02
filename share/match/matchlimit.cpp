#include "pch.h"
#include "matchlimit.h"
#include "util/gametime.h"
#include "table/globalconfig.h"

MatchLimit::MatchLimit()
{
	SetParam(GetGlobalConfig().CustomBattleMatcherCount
			, GetGlobalConfig().CustomBattleMatcherCD
			, GetGlobalConfig().CustomBattleMatcherPoint);
}

MatchLimit::~MatchLimit()
{

}

void MatchLimit::SetParam(int count, int cd, int point)
{
	limitcount = count;
	limitcd = cd;
	limitpoint = point;
}

void MatchLimit::Update()
{
	// delete timeout item
	time_t nowtime = GameTime::GetTime();
	for (auto i = m_roleid2limit.begin(); i != m_roleid2limit.end();)
	{
		for (auto j = i->second.begin(); j != i->second.end();)
		{
			if (nowtime >= j->second.timestamp + limitcd)
			{
				i->second.erase(j++);
			}
			else
			{
				++j;
			}
		}

		if (i->second.empty())
		{
			m_roleid2limit.erase(i++);
		}
		else
		{
			++i;
		}
	}
}

bool MatchLimit::IsNeedCheck(int point)
{
	return limitpoint > point;
}

void MatchLimit::Push(UINT64 roleida, int pointa, UINT64 roleidb, int pointb)
{
	if (IsNeedCheck(pointa))
	{
		InnerPush(roleida, roleidb);
	}
	if (IsNeedCheck(pointb))
	{
		InnerPush(roleidb, roleida);
	}
}

void MatchLimit::InnerPush(UINT64 roleid, UINT64 other)
{
	LimitRecord& limit = m_roleid2limit[roleid][other];
	if (0 == limit.count)
	{
		limit.timestamp = GameTime::GetTime();
	}
	++limit.count;

	if (limit.count > limitcount)
	{
		SSWarn<<"roleid:"<<roleid<<" limit count:"<<limit.count<<END;
	}
}

bool MatchLimit::IsOne2OneLimit(UINT64 roleida, int pointa, UINT64 roleidb, int pointb)
{
	if (IsNeedCheck(pointa))
	{
		if (IsLimit(roleida, roleidb))
		{
			return true;
		}
	}
	if (IsNeedCheck(pointb))
	{
		if (IsLimit(roleidb, roleida))
		{
			return true;
		}
	}
	return false;
}

bool MatchLimit::IsLimit(UINT64 roleid, UINT64 other)
{
	auto i = m_roleid2limit.find(roleid);
	if (i == m_roleid2limit.end())
	{
		return false;
	}
	else
	{
		auto j = i->second.find(other);
		if (j == i->second.end())
		{
			return false;
		}
		else
		{
			return j->second.count >= limitcount;
		}
	}
}
