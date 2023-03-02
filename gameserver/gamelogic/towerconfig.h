#ifndef __TOWERCONFIG_H__
#define __TOWERCONFIG_H__
#include "table/TeamTowerRewardTable.h"
#include "table/ExpeditionTable.h"

class TowerConfig
{
public:
	TowerConfig();
	~TowerConfig();
	DECLARE_SINGLETON(TowerConfig)

	bool Init();
	void Uninit();

	const TeamTowerRewardTable& GetTowerTable(){return m_oTeamTowerTable;}

	//(begin, end]
	const TeamTowerRewardTable::RowData* GetTowerReward(int hardLevel, int beginFloor, int reachFloor, std::map<int, int>& reward);
	const TeamTowerRewardTable::RowData* GetTowerTableData(int hardLevel, int floor);

	int GetHardLevel(ExpeditionTable::RowData *pRow);
	int GetHardLevel(UINT32 sceneID);
	int GetSweepEachLevelTime(int hardLevel);

	bool IsTopestFloor(int hardLevel, int floor); 
	std::vector<TeamTowerRewardTable::RowData*> GetTowerFloorHasFirstPassReward(int hardLevel);
private:
	TeamTowerRewardTable m_oTeamTowerTable;

};

#endif // 