#ifndef _MARRIAGE_TABLE_MGR_H_
#define _MARRIAGE_TABLE_MGR_H_

#include "tablemgr/tablehelper.h"
#include "table/WeddingLoverLiveness.h"

class MarriageTableMgr : public TableHelper<MarriageTableMgr>
{
public:
	bool Init();
	void Uninit();

	bool DoLoadFile();
	void ClearFile();

	WeddingLoverLiveness::RowData* GetLivenessData(UINT32 index);
	WeddingLoverLiveness& GetLivenessTable() { return m_liveness; }
	bool HasNewChest(UINT32 oldValue, UINT32 newValue);

private:
	WeddingLoverLiveness m_liveness;
};

#endif