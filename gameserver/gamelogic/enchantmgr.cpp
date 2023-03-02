#include "pch.h"
#include "enchantmgr.h"
#include "item.h"
#include "itemconfig.h"
#include "globalconfig.h"
#include "bagtransition.h"
#include "unit/role.h"
#include <cmath>

INSTANCE_SINGLETON(EnchantMgr)

EnchantMgr::EnchantMgr()
{

}

EnchantMgr::~EnchantMgr()
{

}

bool EnchantMgr::Init()
{
	return LoadFile();
}

void EnchantMgr::Uninit()
{

}

bool EnchantMgr::CheckFile()
{
	EnchantEquip tmpEnchantEquip;
	if (!tmpEnchantEquip.LoadFile("table/EnchantEquip.txt"))
	{
		LogWarn("load file table/EnchantEquip.txt failed");
		return false;
	}
	for (auto i = tmpEnchantEquip.Table.begin(); i != tmpEnchantEquip.Table.end(); ++i)
	{
		EnchantConf* conf = *i;
		if (conf->BaodiCount == 0 || conf->BaodiAttribute[0] == 0)
		{
			continue;
		}
		if (conf->BaodiAttribute[1] > conf->BaodiAttribute[2])
		{
			LogWarn("EnchantID [%u], BaodiAttribute is invalid", conf->EnchantID);
			return false;
		}
		UINT32 attrMin = -1, attrMax = -1;
		for (auto j = (*i)->Attribute.begin(); j != (*i)->Attribute.end(); ++j)
		{
			if ((*j)[1] > (*j)[2])
			{
				LogWarn("EnchantID [%u], Attribute range is invalid", conf->EnchantID);
				return false;
			}
			if ((*j)[0] == conf->BaodiAttribute[0])
			{
				if (attrMin == -1)
				{
					attrMin = (*j)[1];
					attrMax = (*j)[2];
				}
				else
				{
					attrMin = std::min(attrMin, (*j)[1]);
					attrMax = std::max(attrMax, (*j)[2]);
				}
			}
		}
		if (attrMin == -1)
		{
			LogWarn("EnchantID [%u], BaodiAttribute attrid not in Attribute", conf->EnchantID);
			return false;
		}
		if (attrMin > conf->BaodiAttribute[1] || attrMax < conf->BaodiAttribute[2])
		{
			LogWarn("EnchantID [%u], BaodiAttribute not in Attribute", conf->EnchantID);
			return false;
		}
	}

	return true;
}

bool EnchantMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oTable.LoadFile("table/EnchantEquip.txt");

	return true;
}

void EnchantMgr::ClearFile()
{
	m_oTable.Clear();
}

ItemChangeAttr EnchantMgr::GetRandomEnchantAttr(UINT32 enchantid)
{
	ItemChangeAttr attr;
	EnchantEquip::RowData* data = m_oTable.GetByEnchantID(enchantid);
	if (data == NULL)
	{
		return attr;
	}
	UINT32 sum = 0;
	for (UINT32 i = 0; i < data->Attribute.size(); ++i)
	{
		sum += data->Attribute[i][3];
	}
	if (sum == 0)
	{
		LogWarn("enchantid:%u in EnchantEquip.txt, Attribute random size is 0", enchantid);
		return attr;
	}
	UINT32 random = XRandom::randInt(1, sum + 1), count = 0;
	for (UINT32 i = 0; i < data->Attribute.size(); ++i)
	{
		count += data->Attribute[i][3];
		if (count >= random)
		{
			attr.AttrID = data->Attribute[i][0];
			attr.AttrValue = XRandom::randInt(data->Attribute[i][1], data->Attribute[i][2] + 1);
			return attr;
		}
	}
	return attr;
}

KKSG::ErrorCode EnchantMgr::DoTransfer(Role* pRole, XItem* origin, XItem* dest, bool isNotify)
{
	if (pRole == NULL || origin == NULL || dest == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (origin->enchant == NULL)
	{
		return KKSG::ERR_ENCHANT_NOTHAVE;
	}

	ItemConf* itemConf = ItemConfig::Instance()->GetItemConf(dest->itemID);
	if (itemConf == NULL)
	{
		LogWarn("dest equipid [%u] not in ItemList.txt", dest->itemID);
		return KKSG::ERR_UNKNOWN;
	}
	if (itemConf->ReqLevel < GetGlobalConfig().GetInt("EnchantMinLevel", 33))
	{
		return KKSG::ERR_ENCHANT_MINLEVEL;
	}

	std::swap(origin->enchant, dest->enchant);
	if (origin->enchant != NULL) origin->enchant->mapBaodiCount.clear();
	if (dest->enchant != NULL) dest->enchant->mapBaodiCount.clear();

	if (isNotify)
	{
		BagUpdateItemTransition transition(pRole);
		transition.AddChangedItem(origin);
		transition.AddChangedItem(dest);
		transition.NotifyClient();
	}

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode EnchantMgr::CanEnchantWearEquip(UINT32 equipid, UINT32 enchantid)
{
	EnchantConf* conf = EnchantMgr::Instance()->GetEnchantConf(enchantid);
	if (conf == NULL)
	{
		LogWarn("enchantid [%u] not in Enchant.txt", enchantid);
		return KKSG::ERR_UNKNOWN;
	}

	ItemConf* itemConf = ItemConfig::Instance()->GetItemConf(equipid);
	if (itemConf == NULL)
	{
		LogWarn("equipid [%u] not in ItemList.txt", equipid);
		return KKSG::ERR_UNKNOWN;
	}
	if (itemConf->ReqLevel < GetGlobalConfig().GetInt("EnchantMinLevel", 33))
	{
		return KKSG::ERR_ENCHANT_MINLEVEL;
	}
	
	EquipConf* equipConf = ItemConfig::Instance()->GetEquipConf(equipid);
	if (equipConf == NULL)
	{
		LogWarn("equipid [%u] not in EquipList.txt", equipid);
		return KKSG::ERR_UNKNOWN;
	}

	bool canEnchant = false;
	for (UINT32 i = 0; i < conf->Pos.size(); ++i)
	{
		if (conf->Pos[i] == equipConf->EquipPos)
		{
			canEnchant = true;
			break;
		}
	}
	if (!canEnchant)
	{
		return KKSG::ERR_ENCHANT_WRONGPOS;
	}	

	return KKSG::ERR_SUCCESS;
}

std::pair<UINT32, UINT32> EnchantMgr::GetEnchantAttrValueRange(UINT32 enchantid, UINT32 attrid)
{
	EnchantEquip::RowData* data = m_oTable.GetByEnchantID(enchantid);
	if (data == NULL)
	{
		return std::make_pair(0, 0);
	}
	UINT32 attrMin = 0, attrMax = 0;
	for (UINT32 i = 0; i < data->Attribute.size(); ++i)
	{
		if (attrid == data->Attribute[i][0])
		{
			if (attrMin == 0) attrMin = data->Attribute[i][1];
			else attrMin = std::min(attrMin, data->Attribute[i][1]);
			attrMax = std::max(attrMax, data->Attribute[i][2]);
		}
	}
	return std::make_pair(attrMin, attrMax);
}