#include "pch.h"
#include "item.h"
#include "pb/project.pb.h"
#include "util/XRandom.h"
#include "unit/combatattribute.h"
#include "itemconfig.h"
#include "jademgr.h"
#include "foreach.h"
#include "enhanceequipmgr.h"
#include "globalconfig.h"
#include "bagtransition.h"
#include <math.h>
#include "time.h"
#include "emblemmgr.h"
#include "randomattrmgr.h"
#include "utility/tlogger.h"
#include "artifactmgr.h"
#include "effectconfig.h"
#include "util/gametime.h"
#include "artifactconfig.h"

void XItem::Item2KKSGItem(const XItem *pItem, KKSG::Item *pDBItem)
{
	if (pDBItem == NULL) 
	{
		return;
	}

	pDBItem->Clear();

	if (pItem == NULL)
	{
		pDBItem->set_uid(0);
		pDBItem->set_itemtype(0);
		pDBItem->set_itemid(0);
		pDBItem->set_itemcount(0);
		return;
	}

	///> 基础
	pDBItem->set_uid(pItem->uid);
	pDBItem->set_itemtype(pItem->type);
	pDBItem->set_itemid(pItem->itemID);
	pDBItem->set_itemcount(pItem->itemCount);
	pDBItem->set_isbind(pItem->isbind);
	pDBItem->set_cooldown(pItem->cooldown);
	pDBItem->set_expirationtime(pItem->expirationTime);

	if (EQUIP != pItem->type)
	{
		for (auto i = pItem->changeAttr.begin(); i != pItem->changeAttr.end(); i++)
		{
			pDBItem->add_attrid(i->AttrID);
			pDBItem->add_attrvalue(i->AttrValue);
		}
	}

	for (auto i = pItem->effects.begin(); i != pItem->effects.end(); i++)
	{
		*pDBItem->add_effects() = *i;
	}

	///> 额外
	if (EQUIP == pItem->type)
	{
		if (pItem->enhanceInfo != NULL)
		{
			pDBItem->set_enhancelevel(pItem->enhanceInfo->level);
			pDBItem->set_enhancecount(pItem->enhanceInfo->count);
			for (auto j = pItem->enhanceInfo->attrs.begin(); j != pItem->enhanceInfo->attrs.end(); j++)
			{
				pDBItem->add_enhanceattrid(j->AttrID);
				pDBItem->add_enhanceattrvalue(j->AttrValue);
			}
		}
		if (pItem->jadeInfo != NULL)
		{
			KKSG::ItemJade* pItemJade = pDBItem->mutable_itemjade();
			//pItemJade->set_slotinfo(pItem->jadeInfo->slotInfo);
			for (auto j = pItem->jadeInfo->vecJadeSingle.begin(); j != pItem->jadeInfo->vecJadeSingle.end(); j++)
			{
				KKSG::ItemJadeSingle* pItemJadeSingle = pItemJade->add_itemjadesingle();
				pItemJadeSingle->set_slotpos(j->slotPos);
				pItemJadeSingle->set_itemid(j->jadeid);
				for (auto k = j->attrs.begin(); k != j->attrs.end(); ++k)
				{
					pItemJadeSingle->add_attrid(k->AttrID);
					pItemJadeSingle->add_attrvalue(k->AttrValue);
				}
			}
		}
		if (pItem->randAttr != NULL)
		{
			KKSG::ItemRandAttr* itemprefix = pDBItem->mutable_randattr();
			for (auto i = pItem->randAttr->attrs.begin(); i != pItem->randAttr->attrs.end(); i++)
			{
				KKSG::AttributeInfo* attribute = itemprefix->add_attrs();
				attribute->set_id(i->AttrID);
				attribute->set_value(i->AttrValue);
			}
		}
		if (pItem->enchant != NULL)
		{
			KKSG::ItemEnchant* itemenchant = pDBItem->mutable_enchant();
			pItem->enchant->ToKKSG(itemenchant);
		}
		if (pItem->forge != NULL)
		{
			KKSG::ItemForge* itemforge = pDBItem->mutable_forge();
			for (auto i = pItem->forge->attrs.begin(); i != pItem->forge->attrs.end(); i++)
			{
				KKSG::AttributeInfo* attribute = itemforge->add_attrs();
				attribute->set_id(i->AttrID);
				attribute->set_value(i->AttrValue);
			}
			KKSG::AttributeInfo* unReplacedAttr = itemforge->mutable_unreplacedattr();
			unReplacedAttr->set_id(pItem->forge->unReplacedAttr.AttrID);
			unReplacedAttr->set_value(pItem->forge->unReplacedAttr.AttrValue);
			for (auto i = pItem->forge->haveAttrs.begin(); i != pItem->forge->haveAttrs.end(); ++i)
			{
				itemforge->add_haveattrs(*i);
			}
		}
		if (pItem->fuse != NULL)
		{
			KKSG::ItemFuse* itemfuse = pDBItem->mutable_fuse();
			itemfuse->set_fuselevel(pItem->fuse->fuseLevel);
			itemfuse->set_fuseexpcount(pItem->fuse->fuseExpCount);
		}
		pDBItem->set_smeltcount(pItem->smeltCount);
	}
	else if (EMBLEM == pItem->type)
	{
		if (pItem->emblemInfo != NULL)
		{
			pDBItem->set_emblemthirdslot(pItem->emblemInfo->thirdSlot);
			pDBItem->set_ebslottype(pItem->emblemInfo->slotType);
		}
		pDBItem->set_smeltcount(pItem->smeltCount);
	}
	else if (CIRCLE_DWAW == pItem->type)
	{
		for (UINT32 i = 0; i < pItem->circleDrawInfo.size(); i++)
		{
			KKSG::CircleDrawData* data = pDBItem->add_circledrawdatas();
			data->set_itemid(pItem->circleDrawInfo[i][0]);
			data->set_itemcount(pItem->circleDrawInfo[i][1]);
			data->set_prob(pItem->circleDrawInfo[i][2]);
			data->set_index(i);
		}
	}
}

void XItem::KKSGItem2Item(XItem *pItem, const KKSG::Item *pDBItem)
{
	pItem->Reset();

	const KKSG::Item &rDBItem = *pDBItem;
	///> 基础
	pItem->uid = rDBItem.uid();
	pItem->itemID = rDBItem.itemid();
	pItem->itemCount = rDBItem.itemcount();
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(pItem->itemID);
	if (conf == NULL) pItem->type = rDBItem.itemtype();
	else pItem->type = conf->ItemType;
	pItem->isbind = rDBItem.isbind();
	pItem->cooldown = rDBItem.cooldown();
	pItem->expirationTime = rDBItem.expirationtime();

	if (EQUIP != pItem->type)
	{
		ItemChangeAttr attr;
		for (int i = 0; i < rDBItem.attrid_size(); i++)
		{
			attr.AttrID = rDBItem.attrid(i);
			attr.AttrValue = rDBItem.attrvalue(i);
			pItem->changeAttr.push_back(attr);
		}
	}

	for (int i = 0; i < pDBItem->effects_size(); i++)
	{
		pItem->effects.push_back(pDBItem->effects(i));
	}	

	///> 额外
	if (EQUIP == pItem->type)
	{
		if (rDBItem.has_enhancelevel() && rDBItem.has_enhancecount())
		{
			EnhanceInfo* pEnhanceInfo = new EnhanceInfo;
			pEnhanceInfo->level = rDBItem.enhancelevel();
			pEnhanceInfo->count = rDBItem.enhancecount();
			ItemChangeAttr enhanceAttr;
			for (int j = 0; j < rDBItem.enhanceattrid_size(); j++)
			{
				enhanceAttr.AttrID = rDBItem.enhanceattrid(j);
				enhanceAttr.AttrValue = rDBItem.enhanceattrvalue(j);
				pEnhanceInfo->attrs.push_back(enhanceAttr);
			}
			pItem->enhanceInfo = pEnhanceInfo;
		}
		if (rDBItem.has_itemjade())
		{
			JadeInfo* pJadeInfo = new JadeInfo;
			const KKSG::ItemJade& oItemJade = rDBItem.itemjade();
			//pJadeInfo->slotInfo = oItemJade.slotinfo();
			for (int j = 0; j < oItemJade.itemjadesingle_size(); j++)
			{
				JadeSingle stJadeSingle;
				stJadeSingle.slotPos = oItemJade.itemjadesingle(j).slotpos();
				stJadeSingle.jadeid = oItemJade.itemjadesingle(j).itemid();
				for (int k = 0; k < oItemJade.itemjadesingle(j).attrid_size(); ++k)
				{
					ItemChangeAttr stAttr;
					stAttr.AttrID = oItemJade.itemjadesingle(j).attrid(k);
					stAttr.AttrValue = oItemJade.itemjadesingle(j).attrvalue(k);
					stJadeSingle.attrs.push_back(stAttr);
				}
				pJadeInfo->vecJadeSingle.push_back(stJadeSingle);
			}
			pItem->jadeInfo = pJadeInfo;
		}
		if (rDBItem.has_randattr())
		{
			RandomAttr* randattr = new RandomAttr;
			const KKSG::ItemRandAttr& temp = rDBItem.randattr();
			for (int i = 0; i < temp.attrs_size(); i++)
			{
				ItemChangeAttr attr;
				attr.AttrID = temp.attrs(i).id();
				attr.AttrValue = temp.attrs(i).value();
				randattr->attrs.push_back(attr);
			}
			pItem->randAttr = randattr;
		}
		if (rDBItem.has_enchant())
		{
			pItem->enchant = new EnchantAttr;
			pItem->enchant->ToThis(&rDBItem.enchant());
		}
		if (rDBItem.has_forge())
		{
			ForgeAttr* forge = new ForgeAttr;
			const KKSG::ItemForge& temp = rDBItem.forge();
			for (int i = 0; i < temp.attrs_size(); i++)
			{
				ItemChangeAttr attr;
				attr.AttrID = temp.attrs(i).id();
				attr.AttrValue = temp.attrs(i).value();
				forge->attrs.push_back(attr);
			}	
			if (temp.has_unreplacedattr())
			{
				forge->unReplacedAttr.AttrID = temp.unreplacedattr().id();
				forge->unReplacedAttr.AttrValue = temp.unreplacedattr().value();
			}
			for (int i = 0; i < temp.haveattrs_size(); ++i)
			{
				forge->haveAttrs.insert(temp.haveattrs(i));
			}
			pItem->forge = forge;
		}
		if (rDBItem.has_fuse())
		{
			FuseAttr* fuse = new FuseAttr;
			const KKSG::ItemFuse& temp = rDBItem.fuse();
			fuse->fuseLevel = temp.fuselevel();
			fuse->fuseExpCount = temp.fuseexpcount();
			pItem->fuse = fuse;
		}
		if (rDBItem.has_smeltcount())
		{
			pItem->smeltCount = rDBItem.smeltcount();
			RandomAttrMgr::Instance()->FixSmeltCount(pItem);
		}
		else
		{
			RandomAttrMgr::Instance()->InitSmeltCountForOldItem(pItem);
		}
	}
	else if (EMBLEM == pItem->type)
	{
		if (rDBItem.has_emblemthirdslot())
		{
			///> 认为是属性纹章
			EmblemInfo* pEmblemInfo = new EmblemInfo;
			pEmblemInfo->thirdSlot = rDBItem.emblemthirdslot();
			pEmblemInfo->slotType = rDBItem.ebslottype();
			pItem->emblemInfo = pEmblemInfo;
			if (rDBItem.has_smeltcount())
			{
				pItem->smeltCount = rDBItem.smeltcount();
				CEmblemMgr::Instance()->FixSmeltCount(pItem);
			}
			else
			{
				CEmblemMgr::Instance()->InitSmeltCountForOldItem(pItem);
			}
		}
	}
	else if (CIRCLE_DWAW == pItem->type)
	{
		for (int i = 0; i < rDBItem.circledrawdatas_size(); i++)
		{
			const KKSG::CircleDrawData& data = rDBItem.circledrawdatas(i);
			Sequence<UINT32, 3> tmp;
			tmp[0] = data.itemid();
			tmp[1] = data.itemcount();
			tmp[2] = data.prob();
			pItem->circleDrawInfo.push_back(tmp);
		}
	}
}

XItem::XItem()
{
	InitBaseData();
}

XItem::XItem(UINT64 id, UINT32 itemID_, int Count, UINT32 nType)
{
	InitBaseData();

	uid = id;
	itemID = itemID_;
	itemCount = Count;
	type = nType;

	Init();
}

XItem::XItem(const XItem &item)
{
	InitBaseData();

	operator=(item);
}

const XItem& XItem::operator=(const XItem& stItem)
{
	if (&stItem == this)
	{
		return *this;
	}

	uid = stItem.uid;
	type = stItem.type;
	itemID = stItem.itemID;
	itemCount = stItem.itemCount;
	isbind = stItem.isbind;
	cooldown = stItem.cooldown;	
	expirationTime = stItem.expirationTime;
	changeAttr = stItem.changeAttr;
	effects = stItem.effects;
	circleDrawInfo = stItem.circleDrawInfo;
	smeltCount = stItem.smeltCount;

	ClearExtendInfo();
	CopyExtendInfo(stItem);
	return *this;
}

XItem::~XItem()
{
	changeAttr.clear();
	effects.clear();
	circleDrawInfo.clear();
	ClearExtendInfo();
}

void XItem::ClearExtendInfo()
{
	if (enhanceInfo != NULL)
	{
		delete enhanceInfo;
		enhanceInfo = NULL;
	}
	if (emblemInfo != NULL)
	{
		delete emblemInfo;
		emblemInfo = NULL;
	}
	if (jadeInfo != NULL)
	{
		delete jadeInfo;
		jadeInfo = NULL;
	}
	if (randAttr != NULL)
	{
		delete randAttr;
		randAttr = NULL;
	}
	if (enchant != NULL)
	{
		delete enchant;
		enchant = NULL;
	}
	if (forge != NULL)
	{
		delete forge;
		forge = NULL;
	}
	if (fuse != NULL)
	{
		delete fuse;
		fuse = NULL;
	}
}

void XItem::CopyExtendInfo(const XItem& stItem)
{
	if (stItem.enhanceInfo)
	{
		enhanceInfo = new EnhanceInfo;
		*enhanceInfo = *stItem.enhanceInfo;
	}
	if (stItem.emblemInfo)
	{
		emblemInfo = new EmblemInfo;
		*emblemInfo = *stItem.emblemInfo;
	}
	if (stItem.jadeInfo)
	{
		jadeInfo = new JadeInfo;
		*jadeInfo = *stItem.jadeInfo;
	}
	if (stItem.randAttr)
	{
		randAttr = new RandomAttr;
		*randAttr = *stItem.randAttr;
	}
	if (stItem.enchant)
	{
		enchant = new EnchantAttr;
		*enchant = *stItem.enchant;
	}
	if (stItem.forge)
	{
		forge = new ForgeAttr;
		*forge = *stItem.forge;
	}
	if (stItem.fuse)
	{
		fuse = new FuseAttr;
		*fuse = *stItem.fuse;
	}
}

void XItem::Init()
{
	ItemConf* pConf = ItemConfig::Instance()->GetItemConf(itemID);
	if (pConf != NULL && pConf->TimeLimit != 0)
	{
		expirationTime = (UINT32)GameTime::GetTime() + pConf->TimeLimit;
	}
	if (EQUIP == type)
	{
		EquipConf *pEquipConf = ItemConfig::Instance()->GetEquipConf(itemID);
		if (pEquipConf != NULL)
		{
			// jade
			jadeInfo = new JadeInfo;
			// random attr
			RandomAttrMgr::Instance()->InitRandomAttr(this);
		}
	}
	else if (EMBLEM == type)
	{
		EmblemBasicConf* pEmblemConf = ItemConfig::Instance()->GetEmblemBasicConf(itemID);
		if (pEmblemConf != NULL)
		{
			if (pEmblemConf->EmblemType <= EmblemBoundary)//属性纹章
			{
				CEmblemMgr::Instance()->InitAttrEmblemAttr(this);
				UINT32 random = XRandom::randInt(1,101);
				emblemInfo = new EmblemInfo;
				if (random <= pEmblemConf->ThirdProb)
				{
					emblemInfo->thirdSlot = 1;
				}
				else 
				{
					emblemInfo->thirdSlot = 0;
				}
			}
		}
	}
	else if (JADE == type)
	{
		JadeConf* pJadeConf = ItemConfig::Instance()->GetJadeConf(itemID);
		if (pJadeConf != NULL)
		{
			foreach (i in pJadeConf->JadeAttributes)
			{
				ItemChangeAttr attr;
				attr.AttrID = i->seq[0];
				attr.AttrValue = i->seq[1];
				changeAttr.push_back(attr);
			}
		}
	}
	else if (CIRCLE_DWAW == type)
	{
		ItemConfig::Instance()->GetCircleDrawGive(itemID,circleDrawInfo);	
	}
	else if (ARTIFACT == type)
	{
		ArtifactMgr::Instance()->InitArtifactData(this);
	}
}

void XItem::Init(UINT32 itemID_, int Count)
{
	Reset();

	itemID = itemID_;
	itemCount = Count;

	ItemConf *pConf = ItemConfig::Instance()->GetItemConf(itemID);
	assert(pConf != NULL);
	type = pConf->ItemType;

	Init();
}

void XItem::InitBaseData()
{
	uid = 0;
	type = 0;
	itemID = 0;
	itemCount = 0;
	isbind = false;
	cooldown = 0;
	expirationTime = 0;
	smeltCount = 0;
	enhanceInfo = NULL;
	emblemInfo = NULL;
	jadeInfo = NULL;
	randAttr = NULL;
	enchant = NULL;
	forge = NULL;	
	fuse = NULL;
}

void XItem::Reset()
{
	ClearExtendInfo();

	changeAttr.clear();
	effects.clear();
	circleDrawInfo.clear();

	InitBaseData();
}

void XItem::ApplyItemEffect(CombatAttribute *combatAttr, Role* pRole)
{
	if (EQUIP == type)
	{
		EquipConf *pEquipConf = ItemConfig::Instance()->GetEquipConf(itemID);
		if (pEquipConf != NULL)
		{
			for (auto i = pEquipConf->Attributes.begin(); i != pEquipConf->Attributes.end(); i++)
			{
				combatAttr->AddAttr((CombatAttrDef)i->seq[0], i->seq[1]);
			}
		}
		if (enhanceInfo != NULL && enhanceInfo->level != 0)
		{
			ApplyEffectAux(enhanceInfo->attrs, combatAttr);
		}
		if (jadeInfo != NULL && jadeInfo->vecJadeSingle.size() != 0)
		{
			jadeInfo->ApplyEffect(combatAttr);
		}
		if (randAttr != NULL)
		{
			ApplyEffectAux(randAttr->attrs, combatAttr);
		}
		if (enchant != NULL)
		{
			enchant->ApplyEffect(combatAttr);
		}
		if (forge != NULL)
		{
			ApplyEffectAux(forge->attrs, combatAttr);
		}
		if (fuse != NULL)
		{
			fuse->ApplyEffect(combatAttr, itemID, pRole);
		}
	}
	else
	{
		ApplyEffectAux(changeAttr, combatAttr);
	}
}

void XItem::GetEquipDifferentPartAttr(std::vector<CombatAttribute>& combatAttrs) //0是基础, 1是强化, 2是龙玉, 3是锻造, 4是附魔
{
	if (EQUIP == type)
	{
		if (combatAttrs.size() < 5) 
		{
			LogWarn("The vector size is not enough");
			return;
		}
		ApplyEffectAux(changeAttr, &combatAttrs[0]);
		if (enhanceInfo != NULL && enhanceInfo->level != 0)
		{
			ApplyEffectAux(enhanceInfo->attrs, &combatAttrs[1]);
		}
		if (jadeInfo != NULL && jadeInfo->vecJadeSingle.size() != 0)
		{
			jadeInfo->ApplyEffect(&combatAttrs[2]);
		}
		if (randAttr != NULL)
		{
			ApplyEffectAux(randAttr->attrs, &combatAttrs[0]);
		}
		if (forge != NULL)
		{
			ApplyEffectAux(forge->attrs, &combatAttrs[3]);
		}
		if (enchant != NULL)
		{
			enchant->ApplyEffect(&combatAttrs[4]);
		}
	}
}

static void ChangeAttr(const ItemChangeAttr& stChangeAttr, std::vector<ItemChangeAttr> &vecAttr)
{
	auto i = std::find_if(vecAttr.begin(), vecAttr.end(), EqualAttrID(stChangeAttr.AttrID));
	if (i == vecAttr.end())
	{
		vecAttr.push_back(stChangeAttr);
	}
	else
	{
		(*i).AttrValue = stChangeAttr.AttrValue;
	}
}

static void AddAttr(const ItemChangeAttr& stChangeAttr, std::vector<ItemChangeAttr> &vecAttr)
{
	auto i = std::find_if(vecAttr.begin(), vecAttr.end(), EqualAttrID(stChangeAttr.AttrID));
	if (i == vecAttr.end())
	{
		vecAttr.push_back(stChangeAttr);
	}
	else
	{
		(*i).AttrValue += stChangeAttr.AttrValue;
	}
}

bool XItem::ChangeEnhanceAttr(std::vector<ItemChangeAttr>& attrs)
{
	if (NULL == enhanceInfo)
	{
		SSWarn<<"pEnhanceInfo is NULL, Item Unique Id:"<<uid<<END;
		return false;	
	}

	enhanceInfo->attrs = attrs;
	return true;
}

bool XItem::ChangeItemAttr(const ItemChangeAttr& stChangeAttr)
{
	ChangeAttr(stChangeAttr, changeAttr);
	return true;
}

UINT32 XItem::GetType(UINT32 dwItemId)
{
	ItemConf* pItemConf = ItemConfig::Instance()->GetItemConf(dwItemId);
	if (NULL == pItemConf)
	{
		return (UINT32)(-1);
	}
	return pItemConf->ItemType;
}

UINT32 XItem::GetQuality(UINT32 dwItemId)
{
	ItemConf* pItemConf = ItemConfig::Instance()->GetItemConf(dwItemId);
	if (NULL == pItemConf)
	{
		return (UINT32)(-1);
	}
	return pItemConf->ItemQuality;
}

UINT32 XItem::GetLevel(UINT32 dwItemId)
{
	ItemConf* itemconf = ItemConfig::Instance()->GetItemConf(dwItemId);
	if (NULL == itemconf)
	{
		return (UINT32)(-1);
	}
	return itemconf->ReqLevel;
}

UINT32 XItem::GetPos(UINT32 dwItemId)
{
	EquipConf* itemconf = ItemConfig::Instance()->GetEquipConf(dwItemId);
	if (NULL == itemconf)
	{
		return (UINT32)(-1);
	}
	return itemconf->EquipPos;
}

bool XItem::ChangeJade(const JadeSingle& stJadeSingle, bool bIsDetach)
{
	if (NULL == jadeInfo)
	{
		SSWarn<<"pJadeInfo is NULL, Item Unique Id:"<<uid<<END;
		return false;
	}
	auto i = std::find_if(jadeInfo->vecJadeSingle.begin(), jadeInfo->vecJadeSingle.end(), EqualJadeSlot(stJadeSingle.slotPos));
	
	if (bIsDetach)
	{
		if (i == jadeInfo->vecJadeSingle.end())
		{
			return false;
		}
		jadeInfo->vecJadeSingle.erase(i);
		return true;
	}

	if (i == jadeInfo->vecJadeSingle.end())
	{
		jadeInfo->vecJadeSingle.push_back(stJadeSingle);
	}
	else
	{
		(*i).Reset();	
		(*i) = stJadeSingle;
	}
	return true;
}

bool XItem::DecomposeItem(UINT32 count, std::map<UINT32, UINT32>& items, std::map<UINT32, UINT32>& jades)
{
	if (count > itemCount) count = itemCount;
	switch (type)
	{
	case EQUIP:
		{
			EquipConf* config = ItemConfig::Instance()->GetEquipConf(itemID);
			if (NULL == config)
			{
				SSWarn<<"decompose item, item id:"<<itemID<<" not find"<<END;
				break;
			}
			// basic 
			ItemConfig::Instance()->GetDecomposedDropEquip(config, items);
			// jade
			CJadeMgr::Instance()->GetAllJade(this, jades);
			// smelt
			for (auto i = config->SmeltNeedItem.begin(); i != config->SmeltNeedItem.end(); ++i)
			{
				items[(*i)[0]] += (*i)[1] * smeltCount * config->ReturnSmeltStoneRate / 100;
			}
			break;
		}
	case EMBLEM:
		{
			EmblemBasicConf* conf = ItemConfig::Instance()->GetEmblemBasicConf(itemID);
			if (NULL == conf)
			{
				break;
			}
			for (auto i = conf->SmeltNeedItem.begin(); i != conf->SmeltNeedItem.end(); ++i)
			{
				items[(*i)[0]] += (*i)[1] * smeltCount * conf->ReturnSmeltStoneRate / 100;
			}
			break;
		}
	case SUPPLEMENT:
		{
			SupplementConf* config = ItemConfig::Instance()->GetSupplementConf(itemID);
			if (NULL != config)
			{
				items[config->decompose[0]] += config->decompose[1]*count;
			}
			break;
		}
	default:
		break;
	}
	ItemConfig::Instance()->GetDecomposedItem(itemID, count, items);
	return true;
}

float XItem::GetDecomposeParam()
{
	int random = XRandom::randInt(1, 101);
	if (random <= GetGlobalConfig().DecomposeRandom)
	{
		return GetGlobalConfig().DecomposeParam;
	}
	return 1.0f;
}

bool XItem::IsCoolDown()
{
	UINT32 nowtime = (UINT32)time(NULL);
	return nowtime >= cooldown;
}

bool XItem::IsExpire(UINT32 nowTime)
{
	return expirationTime != 0 && expirationTime < nowTime;
}

bool XItem::CheckProfession(UINT32 itemID, int profession)
{
	int basicUsedProfession = ItemConfig::Instance()->GetProfession(itemID);
	if (basicUsedProfession != 0)
	{
		return basicUsedProfession == ProfessionMgr::Instance()->GetBasicProfession(profession);
	}
	return true;
}

bool XItem::IsJewelry(int pos)
{
	if (pos == Necklace || pos == Earrings || pos == Ring1)
	{
		return true;
	}

	return false;
}

bool XItem::IsWeapon(int pos)
{
	if (pos >= HeadGear || pos <= SecondaryWeapon)
	{
		return true;
	}

	return false;
}

bool XItem::IsTreasure()
{
	if(type == EQUIP && randAttr && randAttr->attrs.size() >= 3)
	{
		return true;
	}
	if(type == EMBLEM && emblemInfo && emblemInfo->thirdSlot >= 1)
	{
		return true;
	}
	return false;
}

void XItem::PrintEquipTLog(Role* pRole, XItem* pItem, int oper, std::vector<ItemDesc>& vlist, UINT32 id, UINT32 preid)
{
	if (pRole == NULL || pItem == NULL || pItem->type != EQUIP)
	{
		return;
	}
	TItemSysFlow oLog(pRole);
	oLog.m_ItemID = pItem->itemID;
	if (GetLevel(pItem->itemID) != (UINT32)-1)
	{
		oLog.m_ItemLv = GetLevel(pItem->itemID);
	}
	oLog.m_Op = oper;
	if (GetQuality(pItem->itemID) != (UINT32)-1)
	{
		oLog.m_Quality = GetQuality(pItem->itemID);
	}
	oLog.m_ItemUid = pItem->uid;
	if (oper == TX_Item_Enchant)
	{
		oLog.m_EnchantID = id;
	}
	else if (oper == TX_Item_JadeEquip || oper == TX_Item_JadeOff)
	{
		oLog.m_JadeID = id;
	}
	else if (oper == TX_Item_JadeUp)
	{
		oLog.m_JadeID = id;
		oLog.m_PreJadeID = preid;
	}
	oLog.m_Attr.SetItem(pItem);
	oLog.m_ItemList.m_vList = vlist;
	FuseAttr* pFuse = pItem->fuse;
	if (pFuse)
	{
		oLog.m_FuseExp = pFuse->fuseExpCount;
		oLog.m_FuseLevel = pFuse->fuseLevel;
	}
	oLog.Do();
}

void XItem::PrintEmblemTLog(Role* pRole, XItem* pItem, int oper, INT32 slot, std::vector<ItemDesc>& vlist, const std::string& strTrans)
{
	if (pRole == NULL || pItem == NULL || pItem->type != EMBLEM)
	{
		return;
	}
	TEmblemFlow oLog(pRole);
	if (!strTrans.empty())
	{
		oLog.SetTransTag(strTrans);
	}
	oLog.m_EmblemID = pItem->itemID;
	if (GetLevel(pItem->itemID) != (UINT32)-1)
	{
		oLog.m_EmblemLv = GetLevel(pItem->itemID);
	}
	oLog.m_Op = oper;
	if (GetQuality(pItem->itemID) != (UINT32)-1)
	{
		oLog.m_QualityID = GetQuality(pItem->itemID);
	}
	if (pItem->emblemInfo == NULL) oLog.m_ThirdSlot = 0;
	else oLog.m_ThirdSlot = pItem->emblemInfo->thirdSlot;
	oLog.m_EmblemUid = pItem->uid;
	UINT32 count = pItem->changeAttr.size();
	for (UINT32 i = 0; i < count && i < TXLOG_EMBLEMATTRNUM; i ++)
	{
		TEmbleAttr oAttr;
		oAttr.m_AttrID = pItem->changeAttr[i].AttrID;
		oAttr.m_AttrValue = pItem->changeAttr[i].AttrValue;
		const AttrEmblemSingle* pSingle = CEmblemMgr::Instance()->GetAttrEmblemSingle(pItem->itemID, i, oAttr.m_AttrID);
		if (pSingle)
		{
			oAttr.m_AttrFirst = pSingle->attrRange[0];
			oAttr.m_AttrSecond = pSingle->attrRange[1];
		}
		if (slot == i)
		{
			oLog.m_SmeltAttrID = pItem->changeAttr[i].AttrID;
		}
		oLog.m_vAttrList.push_back(oAttr);
	}
	for (UINT32 i = count; i < TXLOG_EMBLEMATTRNUM; i ++)
	{
		TEmbleAttr oAttr;
		oLog.m_vAttrList.push_back(oAttr);
	}
	oLog.m_AttrNum = count;
	
	oLog.m_ItemList.m_vList = vlist;
	oLog.Do();
}

void XItem::PrintArtifactTLog(Role* pRole, XItem* pItem, int oper, const std::string& strTrans, bool isFlag, UINT32 param)
{
	if (NULL == pRole || NULL == pItem || pItem->type != ARTIFACT)
	{
		return;
	}
	TArtifactFlow oLog(pRole);
	if (!strTrans.empty())
	{
		oLog.SetTransTag(strTrans);
	}
	oLog.m_ArtifactID = pItem->itemID;
	oLog.m_ArtifactUid = pItem->uid;
	oLog.m_Op = oper;

	if (GetQuality(pItem->itemID) != (UINT32)-1)
	{
		oLog.m_QualityID = GetQuality(pItem->itemID);
	}
	for (UINT32 i = 0; i < pItem->effects.size(); i ++)
	{
		oLog.m_EffectIDList.push_back(pItem->effects[i].effectid());
	}

	for (UINT32 i = 0; i < pItem->changeAttr.size(); i ++)
	{
		TBaseAttr attr;
		attr.m_AttrID = pItem->changeAttr[i].AttrID;
		attr.m_AttrValue = pItem->changeAttr[i].AttrValue;
		if (attr.m_AttrID)
		{
			ArtifactConfig::Instance()->GetArtifactAttrRange(pItem->itemID, i, attr.m_AttrID, attr.m_AttrFirst, attr.m_AttrSecond);
		}
		oLog.m_AttrList.push_back(attr);
	}
	if (oper == Tx_Artifact_Fuse)
	{
		oLog.m_FuseResult = isFlag?0:1;
		oLog.m_FuseUsedStone = param;
	}
	else if (oper == Tx_Artifact_Inscription)
	{
		oLog.m_InscriptionOp = isFlag?0:1;
		oLog.m_InscriptionSkillEffectID = param;
	}
	oLog.Do();
}

void XItem::DetachSameJade(std::vector<JadeSingle>& jadesingles)
{
	if (NULL == jadeInfo)
	{
		return;
	}
	std::vector<std::pair<UINT32, UINT32>> tempinfos;
	for (auto i = jadeInfo->vecJadeSingle.begin(); i != jadeInfo->vecJadeSingle.end(); ++i)
	{
		bool isdetach = false;
		UINT32 type = 0;
		UINT32 attrid = 0;
		JadeConf* pJadeConf = ItemConfig::Instance()->GetJadeConf(i->jadeid);
		if (NULL == pJadeConf)
		{
			SSWarn<<"pJadeConf is NULL, item id:"<<i->jadeid<<END;
			continue;
		}
		type = pJadeConf->JadeEquip;
		if (i->attrs.empty())
		{
			continue;
		}
		attrid = i->attrs[0].AttrID;

		for (auto j = tempinfos.begin(); j != tempinfos.end(); ++j)
		{
			if (type == j->first && attrid == j->second)	
			{
				isdetach = true;
			}
		}
		tempinfos.push_back(std::make_pair(type, attrid));

		if (isdetach)
		{
			jadesingles.push_back(*i);
		}
	}
}

KKSG::EmblemSlotType XItem::GetEmSlotType()
{
	if (emblemInfo == NULL)
	{
		return KKSG::EmblemSlotType_None;
	}
	return emblemInfo->slotType;
}

void XItem::SetEmSlotType(KKSG::EmblemSlotType slotType)
{
	if (emblemInfo == NULL)
	{
		return ;
	}
	emblemInfo->slotType = slotType;
}
