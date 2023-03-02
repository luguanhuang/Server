#ifndef __FINDBACKCONFIG_H__
#define __FINDBACKCONFIG_H__
#include "table/ExpBackTable.h"
#include "table/ItemBackTable.h"

class FindBackConfig
{
public:
	FindBackConfig();
	~FindBackConfig();
	DECLARE_SINGLETON(FindBackConfig)

	bool Init();
	void Uninit();

	void ClearFile();
	bool CheckFile();
	bool LoadFile();

	const ExpBackTable& GetExpBackTable(){return m_oExpBackTable;}
	const ExpBackTable::RowData* GetExpBackTableRowData(int type);

	const ItemBackTable::RowData* GetItemBackTableRowData(int id, int level=1);
	ItemBackTable& GetItemBackTableData();

private:
	ExpBackTable m_oExpBackTable;
	ItemBackTable m_oItemBackTable;

};

#endif // __FINDBACKCONFIG_H__