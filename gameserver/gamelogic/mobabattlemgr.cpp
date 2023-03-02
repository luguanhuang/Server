#include "pch.h"
#include "mobabattlemgr.h"

MobaTableData::MobaTableData()
{
	m_maxLevel = 1;
	m_mobMaxLevel = 0;
}

MobaTableData::~MobaTableData()
{

}

bool MobaTableData::HandlerTableData()
{
	if (!m_levelTable.LoadFile("table/MobaLevel.txt"))
	{
		LogWarn("Load MobaLevel.txt failed");
		return false;
	}
	if (m_levelTable.Table.empty())
	{
		LogWarn("MobaLevel.txt is empty");
		return false;
	}
	for (auto i = m_levelTable.Table.begin(); i != m_levelTable.Table.end(); i++)
	{
		m_levelMap[(*i)->Level] = *i;
	}
	m_maxLevel = 0;
	for (auto i = m_levelMap.begin(); i != m_levelMap.end(); i++)
	{
		m_maxLevel++;
		if (i->second->Level != m_maxLevel)
		{
			LogWarn("MobaLevel.txt's level is not continuous");
			return false;
		}	
	}

	if (!m_heroTable.LoadFile("table/MobaHero.txt"))
	{
		LogWarn("Load MobaHero.txt failed");
		return false;
	}
	for (auto i = m_heroTable.Table.begin(); i != m_heroTable.Table.end(); i++)
	{
		m_heroMap[(*i)->HeroID][(*i)->Level] = *i;
	}
	for (auto i = m_heroMap.begin(); i != m_heroMap.end(); i++)
	{
		if (i->second.size() != m_maxLevel ||
			i->second.begin()->first != 1 ||
			(--i->second.end())->first != m_maxLevel)
		{
			LogWarn("MobaHero.txt, HeroID [%u], level error", i->first);
			return false;
		}
	}

	if (!m_killExpTable.LoadFile("table/MobaKillExp.txt"))
	{
		LogWarn("Load MobaKillExp.txt failed");
		return false;
	}

	for (auto i = m_killExpTable.Table.begin(); i != m_killExpTable.Table.end(); i++)
	{
		m_killExpMap[std::make_pair((*i)->AssistNum, (*i)->RangeNum)] = *i;
	}

	if (!m_mobtable.LoadFile("table/MobaMob.txt"))
	{
		LogWarn("Load MobaMob.txt failed");
		return false;
	}

	m_group2mobInfo[MOBA_GROUP_1].group = MOBA_GROUP_1;
	m_group2mobInfo[MOBA_GROUP_2].group = MOBA_GROUP_2;
	MobaMobInfo& info1 = m_group2mobInfo[MOBA_GROUP_1];
	MobaMobInfo& info2 = m_group2mobInfo[MOBA_GROUP_2];

	for (auto i = m_mobtable.Table.begin(); i != m_mobtable.Table.end(); ++i)
	{
		MobaMobTable::RowData* rowData = *i;
		UINT32 level = rowData->level << 4;
		for (auto j = rowData->moba.begin(); j != rowData->moba.end(); ++j)
		{
			UINT32 key = level | (*j)[0];
			UINT32 value = (*j)[1];
			info1.mobIDs[key] = value;
		}
		for (auto j = rowData->mobb.begin(); j != rowData->mobb.end(); ++j)
		{
			UINT32 key = level | (*j)[0];
			UINT32 value = (*j)[1];
			info2.mobIDs[key] = value;
		}
		m_mobMaxLevel = std::max(m_mobMaxLevel, rowData->level);
	}

	if (!m_miniMapTable.LoadFile("table/MobaMiniMap.txt"))
	{
		LogWarn("Load MobaMiniMap.txt failed");
		return false;
	}
	if (m_miniMapTable.Table.size() != m_miniMapTable.PosIndex_Index.size())
	{
		LogWarn("MobaMiniMap.txt, PosIndex is duplicate key");
		return false;
	}
	for (auto i = m_miniMapTable.Table.begin(); i != m_miniMapTable.Table.end(); ++i)
	{
		if ((*i)->PosIndex == 0 || (*i)->Number == 0)
		{
			LogWarn("MobaMiniMap.txt, PosIndex or Number error");
			return false;
		}
	}

	return true;
}

MobaBattleMgr::MobaBattleMgr()
{

}

MobaBattleMgr::~MobaBattleMgr()
{

}

bool MobaBattleMgr::Init()
{
	return HandlerTableData();
}

void MobaBattleMgr::Uninit()
{
	
}

bool MobaBattleMgr::CheckFile()
{
	MobaTableData tmpTableData;
	return tmpTableData.HandlerTableData();
}

bool MobaBattleMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandlerTableData();
	
	return true;
}

void MobaBattleMgr::ClearFile()
{
	m_maxLevel = 1;
	m_levelTable.Clear();
	m_levelMap.clear();
	m_heroTable.Clear();
	m_heroMap.clear();
	m_killExpTable.Clear();
	m_killExpMap.clear();
	m_mobMaxLevel = 0;
	m_mobtable.Clear();
	m_group2mobInfo.clear();
	m_miniMapTable.Clear();
}

UINT32 MobaBattleMgr::GetMobaMaxLevel()
{
	return m_maxLevel;
}

const MobaLevelConf* MobaBattleMgr::GetMobaLevelConf(UINT32 level)
{
	auto iter = m_levelMap.find(level);
	if (iter != m_levelMap.end())
	{
		return iter->second;
	}
	return NULL;
}

const MobaHeroConf* MobaBattleMgr::GetMobaHeroConf(UINT32 heroid, UINT32 level)
{
	auto iter = m_heroMap.find(heroid);
	if (iter == m_heroMap.end())
	{
		return NULL;
	}
	auto iter2 = iter->second.find(level);
	return iter2 == iter->second.end() ? NULL : iter2->second;
}

const std::map<UINT32, MobaHeroConf*>* MobaBattleMgr::GetOneMobaHeroConf(UINT32 heroid)
{
	auto iter = m_heroMap.find(heroid);
	if (iter == m_heroMap.end())
	{
		return NULL;
	}
	return &iter->second;
}

const MobaKillExpConf* MobaBattleMgr::GetMobaKillExpConf(UINT32 assistNum, UINT32 rangeNum)
{
	auto iter = m_killExpMap.find(std::make_pair(assistNum, rangeNum));
	if (iter != m_killExpMap.end())
	{
		return iter->second;
	}
	return NULL;
}

UINT32 MobaBattleMgr::GetMobID(UINT32 group, UINT32 level, UINT32 type)
{
	auto it = m_group2mobInfo.find(group);
	if (it == m_group2mobInfo.end())
		return 0;

	UINT32 key = level << 4 | type;
	auto it2 = it->second.mobIDs.find(key);
	if (it2 == it->second.mobIDs.end())
		return 0;

	return it2->second;
}

const MobaMiniMapConf* MobaBattleMgr::GetMobaMiniMapConf(UINT32 posIndex)
{
	return m_miniMapTable.GetByPosIndex(posIndex);
}