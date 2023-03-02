#ifndef __LIVECONFIG_H__
#define __LIVECONFIG_H__

#include "table/LiveTable.h"

class LiveConfig
{
	LiveConfig();
	~LiveConfig();
	DECLARE_SINGLETON(LiveConfig)

public:
	bool Init();
	void Uninit();
	bool LoadFile();
	int GetLiveTypeBySceneType(int sceneType);
	int GetLiveTypeByDNExpType(int expType);
	const LiveTable::RowData* GetLiveTableData(int type);
	void GetLiveTableDataList(int page, std::vector<LiveTable::RowData*>& data);
private:
	LiveTable m_liveTable;

};

#endif // 