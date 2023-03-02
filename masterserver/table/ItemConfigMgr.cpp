#include "pch.h"
#include "ItemConfigMgr.h"
#include "define/itemdef.h"

INSTANCE_SINGLETON(CItemConfig)

CItemConfig::CItemConfig()
{

}

CItemConfig::~CItemConfig()
{

}

bool CItemConfig::Init()
{
	return LoadFile();
}

void CItemConfig::Uninit()
{
	ClearFile();
}

bool CItemConfig::Reload()
{
	Uninit();
	return Init();
}

bool CItemConfig::CheckFile()
{
	ItemList tmpItemList;
	if (!tmpItemList.LoadFile("table/ItemList.txt"))
	{
		LogWarn("Load ItemList.txt failed!");
		return false;
	}

	return true;
}

bool CItemConfig::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();
	
	m_ItemTable.LoadFile("table/ItemList.txt");

	return true;
}

void CItemConfig::ClearFile()
{
	m_ItemTable.Clear();
}

const std::string CItemConfig::GetItemName(int ItemID, int prof)
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

INT32 CItemConfig::GetOverlap(UINT32 itemid)
{
	ItemConf* conf = GetItemConf(itemid);
	return NULL == conf ? 0 : conf->Overlap;
}

std::string CItemConfig::GetItemProfessionName(UINT32 ItemID, int profession)
{
	auto pConf = GetItemConf(ItemID);
	if(pConf && 12 == pConf->ItemType)// 装备原型
	{
		return GetItemName(profession/10 * 100000 + ItemID);
	}
	return GetItemName(ItemID);
}

UINT32 CItemConfig::ConvertItemId(UINT32 itemId, int profession)
{
	auto pConf = GetItemConf(itemId);
	if (pConf == NULL)
	{
		return itemId;
	}
	if (pConf->ItemType == EQUIP_PROTO)
	{
		return profession / 10 * 100000 + itemId;
	}
	return itemId;
}

INT32 CItemConfig::GetItemID(const std::string& strItemName)
{
	for (auto i = m_ItemTable.Table.begin(); i != m_ItemTable.Table.end(); i ++)
	{
		for (auto it = (*i)->ItemName.begin(); it != (*i)->ItemName.end(); it ++)
		{
			if ((*it) == strItemName)
			{
				return (*i)->ItemID;
			}
		}
	}
	return 0;
}
