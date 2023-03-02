#ifndef _PARTNER_TABLE_MGR_H_
#define _PARTNER_TABLE_MGR_H_

#include "util/utilsingleton.h"

#include "table/PartnerTable.h"
#include "table/PartnerLivenessTable.h"
#include "table/ShopTable.h"
#include "table/ShopTypeTable.h"

class PartnerTableMgr : public Singleton<PartnerTableMgr>
{
public:
	PartnerTableMgr();
	~PartnerTableMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetLevel(UINT32 degress);
	PartnerTable::RowData* GetPartnerData(UINT32 level);
	bool HasNewCanTakChest(UINT32 level, UINT32 oldValue, UINT32 newValue);
	PartnerLivenessTable::RowData* GetLivenessTable(UINT32 level, UINT32 index);
	void GetCanTakeChestIndex(UINT32 level, UINT32 liveness, std::vector<UINT32>& index);

	void RandomShop(UINT32 level, std::vector<UINT32>& shopItems);
	UINT32 RandomOne(UINT32 level, UINT32 slot);
	ShopTable::RowData* GetPartnerShopItem(UINT32 id);
	std::vector<UINT32>& GetShopRefreshTime() { return m_shopRefreshTime; }
	UINT32 GetCostNum(ShopTable::RowData* pData);

private:
	void AddRandomItem(UINT32 level, UINT32 slot, UINT32 rate, UINT32 shopId);
	UINT32 RandomOne(std::map<UINT32, UINT32>& randMap);

private:
	PartnerTable m_partner;
	PartnerLivenessTable m_liveness;
	ShopTypeTable m_shopType;
	ShopTable m_shop;

	std::vector<UINT32> m_shopRefreshTime;
	std::map<UINT32, UINT32> m_degree2level;	// 亲密度对应等级
	std::map<UINT32, std::map<UINT32, PartnerLivenessTable::RowData*>> m_livenessChest; // key1: 等级 key2: 宝箱编号
	std::map<UINT32, std::map<UINT32, UINT32>> m_shopRandMap;							// key: level * 100 + slot key2 概率 value: id
};


#endif