#include "pch.h"
#include "liveconfig.h"
#include "pb/project.pb.h"
#include "foreach.h"

INSTANCE_SINGLETON(LiveConfig)

LiveConfig::LiveConfig()
{
}

LiveConfig::~LiveConfig()
{

}

bool LiveConfig::Init()
{
	return LoadFile();
}

void LiveConfig::Uninit()
{
	m_liveTable.Clear();
}

bool LiveConfig::LoadFile()
{
	m_liveTable.Clear();
	if (!m_liveTable.LoadFile("table/LiveTable.txt"))
	{
		LogWarn("load live config table/LiveTable.txt failed!");
		return false;
	}

	return true;
}

int LiveConfig::GetLiveTypeByDNExpType(int expType)
{
	foreach(i in m_liveTable.Table)
	{
		if ((*i)->DNExpType == expType)
		{
			return (*i)->Type;
		}
	}

	return 0;
}

const LiveTable::RowData* LiveConfig::GetLiveTableData(int type)
{
	foreach(i in m_liveTable.Table)
	{
		if ((*i)->Type == type)
		{
			return (*i);
		}
	}

	return NULL;
}

int LiveConfig::GetLiveTypeBySceneType(int sceneType)
{
	foreach(i in m_liveTable.Table)
	{
		if ((*i)->SceneType == sceneType)
		{
			return (*i)->Type;
		}
	}

	return 0;
}
