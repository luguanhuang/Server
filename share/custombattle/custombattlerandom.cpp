#include "pch.h"
#include "custombattlerandom.h"
#include "custombattle.h"
#include "util/XRandom.h"
#include "table/globalconfig.h"

#define CUSTOMBATTLE_RANDOM_MAX (GetGlobalConfig().CustomBattleRandomMax)
#define CUSTOMBATTLE_RANDOM_COUNT (GetGlobalConfig().CustomBattleRandomCount)

CustomBattleRandom::CustomBattleRandom()
{

}

CustomBattleRandom::~CustomBattleRandom()
{

}

void CustomBattleRandom::Add(CustomBattle* battle, UINT32 frienddegree, bool isfriend, bool isguild)
{
	///> 官方比赛和已经结束的比赛
	if (battle->IsEnd() || battle->IsGM())
	{
		return;
	}
	if (!isfriend && !isguild)
	{
		///> 不推送有社会关系，有密码
		if (!battle->IsFriendOnly() && !battle->IsGuildOnly() && !battle->HasPassword())
		{
			m_randoms.push_back(battle);
		}
	}
	else
	{
		if ((battle->IsFriendOnly() && isfriend)
			|| (battle->IsGuildOnly() && isguild))
		{
			UINT32 value = (frienddegree << 16) + isguild ? 1 : 0;
			CustomBattleRandomUnit unit;
			unit.battle = battle;
			unit.value = value;
			m_randomrelated.insert(unit);
		}
	}
}

std::vector<CustomBattle*> CustomBattleRandom::Get()
{
	std::vector<CustomBattle*> randoms;

	int leftcount = CUSTOMBATTLE_RANDOM_COUNT > (int)m_randomrelated.size() ?  CUSTOMBATTLE_RANDOM_COUNT - (int)m_randomrelated.size() : 0;
	if (0 == leftcount)
	{
		int limit = 0;
		for (auto i = m_randomrelated.begin(); i != m_randomrelated.end(); ++i)
		{
			if (++limit > CUSTOMBATTLE_RANDOM_COUNT)
			{
				break;
			}
			randoms.push_back(i->battle);
		}
		return randoms;
	}
	else
	{
		for (auto i = m_randomrelated.begin(); i != m_randomrelated.end(); ++i)
		{
			randoms.push_back(i->battle);
		}
		int allcount = (int)m_randoms.size();
		if (leftcount >= allcount)
		{
			for (auto i = m_randoms.begin(); i != m_randoms.end(); ++i)
			{
				randoms.push_back(*i);
			}
			return randoms;
		}
		else
		{
			std::map<UINT64, CustomBattle*> temprandoms;
			while (leftcount > 0)
			{
				int index = XRandom::randInt(0, allcount);	
				temprandoms[index] = m_randoms[index];	
				--leftcount;
			}
			for (auto i = temprandoms.begin(); i != temprandoms.end(); ++i)
			{
				randoms.push_back(i->second);
			}
			return randoms;
		}
	}
}
