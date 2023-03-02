#include "pch.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "custombattle/custombattlematcher.h"
#include "custombattle/custombattleconfig.h"

#define MATCH_PAIR 2

#define MATCHER_UPDATE (GetGlobalConfig().CustomBattleMatcherUpdate)
#define SYSTEMMATCH_LIMIT (GetGlobalConfig().CustomBattleMatcherSystemLimit)
#define CUTOMBATTLE_PK_SYSTEM 1

CustomBattleMatcherMgr::CustomBattleMatcherMgr()
{
	m_timer = INVALID_HTIMER;	
}

CustomBattleMatcherMgr::~CustomBattleMatcherMgr()
{

}

bool CustomBattleMatcherMgr::Init()
{
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, MATCHER_UPDATE, -1, __FILE__, __LINE__);
	return true;
}

void CustomBattleMatcherMgr::Uninit()
{
	if (INVALID_HTIMER != m_timer)
	{
		CTimerMgr::Instance()->KillTimer(m_timer);
		m_timer = INVALID_HTIMER;
	}
}

void CustomBattleMatcherMgr::ExpanMatch()
{
	time_t nowtime = GameTime::GetTime();
	UINT32 expandtime = GetGlobalConfig().CustomBattleMatchExpandTime;
	UINT32 expandmax = GetGlobalConfig().CustomBattleMatchExpandMax;
	UINT32 max = CCustomBattleConfig::Instance()->WinLimit(CUTOMBATTLE_PK_SYSTEM);
	UINT32 min = 0;
	for (auto i = m_index2matchids.begin(); i != m_index2matchids.end(); ++i)
	{
		if (i->second.timestamp + expandtime <= nowtime)
		{
			i->second.timestamp = nowtime + expandtime;
			if (!i->second.matchids.empty())
			{
				UINT32 tempmin = *i->second.matchids.begin();
				UINT32 tempmax = *(--i->second.matchids.end());
				if (tempmin >= min + 1)
				{
					if (tempmin + expandmax > i->second.matchid)
					{
						i->second.matchids.insert(tempmin - 1);
						m_systemmatchers[CUTOMBATTLE_PK_SYSTEM][tempmin - 1].AddMatchUnit(i->first, 1);
						SSInfo<<"roleid:"<<i->first<<" expand:"<<tempmin - 1<<END;
					}
				}
				if (tempmax <= max - 1)
				{
					if (tempmax < expandmax + i->second.matchid)
					{
						i->second.matchids.insert(tempmax + 1);
						m_systemmatchers[CUTOMBATTLE_PK_SYSTEM][tempmax + 1].AddMatchUnit(i->first, 1);
						SSInfo<<"roleid:"<<i->first<<" expand:"<<tempmax + 1<<END;
					}
				}
			}
		}
	}
}

///> 平滑处理
void CustomBattleMatcherMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	// system pk match 
	CustomBattleMatchSceneInfo systeminfo;
	systeminfo.issystem = true;
	systeminfo.mapid = CustomBattlePk_SceneId;
	UINT32 limit = 0;
	bool islimit = false;
	for (auto i = m_systemmatchers.begin(); i != m_systemmatchers.end(); ++i)
	{
		systeminfo.battleid = i->first;
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{

			if (DoMatch(j->first, systeminfo, j->second, limit))
			{
				islimit = true;
				break;
			}
		}
		if (islimit)
		{
			break;
		}
	}
	ExpanMatch();

	// pk match
	UINT32 pklimit = 0;
	for (auto i = m_pkcustommatchers.begin(); i != m_pkcustommatchers.end(); ++i)
	{
		CustomBattleMatcherPk& pkmatcher = i->second;
		if (pkmatcher.Upate(pklimit))
		{
			break;
		}
	}
}

KKSG::ErrorCode CustomBattleMatcherMgr::AddSystemMatcher(KKSG::CustomBattleMatchInfo& info)
{
	info.mutable_role()->set_timestamp((UINT32)GameTime::GetTime());

	m_systemmatchers[info.battle().battleuid()][info.battle().matchid()].AddMatchUnit(info.role().roleid(), 1);
	m_index2info[info.role().roleid()] = info;

	m_index2matchids[info.role().roleid()].timestamp = GameTime::GetTime();
	m_index2matchids[info.role().roleid()].matchid = info.battle().matchid();
	m_index2matchids[info.role().roleid()].matchids.insert(info.battle().matchid());

	SSInfo<<"add system match, role:"<<info.role().roleid()<<" battle:"<<info.battle().battleuid()<<" matchid:"<<info.battle().matchid()<<END;
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CustomBattleMatcherMgr::DelSystemMatcher(UINT64 index)
{
	auto i = m_index2info.find(index);
	if (i == m_index2info.end())
	{
		return KKSG::ERR_FAILED;
	}

	KKSG::CustomBattleMatchInfo* info = &i->second;
	if (!info->battle().issystem())
	{
		return KKSG::ERR_FAILED;
	}

	auto j = m_index2matchids.find(index);
	if (j != m_index2matchids.end())
	{
		for (auto k = j->second.matchids.begin(); k != j->second.matchids.end(); ++k)
		{
			if (m_systemmatchers[info->battle().battleuid()][*k].DelMatchUnit(index))
			{
				SSInfo<<"delete system match, role:"<<info->role().roleid()<<" battle:"<<info->battle().battleuid()<<" matchid:"<<*k<<END;
			}
		}
	}
	m_index2matchids.erase(j);
	m_index2info.erase(i);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CustomBattleMatcherMgr::AddCustomMatcher(KKSG::CustomBattleMatchInfo& info, bool isofficial)
{
	if (KKSG::CustomBattle_PK_Normal == info.battle().type())
	{
		//info.mutable_role()->set_timestamp((UINT32)GameTime::GetTime());
		m_index2info[info.role().roleid()] = info;
		UINT64 uid = info.battle().battleuid();
		auto i = m_pkcustommatchers.find(uid);
		if (i == m_pkcustommatchers.end())
		{
			CustomBattleMatcherPk matchpk(this, uid, info.battle().isfair());
			matchpk.SetOfficial(isofficial);
			m_pkcustommatchers[info.battle().battleuid()] = matchpk;
		}
		m_pkcustommatchers[uid].Add(info.role(), (UINT32)GameTime::GetTime());

		SSInfo<<"add custom match, role:"<<info.role().roleid()<<" battle:"<<uid<<" point:"<<info.role().point()<<END;
		return KKSG::ERR_SUCCESS;
	}
	else if (KKSG::CustomBattle_PKTwo_Normal == info.battle().type())
	{
		///> 不在这匹配，走通用匹配

		return KKSG::ERR_FAILED;
	}
	else
	{
		return KKSG::ERR_FAILED;
	}
}


KKSG::ErrorCode CustomBattleMatcherMgr::DelCustomMatcher(UINT64 index)
{
	auto i = m_index2info.find(index);
	if (i == m_index2info.end())
	{
		return KKSG::ERR_FAILED;
	}
	KKSG::CustomBattleMatchInfo* info = &i->second;
	SSInfo<<"delete custom match, role:"<<info->role().roleid()<<" battle:"<<info->battle().battleuid()<<" point:"<<info->role().point()<<END;

	//m_systemmatchers[info->battle().battleuid()][info->battle().matchid()].DelMatchUnit(index);
	m_pkcustommatchers[info->battle().battleuid()].Remove(info->role().roleid());
	m_index2info.erase(i);

	return KKSG::ERR_SUCCESS;
}

void CustomBattleMatcherMgr::RollBack(UINT32 matchid, UINT64 index)
{
	auto i = m_index2info.find(index);
	if (i != m_index2info.end())
	{
		if (i->second.battle().issystem())
		{
			m_systemmatchers[i->second.battle().battleuid()][matchid].AddMatchUnit(index, 1);
		}
		else
		{
			//m_custommatchers[i->second.battle().battleuid()][i->second.battle().matchid()].AddMatchUnit(index, 1);
		}
	}
}

bool CustomBattleMatcherMgr::DoMatch(UINT32 matchid, CustomBattleMatchSceneInfo& info, MatchMachine& machine, UINT32& limit)
{
	std::vector<std::vector<UINT64>> result;
	machine.DoMatch(result);
	
	bool islimit = false;
	std::vector<std::vector<UINT64>> match;
	for (auto i = result.begin(); i != result.end();)
	{
		match.push_back(*i);

		if (match.size() == MATCH_PAIR)
		{
			if (match[0].size() == 1)
			{
				DelOtherSystemMatch(match[0][0], matchid, info.battleid);
			}
			if (match[1].size() == 1)
			{
				DelOtherSystemMatch(match[1][0], matchid, info.battleid);
			}

			DoBattlePair(info, match[0], match[1]);
			match.clear();
			if ((int)(++limit) > SYSTEMMATCH_LIMIT)
			{
				islimit = true;
			}
		}
		i = result.erase(i);

		if (islimit)
		{
			SSInfo<<"limit:"<<limit<<" left:"<<result.size()<<END;
			break;
		}
	}
	///> 归还落单的
	if (!match.empty())
	{
		for (auto i = match[0].begin(); i != match[0].end(); ++i)
		{
			RollBack(matchid, *i);
		}
	}
	for (auto i = result.begin(); i != result.end(); ++i)
	{
		for (auto j = i->begin(); j != i->end(); ++j)
		{
			RollBack(matchid, *j);
		}
	}
	return islimit;
}

KKSG::CustomBattleMatchInfo* CustomBattleMatcherMgr::GetMatchInfo(UINT64 index)
{
	auto i = m_index2info.find(index);
	if (i == m_index2info.end())
	{
		return NULL;
	}
	else
	{
		return &i->second;
	}
}

void CustomBattleMatcherMgr::TryDel(UINT64 index)
{
	if (KKSG::ERR_SUCCESS != DelSystemMatcher(index))
	{
		DelCustomMatcher(index);
	}
}

void CustomBattleMatcherMgr::DelOtherSystemMatch(UINT64 index, UINT32 matchid, UINT64 battleid)
{
	auto j = m_index2matchids.find(index);
	if (j != m_index2matchids.end())
	{
		for (auto k = j->second.matchids.begin(); k != j->second.matchids.end(); ++k)
		{
			if (*k == matchid)
			{
				continue;
			}
			if (m_systemmatchers[battleid][*k].DelMatchUnit(index))
			{
				SSInfo<<"delete system match, role:"<<index<<" battle:"<<battleid<<" matchid:"<<*k<<END;
			}
		}
	}
	m_index2matchids.erase(j);
}
