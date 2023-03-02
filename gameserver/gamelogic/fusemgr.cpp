#include "pch.h"
#include "fusemgr.h"
#include "itemconfig.h"
#include "item.h"
#include "bagtransition.h"
#include "unit/role.h"


FuseMgr::FuseMgr()
{

}

FuseMgr::~FuseMgr()
{

}

bool FuseMgr::Init()
{
	return LoadFile();
}

void FuseMgr::Uninit()
{

}

bool FuseMgr::CheckFile()
{
	EquipFusionTable tmpTable;
	if (!tmpTable.LoadFile("table/EquipFusion.txt"))
	{
		LogWarn("Load table/EquipFusion.txt failed");
		return false;
	}

	EquipFusionExpTable tmpExpTable;
	if (!tmpExpTable.LoadFile("table/EquipFusionExp.txt"))
	{
		LogWarn("Load table/EquipFuionExp.txt failed");
		return false;
	}

	return true;
}

bool FuseMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oTable.LoadFile("table/EquipFusion.txt");
	m_oExpTable.LoadFile("table/EquipFusionExp.txt");

	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++i)
	{
		m_mapFusionConf[MakeKey((*i)->Profession, (*i)->Slot, (*i)->EquipType, (*i)->BreakNum)] = *i;
	}

	return true;
}

void FuseMgr::ClearFile()
{
	m_oTable.Clear();
	m_oExpTable.Clear();
	m_mapFusionConf.clear();
}

KKSG::ErrorCode FuseMgr::FuseEquipAddExp(Role* pRole, XItem* pItem, UINT32 itemID)
{
	if (pRole == NULL || pItem == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	EquipConf* conf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (conf == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (!conf->FuseCanBreakNum)
	{
		return KKSG::ERR_CANT_FUSE;
	}
	FuseAttr* &fuse = pItem->fuse;
	if (fuse == NULL)
	{
		fuse = new FuseAttr;
	}
	if (fuse->fuseLevel > conf->FuseCanBreakNum)
	{
		LogError("itemid [%u], uid [%llu], fuseLevel [%u] > FuseCanBreakNum [%u], table error?", pItem->itemID, pItem->uid, fuse->fuseLevel, conf->FuseCanBreakNum);
		return KKSG::ERR_UNKNOWN;
	}
	UINT32 basicProfession = ProfessionMgr::Instance()->GetBasicProfession(pRole->GetProfession());
	const EquipFusionConf* fusionConf = GetFusionConf(basicProfession, conf->EquipPos, conf->EquipType, fuse->fuseLevel);
	if (fusionConf == NULL)
	{
		return KKSG::ERR_CANT_FUSE;
	}
	if (fuse->fuseExpCount >= fusionConf->LevelNum * fusionConf->NeedExpPerLevel)
	{
		return KKSG::ERR_FUSE_EXPMAX;
	}
	bool isHave = false;
	for (auto i = conf->FuseCoreItems.begin(); i != conf->FuseCoreItems.end(); ++i)
	{
		if (*i == itemID)
		{
			isHave = true;
			break;
		}
	}
	if (!isHave)
	{
		LogWarn("FuseEquipAddExp, EquipID [%u], Materials itemid [%u] not in table", conf->ItemID, itemID);
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}
	const EquipFusionExpConf* expConf = m_oExpTable.GetByCoreItemId(itemID);
	if (expConf == NULL)
	{
		LogWarn("FuseEquipAddExp, EquipID [%u], itemid [%u] not in EquipFusionExp.txt", conf->ItemID, itemID);
		return KKSG::ERR_FAILED;
	}
	UINT32 num = fusionConf->NeedExpPerLevel / expConf->AddExp;
	if (fusionConf->NeedExpPerLevel % expConf->AddExp != 0)
	{
		++num;
	}
	std::vector<ItemDesc> itemlist;
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Equip, ItemFlow_Equip_FuseAddExp);
	if (!take.TakeItem(itemID, num))
	{
		take.RollBack();
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}
	for (auto i = expConf->AssistItemId.begin(); i != expConf->AssistItemId.end(); ++i)
	{
		if (!take.TakeItem((*i)[0], (*i)[1] * num))
		{
			take.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		itemlist.push_back(ItemDesc((*i)[0], (*i)[1] * num));
	}
	take.NotifyClient();

	fuse->fuseExpCount += num * expConf->AddExp;

	BagUpdateItemTransition update(pRole);
	update.AddChangedItem(pItem);
	update.NotifyClient();

	pRole->RecalcuateAttributeAndNotify(RecalAttr_FuseEquip);	

	itemlist.push_back(ItemDesc(itemID, num));
	XItem::PrintEquipTLog(pRole, pItem, TX_Item_Fuse, itemlist);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode FuseMgr::FuseEquipBreak(Role* pRole, XItem* pItem, UINT32 itemID)
{
	if (pRole == NULL || pItem == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	EquipConf* conf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (conf == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (!conf->FuseCanBreakNum)
	{
		return KKSG::ERR_CANT_FUSE;
	}
	FuseAttr* &fuse = pItem->fuse;
	if (fuse == NULL)
	{
		fuse = new FuseAttr;
	}
	UINT32 basicProfession = ProfessionMgr::Instance()->GetBasicProfession(pRole->GetProfession());
	const EquipFusionConf* fusionConf = GetFusionConf(basicProfession, conf->EquipPos, conf->EquipType, fuse->fuseLevel);
	if (fusionConf == NULL)
	{
		return KKSG::ERR_CANT_FUSE;
	}
	if (fuse->fuseExpCount < fusionConf->LevelNum * fusionConf->NeedExpPerLevel)
	{
		return KKSG::ERR_FUSE_BREAKLACKEXP;
	}
	if (fuse->fuseLevel >= conf->FuseCanBreakNum)
	{
		return KKSG::ERR_FUSE_MAX;
	}
	const EquipFusionConf* tmpFusionConf = GetFusionConf(basicProfession, conf->EquipPos, conf->EquipType, fuse->fuseLevel + 1);
	if (tmpFusionConf == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	std::vector<ItemDesc> itemlist;
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Equip, ItemFlow_Equip_FuseBreak);
	for (auto i = fusionConf->BreakNeedMaterial.begin(); i != fusionConf->BreakNeedMaterial.end(); ++i)
	{
		if (!take.TakeItem((*i)[0], (*i)[1]))
		{
			take.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		itemlist.push_back(ItemDesc((*i)[0], (*i)[1]));
	}
	take.NotifyClient();

	++fuse->fuseLevel;
	fuse->fuseExpCount -= fusionConf->LevelNum * fusionConf->NeedExpPerLevel;

	BagUpdateItemTransition update(pRole);
	update.AddChangedItem(pItem);
	update.NotifyClient();

	pRole->RecalcuateAttributeAndNotify(RecalAttr_FuseEquip);

	XItem::PrintEquipTLog(pRole, pItem, TX_Item_FuseBreak, itemlist);
	return KKSG::ERR_SUCCESS;
}

const EquipFusionConf* FuseMgr::GetFusionConf(UINT32 profession, UINT32 slot, UINT32 equipType, UINT32 breakNum)
{
	auto iter = m_mapFusionConf.find(MakeKey(profession, slot, equipType, breakNum));
	if (iter != m_mapFusionConf.end())
	{
		return iter->second;
	}
	return NULL;
}

UINT64 FuseMgr::MakeKey(UINT32 profession, UINT32 slot, UINT32 equipType, UINT32 breakNum)
{
	return (UINT64)profession << 48 | (UINT64)slot << 32 | equipType << 16 | breakNum;
}
