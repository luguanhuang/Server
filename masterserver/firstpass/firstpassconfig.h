#ifndef __FIRSTPASSCONFIG_H__
#define __FIRSTPASSCONFIG_H__
#include "table/FirstPassTable.h"
#include "table/FirstPassReward.h"

class CFirstPassConfig
{
public:
	CFirstPassConfig();
	~CFirstPassConfig();
	DECLARE_SINGLETON(CFirstPassConfig)

	bool Init();
	void Uninit();
	void Clear();
	bool LoadFile();

	const FirstPassTable::RowData* GetFirstPassDataByScene(int sceneID);
	const FirstPassTable::RowData* GetFirstPassDataByID(int firstPassID);
	const std::vector<FirstPassTable::RowData*>& GetFirstPassData();
	const FirstPassReward::RowData* GetFirstPassRewardData(int rewardID, int rank);

private:
	FirstPassTable m_firstPassTable;
	FirstPassReward m_firstPassRewardTable;
	std::map<UINT32, FirstPassTable::RowData*> m_sceneid2FirstPass;
};

#endif // __FINDBACKCONFIG_H__