#ifndef __COMPETEDRAGONCONFIG_H__
#define __COMPETEDRAGONCONFIG_H__
#include "table/CompeteDragonTable.h"
#include "table/CompeteDragonRankRewardTable.h"
#include "timer.h"

struct SGlobalCompeteDragonInfo
{
	UINT32 syncTime;
	UINT32 updateTime;
	UINT32 levelSealType;
	UINT32 curExpID;
	SGlobalCompeteDragonInfo()
	{
		syncTime = 0;
		updateTime = 0;
		levelSealType = 0;
		curExpID = 0;
	}
};

class CompeteDragonConfig
{
public:
	CompeteDragonConfig();
	~CompeteDragonConfig();
	DECLARE_SINGLETON(CompeteDragonConfig)

	bool OnTimer();

	bool Init();
	void Uninit();
	bool LoadFile();
	CompeteDragonTable::RowData* GetCompeteDragonInfo();
	UINT32 GetExpID();
	void FillGlobalInfo(const std::string& value);
	void UpdateGlobalInfo(UINT32 time);
	UINT32 GetResetTime(){return m_globalCompeteDragonInfo.updateTime;}
private:
	SGlobalCompeteDragonInfo m_globalCompeteDragonInfo;
	CompeteDragonTable m_competeDragonTable;
	CompeteDragonRankRewardTable m_competeDragonRankTable;
	UINT32 m_timer;
};
#endif // 
