#include "pch.h"
#include "bag.h"
#include "idip/idip.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
CBag::CBag(KKSG::RoleAllInfo* pInfo)
{
	pRoleInfo = pInfo;
	memset(m_VirtualItem, 0, sizeof(m_VirtualItem));
	Init();
}
CBag::~CBag()
{
	 for (UINT32 i = 0; i < m_Items.size(); i ++)
	 {
		 delete m_Items[i];
		 m_Items[i] = NULL;
	 }
}
void CBag::Init()
{
	int N = (VIRTUAL_ITEM_MAX > pRoleInfo->bag().virtualitems_size()) ? pRoleInfo->bag().virtualitems_size() : VIRTUAL_ITEM_MAX;
	for (int j = 0; j < N; ++j)
	{
		m_VirtualItem[j] = pRoleInfo->bag().virtualitems(j);
	}

	for (int i = 0; i < pRoleInfo->bag().items_size(); i ++)
	{
		KKSG::Item* pItem = new KKSG::Item();
		pItem->CopyFrom(pRoleInfo->bag().items(i));
		m_Items.push_back(pItem);
	}

	for (int i = 0; i < pRoleInfo->designatinorecord().designationdata_size(); i ++)
	{
		m_Designations.push_back(pRoleInfo->designatinorecord().designationdata(i));
	}

	for (int i = 0; i < pRoleInfo->fashionrecord().bodyfashion_size(); i ++)
	{
		m_BodyFashion.push_back(pRoleInfo->fashionrecord().bodyfashion(i));
	}

	for (int i = 0; i < pRoleInfo->fashionrecord().bagfashion_size(); i ++)
	{
		m_BagFashion[pRoleInfo->fashionrecord().bagfashion(i).uid()] =  pRoleInfo->fashionrecord().bagfashion(i);
	}

	for(int i = 0; i < pRoleInfo->bag().equips_size(); i ++)
	{
		m_BodyItems.push_back(pRoleInfo->bag().equips(i));
	}

	for (int i = 0; i < pRoleInfo->bag().emblems_size(); i ++)
	{
		m_BodyEmblems.push_back(pRoleInfo->bag().emblems(i));
	}
}
void CBag::Save()
{
	pRoleInfo->mutable_bag()->clear_virtualitems();
	for (int i = 0; i < VIRTUAL_ITEM_MAX; ++i)
	{
		pRoleInfo->mutable_bag()->add_virtualitems(m_VirtualItem[i]);
	}

	pRoleInfo->mutable_bag()->clear_items();
	for (UINT32 i = 0; i < m_Items.size(); ++ i)
	{
		if (m_Items[i]->itemid() && m_Items[i]->itemcount())
		{
			KKSG::Item* item = pRoleInfo->mutable_bag()->add_items();
			item->CopyFrom(*m_Items[i]);
		}
	}

	pRoleInfo->mutable_designatinorecord()->clear_designationdata();
	for (UINT32 i = 0; i < m_Designations.size(); ++ i)
	{
		KKSG::StcDesignationInfo* pData = pRoleInfo->mutable_designatinorecord()->add_designationdata();
		pData->CopyFrom(m_Designations[i]);
	}

	pRoleInfo->mutable_fashionrecord()->clear_bodyfashion();
	for (auto i = m_BodyFashion.begin(); i != m_BodyFashion.end(); i ++)
	{
			KKSG::FashionData* pFash = pRoleInfo->mutable_fashionrecord()->add_bodyfashion();
			pFash->CopyFrom(*i);
	}

	pRoleInfo->mutable_fashionrecord()->clear_bagfashion();
	for (auto i = m_BagFashion.begin(); i != m_BagFashion.end(); i ++)
	{
			KKSG::FashionData* pFashionData = pRoleInfo->mutable_fashionrecord()->add_bagfashion();
			pFashionData->CopyFrom(i->second);
	}

	pRoleInfo->mutable_bag()->clear_equips();
	for (auto i = m_BodyItems.begin(); i != m_BodyItems.end(); i ++)
	{
			KKSG::Item* pBodyItem = pRoleInfo->mutable_bag()->add_equips();
			pBodyItem->CopyFrom(*i);
	}

	pRoleInfo->mutable_bag()->clear_emblems();
	for (auto i = m_BodyEmblems.begin(); i != m_BodyEmblems.end(); i ++)
	{
			KKSG::Item* pBodyEmblem = pRoleInfo->mutable_bag()->add_emblems();
			pBodyEmblem->CopyFrom(*i);
	}
	return ;
}
bool CBag::IsVirtualItem(UINT32 itemid)
{
	return itemid < VIRTUAL_ITEM_MAX;
}
//游戏币修改
void CBag::ReduceVirtualItem(UINT32 itemid, INT64 Count)
{
	Count = Count>0?Count:-1*Count;
	if (IsVirtualItem(itemid))
	{
		if (m_VirtualItem[itemid] < Count)//全部扣除
		{
			m_VirtualItem[itemid] = 0;
		}
		else
		{
			m_VirtualItem[itemid] -= Count;
		}
	}
}

//扣除
bool CBag::ReduceItem(UINT32 itemid, INT64 Count)
{
	Count = Count>0?Count:-1*Count;
	if (IsVirtualItem(itemid))
	{
		ReduceVirtualItem(itemid, Count);
		return true;
	}
	SortCount(m_Items);
	for (auto  i = m_Items.begin(); i != m_Items.end();)
	{
		if (itemid == (*i)->itemid())
		{
			INT64 tempcount = (*i)->itemcount();
			INT64 subcount = (tempcount >= Count) ? Count : tempcount;
			Count -= subcount;
			(*i)->set_itemcount(UINT32(tempcount-subcount));
			if (Count == 0)
			{
				break;
			}
		}
		if ((*i)->itemcount() == 0)
		{
			delete (*i);
			(*i) = NULL;
			i = m_Items.erase(i);
		}
		else
		{
			i ++;
		}
	}
	return true;
}

INT32 CBag::CountItem(UINT32 itemid)
{
	INT64 tempcount = 0;
	if (IsVirtualItem(itemid))
	{
		tempcount = m_VirtualItem[itemid];
	}
	else
	{
		for (UINT32 i = 0; i < m_Items.size(); ++ i)
		{
			if (itemid == m_Items[i]->itemid())
			{
				tempcount += m_Items[i]->itemcount();
			}
		}
	}
	return (INT32)tempcount;
}
void CBag::FindItem(UINT32 itemID, std::vector<KKSG::Item*>& items, ItemBind filter)
{
	for (UINT32 i = 0; i < m_Items.size(); ++ i)
	{
		if (itemID != m_Items[i]->itemid() || m_Items[i]->cooldown() != 0)
		{
			continue;
		}
		if (Bind_ALL == filter || m_Items[i]->isbind() == (filter == Bind_Is))
		{
			items.push_back(m_Items[i]);
		}
	}
}
void CBag::AddNewItem(UINT32 itemid, INT32 iType, INT64 lCount, bool isbind)
{
	KKSG::Item* pItem = new KKSG::Item();
	pItem->set_uid(0);
	pItem->set_itemid(itemid);
	pItem->set_itemcount(lCount);
	pItem->set_itemtype(iType);
	pItem->set_isbind(isbind);
	m_Items.push_back(pItem);
}
bool CBag::AddItem(UINT32 itemID, INT64 allcount, bool isbind)
{
	if (IsVirtualItem(itemID))
	{
		m_VirtualItem[itemID] += allcount;
		return true;
	}

	ItemConf *pConf = CItemConfig::Instance()->GetItemConf(itemID);
	if (NULL == pConf)
	{
		return false;
	}
	INT32 overlap = CItemConfig::Instance()->GetOverlap(itemID);
	if (1 == overlap)//目前不支持有随机属性的离线添加;
	{
		return false;
	}
	std::vector<KKSG::Item*> items;
	FindItem(itemID, items, (ItemBind)isbind);
	SortCount(items);

	KKSG::Item* item = items.empty() ? NULL : items[0];
	if (0 == overlap)
	{
		///> 无限堆叠
		if (NULL == item)
		{
			AddNewItem(itemID, pConf->ItemType, allcount, isbind);
		}
		else
		{
			item->set_itemcount(item->itemcount()+allcount);
		}
	}
	else
	{
		if (NULL != item)
		{
			int left = overlap - item->itemcount();
			if (left > 0)
			{
				int tempcount = left > allcount ? allcount : left;	
				item->set_itemcount(item->itemcount()+tempcount);
				allcount -= left;
			}
		}
		if (allcount > 0)
		{
			int left = allcount % overlap;
			int part = (int)(allcount/overlap);
			for (int i = 0; i < part; ++i)
			{
				AddNewItem(itemID, pConf->ItemType, overlap, isbind);
			}
			if (left > 0)
			{
				AddNewItem(itemID, pConf->ItemType, left, isbind);
			}
		}
	}
	return true;
}
bool CBag::SetItemCount(UINT32 itemid, INT32 Count)
{
	INT32 iCurCount = CountItem(itemid);
	INT32 iTempCount = abs(iCurCount - Count);
	if (Count > iCurCount) //add
	{
		 return AddItem(itemid, iTempCount);
	}
	else
	{
		return ReduceItem(itemid, iTempCount);
	}
	return true;
}

bool CBag::SetScore(UINT32 uType, INT32 Count)
{
	if (uType == SCORE_TYPE_DRAGON_COIN || uType == SCORE_TYPE_ALL)
	{
		if (!SetItemCount(DRAGON_COIN, Count))
		{
			return false;
		}
	}
	if (uType == SCORE_TYPE_ARENA_COIN || uType == SCORE_TYPE_ALL)
	{
		if (!SetItemCount(ARENA_COIN, Count))
		{
			return false;
		}
	}
	if (uType == SCORE_TYPE_GODDNESS_TEARS || uType == SCORE_TYPE_ALL)
	{
		if (!SetItemCount(GODDNESS_TEARS_ID, Count))
		{
			return false;
		}
	}
	if (uType == SCORE_TYPE_GUILD_CONTRIBUTE || uType == SCORE_TYPE_ALL)
	{
		if (!SetItemCount(GUILD_CONTRIBUTE, Count))
		{
			return false;
		}
	}

	if (uType == SCORE_TYPE_PKPOINT || uType == SCORE_TYPE_ALL)
	{
		pRoleInfo->mutable_pkrecord()->set_point(Count);
	}

	if (uType == SCORE_TYPE_ARENA_POINT || uType == SCORE_TYPE_ALL)
	{
		pRoleInfo->mutable_arenarecord()->set_point(Count);
	}

	return true;
}

bool CBag::CompareCount(KKSG::Item* fir, KKSG::Item* sec) 
{ 
	if (fir->cooldown() != sec->cooldown())
	{
		return fir->cooldown() > sec->cooldown();
	}
	if (fir->itemid() != sec->itemid())
	{
		return fir->itemid() < sec->itemid();
	}
	return fir->itemcount() < fir->itemcount();
}

void CBag::SortCount(std::vector<KKSG::Item*>& items)
{
	std::sort(items.begin(), items.end(), CompareCount);
}

void CBag::DelDesignationID(UINT32 ID)
{
	auto it = std::find_if(m_Designations.begin(), m_Designations.end(), EqualDesignationID(ID));
	if (it != m_Designations.end())
	{
		m_Designations.erase(it);
	}
	if (ID == pRoleInfo->designatinorecord().abilitydesignationid())
	{
		pRoleInfo->mutable_designatinorecord()->set_abilitydesignationid(0);
	}
	if (ID == pRoleInfo->designatinorecord().coverdesignationid())
	{
		pRoleInfo->mutable_designatinorecord()->set_coverdesignationid(0);
	}
}

bool CBag::DelFashion(UINT32 itemid, UINT64 uid)
{
	auto it = m_BagFashion.find(uid);
	if (it != m_BagFashion.end() && it->second.itemid() == itemid)
	{
		m_BagFashion.erase(it);
		return true;
	}
	auto i = std::find_if(m_BodyFashion.begin(), m_BodyFashion.end(), EqualID(uid));
	if (i != m_BodyFashion.end() && i->itemid() == itemid)
	{
		KKSG::FashionData oData;
		i->CopyFrom(oData);
		return true;
	}
	return false;
}

bool CBag::DelItem(UINT32 itemid, UINT64 uid)
{
	for (auto  i = m_Items.begin(); i != m_Items.end();)
	{
		if ((*i)->uid() == uid && (*i)->itemid() == itemid)
		{
			delete (*i);
			(*i) = NULL;
			m_Items.erase(i);
			return true;
		}
		i ++;
	}
	return false;
}
bool CBag::DelItemByUid(UINT32 itemid, UINT64 uid)
{
	auto it = std::find_if(m_BodyItems.begin(), m_BodyItems.end(), EqualID(uid));
	if (it != m_BodyItems.end() && it->itemid() == itemid)
	{
		KKSG::Item oData;
		it->CopyFrom(oData);
		return true;
	}
	return DelItem(itemid, uid);
}

bool CBag::DelEmblemByUid(UINT32 itemid, UINT64 uid)
{
	auto it = std::find_if(m_BodyEmblems.begin(), m_BodyEmblems.end(), EqualID(uid));
	if (it != m_BodyEmblems.end() && it->itemid() == itemid)
	{
		KKSG::Item oData;
		it->CopyFrom(oData);
		return true;
	}
	return DelItem(itemid, uid);
}

bool CBag::DelItemJade(KKSG::ItemJade* pData, INT32 jadeid)
{
	if (NULL == pData)
	{
		return false;
	}
	std::vector<KKSG::ItemJadeSingle> vList;
	bool isfind = false;
	for (INT32 i = 0; i < pData->itemjadesingle_size(); i ++)
	{
		if (pData->itemjadesingle(i).itemid() == jadeid)
		{
			isfind = true;
			continue;
		}
		vList.push_back(pData->itemjadesingle(i));
	}
	if (!isfind)
	{
		return false;
	}
	pData->clear_itemjadesingle();
	for (INT32 i = 0; i < vList.size(); i ++)
	{
		KKSG::ItemJadeSingle* pJade = pData->add_itemjadesingle();
		pJade->CopyFrom(vList[i]);
	}
	return true;
}

bool CBag::DelItemJade(UINT32 itemid, UINT64 uid, INT32 jadeid)
{
	auto it = std::find_if(m_BodyItems.begin(), m_BodyItems.end(), EqualID(uid));
	if (it != m_BodyItems.end() && it->itemid() == itemid)
	{
		if (DelItemJade(it->mutable_itemjade(), jadeid))
		{
			return true;
		}
	}
	for (auto  i = m_Items.begin(); i != m_Items.end(); i ++)
	{
		if ((*i)->uid() == uid && (*i)->itemid() == itemid)
		{
			if (DelItemJade((*i)->mutable_itemjade(), jadeid))
			{
				return true;
			}
		}
	}
	return false;
}
