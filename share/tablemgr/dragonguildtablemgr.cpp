#include "pch.h"
#include "dragonguildtablemgr.h"
#include "util/XRandom.h"
#include "table/globalconfig.h"
#include "foreach.h"

DragonGuildTableMgr::DragonGuildTableMgr()
{

}

DragonGuildTableMgr::~DragonGuildTableMgr()
{

}

bool DragonGuildTableMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void DragonGuildTableMgr::Uninit()
{
	ClearFile();
}

bool DragonGuildTableMgr::CheckFile()
{
	DragonGuildTable dragon;
	if (!dragon.LoadFile("table/DragonGuild.txt"))
	{
		LogError("load table/DragonGuild.txt failed.");
		return false;
	}

	DragonGuildConfigTable configTable;
	if(!configTable.LoadFile("table/DragonGuildConfig.txt"))
	{
		LogError("load table/DragonGuildConfig.txt failed.");
		return false;
	}

	DragonGuildPermissionTable permissTable;
	if (!permissTable.LoadFile("table/DragonGuildPermission.txt"))
	{
		LogError("load table/DragonGuildPermission.txt failed.");
		return false;
	}

	DragonGuildLivenessTable tmp2;
	if (!tmp2.LoadFile("table/DragonGuildLiveness.txt"))
	{
		LogError("load table/DragonGuildLiveness.txt failed");
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
	DragonGuildTaskTable tmp5;
	if (!tmp5.LoadFile("table/DragonGuildTask.txt"))
	{
		LogError("load table/DragonGuildTask.txt");
		return false;
	}
	DragonGuildAchieveTable tmp6;
	if (!tmp6.LoadFile("table/DragonGuildAchieve.txt"))
	{
		LogError("load table/DragonGuildAchieve.txt");
		return false;
	}
	return true;
}

bool DragonGuildTableMgr::LoadFile()
{
	if (!CheckFile())	
	{
		return false;
	}
	ClearFile();
	m_dragonguild.LoadFile("table/DragonGuild.txt");
	m_dragonguildconfig.LoadFile("table/DragonGuildConfig.txt");
	m_permissionconfig.LoadFile("table/DragonGuildPermission.txt");
	m_liveness.LoadFile("table/DragonGuildLiveness.txt");
	m_shop.LoadFile("table/Shop.txt");
	m_shopType.LoadFile("table/ShopType.txt");
	//老公会新增讨伐任务将在下一周期刷出
	m_task.LoadFile("table/DragonGuildTask.txt");
	//老公会新成就会在第二天补充
	m_achivement.LoadFile("table/DragonGuildAchieve.txt");
	
	for (size_t i = 0; i < m_task.Table.size(); i++)
	{
		DragonGuildTaskTable::RowData* data = m_task.Table[i];
		m_taskScene2id[data->SceneID].push_back(data);
		
	}

	for (size_t i = 0; i < m_achivement.Table.size(); i++)
	{
		DragonGuildAchieveTable::RowData* data = m_achivement.Table[i];
		m_achieveScene2id[data->SceneID].push_back(data);
	}
	

	for (size_t i = 0; i < m_liveness.Table.size(); i++)
	{
		DragonGuildLivenessTable::RowData* data = m_liveness.Table[i];
		for (UINT32 j = data->level[0]; j <= data->level[1]; j++)
		{
			m_livenessChest[j][data->index] = data;
		}
	}

	for (size_t i = 0; i < m_shop.Table.size(); i++)
	{
		ShopTable::RowData* data = m_shop.Table[i];
		if (data->Type != GetGlobalConfig().DragonGuildShopID)
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
		if (data->ShopID == GetGlobalConfig().DragonGuildShopID)
		{
			m_shopRefreshTime = data->ShopCycle;
		}
	}

	return true;
}

void DragonGuildTableMgr::ClearFile()
{
	m_dragonguild.Clear();
	m_dragonguildconfig.Clear();
	m_permissionconfig.Clear();
	m_task.Clear();
	m_achivement.Clear();
	m_liveness.Clear();
	m_shopType.Clear();
	m_shop.Clear();

	m_shopRefreshTime.clear();
	m_degree2level.clear();
	m_livenessChest.clear();
	m_shopRandMap.clear();
	m_taskScene2id.clear();
	m_achieveScene2id.clear();
}

UINT32 DragonGuildTableMgr::GetLevel(UINT32 degree)
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

DragonGuildConfigTable::RowData* DragonGuildTableMgr::GetDragonGuildConfig(UINT32 level)
{
	return m_dragonguildconfig.GetByDragonGuildLevel(level);
}

bool DragonGuildTableMgr::HasNewCanTakChest(UINT32 level, UINT32 oldValue, UINT32 newValue)
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

DragonGuildLivenessTable::RowData* DragonGuildTableMgr::GetLivenessTable(UINT32 level, UINT32 index)
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

std::vector<UINT32> DragonGuildTableMgr::GetTaskChestList(KKSG::DragonGuildTaskType taskType, UINT32 taskID )
{
	std::vector<UINT32>vec;
	vec.clear();
	if(taskType == KKSG::TASK_NORMAL)
	{
		DragonGuildTaskTable::RowData* data = m_task.GetBytaskID(taskID);
		if(data)
		{
			return data->dropID;
		}
	}
	else if(taskType == KKSG::TASK_ACHIVEMENT)
	{
		DragonGuildAchieveTable::RowData* data = m_achivement.GetByID(taskID);
		if(data)
		{
			return data->dropID;
		}
	}
	return vec;
}

void DragonGuildTableMgr::GetCanTakeChestIndex(UINT32 level, UINT32 liveness, std::vector<UINT32>& index)
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

void DragonGuildTableMgr::AddRandomItem(UINT32 level, UINT32 slot, UINT32 rate, UINT32 shopId)
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

void DragonGuildTableMgr::RandomShop(UINT32 level, std::vector<UINT32>& shopItems)
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

void DragonGuildTableMgr::RandomTask(UINT32 worldLevel, std::vector<UINT32>& taskList)
{
	vector<UINT32>randList;
	randList.clear();
	for(auto it = m_task.Table.begin(); it != m_task.Table.end(); it++)
	{
		if((*it)->worldlevel[0] < worldLevel && (*it)->worldlevel[1] > worldLevel)
			randList.push_back((*it)->taskID);
	}
	UINT32 count = GetGlobalConfig().DragonGuildWeekTaskCount;
	count = count < randList.size()? count:randList.size();
	//随机取count个任务
	for(UINT32 i = 0; i< count; ++i)            
	{
		UINT32 randnum = XRandom::randInt(0, randList.size());
		taskList.push_back( randList[randnum] );
		auto it = randList.begin()+randnum;
		randList.erase(it);
	}
}

void DragonGuildTableMgr::GetAchiveList(std::vector<UINT32>& achiveList)
{
	for(size_t i = 0; i < m_achivement.Table.size(); i++)
	{
		DragonGuildAchieveTable::RowData* data = m_achivement.Table[i];
		achiveList.push_back(data->ID);
	}
}

void DragonGuildTableMgr::GetTaskList(std::vector<UINT32>& taskList)
{
	for(size_t i = 0; i < m_task.Table.size(); i++)
	{
		DragonGuildTaskTable::RowData* data = m_task.Table[i];
		taskList.push_back(data->taskID);
	}
}



UINT32 DragonGuildTableMgr::RandomOne(UINT32 level, UINT32 slot)
{
	UINT32 key = level * 100 + slot;
	auto it = m_shopRandMap.find(key);
	if (it == m_shopRandMap.end())
	{
		return 0;
	}
	return RandomOne(it->second);
}

UINT32 DragonGuildTableMgr::RandomOne(std::map<UINT32, UINT32>& randMap)
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

ShopTable::RowData* DragonGuildTableMgr::GetDragonGuildShopItem(UINT32 id)
{
	return m_shop.GetByID(id);
}

UINT32 DragonGuildTableMgr::GetCostNum(ShopTable::RowData* pData)
{
	double percent = 1.0f;
	if (pData->Benefit[0] == 2)
	{
		percent = (double)pData->Benefit[1] / (double)(100);
	}
	return (UINT32)(pData->ConsumeItem[1] * percent);
}

DragonGuildTaskTable::RowData* DragonGuildTableMgr::GetTaskRow(UINT32 taskID)
{
	return m_task.GetBytaskID(taskID);
}
DragonGuildAchieveTable::RowData* DragonGuildTableMgr::GetAchiveRow(UINT32 taskID)
{
	return m_achivement.GetByID(taskID);
}