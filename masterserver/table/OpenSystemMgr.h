#ifndef __OPENSYSTEMMGR_H__
#define __OPENSYSTEMMGR_H__

#include "table/OpenSystemTable.h"


class OpenSystemMgr
{
	OpenSystemMgr();
	~OpenSystemMgr();
	DECLARE_SINGLETON(OpenSystemMgr)

public:

	bool Init();
	void Uninit();
	
	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	int GetPlayerLevelBySystemID(int systemID);
	OpenSystemTable::RowData* GetSystemRowData(int systemID);

	//idip
	bool IsClosedSystem(UINT32 systemID);
	void IdipOpSystem(UINT32 systemID, bool opOpen);
	const std::set<UINT32>& GetIdipSystemClose() { return m_idipSystemClose; }

private:
	bool IsIdipCloseSystem(UINT32 systemID);
	bool LoadIdipOpSystem();
	void SaveIdipOpSystem();

private:
	OpenSystemTable  m_oSystemTable;

	std::set<UINT32> m_idipSystemClose;
};

#endif // __OPENSYSTEMMGR_H__

