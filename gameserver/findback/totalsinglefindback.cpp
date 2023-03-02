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
#include "totalsinglefindback.h"


OneDayTotalSingleSubtypeInfo::OneDayTotalSingleSubtypeInfo(BaseFindBackInfo* baseFindBack, int type, int day):OneDayTypeInfo(baseFindBack, type, day)
{
}

int OneDayTotalSingleSubtypeInfo::GetOneDayMaxCount(Role* role, int type)
{
	if (role && XCommon::IsDaySame(role->GetCreateTime(), m_day))
	{
		if (type == KKSG::FATIGUE_RECOVER)
		{
			if (GetGlobalConfig().FatigueRegeneration <= 0)
			{
				return 0;
			}
			int temp = (XCommon::GetDayBeginTime(m_day) + XCommon::GetOneDayTime() - role->GetCreateTime()) / 60 / GetGlobalConfig().FatigueRegeneration;
			int max = OneDayTypeInfo::GetOneDayMaxCount(role, type);

			return temp > max ? max : temp;
		}
	}

	return OneDayTypeInfo::GetOneDayMaxCount(role, type);
}

TotalSigleSubtypeFindBackInfo::TotalSigleSubtypeFindBackInfo(int type, CItemFindBackInfo* info):BaseFindBackInfo(type, info)
{

}

UINT32 TotalSigleSubtypeFindBackInfo::CalcFindBackRewardAndCost(int type, int count, std::map<int, int>& costs, std::map<int, int>& rewards)
{
	int costDragon = 0;
	int costGold = 0;
	int exchangeAddGold = 0;
	int cost = 0;

	std::map<int, int> dragonItems;
	std::map<int, int> goldItems;
	int goldcost = 0;
	int dragoncost = 0;
	if(!GetOneFindBackCost(dragonItems, goldItems, dragoncost, goldcost))
	{
		return KKSG::ERR_FAILED;
	}
	if (type == FindBack_Dragon)
	{
		cost = dragoncost * count;
		for (auto iter = dragonItems.begin(); iter != dragonItems.end(); ++iter)
		{
			rewards[iter->first] += iter->second * count;
		}
	}
	else if (type == FindBack_Gold)
	{
		cost = goldcost * count;
		for (auto iter = goldItems.begin(); iter != goldItems.end(); ++iter)
		{
			rewards[iter->first] += iter->second * count;
		}
	}

	costs[type] = cost;
	
	return KKSG::ERR_SUCCESS;
}

UINT32 TotalSigleSubtypeFindBackInfo::GetCanFindBackCount()
{
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (!data)
	{
		return 0;
	}
	if (data->ItemDragonCoin.empty())
	{
		return 0;
	}
	int temp = 0;
	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
	{
		temp += iter->second->GetLeftFindBackCount(GetRole());
	}

	if (!data->ItemDragonCoin[0].seq[1])
	{
		return 0;
	}
	return temp / data->ItemDragonCoin[0].seq[1];
}

void TotalSigleSubtypeFindBackInfo::AddAlreadyFindBackCount(int count)
{
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (!data)
	{
		return;
	}
	if (data->ItemDragonCoin.empty())
	{
		return;
	}

	count *= data->ItemDragonCoin[0].seq[1];
	
	for (auto iter = m_usedCountMap_h.rbegin(); iter != m_usedCountMap_h.rend() && count > 0; ++iter)
	{
		int temp = iter->second->GetLeftFindBackCount(GetRole());
		if (!temp)
		{
			continue;
		}
		if (temp >= count)
		{
			iter->second->m_alreadyFindBackCount += count;
			count = 0;
		}
		else
		{
			iter->second->m_alreadyFindBackCount += temp;
			count -= temp;
		}
	}

}

OneDayTypeInfo* TotalSigleSubtypeFindBackInfo::CreateOneDayBackInfo(int day)
{
	return new OneDayTotalSingleSubtypeInfo(this, m_type, day);
}

bool TotalSigleSubtypeFindBackInfo::GetOneFindBackCost(std::map<int, int>& dragonItems, std::map<int, int>& goldItems, int& dragonCost, int& goldCost)
{
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (!data)
	{
		return false;
	}
	if (m_usedCountMap_h.empty())
	{
		return false;
	}
	if (data->ItemDragonCoin.empty())
	{
		return false;
	}

	goldCost = data->CostGold;
	dragonCost = data->CostDragonCoin;

	for (int i = 0; i < data->ItemDragonCoin.size(); ++i)
	{
		dragonItems[data->ItemDragonCoin[i].seq[0]] += data->ItemDragonCoin[i].seq[1];
	}
	
	for (int i = 0; i < data->ItemGold.size(); ++i)
	{
		goldItems[data->ItemGold[i].seq[0]] += data->ItemGold[i].seq[1];
	}

	//仅一天的情况
	auto& ref = m_usedCountMap_h.begin()->second->m_onceBackExp;
	if (ref.find(DRAGON_COIN) != ref.end() && dragonItems.find(EXP) != dragonItems.end())
	{
		dragonItems[EXP] = ref[DRAGON_COIN];
	}
	if (ref.find(GOLD) != ref.end() && goldItems.find(EXP) != goldItems.end())
	{
		goldItems[EXP] = ref[GOLD];
	}

	return true;
}

void TotalSigleSubtypeFindBackInfo::GenerateFindBackInfo(KKSG::ItemFindBackInfoRes& roRes)
{
	if (m_usedCountMap_h.empty())
	{
		return;
	}
	int totalLeft = 0;
	int totalAlreadyFindBack = 0;
	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
	{
		totalAlreadyFindBack += iter->second->m_alreadyFindBackCount;
		totalLeft += iter->second->GetLeftFindBackCount(GetRole());
	}
	if (!totalLeft && !totalAlreadyFindBack)
	{
		return;
	}
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (!data)
	{
		return;
	}
	if (data->ItemDragonCoin.empty())
	{
		return;
	}
	if (!data->ItemDragonCoin[0].seq[1])
	{
		return;
	}
	int left = totalLeft / data->ItemDragonCoin[0].seq[1];

	KKSG::ItemFindBackInfo2Client temp;

	int totalGold = 0;
	int totalDragon = 0;

	std::map<int, int> dragonItems;
	std::map<int, int> goldItems;

	if (!GetOneFindBackCost(dragonItems, goldItems,totalDragon, totalGold))
	{
		return;
	}

	temp.set_id((KKSG::ItemFindBackType)m_type);
	temp.set_findbackcount(left);
	temp.set_daytime(0);
	temp.set_rolelevel(m_usedCountMap_h.begin()->second->m_level);

	temp.set_dragoncoincost(totalDragon);
	temp.set_goldcoincost(totalGold);
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

	KKSG::ItemFindBackInfo2Client* info = roRes.add_backinfo();
	info->CopyFrom(temp);

	return;
}



FatigueRecoverFindBackInfo::FatigueRecoverFindBackInfo(int type, CItemFindBackInfo* info) : TotalSigleSubtypeFindBackInfo(type, info)
{
}

UINT32 FatigueRecoverFindBackInfo::CalcFindBackRewardAndCost(int type, int count, std::map<int, int>& costs, std::map<int, int>& rewards)
{
	if (m_usedCountMap_h.empty())
	{
		return KKSG::ERR_FAILED;
	}
	int costDragon = 0;
	int costGold = 0;
	int exchangeAddGold = 0;
	int cost = 0;

	std::map<int, int> dragonItems;
	std::map<int, int> goldItems;
	int goldcost = 0;
	int dragoncost = 0;
	int totalAlreadyFindBack = 0;
	int totalLeft = 0;
	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
	{
		std::map<int, int> tempDragonItems;
		std::map<int, int> tempGoldItems;
		int tempDragon = 0;
		int tempGold = 0;
		totalAlreadyFindBack += iter->second->m_alreadyFindBackCount;
		totalLeft += iter->second->GetLeftFindBackCount(GetRole());
		iter->second->GetOnceFindBackCost(GetRole(), tempDragonItems, tempGoldItems, tempDragon, tempGold);
		iter->second->CheckUpdateExpInfo(tempDragonItems, tempGoldItems);
		for (auto i = tempDragonItems.begin(); i != tempDragonItems.end(); ++i)
		{
			dragonItems[i->first] += i->second;
		}
		for (auto i = tempGoldItems.begin(); i != tempGoldItems.end(); ++i)
		{
			goldItems[i->first] += i->second;
		}
		dragoncost += tempDragon;
		goldcost += tempGold;
	}
	if (!totalLeft && !totalAlreadyFindBack)
	{
		return KKSG::ERR_FAILED;
	}

	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	if (data->ItemDragonCoin.empty())
	{
		return KKSG::ERR_FAILED;
	}

	if (type == FindBack_Dragon)
	{
		cost = dragoncost * count;
		for (auto iter = dragonItems.begin(); iter != dragonItems.end(); ++iter)
		{
			rewards[iter->first] += iter->second * count;
		}
	}
	else if (type == FindBack_Gold)
	{
		cost = goldcost * count;
		for (auto iter = goldItems.begin(); iter != goldItems.end(); ++iter)
		{
			rewards[iter->first] += iter->second * count;
		}
	}

	costs[type] = cost;

	return KKSG::ERR_SUCCESS;
}

UINT32 FatigueRecoverFindBackInfo::GetCanFindBackCount()
{
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (!data)
	{
		return 0;
	}
	if (data->ItemDragonCoin.empty())
	{
		return 0;
	}
	int temp = 0;
	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
	{
		temp += iter->second->GetLeftFindBackCount(GetRole());
	}

	if (temp)
	{
		return 1;
	}

	return 0;
}

void FatigueRecoverFindBackInfo::AddAlreadyFindBackCount(int count)
{
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (!data)
	{
		return;
	}
	if (data->ItemDragonCoin.empty())
	{
		return;
	}

	for (auto iter = m_usedCountMap_h.rbegin(); iter != m_usedCountMap_h.rend() && count > 0; ++iter)
	{
		int temp = iter->second->GetLeftFindBackCount(GetRole());
		if (!temp)
		{
			continue;
		}

		iter->second->m_alreadyFindBackCount += temp;
	}

}

OneDayTypeInfo* FatigueRecoverFindBackInfo::CreateOneDayBackInfo(int day)
{
	return new OneDayFatigueRecoverInfo(this, m_type, day);
}

void FatigueRecoverFindBackInfo::GenerateFindBackInfo(KKSG::ItemFindBackInfoRes& roRes)
{
	if (m_usedCountMap_h.empty())
	{
		return;
	}
	int totalLeft = 0;
	int totalAlreadyFindBack = 0;

	int dragonCost = 0;
	int goldCost = 0;
	std::map<int,int> dragonItems;
	std::map<int,int> goldItems;
	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
	{
		std::map<int,int> tempDragonItems;
		std::map<int,int> tempGoldItems;
		int tempDragon = 0;
		int tempGold = 0;
		totalAlreadyFindBack += iter->second->m_alreadyFindBackCount;
		totalLeft += iter->second->GetLeftFindBackCount(GetRole());
		iter->second->GetOnceFindBackCost(GetRole(), tempDragonItems, tempGoldItems, tempDragon, tempGold);
		iter->second->CheckUpdateExpInfo(tempDragonItems, tempGoldItems);
		for (auto i = tempDragonItems.begin(); i != tempDragonItems.end(); ++i)
		{
			dragonItems[i->first] += i->second;
		}
		for (auto i = tempGoldItems.begin(); i != tempGoldItems.end(); ++i)
		{
			goldItems[i->first] += i->second;
		}
		dragonCost += tempDragon;
		goldCost += tempGold;
	}
	if (!totalLeft && !totalAlreadyFindBack)
	{
		return;
	}

	KKSG::ItemFindBackInfo2Client temp;

	temp.set_id((KKSG::ItemFindBackType)m_type);
	temp.set_findbackcount(!totalLeft&& totalAlreadyFindBack ? 0 : 1);
	temp.set_daytime(0);
	temp.set_rolelevel(m_usedCountMap_h.begin()->second->m_level);

	if (dragonItems.empty())
	{
		//给客户端置灰使用 下次版本需要调整置灰规则
		auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type, m_usedCountMap_h.begin()->second->m_level);
		if (!data)
		{
			return;
		}

		for (auto iter = data->ItemDragonCoin.begin(); iter != data->ItemDragonCoin.end(); ++iter)
		{
			dragonItems[iter->seq[0]] = 0;
		}
		for (auto iter = data->ItemGold.begin(); iter != data->ItemGold.end(); ++iter)
		{
			goldItems[iter->seq[0]] = 0;
		}
	}
	//m_usedCountMap_h.begin()->second->CheckUpdateExpInfo(dragonItems, goldItems);

	temp.set_dragoncoincost(dragonCost);
	temp.set_goldcoincost(goldCost);
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

	KKSG::ItemFindBackInfo2Client* info = roRes.add_backinfo();
	info->CopyFrom(temp);

	return;

}

OneDayFatigueRecoverInfo::OneDayFatigueRecoverInfo(BaseFindBackInfo* baseFindBack, int type, int day):OneDayTotalSingleSubtypeInfo(baseFindBack, type, day)
{
}

bool OneDayFatigueRecoverInfo::GetOnceFindBackCost(Role* role, std::map<int, int>& dragonItems, std::map<int, int>& goldItems, int& dragonCost, int& goldCost)
{
	int leftcount = GetLeftFindBackCount(m_pBaseFindBack->GetRole());
	if (!leftcount)
	{
		return false;
	}
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type, m_level);
	if (!data)
	{
		return false;
	}

	goldCost = data->CostGold * leftcount / 10 + (data->CostGold * leftcount % 10 ? 1 : 0);
	dragonCost = data->CostDragonCoin * leftcount / 10 + (data->CostDragonCoin* leftcount % 10 ? 1 : 0);

	for (auto iter = data->ItemDragonCoin.begin(); iter != data->ItemDragonCoin.end(); ++iter)
	{
		dragonItems[iter->seq[0]] = iter->seq[1]* leftcount/10;
	}
	for (auto iter = data->ItemGold.begin(); iter != data->ItemGold.end(); ++iter)
	{
		goldItems[iter->seq[0]] = iter->seq[1]* leftcount/10;
	}
	
	return true;
}

DiceFindBackInfo::DiceFindBackInfo(int type, CItemFindBackInfo* info) : TotalSigleSubtypeFindBackInfo(type, info)
{
}
