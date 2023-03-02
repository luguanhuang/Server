#ifndef _DESIGNATION_TABLE_MGR_H_
#define _DESIGNATION_TABLE_MGR_H_

#include "util/utilsingleton.h"
#include "table/DesignationTable.h"


class DesignationTableMgr : public Singleton<DesignationTableMgr>
{
public:
	DesignationTableMgr();
	~DesignationTableMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	const DesignationTable::RowData* GetRowData(UINT32 nID)
	{
		return m_Table.GetByID(nID);
	}
private:
	DesignationTable m_Table;
};


#endif