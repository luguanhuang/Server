#include "pch.h"
#include "firstpassconfig.h"
#include "foreach.h"

INSTANCE_SINGLETON(FirstPassConfig)

FirstPassConfig::FirstPassConfig()
{
}

FirstPassConfig::~FirstPassConfig()
{

}

bool FirstPassConfig::Init()
{
	if (!m_firstPassTable.LoadFile("table/FirstPass.txt"))
	{
		LogWarn("Load FirstPass.txt failed!");
		return false;
	}

	if (!m_firstPassRewardTable.LoadFile("table/FirstPassReward.txt"))
	{
		LogWarn("Load FirstPassReward.txt failed!");
		return false;
	}

	for (auto i = m_firstPassTable.Table.begin(); i != m_firstPassTable.Table.end(); i++)
	{
		if ((*i)->SceneID.empty())
		{
			LogWarn("FirstPass.txt, id [%u], sceneid is empty", (*i)->ID);
			return false;
		}
		for (auto j = (*i)->SceneID.begin(); j != (*i)->SceneID.end(); j++)
		{
			m_sceneid2FirstPass[*j] = *i;
		}
	}

	return true;
}

void FirstPassConfig::Uninit()
{

}

void FirstPassConfig::Clear()
{
	m_firstPassTable.Clear();
	m_firstPassRewardTable.Clear();
	m_sceneid2FirstPass.clear();
}

bool FirstPassConfig::LoadFile()
{
	Clear();

	return Init();
}

const FirstPassTable::RowData* FirstPassConfig::GetFirstPassDataByScene(int sceneID)
{
	auto iter = m_sceneid2FirstPass.find(sceneID);
	if (iter != m_sceneid2FirstPass.end())
	{
		return iter->second;
	}

	return NULL;
}

const FirstPassTable::RowData* FirstPassConfig::GetFirstPassDataByID(int firstPassID)
{
	foreach(i in m_firstPassTable.Table)
	{
		if ((*i)->ID == firstPassID)
		{
			return *i;
		}
	}

	return NULL;
}

const std::vector<FirstPassTable::RowData*>& FirstPassConfig::GetFirstPassData()
{
	return m_firstPassTable.Table;
}

const FirstPassReward::RowData* FirstPassConfig::GetFirstPassRewardData(int rewardID, int rank)
{
	foreach(i in m_firstPassRewardTable.Table)
	{
		if ((*i)->ID == rewardID && rank >= (*i)->Rank.seq[0] && rank <= (*i)->Rank.seq[1])
		{
			return (*i);
		}
	}
	
	return NULL;
}
