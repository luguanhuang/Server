#include "pch.h"
#include "partnertablemgr.h"
#include "util/XRandom.h"
#include "table/globalconfig.h"

PartnerTableMgr::PartnerTableMgr()
{

}

PartnerTableMgr::~PartnerTableMgr()
{

}

bool PartnerTableMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void PartnerTableMgr::Uninit()
{
	ClearFile();
}

bool PartnerTableMgr::CheckFile()
{
	PartnerTable tmp1;
	if (!tmp1.LoadFile("table/Partner.txt"))
	{
		LogError("load table/Partner.txt failed");
		return false;
	}
	PartnerLivenessTable tmp2;
	if (!tmp2.LoadFile("table/PartnerLiveness.txt"))
	{
		LogError("load table/PartnerLiveness.txt failed");
		return false;
	}
	ShopTypeTable tmp3;
	if (!tmp3.LoadFile("table/ShopType.txt"))
	{
		LogError("load table/ShopType.txt failed");
		return false;
	}
	ShopTable tmp4;
	if (!tmp4.LoadFile("table/Shop.txt"))
	{
		LogError("load table/Shop.txt");
		return false;
	}
	return true;
}

bool PartnerTableMgr::LoadFile()
{
	if (!CheckFile())	
	{
		return false;
	}
	m_partner.LoadFile("table/Partner.txt");
	m_liveness.LoadFile("table/PartnerLiveness.txt");
	m_shop.LoadFile("table/Shop.txt");
	m_shopType.LoadFile("table/ShopType.txt");

	for (size_t i = 0; i < m_partner.Table.size(); i++)
	{
		PartnerTable::RowData* data = m_partner.Table[i];
		m_degree2level[data->degree] = data->level;
	}

	for (size_t i = 0; i < m_liveness.Table.size(); i++)
	{
		PartnerLivenessTable::RowData* data = m_liveness.Table[i];
		for (UINT32 j = data->level[0]; j <= data->level[1]; j++)
		{
			m_livenessChest[j][data->index] = data;
		}
	}

	for (size_t i = 0; i < m_shop.Table.size(); i++)
	{
		ShopTable::RowData* data = m_shop.Table[i];
		if (data->Type != GetGlobalConfig().PartnerShopId)
		{
			continue;
		}
		for (UINT32 j = data->LevelShow[0]; j <= data->LevelShow[1]; ++j)
		{
			AddRandomItem(j, data->Slot[0], data->Slot[1], data->ID);
		}
	}

	for (size_t i = 0; i < m_shopType.Table.size(); i++)
	{
		ShopTypeTable::RowData* data = m_shopType.Table[i];
		if (data->ShopID == GetGlobalConfig().PartnerShopId)
		{
			m_shopRefreshTime = data->ShopCycle;
		}
	}

	return true;
}

void PartnerTableMgr::ClearFile()
{
	m_partner.Clear();
	m_liveness.Clear();
	m_degree2level.clear();
	m_livenessChest.clear();
	m_shop.Clear();
	m_shopRandMap.clear();

	m_shopType.Clear();
	m_shopRefreshTime.clear();
}

UINT32 PartnerTableMgr::GetLevel(UINT32 degree)
{
	if (m_degree2level.empty())
	{
		return 1;
	}

	auto it = m_degree2level.upper_bound(degree);
	if (it == m_degree2level.begin())
	{
		return it->first;
	}
	it--;
	if (it == m_degree2level.end())
	{
		return m_degree2level.rbegin()->second;
	}
	return it->second;
}

PartnerTable::RowData* PartnerTableMgr::GetPartnerData(UINT32 level)
{
	return m_partner.GetBylevel(level);
}

bool PartnerTableMgr::HasNewCanTakChest(UINT32 level, UINT32 oldValue, UINT32 newValue)
{
	auto it = m_livenessChest.find(level);
	if (it == m_livenessChest.end())
	{
		return false; 
	}
	auto& m = it->second;
	for (auto i = m.begin(); i != m.end(); ++i)
	{
		auto data = i->second;
		if (oldValue < data->liveness && newValue >= data->liveness)
		{
			return true;
		}
	}
	return false;
}

PartnerLivenessTable::RowData* PartnerTableMgr::GetLivenessTable(UINT32 level, UINT32 index)
{
	auto it = m_livenessChest.find(level);
	if (it == m_livenessChest.end())
	{
		return NULL;
	}
	auto it2 = it->second.find(index);
	if (it2 == it->second.end())
	{
		return NULL;
	}
	return it2->second;
}

void PartnerTableMgr::GetCanTakeChestIndex(UINT32 level, UINT32 liveness, std::vector<UINT32>& index)
{
	auto it = m_livenessChest.find(level);
	if (it == m_livenessChest.end())
	{
		return ;
	}
	auto& m = it->second;
	for (auto i = m.begin(); i != m.end(); ++i)
	{
		auto data = i->second;
		if (liveness >= data->liveness)
		{
			index.push_back(data->index);
		}
	}
}

void PartnerTableMgr::AddRandomItem(UINT32 level, UINT32 slot, UINT32 rate, UINT32 shopId)
{
	UINT32 key = level * 100 + slot;
	auto& randMap = m_shopRandMap[key];
	UINT32 sumRate = 0;
	if (!randMap.empty())
	{
		sumRate = randMap.rbegin()->first;
	}
	sumRate += rate;
	randMap[sumRate] = shopId;
}

void PartnerTableMgr::RandomShop(UINT32 level, std::vector<UINT32>& shopItems)
{
	UINT32 key = level * 100;
	auto it = m_shopRandMap.lower_bound(key);
	for ( ; it != m_shopRandMap.end(); ++it)
	{
		if (it->first / 100 > level)
		{
			break;
		}
		UINT32 slot = it->first % 100;
		UINT32 shopItemId = RandomOne(it->second);
		shopItems.push_back(shopItemId);
	}
}

UINT32 PartnerTableMgr::RandomOne(UINT32 level, UINT32 slot)
{
	UINT32 key = level * 100 + slot;
	auto it = m_shopRandMap.find(key);
	if (it == m_shopRandMap.end())
	{
		return 0;
	}
	return RandomOne(it->second);
}

UINT32 PartnerTableMgr::RandomOne(std::map<UINT32, UINT32>& randMap)
{
	if (randMap.empty())	
	{
		return 0;
	}
	if (randMap.size() == 1)
	{
		return randMap.begin()->second;
	}

	UINT32 value = XRandom::randInt(0, randMap.rbegin()->first);
	auto it = randMap.upper_bound(value);
	if (it == randMap.end())
	{
		return randMap.rbegin()->second;
	}
	return it->second;
}

ShopTable::RowData* PartnerTableMgr::GetPartnerShopItem(UINT32 id)
{
	return m_shop.GetByID(id);
}

UINT32 PartnerTableMgr::GetCostNum(ShopTable::RowData* pData)
{
	double percent = 1.0f;
	if (pData->Benefit[0] == 2)
	{
		percent = (double)pData->Benefit[1] / (double)(100);
	}
	return (UINT32)(pData->ConsumeItem[1] * percent);
}
