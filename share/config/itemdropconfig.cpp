#include "pch.h"
#include "itemdropconfig.h"

INSTANCE_SINGLETON(ItemDropConfig);

ItemDropConfig::ItemDropConfig()
{

}

ItemDropConfig::~ItemDropConfig()
{
}

bool ItemDropConfig::Init()
{
	return LoadFile();
}

void ItemDropConfig::Uninit()
{
	ClearFile();
}

bool ItemDropConfig::CheckFile()
{
	DropList tempdroptable;	
	if (!tempdroptable.LoadFile("table/DropList.txt"))
	{
		SSWarn<<"load file table/DropList.txt failed"<<END;
		return false;
	}
	return true;
}

bool ItemDropConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_droptable.LoadFile("table/DropList.txt");

	for (auto i = m_droptable.Table.begin(); i != m_droptable.Table.end(); ++i)
	{
		DropConf* conf = *i;

		DropLevelInfo* levelinfo = NULL;
		auto j = m_id2drop.find(conf->DropID);
		if (j == m_id2drop.end())
		{
			levelinfo = new DropLevelInfo();
			m_id2drop[conf->DropID] = levelinfo;
		}
		else
		{
			levelinfo = j->second;
		}

		UINT32 level = conf->DropLevel[0] * 1000  + conf->DropLevel[1];
		DropInfo* info = NULL;
		auto k = levelinfo->m_level2drop.find(level);
		if (k == levelinfo->m_level2drop.end())
		{
			info =  new DropInfo();
			info->end += conf->DropProb;
			levelinfo->m_level2drop[level] = info;
		}
		else
		{
			info = k->second;
			info->end += conf->DropProb;
		}
		
		DropItem drop;
		drop.prop = info->end;
		drop.confprop = conf->DropProb;
		drop.item = ItemDesc(conf->ItemID, conf->ItemCount, conf->ItemBind);

		info->items.push_back(drop);
	}

	for (auto j = m_id2drop.begin(); j != m_id2drop.end(); ++j)
	{
		DropLevelInfo* info = j->second;
		if (info->m_level2drop.find(0) != info->m_level2drop.end())
		{
			DropInfo* tempinfo = info->m_level2drop[0];
			for (auto k = info->m_level2drop.begin(); k != info->m_level2drop.end(); ++k)
			{
				if (0 != k->first)
				{
					for (auto l = tempinfo->items.begin(); l != tempinfo->items.end(); ++l)
					{
						k->second->end += l->confprop;

						DropItem temp;
						temp.prop = k->second->end;
						temp.confprop = l->confprop;
						temp.item = l->item;

						k->second->items.push_back(temp);
					}
				}
			}
		}
	}

	return true;
}

void ItemDropConfig::ClearFile()
{
	m_droptable.Clear();
	for (auto i = m_id2drop.begin(); i != m_id2drop.end(); ++i)
	{
		delete i->second;
		i->second = NULL;
	}
	m_id2drop.clear();
}

const ItemDesc* ItemDropConfig::GetRandomDrop(UINT32 dropid, UINT32 level, XRandomType randType)
{
	auto i = m_id2drop.find(dropid);
	if (i == m_id2drop.end())
	{
		return NULL;
	}
	DropInfo* info = NULL;
	DropLevelInfo* levelinfo = i->second;
	for (auto j = levelinfo->m_level2drop.begin(); j != levelinfo->m_level2drop.end(); ++j)
	{
		if (0 == level)
		{
			info = j->second;
			break;
		}
		if (0 == j->first)
		{
			info = j->second;
		}
		UINT32 begin = j->first/1000;
		UINT32 end = j->first%1000;
		if (level >= begin && level <= end)
		{
			info = j->second;
			break;
		}
	}
	if (NULL == info)
	{
		return NULL;
	}
	UINT32 random = GetRandomInt(1, info->end + 1, randType);
	for (auto k = info->items.begin(); k != info->items.end(); ++k)
	{
		if (random <= k->prop)
		{
			if (0 == k->item.itemID) //没有随机到物品
			{
				return NULL;
			}
			return &k->item;
		}
	}
	return NULL;
}

void ItemDropConfig::GetAllDropItem(UINT32 dropid, std::vector<ItemDesc>& vecItems, int* hasNullItem)
{
	auto i = m_id2drop.find(dropid);
	if (i == m_id2drop.end())
	{
		return;
	}
	if (hasNullItem != NULL) *hasNullItem = 0;
	for (auto j = i->second->m_level2drop.begin(); j != i->second->m_level2drop.end(); j++)
	{
		const DropInfo* info = j->second;
		for (auto k = info->items.begin(); k != info->items.end(); k++)
		{
			if (0 == k->item.itemID) //0意味着没有这个物品
			{
				if (hasNullItem != NULL) *hasNullItem = 1;
				continue;
			}
			vecItems.push_back(k->item);
		}
	}
}

void ItemDropConfig::Test()
{
	for (auto i = m_id2drop.begin(); i != m_id2drop.end(); ++i)
	{
		const ItemDesc* item = GetRandomDrop(i->first, 40);
		if (NULL == item)
		{
			assert(false);
		}
	}
}

UINT32 ItemDropConfig::GetRandomInt(UINT32 Min, UINT32 Max, XRandomType randType)
{
	switch(randType)
	{
	case SpriteRandom:
		{
			return XSpriteRandom::randInt(Min, Max);
		}
	case PandoraRandom:
		{
			return XPandoraRandom::randInt(Min, Max);
		}
	default:
		{
			return XRandom::randInt(Min, Max);
		}
	}
}
