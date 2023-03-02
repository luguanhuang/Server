#ifndef __HELLDOORMGR_H__
#define __HELLDOORMGR_H__

#include "table/HellDoorDropTable.h"
class Role;
struct ItemDesc;

class HellDoorDropMgr
{
	HellDoorDropMgr();
	~HellDoorDropMgr();
	DECLARE_SINGLETON(HellDoorDropMgr);
public:
	bool Init();
	void Uninit();
	bool LoadFile();
	void ClearFile();
	bool CheckFile();
	HellDoorDropTable::RowData *GetData(UINT32 nID);
	HellDoorDropTable* GetTable();
	void GetItem(Role *pRole,UINT32 nId,std::vector<const ItemDesc*> &refItemList);

private:
	HellDoorDropTable m_Table;
};

#endif // __HELLDOORMGR_H__