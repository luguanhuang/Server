#ifndef __ITEMCONFIGMGR_H__
#define __ITEMCONFIGMGR_H__

#include "table/ItemList.h"

typedef ItemList::RowData  ItemConf;
class CItemConfig
{
	CItemConfig();
	~CItemConfig();
	DECLARE_SINGLETON(CItemConfig)

public:

	bool Init();
	bool Reload();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	ItemConf  *GetItemConf(int itemID) { return m_ItemTable.GetByItemID(itemID); }
	const std::string GetItemName(int ItemID, int prof=1);
	std::string GetItemProfessionName(UINT32 ItemID, int profession);
	INT32 GetItemID(const std::string& strItemName);

	INT32 GetOverlap(UINT32 itemid);

	UINT32 ConvertItemId(UINT32 itemId, int profession);

private:
	ItemList m_ItemTable;
};
#endif
