#include "pch.h"
#include "findbackrecord.h"
#include "itemfindback.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "findbackconfig.h"
#include "foreach.h"
#include "unit/systeminfo.h"
#include "nestfindback.h"
#include "gamelogic/nestconfig.h"
#include "util/XCommon.h"
#include "itemfindbackbase.h"
#include "gamelogic/rolemisc.h"

OneDayNestInfo::OneDayNestInfo(BaseFindBackInfo* baseFindBackInfo, int day):OneDayTypeInfo(baseFindBackInfo, KKSG::NestBack, day)
{

}

bool OneDayNestInfo::ConvertSingleSubtype(Role* role, KKSG::ItemFindBackInfo2Client& temp, int leftCount)
{
	std::map<int, int> dragonItems;
	std::map<int, int> goldItems;
	int totalGold = 0;
	int totalDragon = 0;

	int left = leftCount;

	UINT32 expID = m_subTypeInfos->m_towerLevel;
	if(!expID)
		return false;
	auto tableData = NestConfig::Instance()->GetNestListTableData(expID);
	if (tableData)
	{
		for (auto i = tableData->FindBackItems.begin(); i != tableData->FindBackItems.end(); ++i)
		{
			dragonItems[i->seq[0]] += i->seq[1];
		}
	}
	else
		return false;

	auto itemBackData = FindBackConfig::Instance()->GetItemBackTableRowData(KKSG::NestBack, 1);
	if (!itemBackData)
	{
		return false;
	}

	totalDragon = itemBackData->CostDragonCoin;

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

	return true;
}


NestFindBackInfo::NestFindBackInfo(CItemFindBackInfo* info):BaseFindBackInfo(KKSG::NestBack, info)
{
}

//void NestFindBackInfo::AddUseCount(int count, int dayBegin)
//{
//	int isToday = XCommon::IsDaySame(dayBegin, time(0));
//	UINT32 nestExpID = GetRole()->Get<RoleMisc>()->GetPassedHardestNestExpID();
//
//	if (m_usedCountMap_h.find(dayBegin) != m_usedCountMap_h.end())
//	{
//		m_usedCountMap_h[dayBegin]->m_subTypeInfos->m_todayUseCount += count;
//		m_usedCountMap_h[dayBegin]->m_subTypeInfos->m_towerLevel = nestExpID;
//
//		return;
//	}
//	if (!isToday)
//	{
//		OneDayTypeInfo * info = CreateOneDayBackInfo(dayBegin);
//		info->m_subTypeInfos->m_todayUseCount = count;
//		info->m_subTypeInfos->m_towerLevel = nestExpID;
//		m_usedCountMap_h[dayBegin] = info;
//		return;
//	}
//
//	m_usedCountMap->m_subTypeInfos->m_todayUseCount += count;
//	m_usedCountMap->m_subTypeInfos->m_towerLevel = nestExpID;
//}

OneDayTypeInfo* NestFindBackInfo::CreateOneDayBackInfo(int day)
{
	return new OneDayNestInfo(this, day);
}

UINT32 NestFindBackInfo::GetOneDayInfoParam()
{
	UINT32 nestExpID = GetRole()->Get<RoleMisc>()->GetPassedHardestNestExpID();
	return nestExpID;
}

