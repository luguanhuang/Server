#include "pch.h"
#include "findbackrecord.h"
#include "itemfindback.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include "findback/ptcg2c_expfindbackntf.h"
#include <time.h>
#include "findbackconfig.h"
#include "foreach.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/towerrecord.h"
#include "gamelogic/towerconfig.h"
#include "unit/systeminfo.h"
#include "findback/ptcg2c_itemfindbackntf.h"
#include "table/OpenSystemMgr.h"
#include "gamelogic/superriskrecord.h"
#include "gamelogic/levelsealMgr.h"
#include "util/gametime.h"
#include "define/systemiddef.h"
#include "towerfindback.h"


TowerFindBackInfo::TowerFindBackInfo(CItemFindBackInfo* info):BaseFindBackInfo(KKSG::TOWER, info)
{
}

//void TowerFindBackInfo::AddUseCount(int count, int dayBegin)
//{
//	int now = time(0);
//	bool isToday = XCommon::IsDaySame(dayBegin, now);
//	int towerLevel = 0;
//	auto towerInfo = GetRole()->Get<CTowerRecord>()->GetTowerInfo(DEFAULT_HARDLEVEL);
//	if (towerInfo)
//	{
//		if (towerInfo->reachTopFloor > towerInfo->curFloor)
//		{
//			towerLevel = towerInfo->reachTopFloor;
//		}
//		else
//			towerLevel = towerInfo->curFloor;
//	}
//	if (m_usedCountMap_h.find(dayBegin) != m_usedCountMap_h.end())
//	{
//		m_usedCountMap_h[dayBegin]->m_subTypeInfos->m_todayUseCount += count;
//		m_usedCountMap_h[dayBegin]->m_subTypeInfos->m_towerLevel = towerLevel;
//		return;
//	}
//	if (!isToday)
//	{
//		OneDayTypeInfo * info = CreateOneDayBackInfo(dayBegin);
//		info->m_subTypeInfos->m_todayUseCount = count;
//		info->m_subTypeInfos->m_towerLevel = towerLevel;
//		m_usedCountMap_h[dayBegin] = info;
//		return;
//	}
//
//	m_usedCountMap->m_subTypeInfos->m_todayUseCount += count;
//	m_usedCountMap->m_subTypeInfos->m_towerLevel = towerLevel;
//}


OneDayTowerInfo::OneDayTowerInfo(BaseFindBackInfo* baseFindBackInfo, int day):OneDayTypeInfo(baseFindBackInfo, KKSG::TOWER, day)
{

}

OneDayTypeInfo* TowerFindBackInfo::CreateOneDayBackInfo(int day)
{
	return new OneDayTowerInfo(this, day);
}

UINT32 TowerFindBackInfo::GetOneDayInfoParam()
{
	int towerLevel = 0;
	auto towerInfo = GetRole()->Get<CTowerRecord>()->GetTowerInfo(DEFAULT_HARDLEVEL);
	if (towerInfo)
	{
		if (towerInfo->reachTopFloor > towerInfo->curFloor)
		{
			towerLevel = towerInfo->reachTopFloor;
		}
		else
			towerLevel = towerInfo->curFloor;
	}

	return towerLevel;
}

bool OneDayTowerInfo::ConvertSingleSubtype(Role* role, KKSG::ItemFindBackInfo2Client& temp, int leftCount)
{
	std::map<int, int> dragonItems;
	std::map<int, int> goldItems;
	int totalGold = 0;
	int totalDragon = 0;

	int left = leftCount;

	if(!m_subTypeInfos->m_towerLevel)
		return false;
	auto tableData = TowerConfig::Instance()->GetTowerReward(DEFAULT_HARDLEVEL, 0, m_subTypeInfos->m_towerLevel, dragonItems);
	if (tableData)
	{

		totalDragon = tableData->DragonCoinFindBackCost;
		totalGold = tableData->GoldCoinCost;
		

		for (auto i = tableData->GoldCoinFindBackItem.begin(); i != tableData->GoldCoinFindBackItem.end(); ++i)
		{
			goldItems[i->seq[0]] += i->seq[1];
		}
	}
	else
		return false;

	temp.set_id((KKSG::ItemFindBackType)m_type);
	temp.set_findbackcount(left);
	temp.set_daytime(m_day);
	temp.set_rolelevel(m_level);

	temp.set_dragoncoincost(totalDragon);
	temp.set_goldcoincost(totalGold);

	CheckUpdateExpInfo(dragonItems, goldItems);

	for (auto iter = dragonItems.begin(); iter != dragonItems.end(); ++iter)
	{
		KKSG::ItemBrief* brief = temp.add_dragoncoinfindbackitems();
		brief->set_itemid(iter->first);
		brief->set_itemcount(iter->second);
	}
	for (auto iter = goldItems.begin(); iter != goldItems.end(); ++iter)
	{
		KKSG::ItemBrief* brief = temp.add_goldcoinfindbackitems();
		brief->set_itemid(iter->first);
		brief->set_itemcount(iter->second);
	}

	return true;
}

bool OneDayTowerInfo::GetOnceFindBackCost(Role* role, std::map<int, int>& dragonItems, std::map<int, int>& goldItems, int& dragonCost, int& goldCost)
{
	int ownCostDragonCoinNum = 0;
	int ownCostGoldNum = 0;
	int costDragon = 0;
	int costGold = 0;
	int exchangeAddGold = 0;
	if (!GetLeftFindBackCount(role))
	{
		return false;
	}
	
	auto tableData = TowerConfig::Instance()->GetTowerReward(DEFAULT_HARDLEVEL, 0, m_subTypeInfos->m_towerLevel, dragonItems);
	if (tableData)
	{

		dragonCost = tableData->DragonCoinFindBackCost;
		goldCost = tableData->GoldCoinCost;
		

		for (auto i = tableData->GoldCoinFindBackItem.begin(); i != tableData->GoldCoinFindBackItem.end(); ++i)
		{
			goldItems[i->seq[0]] += i->seq[1];
		}

		CheckUpdateExpInfo(dragonItems, goldItems);
	}
	else
		return false;

	return true;
}

