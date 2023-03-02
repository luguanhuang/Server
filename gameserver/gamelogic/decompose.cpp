#include "pch.h"
#include "decompose.h"
#include "item.h"
#include "bagtransition.h"
#include "math.h"
#include "unit/role.h"
#include "gamelogic/levelsealMgr.h"

void DecomposeHelper::Init(const std::vector<std::pair<XItem*, UINT32>>& items, float param)
{
	m_items = items;
	m_param = param;
}

bool DecomposeHelper::Fini()
{
	std::vector<std::map<UINT32, UINT32>> items;
	std::map<UINT32, UINT32> jades;
	bool istrue = true;
	std::multimap<UINT32,UINT32> dragon_coins_item;
	for (auto i = m_items.begin(); i != m_items.end(); ++i)
	{
		XItem* pItem = (*i).first;
		UINT32 count = (*i).second;
		
		dragon_coins_item.insert(std::make_pair(pItem->itemID, count));

		std::map<UINT32, UINT32> tempitems;
		istrue = pItem->DecomposeItem(count, tempitems, jades);
		if (!istrue)
		{
			break;
		}
		if (m_param > 1.0f)
		{
			for (auto j = tempitems.begin(); j != tempitems.end(); ++j)
			{
				j->second = (UINT32)ceil((float)(m_param * j->second));
			}	
		}
	
		items.push_back(tempitems);
	}
	if (!istrue)
	{
		return false;
	}

	BagTakeItemTransition taketransition(m_role);
	taketransition.SetReason(ItemFlow_Decompose, ItemFlow_Decompose_Item);
	for (auto i = m_items.begin(); i != m_items.end(); ++i)
	{
		if (!taketransition.TakeUniqueItem((*i).first->uid, (*i).second))
		{
			taketransition.RollBack();
			return false;
		}
	}
	taketransition.NotifyClient();

	BagGiveItemTransition givetransition(m_role);
	givetransition.SetReason(ItemFlow_Decompose, ItemFlow_Decompose_Item);
	for (auto i = items.begin(); i != items.end(); ++i)
	{
		for (auto j = i->begin(); j != i->end(); ++j)
		{
			if (j->first != 0 && j->second != 0)
			{
				givetransition.GiveItem(j->first, j->second);
			}
		}
	}

	// ·µÁú±Ò
	UINT32 sealtype = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;	
	for (auto i = dragon_coins_item.begin(); i != dragon_coins_item.end(); ++i)
	{
		UINT32 dragon_coin_count = (UINT32) (ItemConfig::Instance()->GetReturnPrice(i->first,sealtype)) * i->second;
		if (dragon_coin_count  > 0)
		{
			givetransition.GiveItem(DRAGON_COIN,dragon_coin_count);
		}
	}

	for (auto i = jades.begin(); i != jades.end(); ++i)
	{
		givetransition.GiveItem(i->first, i->second);
	}	
	givetransition.NotifyClient();

	m_role->OnDecompose();

	return true;
}
