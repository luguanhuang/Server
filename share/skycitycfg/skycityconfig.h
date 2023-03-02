#ifndef __SKYCITYCONFIG_H
#define __SKYCITYCONFIG_H

#include "table/SkyArenaReward.h"

class SkyCityConfig
{
	SkyCityConfig();
	~SkyCityConfig();
	DECLARE_SINGLETON(SkyCityConfig);
public:
	bool Init();
	void Uninit();
	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	UINT32 GetLevelSeal(UINT32 nLevelSeal);
	const SkyArenaReward::RowData* GetSkyArenaReward(int nLv, int nFloor);
private:
	SkyArenaReward m_rewardTable;
};

#endif