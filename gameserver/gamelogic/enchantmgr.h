#ifndef __ENCHANTMGR_H__
#define __ENCHANTMGR_H__

#include "EnchantEquip.h"
#include "itemsuffix.h"

typedef EnchantEquip::RowData EnchantConf;

class Role;
struct XItem;
class EnchantMgr
{
	EnchantMgr();
	~EnchantMgr();
	DECLARE_SINGLETON(EnchantMgr)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	EnchantConf* GetEnchantConf(UINT32 enchantid) { return m_oTable.GetByEnchantID(enchantid); }
	ItemChangeAttr GetRandomEnchantAttr(UINT32 enchantid);

	KKSG::ErrorCode DoTransfer(Role* pRole, XItem* origin, XItem* dest, bool isNotify);

	KKSG::ErrorCode CanEnchantWearEquip(UINT32 equipid, UINT32 enchantid);

	std::pair<UINT32, UINT32> GetEnchantAttrValueRange(UINT32 enchantid, UINT32 attrid);

private:
	EnchantEquip m_oTable;
};


#endif