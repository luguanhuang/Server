#include "pch.h"
#include "herobattlemgr.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "util/timespecificmgr.h"
#include "globalconfig.h"
#include "payv2Mgr.h"
#include "payconfig.h"


HeroBattleMgr::HeroBattleMgr()
{

}

HeroBattleMgr::~HeroBattleMgr()
{

}

bool HeroBattleMgr::Init()
{
	return LoadFile();
}

void HeroBattleMgr::Uninit()
{

}

bool HeroBattleMgr::CheckFile()
{
	OverWatchTable tmpTable;
	if (!tmpTable.LoadFile("table/OverWatch.txt"))
	{
		LogWarn("Load OverWatchTable.txt failed!");
		return false;
	}
	if (tmpTable.Table.size() != tmpTable.HeroID_Index.size())
	{
		LogWarn("OverWatch.txt has duplicated key");
		return false;
	}
	UINT32 size = -1;
	for (auto i = tmpTable.Table.begin(); i != tmpTable.Table.end(); i++)
	{
		if (size == -1)
		{
			size = (*i)->BuffID.size();
			continue;
		}
		if (size != (*i)->BuffID.size())
		{
			LogWarn("OverWatch.txt, All BuffID size should be equal");
			return false;
		}
	}

	HeroBattleWeekReward tmpWeekReward;
	if (!tmpWeekReward.LoadFile("table/HeroBattleWeekReward.txt"))
	{
		LogWarn("Load HeroBattleWeekReward.txt failed!");
		return false;
	}

	HeroBattleMapCenter tmpMapCenter;
	if (!tmpMapCenter.LoadFile("table/HeroBattleMapCenter.txt"))
	{
		LogWarn("Load HeroBattleMapCenter.txt failed");
		return false;
	}
	for (auto i = tmpMapCenter.Table.begin(); i != tmpMapCenter.Table.end(); i++)
	{
		UINT32 shouldSize = 0;
		if ((*i)->CenterType == 1)
		{
			shouldSize = 1;
		}
		else if ((*i)->CenterType == 2)
		{
			shouldSize = 2;
		}
		if ((*i)->Param.size() != shouldSize)
		{
			LogWarn("HeroBattleMapCenter.txt param is invalid, sceneid:%u", (*i)->SceneID);
			return false;
		}
	}

	HeroBattleExperienceHero tmpExperienceHero;
	if (!tmpExperienceHero.LoadFile("table/HeroBattleExperienceHero.txt"))
	{
		LogWarn("Load HeroBattleExperienceHero.txt failed");
		return false;
	}

	HeroBattleAncientPower tmpAncientPower;
	if (!tmpAncientPower.LoadFile("table/HeroBattleAncientPower.txt"))
	{
		LogWarn("Load HeroBattleAncientPower.txt failed");
		return false;
	}

	return true;
}

bool HeroBattleMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oTable.LoadFile("table/OverWatch.txt");
	m_oWeekReward.LoadFile("table/HeroBattleWeekReward.txt");
	m_oMapCenter.LoadFile("table/HeroBattleMapCenter.txt");
	m_oExperienceHero.LoadFile("table/HeroBattleExperienceHero.txt");
	m_oAncientPower.LoadFile("table/HeroBattleAncientPower.txt");

	return true;
}

void HeroBattleMgr::ClearFile()
{
	m_oTable.Clear();
	m_oWeekReward.Clear();
	m_oMapCenter.Clear();
	m_oExperienceHero.Clear();
}

OverWatchConf* HeroBattleMgr::GetHeroConf(UINT32 heroid)
{
	return m_oTable.GetByHeroID(heroid);
}

HeroWeekRewardConf* HeroBattleMgr::GetHeroWeekRewardConf(UINT32 id)
{
	return m_oWeekReward.GetByid(id);
}

HeroBattleMapCenterConf* HeroBattleMgr::GetHeroMapCenterConf(UINT32 sceneid)
{
	return m_oMapCenter.GetBySceneID(sceneid);
}

HeroBattleExperienceConf* HeroBattleMgr::GetExperienceHeroConf(UINT32 itemid)
{
	return m_oExperienceHero.GetByItemID(itemid);
}

HeroBattleAncientConf* HeroBattleMgr::GetAncientPowerConf(UINT32 heroid)
{
	return m_oAncientPower.GetByHeroID(heroid);
}

bool HeroBattleMgr::IsInSpecialOpenTime()
{
	tm stTime;
	time_t nowTime = time(NULL);
#ifdef WIN32
	localtime_s(&stTime, &nowTime);
#else
	localtime_r(&nowTime, &stTime);
#endif
	UINT32 min = stTime.tm_hour * 60 + stTime.tm_min;
	for (auto i = GetGlobalConfig().HeroBattleSpecialTime.begin(); i != GetGlobalConfig().HeroBattleSpecialTime.end(); i++)
	{
		UINT32 tmpMin1 = (*i)[0] * 60, tmpMin2 = (*i)[1] * 60;
		if (min >= tmpMin1 && min <= tmpMin2)
		{
			return true;
		}
	}
	return true;
}

void HeroBattleMgr::GetNewRandomFreeHero(std::vector<UINT32>& freehero, Role* pRole)
{
	freehero.clear();

	UINT32 weightSum = 0;
	std::unordered_map<UINT32, UINT32> hero;
	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); i++)
	{
		hero.insert(std::make_pair((*i)->HeroID ,(*i)->weight));
		weightSum += (*i)->weight;
	}

	UINT32 additionNum = 0;
	PayMemberTable::RowData* pRow = CPayConfig::Instance()->GetPayMemberByID(pay_member_1);
	if (pRow != NULL && pRow->HeroBattleFree > 0)
	{
		if (pRole->Get<CPayV2Record>()->GetExpireTimeByID(pay_member_1) > 0)
		{
			additionNum = pRow->HeroBattleFree;
		}
	}
	
	for (UINT32 i = 0; i < GetGlobalConfig().HeroBattleFreeHeroCount + additionNum; i++)
	{
		if (weightSum == 0 || hero.empty())
		{
			break;
		}
		UINT32 rand = XRandom::randInt(1, weightSum + 1), sum = 0;
		for (auto j = hero.begin(); j != hero.end(); j++)
		{
			sum += j->second;
			if (sum >= rand)
			{
				freehero.push_back(j->first);
				weightSum -= j->second;
				hero.erase(j);
				break;
			}
		}
	}
}

void HeroBattleMgr::AddSomeRandomFreeHero(std::vector<UINT32>& freehero, UINT32 num)
{
	UINT32 weightSum = 0;
	std::unordered_map<UINT32, UINT32> hero;
	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); i++)
	{
		bool canAdd = true;
		for (auto j = freehero.begin(); j != freehero.end(); j++)
		{
			if (*j == (*i)->HeroID)
			{
				canAdd = false;
				break;
			}
		}
		if (canAdd)
		{
			hero.insert(std::make_pair((*i)->HeroID ,(*i)->weight));
			weightSum += (*i)->weight;
		}
	}
	for (UINT32 i = 0; i < num; i++)
	{
		if (weightSum == 0 || hero.empty())
		{
			break;
		}
		UINT32 rand = XRandom::randInt(1, weightSum + 1), sum = 0;
		for (auto j = hero.begin(); j != hero.end(); j++)
		{
			sum += j->second;
			if (sum >= rand)
			{
				freehero.push_back(j->first);
				weightSum -= j->second;
				hero.erase(j);
				break;
			}
		}
	}
}