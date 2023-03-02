#ifndef __COMPETEDRAGONCONFIG_H__
#define __COMPETEDRAGONCONFIG_H__
#include "table/CompeteDragonTable.h"
#include "table/CompeteDragonRankRewardTable.h"
#include "timer.h"

class CompeteDragonConfig
{
public:
	CompeteDragonConfig();
	~CompeteDragonConfig();
	DECLARE_SINGLETON(CompeteDragonConfig)

	bool Init();
	void Uninit();
	bool LoadFile();

	CompeteDragonRankRewardTable::RowData* GetCompeteDragonRankData(UINT32 rank);
private:
	CompeteDragonTable m_competeDragonTable;
	CompeteDragonRankRewardTable m_competeDragonRankTable;
};
#endif // 
