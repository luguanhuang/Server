#include "pch.h"
#include "findbackconfig.h"
#include "foreach.h"

INSTANCE_SINGLETON(FindBackConfig)

FindBackConfig::FindBackConfig()
{
}

FindBackConfig::~FindBackConfig()
{

}

bool FindBackConfig::Init()
{
	return LoadFile();
}

void FindBackConfig::Uninit()
{

}

void FindBackConfig::ClearFile()
{
	m_oExpBackTable.Clear();
	m_oItemBackTable.Clear();
}

bool FindBackConfig::CheckFile()
{
	ExpBackTable tempExpBackTable;
	if (!tempExpBackTable.LoadFile("table/ExpBack.txt"))
	{
		LogWarn("Load ExpBack.txt failed!");
		return false;
	}
	ItemBackTable tempItemBackTable;
	if (!tempItemBackTable.LoadFile("table/ItemBack.txt"))
	{
		LogWarn("Load ItemBack.txt failed!");
		return false;
	}
	return true;
}

bool FindBackConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	if (!m_oExpBackTable.LoadFile("table/ExpBack.txt"))
	{
		LogWarn("Load ExpBack.txt failed!");
		return false;
	}

	if (!m_oItemBackTable.LoadFile("table/ItemBack.txt"))
	{
		LogWarn("Load ItemBack.txt failed!");
		return false;
	}

	return true;
}

const ExpBackTable::RowData* FindBackConfig::GetExpBackTableRowData(int type)
{
	for (auto iter = m_oExpBackTable.Table.begin(); iter != m_oExpBackTable.Table.end(); ++iter)
	{
		if ((*iter)->type == type)
		{
			return *iter;
		}
	}

	return NULL;
}

const ItemBackTable::RowData* FindBackConfig::GetItemBackTableRowData(int id, int level)
{
	for (auto iter = m_oItemBackTable.Table.begin(); iter != m_oItemBackTable.Table.end(); ++iter)
	{
		if ((*iter)->ID == id && (*iter)->Level.seq[0] <= level && (*iter)->Level.seq[1] >= level)
		{
			return *iter;
		}
	}

	return NULL;
}

ItemBackTable& FindBackConfig::GetItemBackTableData()
{
	return m_oItemBackTable;
}
