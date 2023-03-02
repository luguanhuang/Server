#include "pch.h"
#include "backflowtablemgr.h"



bool BackFlowTableData::HandlerTableData()
{
	if (!m_serverConfig.LoadFile("table/BackFlowServerConfig.txt"))
	{
		LogWarn("Load BackFlowServerConfig.txt Failed");
		return false;
	}
	if (!m_equipTable.LoadFile("table/BackFlowEquip.txt"))
	{
		LogWarn("Load BackFlowEquip.txt Failed");
		return false;
	}
	for (auto i = m_equipTable.Table.begin(); i != m_equipTable.Table.end(); ++i)
	{
		m_equipMap[std::make_pair((*i)->ServerLevel, (*i)->Profession)].push_back(*i);
	}
	if (!m_emblemTable.LoadFile("table/BackFlowEmblem.txt"))
	{
		LogWarn("Load BackFlowEmblem.txt Failed");
		return false;
	}
	if (!m_spriteTable.LoadFile("table/BackFlowSprite.txt"))
	{
		LogWarn("Load BackFlowSprite.txt Failed");
		return false;
	}

	return true;
}

bool BackFlowTableMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void BackFlowTableMgr::Uninit()
{

}

bool BackFlowTableMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandlerTableData();
	return true;
}

bool BackFlowTableMgr::CheckFile()
{
	BackFlowTableData tmpData;
	return tmpData.HandlerTableData();
}

void BackFlowTableMgr::ClearFile()
{
	m_serverConfig.Clear();
	m_equipTable.Clear();
	m_equipMap.clear();
	m_emblemTable.Clear();
	m_spriteTable.Clear();
}

BackFlowServerConfig::RowData* BackFlowTableMgr::GetServerConfig(UINT32 profession, UINT32 level)
{
	UINT32 basicPro =  profession % 10;
	BackFlowServerConfig::RowData* pData = NULL;
	for (auto it = m_serverConfig.Table.begin(); it != m_serverConfig.Table.end(); ++it)
	{
		if ((*it)->Profession != basicPro)
		{
			continue;
		}
		pData = *it;
		if (pData->ServerLevel >= level)
		{
			return pData;
		}
	}
	return pData;
}

BackFlowServerConfig::RowData* BackFlowTableMgr::GetServerConfigFloor(UINT32 profession, UINT32 level)
{
	UINT32 basicPro =  profession % 10;
	BackFlowServerConfig::RowData* pData = NULL;
	for (auto it = m_serverConfig.Table.rbegin(); it != m_serverConfig.Table.rend(); ++it)
	{
		if ((*it)->Profession != basicPro)
		{
			continue;
		}
		pData = *it;
		if (pData->ServerLevel <= level)
		{
			return pData;
		}
	}
	return pData;
}

BackFlowServerConfig::RowData* BackFlowTableMgr::GetServerConfigByLevel(UINT32 level)
{
	for (auto it = m_serverConfig.Table.begin(); it != m_serverConfig.Table.end(); ++it)
	{
		BackFlowServerConfig::RowData* pData = *it;
		if (pData->ServerLevel >= level)
		{
			return pData;
		}
	}
	if (m_serverConfig.Table.empty())
	{
		return NULL;
	}
	return *m_serverConfig.Table.rbegin();
}

const std::vector<BackFlowEquipConf*>* BackFlowTableMgr::GetEquipConf(UINT32 level, UINT32 profession)
{
	auto iter = m_equipMap.find(std::make_pair(level, profession));
	if (iter != m_equipMap.end())
	{
		return &iter->second;
	}
	return NULL;
}

const BackFlowEmblemConf* BackFlowTableMgr::GetEmblemConf(UINT32 level, UINT32 profession)
{
	for (auto i = m_emblemTable.Table.begin(); i != m_emblemTable.Table.end(); ++i)
	{
		if ((*i)->ServerLevel == level && (*i)->Profession == profession)
		{
			return *i;
		}
	}
	return NULL;
}

const BackFlowSpriteConf* BackFlowTableMgr::GetSpriteConf(UINT32 level, UINT32 profession)
{
	for (auto i = m_spriteTable.Table.begin(); i != m_spriteTable.Table.end(); ++i)
	{
		if ((*i)->ServerLevel == level && (*i)->Profession == profession)
		{
			return *i;
		}
	}
	return NULL;
}





