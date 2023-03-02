#ifndef __ITEMCONFIG_H__
#define __ITEMCONFIG_H__

#include "table/ItemList.h"
#include "table/DropList.h"
#include "table/EquipList.h"
#include "table/ChestList.h"
#include "table/EmblemTable.h"

#include "table/EmblemBasic.h"
#include "table/JadeTable.h"
#include "table/SupplementTable.h"
#include "table/FashionList.h"
#include "table/DoodadTable.h"
#include "table/CircleDrawTable.h"
#include "table/ItemComposeTable.h"
#include "table/EquipSuitTable.h"
#include "table/ProfessionMgr.h"
#include "table/PetFoodTable.h"
#include "table/PetItemTable.h"
#include "table/ItemBuffTable.h"
#include "table/DesignationItem.h"
#include "emblemmgr.h"
#include "itemsuffix.h"
#include "table/BagExpandItemListTable.h"
#include "table/DoodadNotice.h"
#include "table/AuctionDiscountTable.h"
#include "table/AuctItemRecovery.h"
#include "table/ItemTransform.h"
#include "buff/XBuffTemplateManager.h"

typedef ItemList::RowData  ItemConf;
typedef EquipList::RowData EquipConf;
typedef ChestList::RowData ChestConf;
typedef DropList::RowData  DropConf;
typedef DoodadTable::RowData DoodadConf;
typedef EmblemBasic::RowData EmblemBasicConf;
typedef JadeTable::RowData JadeConf;
typedef FashionList::RowData FashionConf;
typedef CircleDrawTable::RowData CircleDrawConf;
typedef SupplementTable::RowData SupplementConf;
typedef ItemComposeTable::RowData ItemComposeTableConf;
typedef PetFoodTable::RowData PetFoodConf;
typedef PetItemTable::RowData PetItemConf;
typedef ItemBuffTable::RowData ItemBuffConf;
typedef ItemTransform::RowData ItemTransformConf;
typedef EquipSuitTable::RowData EquipSuitConf;
typedef DesignationItem::RowData DesignationItemConf;
typedef BagExpandItemListTable::RowData BagExpandConf;

enum DropMode
{
	DROP_ALL = 1,
	DROP_RANDOM = 2,
	DROP_PICK = 3
};

struct Iterator
{
	int index;
	int start;
	int end;
	int totalProb;

	Iterator()
	{
		index = 0;
		start = 0;
		end = 0;
		totalProb = 0;
	}

	virtual ~Iterator()
	{

	}

	bool HasNext()
	{
		return index < end;
	}

	void Advance(int n)
	{
		index += n;
	}

	virtual void OnNext() = 0;

	void Next()
	{
		OnNext();
		++index;
	}

	void Reset()
	{
		index = start;
	}
};

struct DoodadIterator : public Iterator
{
	DoodadConf *pConf;
	
	DoodadIterator()
	{
		pConf = NULL;
	}

	virtual void OnNext();
};

class Role;
class EquipSuitDetector
{
public:
	EquipSuitDetector(int prof);
	~EquipSuitDetector();

	void AddEquip(UINT32 itemID);
	void ApplySuitEffect(CombatAttribute* combatAttr);

private:
	int m_prof;

	void GetSuitEffects(std::vector<ItemChangeAttr>& effects);

	std::map<int, int> m_SuitCounts;
};

class EquipProtoCalc
{
	static const int coff = 100000;
public:
	static UINT32 ConvertToProfessionItemID(UINT32 itemID, int profession)
	{
		int basicProf = ProfessionMgr::Instance()->GetBasicProfession(profession);
		return itemID + basicProf * coff;
	}

	static int GetOriginItemId(UINT32 itemID, UINT32 prof)
	{
		return itemID - coff * prof;
	}

	static int GetItemUsedProfession(UINT32 itemID)
	{
		return itemID / coff;
	}
};

struct ItemDesc;
class ItemConfig
{
	friend struct DropIterator;
	friend struct DoodadIterator;
	friend class  EquipSuitDetector;

	ItemConfig();
	~ItemConfig();
	DECLARE_SINGLETON(ItemConfig)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	ItemConf* GetItemConf(int itemID) { return m_ItemTable.GetByItemID(itemID); }
	const std::string GetItemName(int ItemID, UINT32 prof = 1);
	EquipConf* GetEquipConf(int itemID) { return m_EquipTable.GetByItemID(itemID); }
	//ChestConf *GetChestConf(int itemID) { return m_ChestTable.GetByItemID(itemID); } //现在礼包id还需要看职业，所以不唯一了
	ChestConf* GetChestConf(int itemID, UINT32 proID);
	void GetChestItems(ChestConf* pConf, std::vector<ItemDesc>& vecItems, UINT32 level = 0);

	//EmblemTableConf* GetEmblemTableConf(int nItemId) { return m_EmblemTable.GetByEmblemID(nItemId); }
	EmblemBasicConf* GetEmblemBasicConf(UINT32 itemID) { return m_EmblemBasic.GetByEmblemID(itemID); }	

	FashionConf *GetFashionConf(int itemID) { return m_FashionTable.GetByItemID(itemID); }
	JadeConf* GetJadeConf(int nItemId) {return m_JadeTable.GetByJadeID(nItemId);}
	SupplementConf* GetSupplementConf(int itemID) { return m_suptable.GetByitemid(itemID); }
	ItemComposeTableConf* GetComposeConf(int id) { return m_composeTable.GetByID(id); }
	PetFoodConf* GetPetFoodConf(UINT32 itemID) { return m_petfoodtable.GetByitemid(itemID); }
	PetItemConf* GetPetItemConf(UINT32 itemID) { return m_petitemtable.GetByitemid(itemID); }
	ItemTransformConf* GetItemTransformConf(UINT32 itemID) { return m_transformtable.GetByitemid(itemID); }
	ItemBuffConf* GetBuffConf(UINT32 itemID) { return m_bufftable.GetByItemId(itemID); }
	EquipSuitConf* GetEquipSuitConf(int suitid) { return m_equipSuitTable.GetBySuitID(suitid); }
	const BagExpandConf* GetBagExpandConf(UINT32 itemID) { return m_bagExpandTable.GetByItemId(itemID); }

	ItemComposeTableConf* GetSpComposeConf(UINT32 itemID) { return m_speialCompose.find(itemID) == m_speialCompose.end() ? NULL : m_speialCompose[itemID]; }

	EquipSuitTable& GetEquipSuitTable() { return m_equipSuitTable; }

	UINT32 GetBuffType(UINT32 buffid);

	JadeConf* GetJadeConfByParentItemID(UINT32 itemID);

	const std::vector<ItemDesc>* GetDecompsose(int itemID);

	int GetProfession(UINT32 itemID);
	UINT32 GetOverlap(UINT32 itemID);

	bool GetCircleDrawGive(UINT32 useItemID, std::vector<Sequence<uint,3>>& giveList);
	UINT32 GetCircleDrawNotice(UINT32 useItemID, UINT32 GiveItemID, UINT32 GiveItemCount, UINT32 GiveItemProb, UINT32 GiveItemIndex);

	DoodadIterator GetDoodadIterator(int doodadID);
	DoodadConf *GetRandomDoodadDrop(int doodadID);

	bool IsFashion(int itemID);

	bool GetDecomposedDropEquip(EquipConf* config, std::map<UINT32, UINT32>& items);
	bool GetDecomposedItem(UINT32 itemID, UINT32 itemcount, std::map<UINT32, UINT32>& items);

	std::unordered_set<int> &GetAllFashions() { return m_FashionIDs; }

	int  FindEquipSuit(int prof, UINT32 itemID);
	void AddSuitEffect(int suitID, int suitCount, std::vector<ItemChangeAttr> &output);

	bool GetBind(UINT32 itemID, bool bind);
	bool GetBind(ItemConf* conf, bool bind) { return conf->CanTrade ? bind : true; }

	bool GetPetItem(UINT32 itemID, UINT32 count, UINT32& exp, UINT32& hungry, int& mood);

	XItem GetSell(UINT32 itemID);

	UINT32 GetItemQuailty(UINT32 itemId);
	DesignationItem::RowData* GetDesignation(UINT32 itemID);

	UINT32 ChangeProfession(UINT32 itemid, UINT32 from, UINT32 to);

	UINT32 GetDoodadNoticeID(UINT32 sceneType, UINT32 itemID, UINT32 itemCount);
	// 拍卖行价格
	float GetReturnPrice(const UINT32 item_id, UINT32 sealtype);

	// 是否需要防刷
	bool AuctItemPrevent(const UINT32 item_id);

	// get trans buffs
	BuffDesc GetTransBuff(const ItemTransformConf* conf);	


	UINT32 GetEquipSuitID(UINT32 nItemID);
	UINT32 GetDiffEquipCountInSuit( Role* pRole, UINT32 equipSuitID );
	UINT32 GetJewelrySuitID(UINT32 nSuitID);
	void BuildDesignation();

	UINT32 GetEquipSuitLevel(Role* pRole, UINT64 equipUid, UINT32 needLevel,UINT32 needCount);
	UINT32 GetEquipInJewelrySuit(UINT32 nItemID);

private:
	void BuildChestList();
	void BuildEquipSuit();
	void BuildDoodadDropRange();
	void BuildFashionAndDecompse();
	void BuildItemBuff();
	bool BuildSpecialCompose(std::unordered_map<UINT32, ItemComposeTableConf*>& spCompose);

	void BuildAuctItemPrevent();
	std::unordered_map<UINT32,bool> auctitem_prevent_;

	ItemList m_ItemTable;
	ChestList m_ChestTable;
	EquipList m_EquipTable;	
	EmblemBasic m_EmblemBasic;
	JadeTable m_JadeTable;
	FashionList m_FashionTable;
	DoodadTable m_DoodadTable;
	CircleDrawTable m_CircleDrawTable;
	SupplementTable m_suptable;
	ItemComposeTable m_composeTable;
	EquipSuitTable m_equipSuitTable;
	PetFoodTable m_petfoodtable;
	PetItemTable m_petitemtable;
	ItemBuffTable m_bufftable;
	DesignationItem m_desigTable;
	BagExpandItemListTable m_bagExpandTable;
	DoodadNotice m_doodadNoticeTable;
	ItemTransform m_transformtable;

	std::unordered_map<int, std::vector<ChestConf*>> m_mapChestlist;

	// 拍卖行物品折扣
	AuctionDiscountTable m_adTable;
	// 交易所物品属性
	AuctItemRecovery	auctitem_revover_;
	struct DropRange
	{
		int start;
		int end;
		int totalProb;
	};


	//equip所在的套装的ID  KEY: 装备ID  VALUE：装备所在的套装ID;
	std::map<UINT32, UINT32> m_mapEquipIDSuitID;
	//首饰套装ID列表;
	std::vector<UINT32> m_vecJewelrySuitID;
	//存在于首饰套装中的物品集合;
	std::map<UINT32, UINT32> m_mapEquipInJewelrySuit;

	std::map<UINT32, DesignationItem::RowData*> m_desigItem;
	std::map<UINT32, UINT32> m_buffid2type;
	std::map<UINT32, std::vector<ItemDesc>> m_decompose;
	std::unordered_map<int, DropRange> m_DoodadDropStart;
	std::unordered_set<int> m_FashionIDs;
	std::unordered_map<UINT32, std::vector<std::pair<int, int>>> m_ItemID2SuitID; //itemID -> prof + suitid
	std::unordered_map<UINT32, ItemComposeTableConf*> m_speialCompose;
	std::map<std::pair<UINT32, std::pair<UINT32, UINT32>>, UINT32> m_doodadNoticeMap;
};


#endif // __ITEMCONFIG_H__