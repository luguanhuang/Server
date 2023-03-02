#include "pch.h"
#include "itemsuffix.h"
#include "pb/project.pb.h"
#include "itemconfig.h"
#include "fusemgr.h"
#include "unit/role.h"

ItemChangeAttr::ItemChangeAttr()
:AttrID(0)
,AttrValue(0)
{

}

ItemChangeAttr::ItemChangeAttr(UINT32 nAttrId, UINT32 nAttrValue)
:AttrID(nAttrId)
,AttrValue(nAttrValue)
{
}


void JadeSingle::Reset()
{
	attrs.clear();
}

JadeSingle* JadeInfo::GetJadeSingle(UINT32 dwSlotPos)
{
	auto i = find_if(vecJadeSingle.begin(), vecJadeSingle.end(), EqualJadeSlot(dwSlotPos));
	if (i == vecJadeSingle.end())
	{
		return NULL;
	}
	return &(*i);
}

void EnchantAttr::ToKKSG(KKSG::ItemEnchant* enchant)
{
	enchant->Clear();

	enchant->set_chooseattrid(chooseAttrid);
	for (auto i = mapAllAttrs.begin(); i != mapAllAttrs.end(); ++i)
	{
		enchant->add_enchantids(i->second.enchantid);
		KKSG::AttributeInfo* attribute = enchant->add_allattrs();
		attribute->set_id(i->first);
		attribute->set_value(i->second.attrValue);
	}

	for (auto i = mapBaodiCount.begin(); i != mapBaodiCount.end(); ++i)
	{
		enchant->add_enchantbaodi(i->first);
		enchant->add_baodicount(i->second);
	}
}

void EnchantAttr::ToThis(const KKSG::ItemEnchant* enchant)
{
	if (enchant->has_chooseattrid()) chooseAttrid = enchant->chooseattrid();
	else if (enchant->attrs_size())
	{
		chooseAttrid = enchant->attrs(0).id();
	}
	for (int i = 0; i < enchant->enchantids_size(); ++i)
	{
		if (i >= enchant->allattrs_size())
		{
			break;
		}
		mapAllAttrs[enchant->allattrs(i).id()] = EnchantEachData(enchant->allattrs(i).value(), enchant->enchantids(i));
	}

	if (enchant->attrs_size())
	{
		Update(enchant->attrs(0).id(), enchant->attrs(0).value(), enchant->enchantid());
	}

	for (int i = 0; i < enchant->enchantbaodi_size(); ++i)
	{
		if (i >= enchant->baodicount_size())
		{
			break;
		}
		mapBaodiCount[enchant->enchantbaodi(i)] = enchant->baodicount(i);
	}
}

void EnchantAttr::Update(UINT32 attrid, UINT32 attrvalue, UINT32 enchantid)
{
	if (attrid == 0)
	{
		return;
	}
	if (chooseAttrid == 0)
	{
		chooseAttrid = attrid;
	}
	auto i = mapAllAttrs.find(attrid);
	if (i == mapAllAttrs.end())
	{
		mapAllAttrs[attrid] = EnchantEachData(attrvalue, enchantid);
		return;
	}
	
	if (i->second.attrValue <= attrvalue)
	{
		i->second.attrValue = attrvalue;
		i->second.enchantid = enchantid;
	}
}

UINT32 EnchantAttr::GetBaodiCount(UINT32 enchantid)
{
	auto iter = mapBaodiCount.find(enchantid);
	if (iter == mapBaodiCount.end())
	{
		return 0;
	}
	return iter->second;
}

void EnchantAttr::AddBaodiCount(UINT32 enchantid)
{
	++mapBaodiCount[enchantid];
}

void EnchantAttr::ClearBaodiCount(UINT32 enchantid)
{
	mapBaodiCount.erase(enchantid);
}

void EnchantAttr::ApplyEffect(CombatAttribute* combatAttr)
{
	auto i = mapAllAttrs.find(chooseAttrid);
	if (i != mapAllAttrs.end())
	{
		combatAttr->AddAttr((CombatAttrDef)i->first, i->second.attrValue);
	}
}

KKSG::ErrorCode EnchantAttr::ChangeAttribute(UINT32 attrid)
{
	if (mapAllAttrs.find(attrid) == mapAllAttrs.end())
	{
		return KKSG::ERR_ENCHANT_NOTHAVEATTRID;
	}
	chooseAttrid = attrid;
	return KKSG::ERR_SUCCESS;
}

void FuseAttr::ApplyEffect(CombatAttribute* combatAttr, UINT32 itemID, Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	EquipConf* conf = ItemConfig::Instance()->GetEquipConf(itemID);
	if (conf == NULL)
	{
		return;
	}
	UINT32 basicProf = ProfessionMgr::Instance()->GetBasicProfession(pRole->GetProfession());
	for (UINT32 i = 0; i <= fuseLevel; ++i)
	{
		const EquipFusionConf* fuseConf = FuseMgr::Instance()->GetFusionConf(basicProf, conf->EquipPos, conf->EquipType, i);
		if (fuseConf == NULL)
		{
			LogWarn("EquipFusionConf is NULL, equipid[%u], profession [%u], slot [%u], equipType [%u], fuseLevel [%u]", itemID, basicProf, conf->EquipPos, conf->EquipType, i);
			continue;
		}
		UINT32 num = fuseConf->LevelNum;
		if (i == fuseLevel)
		{
			num = std::min(fuseConf->LevelNum, fuseExpCount / fuseConf->NeedExpPerLevel);
		}
		for (auto j = fuseConf->LevelAddAttr.begin(); j != fuseConf->LevelAddAttr.end(); ++j)
		{
			combatAttr->AddAttr((CombatAttrDef)(*j)[0], 1.0 * (*j)[1] * num);
		}
		if (i != fuseLevel)
		{
			for (auto j = fuseConf->BreakAddAttr.begin(); j != fuseConf->BreakAddAttr.end(); ++j)
			{
				combatAttr->AddAttr((CombatAttrDef)(*j)[0], (*j)[1]);
			}
		}
	}
}