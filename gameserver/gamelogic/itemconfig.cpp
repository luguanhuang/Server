#include "pch.h"
#include "itemconfig.h"
#include "util/XRandom.h"
#include "define/itemdef.h"
#include "foreach.h"
#include "item.h"
#include "globalconfig.h"
#include "unit/role.h"
#include "unit/combatattribute.h"
#include "config/itemdropconfig.h"
#include "gamelogic/bag.h"

INSTANCE_SINGLETON(ItemConfig)

ItemConfig::ItemConfig()
{
}

ItemConfig::~ItemConfig()
{

}

bool ItemConfig::Init()
{
	if (!m_ItemTable.LoadFile("table/ItemList.txt"))
	{
		LogWarn("Load ItemList.txt failed!");
		return false;
	}

	for (auto i = m_ItemTable.Table.begin(); i != m_ItemTable.Table.end(); ++i)
	{
		ItemList::RowData* data = *i;
		switch (data->ItemType)
		{
		case VIRTUAL_ITEM:
			{
				if (data->ItemID >= VIRTUAL_ITEM_MAX)
				{
					LogWarn("itemid:%u is virtual item but id >= VIRTUAL_ITEM_MAX:%u", data->ItemID, VIRTUAL_ITEM_MAX);
					return false;
				}
				break;
			}
		case ATLAS:
			{
				if (data->Overlap != 0)
				{
					LogWarn("itemid:%u, Atlas item's overlap should be 0", data->ItemID);
					return false;
				}
				break;
			}
		case EQUIP:
		case EMBLEM:
		case ARTIFACT:
			{
				data->Overlap = 1;
				if (data->Overlap != 1)
				{
					LogWarn("itemid:%u, overlap should be 1", data->ItemID);
					return false;
				}
				break;
			}
		default:
			{
				break;
			}
		}
	}

	if (!m_ChestTable.LoadFile("table/ChestList.txt"))
	{
		LogWarn("Load ChestList.txt failed!");
		return false;
	}
	for(auto i = m_ChestTable.Table.begin(); i != m_ChestTable.Table.end(); i++)
	{
		if((*i)->DropID.size() != (*i)->Times.size())
		{
			LogWarn("itemid:%u dropid'size != times'size", (*i)->ItemID);
			return false;
		}
	}

	if (!m_EquipTable.LoadFile("table/EquipList.txt"))
	{
		LogWarn("Load EquipList.txt failed!");
		return false;
	}
	/* 龙玉孔现在没有开启操作,全部都是固定的,所以龙玉的3个值必须相等*/
	for(auto i = m_EquipTable.Table.begin(); i != m_EquipTable.Table.end(); i++)
	{
		if((*i)->Jade[0] != (*i)->Jade[1] || (*i)->Jade[1] != (*i)->Jade[2])
		{
			LogWarn("itemid:%u, jade data error", (*i)->ItemID);
			return false;
		}
	}

	if(!m_EmblemBasic.LoadFile("table/EmblemBasic.txt"))
	{
		LogWarn("load file table/EmblemBasic.txt failed");
		return false;
	}

	if (!m_JadeTable.LoadFile("table/Jade.txt"))
	{
		LogWarn("Load jade.txt failed!");
		return false;
	}

	if (!m_FashionTable.LoadFile("table/FashionList.txt"))
	{
		LogWarn("Load FashionList.txt failed!");
		return false;
	}

	if (!m_DoodadTable.LoadFile("table/DoodadList.txt"))
	{
		LogWarn("Load DoodadList.txt failed!");
		return false;
	}

	if(!m_CircleDrawTable.LoadFile("table/CircleDraw.txt"))
	{
		LogWarn("Load CircleDraw.txt failed!");
		return false;
	}

	if (!m_suptable.LoadFile("table/Supplement.txt"))
	{
		SSWarn<<"load file table/Supplement.txt failed"<<END;
		return false;
	}

	if (!m_composeTable.LoadFile("table/ItemCompose.txt"))
	{
		SSWarn<<"load file table/ItemCompose.txt failed"<<END;
		return false;
	}
	if (!BuildSpecialCompose(m_speialCompose))
	{
		return false;
	}

	if (!m_equipSuitTable.LoadFile("table/EquipSuit.txt"))
	{
		SSWarn<<"load file table/EquipSuit.txt failed"<<END;
		return false;
	}

	if (!m_petfoodtable.LoadFile("table/PetFood.txt"))
	{
		SSWarn<<"load file table/PetFood.txt failed"<<END;
		return false;
	}

	if (!m_petitemtable.LoadFile("table/PetItem.txt"))
	{
		SSWarn<<"load file table/PetItem.txt failed"<<END;
		return false;
	}

	if (!m_bufftable.LoadFile("table/ItemBuff.txt"))
	{
		SSWarn<<"load file table/ItemBuff.txt failed"<<END;
		return false;
	}

	if (!m_desigTable.LoadFile("table/DesignationItem.txt"))
	{
		SSWarn<<"load file table/DesignationItem.txt failed"<<END;
		return false;
	}

	if (!m_bagExpandTable.LoadFile("table/BagExpandItemList.txt"))
	{
		LogWarn("Load BagExpandItemList.txt failed");
		return false;
	}

	if (!m_doodadNoticeTable.LoadFile("table/DoodadNotice.txt"))
	{
		LogWarn("Load DoodadNotice.txt failed");
		return false;
	}
	for (auto i = m_doodadNoticeTable.Table.begin(); i != m_doodadNoticeTable.Table.end(); i++)
	{
		m_doodadNoticeMap[std::make_pair((*i)->SceneType, std::make_pair((*i)->ItemID, (*i)->ItemCount))] = (*i)->NoticeID;
	}

	// 世界等级与物品折扣对应表
	if (!m_adTable.LoadFile("table/AuctionDiscount.txt"))
	{
		SSWarn<<"load file table/AuctionDiscount.txt failed"<<END;
		return false;
	}
	
	if(!auctitem_revover_.LoadFile("table/AuctItemRecovery.txt"))
	{
		SSWarn << " load AuctItemRecovery.txt failed " << END;
		return false;
	}	

	if(!m_transformtable.LoadFile("table/ItemTransform.txt"))
	{
		SSWarn << " load table/ItemTransform.txt failed " << END;
		return false;
	}	

	BuildChestList();
	BuildDoodadDropRange();
	BuildEquipSuit();
	BuildFashionAndDecompse();
	BuildItemBuff();

	BuildAuctItemPrevent();
	BuildDesignation();

	return true;
}


void ItemConfig::Uninit()
{

}

bool ItemConfig::CheckFile()
{
	ItemList tmpItemList;
	if (!tmpItemList.LoadFile("table/ItemList.txt"))
	{
		LogWarn("Load ItemList.txt failed!");
		return false;
	}

	for (auto i = tmpItemList.Table.begin(); i != tmpItemList.Table.end(); ++i)
	{
		ItemList::RowData* data = *i;
		switch (data->ItemType)
		{
		case VIRTUAL_ITEM:
			{
				if (data->ItemID >= VIRTUAL_ITEM_MAX)
				{
					LogWarn("itemid:%u is virtual item but id >= VIRTUAL_ITEM_MAX:%u", data->ItemID, VIRTUAL_ITEM_MAX);
					return false;
				}
				break;
			}
		case ATLAS:
			{
				if (data->Overlap != 0)
				{
					LogWarn("itemid:%u, Atlas item's overlap should be 0", data->ItemID);
					return false;
				}
				break;
			}
		case EQUIP:
		case EMBLEM:
		case ARTIFACT:
			{
				if (data->Overlap != 1)
				{
					LogWarn("itemid:%u, overlap should be 1", data->ItemID);
					return false;
				}
				break;
			}
		default:
			{
				break;
			}
		}
	}

	ChestList tmpChestList;
	if (!tmpChestList.LoadFile("table/ChestList.txt"))
	{
		LogWarn("Load ChestList.txt failed!");
		return false;
	}
	for(auto i = tmpChestList.Table.begin(); i != tmpChestList.Table.end(); i++)
	{
		if((*i)->DropID.size() != (*i)->Times.size())
		{
			LogWarn("itemid:%u dropid'size != times'size", (*i)->ItemID);
			return false;
		}
	}

	EquipList tmpEquipList;
	if (!tmpEquipList.LoadFile("table/EquipList.txt"))
	{
		LogWarn("Load EquipList.txt failed!");
		return false;
	}
	/* 龙玉孔现在没有开启操作,全部都是固定的,所以龙玉的3个值必须相等*/
	for(auto i = tmpEquipList.Table.begin(); i != tmpEquipList.Table.end(); i++)
	{
		if ((*i)->Jade[0] != (*i)->Jade[1] || (*i)->Jade[1] != (*i)->Jade[2])
		{
			LogWarn("itemid:%u, jade data error", (*i)->ItemID);
			return false;
		}
	}

	DropList tmpDropList;
	if (!tmpDropList.LoadFile("table/DropList.txt"))
	{
		LogWarn("Load DropList.txt failed!");
		return false;
	}

	FashionList tmpFashionList;
	if (!tmpFashionList.LoadFile("table/FashionList.txt"))
	{
		LogWarn("Load FashionList.txt failed!");
		return false;
	}

	DoodadTable tmpDoodadTable;
	if (!tmpDoodadTable.LoadFile("table/DoodadList.txt"))
	{
		LogWarn("Load DoodadList.txt failed!");
		return false;
	}

	CircleDrawTable tmpCircleDrawTable;
	if (!tmpCircleDrawTable.LoadFile("table/CircleDraw.txt"))
	{
		LogWarn("Load CircleDraw.txt failed!");
		return false;
	}

	DesignationItem tmpDesignTable;
	if (!tmpDesignTable.LoadFile("table/DesignationItem.txt"))
	{
		LogWarn("Load DesignationItem.txt failed!");
		return false;	
	}

	ItemComposeTable tempcomposeTable;
	if (!tempcomposeTable.LoadFile("table/ItemCompose.txt"))
	{
		SSWarn<<"load file table/ItemCompose.txt failed"<<END;
		return false;
	}
	std::unordered_map<UINT32, ItemComposeTableConf*> tmpSpCompose;
	if (!BuildSpecialCompose(tmpSpCompose))
	{
		return false;
	}

	EquipSuitTable tmpEquipSuitTable;
	if (!tmpEquipSuitTable.LoadFile("table/EquipSuit.txt"))
	{
		SSWarn<<"load file table/EquipSuit.txt failed"<<END;
		return false;
	}

	BagExpandItemListTable tmpBagExpandTable;
	if (!tmpBagExpandTable.LoadFile("table/BagExpandItemList.txt"))
	{
		LogWarn("Load BagExpandItemList.txt failed");
		return false;
	}

	DoodadNotice tmpDoodadNoticeTable;
	if (!tmpDoodadNoticeTable.LoadFile("table/DoodadNotice.txt"))
	{
		LogWarn("Load DoodadNotice.txt failed");
		return false;
	}

	AuctionDiscountTable tmpAuctionDiscountTable;
	if (!tmpAuctionDiscountTable.LoadFile("table/AuctionDiscount.txt"))
	{
		LogWarn("Load AuctionDiscount.txt failed");
		return false;
	}

	AuctItemRecovery tmpAuctItemRecovery;
	if(!tmpAuctItemRecovery.LoadFile("table/AuctItemRecovery.txt"))
	{
		SSWarn << " load AuctItemRecovery.txt failed " << END;
		return false;
	}	

	ItemTransform tempItemTransform;
	if (!tempItemTransform.LoadFile("table/ItemTransform.txt"))
	{
		SSWarn<<"load file table/ItemTransform.txt failed"<<END;
		return false;
	}

	return true;
}

bool ItemConfig::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_ItemTable.LoadFile("table/ItemList.txt");
	m_ChestTable.LoadFile("table/ChestList.txt");
	m_EquipTable.LoadFile("table/EquipList.txt");
	m_FashionTable.LoadFile("table/FashionList.txt");
	m_DoodadTable.LoadFile("table/DoodadList.txt");
	m_CircleDrawTable.LoadFile("table/CircleDraw.txt");
	m_desigTable.LoadFile("table/DesignationItem.txt");
	m_composeTable.LoadFile("table/ItemCompose.txt");
	m_equipSuitTable.LoadFile("table/EquipSuit.txt");
	m_bagExpandTable.LoadFile("table/BagExpandItemList.txt");
	m_doodadNoticeTable.LoadFile("table/DoodadNotice.txt");
	m_transformtable.LoadFile("table/ItemTransform.txt");

	// 世界等级与物品折扣对应表
	m_adTable.LoadFile("table/AuctionDiscount.txt");
	// 拍卖行物品属性
	auctitem_revover_.LoadFile("table/AuctItemRecovery.txt");

	BuildChestList();
	BuildDoodadDropRange();
	BuildEquipSuit();
	BuildFashionAndDecompse();
	BuildSpecialCompose(m_speialCompose);

	for (auto i = m_doodadNoticeTable.Table.begin(); i != m_doodadNoticeTable.Table.end(); i++)
	{
		m_doodadNoticeMap[std::make_pair((*i)->SceneType, std::make_pair((*i)->ItemID, (*i)->ItemCount))] = (*i)->NoticeID;
	}

	BuildAuctItemPrevent();
	BuildDesignation();
	return true;
}

void ItemConfig::ClearFile()
{
	//table clear
	m_ItemTable.Clear();
	m_ChestTable.Clear();
	m_EquipTable.Clear();
	m_FashionTable.Clear();
	m_DoodadTable.Clear();
	m_CircleDrawTable.Clear();
	m_desigTable.Clear();
	m_composeTable.Clear();
	m_equipSuitTable.Clear();
	m_bagExpandTable.Clear();
	m_doodadNoticeTable.Clear();

	//data clear
	m_DoodadDropStart.clear();
	//Build
	m_mapChestlist.clear();
	m_FashionIDs.clear();
	m_ItemID2SuitID.clear();
	//decompose
	m_decompose.clear();
	//designation
	m_desigItem.clear();

	m_speialCompose.clear();
	m_doodadNoticeMap.clear();
	
	m_adTable.Clear();
	auctitem_revover_.Clear();
	m_transformtable.Clear();
}

const std::string ItemConfig::GetItemName(int ItemID, UINT32 prof)
{
	std::string name;
	ItemConf* pConf = GetItemConf(ItemID);
	if(NULL != pConf && pConf->ItemName.size())
	{
		if (prof - 1 < pConf->ItemName.size())
		{
			name = pConf->ItemName[prof - 1];
		}
		else
		{
			name = pConf->ItemName[0];
		}
	}
	return name;
}

ChestConf* ItemConfig::GetChestConf(int itemID, UINT32 proID)
{
	auto iter = m_mapChestlist.find(itemID);
	if (iter == m_mapChestlist.end())
	{
		return NULL;
	}
	vector<ChestConf*>& vec = iter->second;

	for (UINT32 i = 0; i < vec.size(); i++)
	{
		if (vec[i]->Profession == proID || 0 == vec[i]->Profession)
		{
			return vec[i];
		}
	}
	return NULL;
}

void ItemConfig::GetChestItems(ChestConf* pConf, std::vector<ItemDesc>& vecItems, UINT32 level)
{
	bool isAll = pConf->SelMode == 1 ? true : false;
	for(UINT32 i = 0; i < pConf->DropID.size(); i++)
	{
		if(isAll)
		{
			if(1 == pConf->Times[i])
			{
				ItemDropConfig::Instance()->GetAllDropItem(pConf->DropID[i], vecItems);
				continue;
			}
			std::vector<ItemDesc> tmpVec;
			ItemDropConfig::Instance()->GetAllDropItem(pConf->DropID[i], tmpVec);
			for(auto j = tmpVec.begin(); j != tmpVec.end(); j++)
			{
				vecItems.push_back(*j);
			}
			continue;
		}
		for(UINT32 j = 0; j < pConf->Times[i]; j++)
		{
			const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(pConf->DropID[i], level);
			if(NULL != item)
			{
				vecItems.push_back(*item);
			}
		}
	}
}

DoodadIterator ItemConfig::GetDoodadIterator(int doodadID)
{
	auto i = m_DoodadDropStart.find(doodadID);
	if (i == m_DoodadDropStart.end())
	{
		return DoodadIterator();
	}
	else
	{
		DoodadIterator iter;
		iter.start = i->second.start;
		iter.end = i->second.end;
		iter.totalProb = i->second.totalProb;
		iter.index = iter.start;
		iter.pConf = m_DoodadTable.Table[iter.index];
		return iter;
	}
}

DoodadConf * ItemConfig::GetRandomDoodadDrop(int doodadID)
{
	if (doodadID == 0)
	{
		return NULL;
	}

	DoodadIterator iter = GetDoodadIterator(doodadID);
	int prob = XRandom::randInt(0, iter.totalProb);
	while (iter.HasNext())
	{
		iter.Next();
		prob -= iter.pConf->DropProb;
		if (prob < 0)
		{
			return iter.pConf;
		}
	}

	LogWarn("doodad id %d Get NULL result", doodadID);
	return NULL;
}

void ItemConfig::BuildChestList()
{
	UINT32 vecSize = m_ChestTable.Table.size();
	for(UINT32 i = 0; i < vecSize; i++)
	{
		m_mapChestlist[m_ChestTable.Table[i]->ItemID].push_back(m_ChestTable.Table[i]);
	}
}

void ItemConfig::BuildEquipSuit()
{
	foreach(i in m_equipSuitTable.Table)
	{
		auto *p = (*i);
		for (unsigned j = 0; j < p->EquipID.size(); ++j)
		{
			m_ItemID2SuitID[p->EquipID[j]].push_back(std::make_pair(p->ProfID, p->SuitID));
		}
	}
}

void ItemConfig::BuildDoodadDropRange()
{
	UINT32 n = m_DoodadTable.Table.size();
	DropRange range;
	range.start = 0;
	range.totalProb = m_DoodadTable.Table[0]->DropProb;
	int currDrop = m_DoodadTable.Table[0]->DoodadID;

	for (UINT32 i = 1; i < n; ++i)
	{
		auto &row = *m_DoodadTable.Table[i];
		if (row.DoodadID != currDrop)
		{
			range.end = i;
			m_DoodadDropStart.insert(std::make_pair(currDrop, range));

			range.start = i;
			range.totalProb = row.DropProb;
			currDrop = row.DoodadID;
		}
		else
		{
			range.totalProb += row.DropProb;
		}
	}

	range.end = n;
	m_DoodadDropStart.insert(std::make_pair(currDrop, range));
}

void ItemConfig::BuildFashionAndDecompse()
{
	for (auto i = m_ItemTable.Table.begin(); i != m_ItemTable.Table.end(); ++i)
	{
		ItemConf* conf = *i;
		if(conf->ItemType == FASHION)
		{
			m_FashionIDs.insert((*i)->ItemID);
		}
		if (!conf->Decompose.empty())
		{
			for (auto j = conf->Decompose.begin(); j != conf->Decompose.end(); ++j)	
			{
				m_decompose[conf->ItemID].push_back(ItemDesc(j->seq[0], j->seq[1]));
			}
		}
	}
}


DesignationItem::RowData* ItemConfig::GetDesignation(UINT32 itemID) 
{
	return m_desigTable.GetByitemid(itemID);
}


void ItemConfig::BuildItemBuff()
{
	for (auto i = m_bufftable.Table.begin(); i != m_bufftable.Table.end(); ++i)
	{
		ItemBuffTable::RowData* data = *i;
		for (auto j = data->Buffs.begin(); j != data->Buffs.end(); ++j)
		{
			m_buffid2type[j->seq[0]] = data->Type;
		}
	}
}

bool ItemConfig::BuildSpecialCompose(std::unordered_map<UINT32, ItemComposeTableConf*>& spCompose)
{
	for (auto i = m_composeTable.Table.begin(); i != m_composeTable.Table.end(); i++)
	{
		ItemComposeTableConf* data = *i;
		if (data->Type == 4)
		{
			if (spCompose.find(data->SrcItem1[0]) != spCompose.end())
			{
				LogWarn("ItemComposeTable error, type 4, srcitem1 id [%u] is not unique", data->SrcItem1[0]);
				return false;
			}
			spCompose[data->SrcItem1[0]] = *i;
		}
	}
	return true;
}

UINT32 ItemConfig::GetBuffType(UINT32 buffid)
{
	auto i = m_buffid2type.find(buffid);
	if (i == m_buffid2type.end())
	{
		return 0;
	}
	else
	{
		return i->second;
	}
}

JadeConf* ItemConfig::GetJadeConfByParentItemID(UINT32 itemID)
{
	for (auto i = m_JadeTable.Table.begin(); i != m_JadeTable.Table.end(); ++i)
	{
		if ((*i)->JadeCompose[1] == itemID)
		{
			return *i;
		}
	}
	return NULL;
}

bool ItemConfig::IsFashion(int itemID)
{
	auto it = m_FashionIDs.find(itemID);
	return it != m_FashionIDs.end();
}

bool ItemConfig::GetDecomposedDropEquip(EquipConf* config, std::map<UINT32, UINT32>& items)
{
	if (config == NULL)
	{
		return true;
	}
	for (auto i = config->DecomposeDropID.begin(); i != config->DecomposeDropID.end(); ++i)
	{
		UINT32 dropid = *i;	
		const ItemDesc* dropconf = ItemDropConfig::Instance()->GetRandomDrop(dropid);
		if (NULL == dropconf)
		{
			if(dropid != 0)
			{
				SSInfo<<"dropconf is NULL, DropId:"<<dropid<<" itemid:"<<config->ItemID<<END;
			}
			continue;
		}
		auto j = items.find(dropconf->itemID);
		if (j == items.end())
		{
			items[dropconf->itemID] = dropconf->itemCount;
		}
		else
		{
			j->second += dropconf->itemCount;
		}
	}
	return true;
}

bool ItemConfig::GetDecomposedItem(UINT32 itemid, UINT32 itemcount, std::map<UINT32, UINT32>& items)
{
	const std::vector<ItemDesc>* decomposes = ItemConfig::Instance()->GetDecompsose(itemid);
	if (NULL != decomposes)
	{
		for (auto i = decomposes->begin(); i != decomposes->end(); ++i)
		{
			auto j = items.find(i->itemID);
			if (j == items.end())
			{
				items[i->itemID] = i->itemCount*itemcount;
			}
			else
			{
				items[i->itemID] += i->itemCount*itemcount;
			}
		}
	}
	return true;
}

UINT32 ItemConfig::GetItemQuailty(UINT32 itemId)
{
	ItemConf* conf = GetItemConf(itemId);
	return conf == NULL ? 0 : conf->ItemQuality;
}

bool ItemConfig::GetCircleDrawGive(UINT32 useItemID,std::vector<Sequence<uint,3>>& giveList )
{
	const UINT32 circleDrawGiveIndex = 8;//固定8个
	giveList.resize(circleDrawGiveIndex);
	std::vector< std::vector<UINT32> > select(circleDrawGiveIndex);
	for(UINT32 i = 0; i < m_CircleDrawTable.Table.size(); i++) 
	{
		CircleDrawConf* conf = m_CircleDrawTable.Table[i];		
		if(conf->UseItemID == useItemID && conf->GiveItemIndex <= circleDrawGiveIndex)
		{
			select[conf->GiveItemIndex - 1].push_back(i);
		}
	}
	for(UINT32 i = 0; i < select.size(); i++)
	{
		if(!select[i].empty())
		{
			UINT32 k = XRandom::randInt(0, select[i].size());
			UINT32 index = select[i][k];
			if(index < m_CircleDrawTable.Table.size())
			{
				CircleDrawConf* conf = m_CircleDrawTable.Table[index];	
				if(conf)
				{
					giveList[i][0] = conf->GiveItemID;
					giveList[i][1] = conf->GiveItemCount;
					giveList[i][2] = conf->GiveItemProb;
				}
			}
		}
	}
	return true;
}

UINT32 ItemConfig::GetCircleDrawNotice(UINT32 useItemID, UINT32 GiveItemID, UINT32 GiveItemCount, UINT32 GiveItemProb, UINT32 GiveItemIndex)
{
	for (UINT32 i = 0; i < m_CircleDrawTable.Table.size(); i++)
	{
		CircleDrawConf* conf = m_CircleDrawTable.Table[i];
		if (conf->UseItemID == useItemID && conf->GiveItemID == GiveItemID && conf->GiveItemCount == GiveItemCount
			&& conf->GiveItemProb == GiveItemProb && conf->GiveItemIndex == GiveItemIndex)
		{
			return conf->Notice;
		}
	}
	return 0;
}

int ItemConfig::FindEquipSuit(int prof, UINT32 itemID)	
{
	auto iter = m_ItemID2SuitID.find(itemID);
	if (iter == m_ItemID2SuitID.end())
	{
		return 0;
	}
	if(iter->second.size() == 1)
	{
		return iter->second[0].second;
	}
	for (UINT32 i = 0; i < iter->second.size(); i++)
	{
		if(prof == iter->second[i].first)
		{
			return iter->second[i].second;
		}
	}
	return 0;
}

void ItemConfig::AddSuitEffect(int suitID, int suitCount, std::vector<ItemChangeAttr> &output)
{
	EquipSuitTable::RowData* data = m_equipSuitTable.GetBySuitID(suitID);
	if (data == NULL)
	{
		return;
	}
	if (data->SuitID == suitID)
	{
#define TEST_EFFECT(i) \
		if (suitCount >= i && data->Effect##i[0] != 0)\
		{\
			output.push_back(ItemChangeAttr((int)data->Effect##i[0], (int)data->Effect##i[1]));\
		}

		TEST_EFFECT(1);
		TEST_EFFECT(2);
		TEST_EFFECT(3);
		TEST_EFFECT(4);
		TEST_EFFECT(5);
		TEST_EFFECT(6);
		TEST_EFFECT(7);
		TEST_EFFECT(8);
		TEST_EFFECT(9);
		TEST_EFFECT(10);
		
#undef TEST_EFFECT
	}
}

void DoodadIterator::OnNext()
{
	pConf = ItemConfig::Instance()->m_DoodadTable.Table[index];
}

EquipSuitDetector::EquipSuitDetector(int prof)
{
	m_prof = ProfessionMgr::Instance()->GetBasicProfession(prof);
}

EquipSuitDetector::~EquipSuitDetector()
{

}

void EquipSuitDetector::AddEquip(UINT32 itemID)
{
	int suitID = ItemConfig::Instance()->FindEquipSuit(m_prof, itemID);
	if (suitID == 0)
	{
		return;
	}

	auto it = m_SuitCounts.find(suitID);
	if (it == m_SuitCounts.end())
	{
		m_SuitCounts[suitID] = 1;
	}
	else
	{
		++it->second;
	}
}

void EquipSuitDetector::GetSuitEffects(std::vector<ItemChangeAttr>& effects)
{
	foreach(i in m_SuitCounts)
	{
		ItemConfig::Instance()->AddSuitEffect(i->first, i->second, effects);
	}
}

void EquipSuitDetector::ApplySuitEffect(CombatAttribute* combatAttr)
{
	std::vector<ItemChangeAttr> effects;
	GetSuitEffects(effects);
	ApplyEffectAux(effects, combatAttr);
}

bool ItemConfig::GetBind(UINT32 itemid, bool bind)
{
	ItemConf* conf = GetItemConf(itemid);
	if (NULL == conf)
	{
		return true;
	}
	if (conf->CanTrade)
	{
		return bind;
	}
	return true;
}

bool ItemConfig::GetPetItem(UINT32 itemid, UINT32 count, UINT32& exp, UINT32& hungry, int& mood)
{
	PetFoodConf* conf = GetPetFoodConf(itemid);
	if (NULL == conf)
	{
		return false;
	}
	exp = conf->exp*count;
	hungry = conf->hungry*count;
	mood = conf->mood*count;
	return true;
}

int ItemConfig::GetProfession(UINT32 itemid)
{
	ItemConf* conf = GetItemConf(itemid);
	return NULL == conf ? 0 : conf->Profession;
}

UINT32 ItemConfig::GetOverlap(UINT32 itemid)
{
	ItemConf* conf = GetItemConf(itemid);
	return NULL == conf ? 0 : conf->Overlap;
}

const std::vector<ItemDesc>* ItemConfig::GetDecompsose(int itemid)
{
	auto i = m_decompose.find(itemid);
	if (i == m_decompose.end())
	{
		return NULL;
	}
	return &i->second;
}

XItem ItemConfig::GetSell(UINT32 itemid)
{
	XItem item;
	ItemConf* conf = GetItemConf(itemid);
	if (NULL != conf)
	{
		item.itemID = conf->Sell[0];
		item.itemCount = conf->Sell[1];
	}
	return item;
}

UINT32 ItemConfig::ChangeProfession(UINT32 itemid, UINT32 from, UINT32 to)
{
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(itemid);
	if (NULL == conf)
	{
		LogError("conf is NULL, itemid [%u]", itemid);
		return itemid;
	}
	///> 职业信息
	if (conf->ItemType == EQUIP && from == conf->Profession)
	{
		UINT32 baseitemid = EquipProtoCalc::GetOriginItemId(itemid, from);
		UINT32 toitemid = EquipProtoCalc::ConvertToProfessionItemID(baseitemid, to); 
		ItemConf* toconf = ItemConfig::Instance()->GetItemConf(toitemid);
		if (NULL != toconf)
		{
			itemid = toitemid;
		}
		else
		{
			LogError("trans conf is NULL, trans equip itemid [%u]", toitemid);
		}
	}
	return itemid;
}

UINT32 ItemConfig::GetDoodadNoticeID(UINT32 sceneType, UINT32 itemID, UINT32 itemCount)
{
	auto iter = m_doodadNoticeMap.find(std::make_pair(sceneType, std::make_pair(itemID, itemCount)));
	if (iter == m_doodadNoticeMap.end())
	{
		return 0;
	}
	return iter->second;
}


float ItemConfig::GetReturnPrice(const UINT32 item_id,const UINT32 sealtype)
{	
	auto pItemConf = GetItemConf(item_id);
	if(NULL == pItemConf)
	{
		SSError << " find item failed itemid = " << item_id << END;
		return 1.0;
	}

	float tmpPrice = float(pItemConf->DecomposeGold);

	for(size_t i = 0; i < m_adTable.Table.size(); ++i)
	{
		auto pConf = m_adTable.Table[i];
		if(pConf->Type == sealtype && pConf->Group == pItemConf->AuctionGroup)
		{
			tmpPrice = (tmpPrice * pConf->Discount);
			break;
		}
	}

   	return tmpPrice;
}


void ItemConfig::BuildAuctItemPrevent()
{
	auctitem_prevent_.clear();
	for(size_t i = 0; i < auctitem_revover_.Table.size(); ++i)
	{
		bool prevent = false;
		auto pConf = auctitem_revover_.Table[i];
		if ( pConf->AuctPrevent > 0)
		{
			prevent = true;
		}
		auctitem_prevent_.insert(std::make_pair(pConf->ItemID,prevent));
	}
}

// 是否需要防刷
bool ItemConfig::AuctItemPrevent(const UINT32 item_id)
{
	auto it = auctitem_prevent_.find(item_id);
	if (it != auctitem_prevent_.end())
	{
		return it->second;
	}

	return false;
}

BuffDesc ItemConfig::GetTransBuff(const ItemTransformConf* conf)
{
	BuffDesc buff;
	int all = 0;
	for (auto i = conf->buff.begin(); i != conf->buff.end(); ++i)
	{
		all += i->seq[0];
	}
	if (0 == all || conf->buff.empty())
	{
		return buff;
	}
	int random = XRandom::randInt(0, all + 1);	
	all = 0;
	for (auto j = conf->buff.begin(); j != conf->buff.end(); ++j)
	{
		all += j->seq[0];
		if (random <= all)
		{
			buff.buffID = j->seq[1];
			buff.BuffLevel = 1;
			break;
		}
	}
	return buff;
}


UINT32 ItemConfig::GetEquipSuitID(UINT32 nItemID)
{
	std::map<UINT32, UINT32>::iterator itFind = m_mapEquipIDSuitID.find(nItemID);
	if( itFind == m_mapEquipIDSuitID.end() )
	{
		return -1; 
	}
	return itFind->second;
}


///>获取某个装备套装中不同装备的数量：相同装备ID的装备数量不重复计算数量
UINT32 ItemConfig::GetDiffEquipCountInSuit( Role* pRole, UINT32 equipSuitID )
{
	if( NULL == pRole )
	{
		return 0; 
	}
	Bag* pBag = pRole->Get<Bag>();
	if( NULL == pBag )
	{
		return 0; 
	}
	EquipSuitTable::RowData* pData = m_equipSuitTable.GetBySuitID(equipSuitID);
	if( NULL == pData )
	{
		return 0; 
	}
	UINT32 diffTypeCount = 0; 
	for( UINT32 i=0; i<pData->EquipID.size(); i++ )
	{
		UINT32 curEquipCount = pBag->CountItemTotal( pData->EquipID[i] );
		if( 0 == curEquipCount )
		{
			continue;
		}
		diffTypeCount += 1; 
	}
	return diffTypeCount;
}

UINT32 ItemConfig::GetJewelrySuitID(UINT32 nSuitID)
{
	auto jewFind = std::find( m_vecJewelrySuitID.begin(), m_vecJewelrySuitID.end(), nSuitID);
	if( jewFind == m_vecJewelrySuitID.end() )
	{
		return -1;
	}
	return *jewFind;
}

UINT32 ItemConfig::GetEquipInJewelrySuit(UINT32 nItemID)
{
	auto iter =  m_mapEquipInJewelrySuit.find(nItemID);
	if(iter == m_mapEquipInJewelrySuit.end())
		return -1;
	return iter->second;
}

void ItemConfig::BuildDesignation()
{
	m_mapEquipIDSuitID.clear();
	m_vecJewelrySuitID.clear();
	m_mapEquipInJewelrySuit.clear();
	for( auto i= m_equipSuitTable.Table.begin(); i!= m_equipSuitTable.Table.end(); i++ )
	{
		if( (*i)->EquipID.size() == 0 )
			continue; 

		for( auto x = (*i)->EquipID.begin(); x != (*i)->EquipID.end(); x++ )
			m_mapEquipIDSuitID.insert( std::make_pair(*x, (*i)->SuitID) );

		UINT32 firstItemId = (*i)->EquipID[0];
		EquipConf* pConf = ItemConfig::Instance()->GetEquipConf( firstItemId );
		if( NULL == pConf )
			continue; 

		if(!XItem::IsJewelry(pConf->EquipPos))
			continue; 

		m_vecJewelrySuitID.push_back( (*i)->SuitID );
		for( auto x= (*i)->EquipID.begin(); x!= (*i)->EquipID.end(); x++)
		{
			m_mapEquipInJewelrySuit.insert( std::make_pair(*x, (*i)->SuitID));
		}
	}
}


///>获取当前装备所在的套装的强化等级
UINT32 ItemConfig::GetEquipSuitLevel(Role* pRole, UINT64 equipUid, UINT32 needLevel,UINT32 needCount)
{
	Bag* pBag = pRole->Get<Bag>();
	if( NULL == pBag )
	{
		return 0;
	}
	XItem* pItem  = pBag->FindUniqueEquipOnBody(equipUid);
	if( NULL == pItem )
	{
		return 0; 
	}

	UINT32 suitId = 0; 
	for( auto i=m_equipSuitTable.Table.begin(); i!=m_equipSuitTable.Table.end(); i++ )
	{
		if( std::find( (*i)->EquipID.begin(), (*i)->EquipID.end(), pItem->itemID) == (*i)->EquipID.end() )
		{
			continue; 
		}
		suitId = (*i)->SuitID;
		break; 
	}


	if( suitId == 0 )
		return 0; 

	EquipSuitTable::RowData* pConfig = m_equipSuitTable.GetBySuitID(suitId);
	if( NULL == pConfig )
		return 0; 
	std::vector<XItem*> vecItems;
	UINT32 nCount = 0;
	for( UINT32 i=0; i<pConfig->EquipID.size(); i++ )
	{
		UINT32 curEquipID = pConfig->EquipID[i];
		vecItems.clear();
		pBag->FindItemsOnBody(curEquipID, vecItems);
		for(auto j = vecItems.begin(); j != vecItems.end(); j++)
		{
			XItem* item = *j;
			if(item->enhanceInfo && item->enhanceInfo->level >= needLevel)
			{
				nCount++;
			}
		}
		if (nCount>=needCount)
		{
			return 1;
		}
	}

	return 0;
}