#include "pch.h"
#include "towerconfig.h"
#include "foreach.h"
#include "table/ExpeditionTable.h"
#include "table/expeditionconfigmgr.h"
#include "table/globalconfig.h"

INSTANCE_SINGLETON(TowerConfig)

TowerConfig::TowerConfig()
{
}

TowerConfig::~TowerConfig()
{

}

bool TowerConfig::Init()
{
	if (!m_oTeamTowerTable.LoadFile("table/TeamTower.txt"))
	{
		LogWarn("Load TeamTower.txt failed!");
		return false;
	}

	return true;
}

void TowerConfig::Uninit()
{

}

const TeamTowerRewardTable::RowData* TowerConfig::GetTowerReward(int hardLevel, int beginFloor, int reachFloor, std::map<int, int>& reward)
{
	auto iter = m_oTeamTowerTable.Table.begin();
	auto reachIter = iter;
	for (; iter != m_oTeamTowerTable.Table.end(); ++iter)
	{
		if ((*iter)->TowerHardLevel == hardLevel && (*iter)->TowerFloor > beginFloor && (*iter)->TowerFloor <= reachFloor)
		{
			foreach(j in (*iter)->Reward)
			{
				reward[(*j).seq[0]] += (*j).seq[1];
			}

			reachIter = iter;
		}
	}
	if (reachIter != m_oTeamTowerTable.Table.end())
	{
		return *reachIter;
	}

	return NULL;
}

const TeamTowerRewardTable::RowData* TowerConfig::GetTowerTableData(int hardLevel, int floor)
{
	for (auto iter = m_oTeamTowerTable.Table.begin(); iter != m_oTeamTowerTable.Table.end(); ++iter)
	{
		if ((*iter)->TowerHardLevel == hardLevel && (*iter)->TowerFloor == floor)
		{
			return *iter;
		}
	}

	return NULL;

}


int TowerConfig::GetHardLevel(ExpeditionTable::RowData *pRow)
{
	if (!pRow || !pRow->RandomSceneIDs.size())
	{
		return 0;
	}
	int sceneID = ExpeditionConfigMgr::Instance()->GetRandomScene(pRow->RandomSceneIDs[0]);
	auto sceneids =  GetGlobalConfig().TeamTowerSceneID;
	int hardLevel = 0;
	foreach(i in sceneids)
	{
		if ((*i).seq[1] == sceneID)
		{
			hardLevel = (*i).seq[0];
			break;
		}
	}

	return hardLevel;
}

int TowerConfig::GetHardLevel(UINT32 sceneID)
{
	auto sceneids =  GetGlobalConfig().TeamTowerSceneID;
	int hardLevel = 0;
	foreach(i in sceneids)
	{
		if ((*i).seq[1] == sceneID)
		{
			hardLevel = (*i).seq[0];
			break;
		}
	}

	return hardLevel;
}

int TowerConfig::GetSweepEachLevelTime(int hardLevel)
{
	foreach(i in  GetGlobalConfig().SweepTowerTime)
	{
		if (i->seq[0] == hardLevel)
		{
			return i->seq[1];
		}
	}

	return 0;
}

bool TowerConfig::IsTopestFloor(int hardLevel, int floor)
{

	if (!GetTowerTableData(hardLevel, floor + 1))
	{
		return true;
	}

	return false;

}

std::vector<TeamTowerRewardTable::RowData*> TowerConfig::GetTowerFloorHasFirstPassReward(int hardLevel)
{
	std::vector<TeamTowerRewardTable::RowData*> vec;
	for (auto iter=  m_oTeamTowerTable.Table.begin(); iter != m_oTeamTowerTable.Table.end(); ++iter)
	{
		if ((*iter)->TowerHardLevel == hardLevel && !(*iter)->FirstPassReward.empty())
		{
			vec.push_back(*iter);
		}
	}

	return vec;
}
