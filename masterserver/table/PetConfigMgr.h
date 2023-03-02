#ifndef __PETCONFIGMGR_H__
#define __PETCONFIGMGR_H__

#include "table/PetInfoTable.h"

class CPetConfigMgr
{
	CPetConfigMgr();
	~CPetConfigMgr();
	DECLARE_SINGLETON(CPetConfigMgr)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	PetInfoTable::RowData* GetConf(UINT32 petid) { return m_table.GetByid(petid); };

private:
	PetInfoTable m_table;
};
#endif
