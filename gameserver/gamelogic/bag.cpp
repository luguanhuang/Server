#include "pch.h"
#include <time.h>
#include "bag.h"
#include "pb/project.pb.h"
#include "item.h"
#include "itemconfig.h"
#include "unit/role.h"
#include "bagtransition.h"
#include "rolenethandler.h"
#include "globalconfig.h"
#include "jademgr.h"
#include "foreach.h"
#include "payv2Record.h"
#include "payv2Mgr.h"
#include "utility/uidhelper.h"
#include "item/ptcg2c_changesupplementntf.h"
#include "designationMgr.h"
#include "gamelogic/enhanceequipmgr.h"
#include "enhance/ptcg2c_notifyenhancesuit.h"
#include "common/ptcg2c_itemchangedntf.h"
#include "event/eventmgr.h"
#include "enchantmgr.h"
#include "utility/tlogger.h"
#include "gamelogic/randomattrmgr.h"
#include "effectconfig.h"
#include "artifactconfig.h"
#include "effectdef.h"
#include "artifactmgr.h"
#include "mail/mailsender.h"
#include "util/gametime.h"
#include "tablemgr/backflowtablemgr.h"
#include "table/PlayerLevelManager.h"
#include "mail/mailconfig.h"

#define EPS 1e-6

#define CallBack(Fun, ...)  { if (listener) listener->Fun( __VA_ARGS__); }

Bag::Bag(Role* pRole)
{
	m_enhanceMaster = 0;
	m_extraSkillEmblemSlot = 0;
	m_pRole = pRole;
	memset(m_Equip, 0, sizeof(m_Equip));
	memset(m_Emblem, 0, sizeof(m_Emblem));
	memset(m_WearedSkillEmblem, 0, sizeof(m_WearedSkillEmblem));
	memset(m_Artifact, 0, sizeof(m_Artifact));
	memset(m_VirtualItem, 0, sizeof(m_VirtualItem));
	m_GMFuseCompensation = true;
	m_GMForgeCompensation = true;
}

static XItem *FindItemInArray(XItem **array_, int sz, UINT64 uid, int *index)
{
	for (int i = 0; i < sz; ++i)
	{
		if (array_[i] == NULL)
		{
			continue;
		}
		if (uid == array_[i]->uid)
		{
			if (index != NULL)
			{
				*index = i;
			}
			return array_[i];
		}
	}

	return NULL;
}

static void ClearXItemArray(XItem **array_, int Size)
{
	for (int i = 0; i < Size; ++i)
	{
		if (array_[i] != NULL)
		{
			delete array_[i];
			array_[i] = NULL;
		}
	}
}

Bag::~Bag()
{
	for (auto i = m_mapItems.begin(); i != m_mapItems.end(); ++i)
	{
		XItem *pItem = i->second;
		delete pItem;
		pItem = NULL;
	}

	for (int i = 0; i < SkillEmblemCount; ++i)
	{
		if (m_WearedSkillEmblem[i] != NULL)
		{
			delete m_WearedSkillEmblem[i];
		}
	}

	ClearXItemArray(m_Equip, EquipCount);
	ClearXItemArray(m_Emblem, EmblemCount);
	ClearXItemArray(m_Artifact, ArtifactCount);
}

void Bag::FirstInit(UINT32 roleNum)
{
	AddAnyItem(ItemDesc(FATIGUE, GetGlobalConfig().MaxRecoverFatigue, false), NULL);
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		_BackFlowFirstInitEquip();
		_BackFlowFirstInitEmblem();	
	}
	m_isModify.Set();
}

bool Bag::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	XItem *pItem = NULL;
	const KKSG::BagContent &roBag = poRoleAllInfo->bag();

	m_enhanceMaster = roBag.enhancesuit();
	m_extraSkillEmblemSlot = roBag.extraskillebslotnum();

	for (int i = 0; i < roBag.equips_size() && i < EquipCount; ++i)
	{
		if (0 != roBag.equips(i).itemid() && 0 != roBag.equips(i).itemcount())
		{
			m_Equip[i] = new XItem();
			XItem::KKSGItem2Item(m_Equip[i], &roBag.equips(i));
		}
	}

	int attriEbIndex = 0;
	int skillEbIndex = AttributeEmblemCount;
	int extraSkillEbIndex = ExtraSkillEmblemStartIndex;
	int wearSkillIndex = 0;
	for (int i = 0; i < roBag.emblems_size(); ++i)
	{
		const KKSG::Item& item = roBag.emblems(i);
		if (item.itemid() == 0 || item.itemcount() == 0)
		{
			continue;
		}
		XItem* pItem = new XItem;
		XItem::KKSGItem2Item(pItem, &item);
		KKSG::EmblemSlotType slotType = pItem->GetEmSlotType();
		if (slotType == KKSG::EmblemSlotType_None)
		{
			// 重新算slottype
			slotType = CEmblemMgr::Instance()->GetSlotTypeByIndex(i);
			pItem->SetEmSlotType(slotType);
		}
		if (slotType == KKSG::EmblemSlotType_None)
		{
			continue;
		}
		if (slotType == KKSG::EmblemSlotType_Attri)
		{
			if (attriEbIndex >= AttributeEmblemCount)
			{
				continue;
			}
			m_Emblem[attriEbIndex++] = pItem;
		}
		else if (slotType == KKSG::EmblemSlotType_Skill)
		{
			if (skillEbIndex >= ExtraSkillEmblemStartIndex)
			{
				continue;
			}
			m_Emblem[skillEbIndex] = pItem;
			const SkEmblem* pSkEmblem = CEmblemMgr::Instance()->GetSkillEmblemByID(pItem->itemID);
			if (pSkEmblem != NULL)
			{
				m_WearedSkillEmblem[skillEbIndex - AttributeEmblemCount] = new SkEmblem(*pSkEmblem);
			}
			skillEbIndex++;
		}
		else if (slotType == KKSG::EmblemSlotType_ExtraSkill)
		{
			if (extraSkillEbIndex >= EmblemCount)
			{
				continue;
			}
			m_Emblem[extraSkillEbIndex] = pItem;
			const SkEmblem* pSkEmblem = CEmblemMgr::Instance()->GetSkillEmblemByID(pItem->itemID);
			if (pSkEmblem != NULL)
			{
				m_WearedSkillEmblem[extraSkillEbIndex - AttributeEmblemCount] = new SkEmblem(*pSkEmblem);
			}
			extraSkillEbIndex++;
		}
	}

	for (int i = 0; i < roBag.artifacts_size() && i < ArtifactCount; ++i)
	{
		if (0 != roBag.artifacts(i).itemid() && 0 != roBag.artifacts(i).itemcount())
		{
			m_Artifact[i] = new XItem();
			XItem::KKSGItem2Item(m_Artifact[i], &roBag.artifacts(i));
		}
	}

	for (int i = 0; i < roBag.expand_size(); ++i)
	{
		const KKSG::BagExpandData& expand = roBag.expand(i);
		m_bagExpand[expand.type()] = std::make_pair(expand.num(), expand.count());
	}

	for (int i = 0; i < roBag.items_size(); ++i)
	{
		pItem = new XItem();
		XItem::KKSGItem2Item(pItem, &roBag.items(i));

		EffectConfig::Instance()->UpdateEffectData(pItem->effects);

		if (FindUniqueItemOnBody(pItem->uid) != NULL)
		{	
			LogWarn("role [%llu], item same uid [%llu] on body and in bag", m_pRole->GetID(), pItem->uid);
			pItem->uid = UIDHelper::CreateUID(UID_Item);
		}
		_InsertToBag(pItem, true);
	}

	int N = (VIRTUAL_ITEM_MAX > roBag.virtualitems_size()) ? roBag.virtualitems_size() : VIRTUAL_ITEM_MAX;
	for (int j = 0; j < N; ++j)
	{
		m_VirtualItem[j] = roBag.virtualitems(j);
	}
	
	if (!roBag.has_fusecompensation() || roBag.fusecompensation() == false)
	{
		_GiveFuseCompensation();
	}

	if (!roBag.has_forgecompensation() || roBag.forgecompensation() == false)
	{
		_GiveForgeCompensation();
	}

	ArrangeEffectData();

	m_isModify.Set();	

	return true;
}

enum
{
	SAVE_EQUIP,
	SAVE_EMBLEM,
	SAVE_ARTIFACT,
	SAVE_ITEM,
};

struct SaveItemToRoleAllInfo
{
	int m_type;
	KKSG::BagContent *m_poBag;

	SaveItemToRoleAllInfo(KKSG::BagContent *poBag) : m_poBag(poBag) 
	{
		m_type = SAVE_EQUIP;
	}

	void operator()(const XItem *item)
	{
		KKSG::Item *pDBItem = NULL;
		switch (m_type)
		{
		case SAVE_EQUIP: pDBItem = m_poBag->add_equips(); break;
		case SAVE_EMBLEM: pDBItem = m_poBag->add_emblems(); break;
		case SAVE_ARTIFACT: pDBItem = m_poBag->add_artifacts(); break;
		case SAVE_ITEM: pDBItem = m_poBag->add_items(); break;
		}
		
		XItem::Item2KKSGItem(item, pDBItem);
	}
};

void Bag::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	assert(m_Equips.size() + m_Emblems.size() + m_Atlas.size() + m_Artifacts.size() + m_Inscription.size() + m_Bag.size() == m_mapItems.size());
	if (m_isModify.TestAndReset())
	{
		SaveItemToRoleAllInfo saver(poRoleAllInfo->mutable_bag());
		saver.m_poBag->Clear();

		saver.m_poBag->set_enhancesuit(m_enhanceMaster);
		saver.m_poBag->set_extraskillebslotnum(m_extraSkillEmblemSlot);

		saver.m_type = SAVE_EQUIP;
		std::for_each(m_Equip, m_Equip + EquipCount, saver);

		saver.m_type = SAVE_EMBLEM;
		std::for_each(m_Emblem, m_Emblem + EmblemCount, saver);

		saver.m_type = SAVE_ARTIFACT;
		std::for_each(m_Artifact, m_Artifact + ArtifactCount, saver);

		saver.m_type = SAVE_ITEM;
		for (auto i = m_mapItems.begin(); i != m_mapItems.end(); ++i)
		{
			saver(i->second);
		}

		if (m_VirtualItem[EXP] != 0)
		{
			LogWarn("exp is not 0");
			m_VirtualItem[EXP] = 0;
		}
		for (int i = 0; i < VIRTUAL_ITEM_MAX; ++i)
		{
			saver.m_poBag->add_virtualitems(m_VirtualItem[i]);
		}

		for (auto i = m_bagExpand.begin(); i != m_bagExpand.end(); ++i)
		{
			KKSG::BagExpandData* pData = saver.m_poBag->add_expand();
			pData->set_type(i->first);
			pData->set_num(i->second.first);
			pData->set_count(i->second.second);
		}
		saver.m_poBag->set_fusecompensation(m_GMFuseCompensation);
		saver.m_poBag->set_forgecompensation(m_GMForgeCompensation);

		roChanged.insert(saver.m_poBag);
	}
}

void Bag::UpdatePerMinute()
{
	UINT32 nowTime = (UINT32)GameTime::GetTime();
	std::vector<std::pair<UINT64, UINT32>> deleteItem;
	for (auto i = m_mapItems.begin(); i != m_mapItems.end(); ++i)
	{
		if (i->second->IsExpire(nowTime))
		{
			deleteItem.push_back(std::make_pair(i->second->uid, i->second->itemCount));
		}
	}

	if (!deleteItem.empty())
	{
		BagTakeItemTransition take(m_pRole);
		take.SetReason(ItemFlow_ItemExpirationTime, ItemFlow_ItemExpirationTime_Take);
		for (auto i = deleteItem.begin(); i != deleteItem.end(); ++i)
		{
			take.TakeUniqueItem(i->first, i->second);
		}
		take.NotifyClient();
	}
}

UINT32 Bag::AddAnyItem(const ItemDesc& desc, BagListener* listener)
{
	if (desc.itemID == 0 || desc.itemCount <= 0)
	{
		SSWarn<<"item id:"<<desc.itemID<<" count:"<<desc.itemCount<<END;
		return 0;
	}

	if (XItem::IsVirtualItem(desc.itemID))
	{
		IncVirtualItem(desc.itemID, desc.itemCount, listener);
		return desc.itemCount;
	}
	return _AddItem(desc, listener);
}

///> 创建一个新的XItem，把pItem的属性copy给新XItem，并把新XItem加入到包裹
UINT32 Bag::AddAnyItem(const XItem *pItem, BagListener* listener)
{
	if (pItem->itemID == 0 || pItem->itemCount <= 0)
	{
		LogWarn("addItem count error: %d,%d", pItem->itemID, pItem->itemCount);
		return 0;
	}

	if (XItem::IsVirtualItem(pItem->itemID))
	{
		IncVirtualItem(pItem->itemID, pItem->itemCount, listener);
		return pItem->itemCount;
	}
	///> 不能堆叠的需要直接拷贝
	if (1 == ItemConfig::Instance()->GetOverlap(pItem->itemID))
	{
		return _AddNewItem(pItem, listener);
	}
	return _AddItem(ItemDesc(pItem->itemID, pItem->itemCount, pItem->isbind, pItem->cooldown), listener);
}

UINT32 Bag::_AddItem(const ItemDesc& desc, BagListener* listener)
{
	std::list<XItem*>* itemList = _GetItemList(desc.itemID);
	if (itemList == NULL)
	{
		return 0;
	}
	UINT32 overlap = ItemConfig::Instance()->GetOverlap(desc.itemID);
	std::vector<XItem*> items;
	UINT32 nowTime = (UINT32)time(NULL);
	for (auto i = itemList->begin(); i != itemList->end(); ++i)
	{
		if ((*i)->cooldown <= nowTime)
		{
			(*i)->cooldown = 0;
		}
		if ((desc.itemID == (*i)->itemID) && (overlap != (*i)->itemCount) && desc.FilterIsSame((*i)->isbind, (*i)->cooldown))
		{
			items.push_back(*i);
		}
	}
	Bag::SortCount(items);
	
	int allcount = (int)desc.itemCount;
	if (allcount < 0)
	{
		LogError("add too many item, it's very dangerous, roleid:%llu, itemid:%u, count:%u", m_pRole->GetID(), desc.itemID, desc.itemCount);
		return 0;
	}

	if (0 == overlap)
	{
		XItem* item = items.empty() ? NULL : items[0];
		///> 无限堆叠
		if (NULL == item)
		{
			return _AddNewItem(desc, listener);
		}
		ChangeItemCount(item, allcount, desc.cooldown, listener);
		return (UINT32)allcount;
	}

	UINT32 ret = 0;
	for (UINT32 i = 0; i < items.size(); ++i)
	{
		int left = overlap - items[i]->itemCount;
		if (left > 0)
		{
			int tempcount = left > allcount ? allcount : left;	
			ChangeItemCount(items[i], tempcount, desc.cooldown, listener);
			allcount -= tempcount;
			ret += tempcount;
		}
		if (allcount <= 0)
		{
			break;
		}
	}
	
	if (allcount > 0)
	{
		int left = allcount % overlap;
		int part = (int)(allcount/overlap);
		for (int i = 0; i < part; ++i)
		{
			ret += _AddNewItem(ItemDesc(desc.itemID, overlap, desc.isbind, desc.cooldown), listener);
		}
		if (left > 0)
		{
			ret += _AddNewItem(ItemDesc(desc.itemID, left, desc.isbind, desc.cooldown), listener);
		}
	}
	return ret;
}

UINT32 Bag::_AddNewItem(const ItemDesc& desc, BagListener* listener)
{
	ItemConf *pConf = ItemConfig::Instance()->GetItemConf(desc.itemID);
	if (pConf == NULL)
	{
		LogWarn("can't create item %u, not configed", desc.itemID);
		return 0;
	}

	std::list<XItem*>* itemList = _GetItemList(desc.itemID);
	if (itemList == NULL)
	{
		return 0;
	}

	UINT32 maxNum = _GetItemListMaxCapacity(desc.itemID);
	if (itemList->size() >= maxNum)
	{
		if (pConf->TimeLimit != 0)
		{
			UINT64 uid = UIDHelper::CreateUID(UID_Item);
			XItem item(uid, desc.itemID, (int)desc.itemCount, pConf->ItemType);
			CallBack(OnBagFullAndSendMail, &item);
		}
		else
		{
			CallBack(OnBagFullAndSendMail, desc);
		}
		return 0;
	}

	UINT64 uid = UIDHelper::CreateUID(UID_Item);
	XItem *pItem = new XItem(uid, desc.itemID, (int)desc.itemCount, pConf->ItemType);
	pItem->SetBind(desc.isbind);
	pItem->SetCoolDown(desc.cooldown);

	if (!_InsertToBag(itemList, pItem, true))
	{
		return 0;
	}

	CallBack(OnAddItem, pItem);

	m_pRole->OnGetItem(pItem);

	m_isModify.Set();

	return desc.itemCount;
}

UINT32 Bag::_AddNewItem(const XItem* pItem, BagListener* listener)
{
	std::list<XItem*>* itemList = _GetItemList(pItem->itemID);
	if (itemList == NULL)
	{
		return 0;
	}

	UINT32 maxNum = _GetItemListMaxCapacity(pItem->itemID);
	if (itemList->size() >= maxNum)
	{
		CallBack(OnBagFullAndSendMail, pItem);
		return 0;
	}

	XItem *pNewItem = new XItem(*pItem);

	if (!_InsertToBag(itemList, pNewItem, true))
	{
		return 0;
	}

	CallBack(OnAddItem, pNewItem);

	m_pRole->OnGetItem(pNewItem);

	m_isModify.Set();

	return pItem->itemCount;
}

bool Bag::_InsertToBag(XItem* pItem, bool isDelete)
{
	std::list<XItem*>* itemList = _GetItemList(pItem->itemID);
	if (itemList == NULL)
	{
		LogWarn("role:%llu, insert to bag failed, itemlist is NULL, uid:%llu, itemid:%u", m_pRole->GetID(), pItem->uid, pItem->itemID);
		if (isDelete)
		{
			delete pItem;
		}
		return false;
	}

	UINT32 maxNum = _GetItemListMaxCapacity(pItem->itemID);
	if (itemList->size() >= maxNum)
	{
		LogError("role:%llu, itemList size:%u >= maxNum:%u, type:%u, itemid:%u, uid:%u", m_pRole->GetID(), itemList->size(), maxNum, pItem->type, pItem->itemID, pItem->uid);
		//return;
	}
	if (pItem->itemCount == 0)
	{
		LogError("role:%llu, itemCount is zero, uid:%llu, itemid:%u", m_pRole->GetID(), pItem->uid, pItem->itemID);
		if (isDelete)
		{
			delete pItem;
		}
		return false;
	}
	return _InsertToBag(itemList, pItem, isDelete);
}

bool Bag::_InsertToBag(std::list<XItem*>* itemList, XItem* pItem, bool isDelete)
{
	if (m_mapItems.find(pItem->uid) != m_mapItems.end()
		|| FindUniqueItemOnBody(pItem->uid) != NULL)
	{
		LogError("role:%llu, insert to bag failed, uid:%llu is already exist, itemid:%u", m_pRole->GetID(), pItem->uid, pItem->itemID);
		if (isDelete)
		{
			delete pItem;
		}
		return false;
	}
	itemList->push_back(pItem);
	m_mapItems.insert(std::make_pair(pItem->uid, pItem));

	return true;
}

std::list<XItem*>* Bag::_GetItemList(UINT32 itemID)
{
	ItemConf *pConf = ItemConfig::Instance()->GetItemConf(itemID);
	if (pConf == NULL)
	{
		LogWarn("role:%llu, can't get itemlist, itemid:%u not configed", m_pRole->GetID(), itemID);
		return NULL;
	}
	if (VIRTUAL_ITEM == pConf->ItemType || XItem::IsVirtualItem(itemID))
	{
		LogWarn("role:%llu, can't get virtual itemlist, itemid:%u", m_pRole->GetID(), itemID);
		return NULL;
	}

	return &_GetItemListByType(pConf->ItemType);
}

std::list<XItem*>& Bag::_GetItemListByType(UINT32 type)
{
	switch (type)
	{
	case EQUIP:
		{
			return m_Equips;
		}
	case EMBLEM:
		{
			return m_Emblems;
		}
	case ATLAS:
		{
			return m_Atlas;
		}
	case ARTIFACT:
		{
			return m_Artifacts;
		}
	case INSCRIPTION:
		{
			return m_Inscription;
		}
	default:
		{
			return m_Bag;
		}
	}
}

void Bag::_DeleteItem(std::list<XItem*>* itemList, std::list<XItem*>::iterator& iter)
{
	if (itemList == NULL)
	{
		return;
	}
	m_mapItems.erase((*iter)->uid);
	delete *iter;
	*iter = NULL;
	itemList->erase(iter);
}

void Bag::_BackFlowFirstInitEquip()
{
	const std::vector<BackFlowEquipConf*>* equipConf = BackFlowTableMgr::Instance()->GetEquipConf(GSConfig::Instance()->GetBackFlowLevel(), m_pRole->GetProfession());
	if (equipConf == NULL)
	{
		return;
	}
	for (auto i = equipConf->begin(); i != equipConf->end(); ++i)
	{
		BackFlowEquipConf* conf = *i;
		ItemConf* itemConf = ItemConfig::Instance()->GetItemConf(conf->EquipID);
		if (itemConf == NULL || itemConf->ItemType != EQUIP)
		{
			LogWarn("BackFlowEquip.txt, EquipID [%u] not in itemlist or type not Equip", conf->EquipID);
			continue;
		}

		UINT64 uid = UIDHelper::CreateUID(UID_Item);
		XItem tmpItem(uid, conf->EquipID, 1, EQUIP);
		BagGiveItemTransition give1(m_pRole);
		give1.SetReason(ItemFlow_BackFlowInit, ItemFlow_BackFlowInit_Equip);
		give1.GiveItem(&tmpItem);
		give1.NotifyClient();

		XItem* pItem = FindUniqueItemInBag(uid);
		if (pItem == NULL)
		{
			LogWarn("BackFlowFirstInitEquip, Already add equip [%u], but can't get Item in bag", conf->EquipID);
			continue;
		}
		KKSG::ErrorCode code = WearEquip(pItem, true, NULL);
		if (code != KKSG::ERR_SUCCESS)
		{
			LogWarn("BackFlowFirstInitEquip, Wear Equip [%u] failed, ErrorCode [%d]", conf->EquipID, code);
			continue;
		}
		double percent = 1.0 * conf->SmeltRate / 100.0;
		if (percent > 1.0) percent = 1.0;
		if (pItem->randAttr != NULL)
		{
			for (UINT32 i = 0; i < pItem->randAttr->attrs.size(); ++i)
			{
				ItemChangeAttr& attr = pItem->randAttr->attrs[i];
				const RandAttrSingle* single = RandomAttrMgr::Instance()->GetRandomAttrSingle(pItem->itemID, i, attr.AttrID);
				if (single == NULL)
				{
					continue;
				}
				attr.AttrValue = single->attrRange[0] + (UINT32)(percent * (single->attrRange[1] - single->attrRange[0]) + EPS);
			}
		}
		const RandAttrSingle* single = RandomAttrMgr::Instance()->GetForgeAttrSingle(pItem->itemID, 0, conf->ForgeAttrID);
		if (single != NULL)
		{
			if (pItem->forge == NULL)
			{
				pItem->forge = new ForgeAttr;
			}
			pItem->forge->attrs.clear();
			pItem->forge->unReplacedAttr.AttrID = pItem->forge->unReplacedAttr.AttrValue = 0;
			pItem->forge->haveAttrs.clear();
			ItemChangeAttr attr;
			attr.AttrID = conf->ForgeAttrID;
			attr.AttrValue = single->attrRange[0] + (UINT32)(percent * (single->attrRange[1] - single->attrRange[0]) + EPS);
			pItem->forge->attrs.push_back(attr);
		}
		std::pair<UINT32, UINT32> range = EnchantMgr::Instance()->GetEnchantAttrValueRange(conf->EnchantAttr[0], conf->EnchantAttr[1]);
		if (range.first != 0 && range.second != 0)
		{
			if (pItem->enchant == NULL)
			{
				pItem->enchant = new EnchantAttr;
			}
			if (range.first > range.second) std::swap(range.first, range.second);
			UINT32 attrValue = range.first + (UINT32)(1.0 * conf->EnchantAttr[2] * (range.second - range.first) / 100.0);
			pItem->enchant->Update(conf->EnchantAttr[1], attrValue, conf->EnchantAttr[0]);
		}
		BagGiveItemTransition give2(m_pRole);
		give2.SetReason(ItemFlow_BackFlowInit, ItemFlow_BackFlowInit_Jade);
		for (auto j = conf->Jade.begin(); j != conf->Jade.end(); ++j)
		{
			give2.GiveItem(*j, 1);
		}
		give2.NotifyClient();
		
		for (UINT32 j = 0; j < conf->Jade.size(); ++j)
		{
			std::vector<XItem*> items;
			FindItemsInBag(conf->Jade[j], items);
			if (!items.empty())
			{
				CJadeMgr::Instance()->AttachJade(m_pRole, pItem, j, *items.begin());
			}
		}
		UINT32 enhanceLevel = PlayerLevelManager::Instance()->GetEnhanceMaxLvl(m_pRole->GetLevel());
		if (conf->EnhanceLevel <= enhanceLevel)
		{
			enhanceLevel = conf->EnhanceLevel;
		}
		if (pItem->enhanceInfo == NULL)
		{
			pItem->enhanceInfo = new EnhanceInfo;
		}
		pItem->enhanceInfo->count = 0;
		pItem->enhanceInfo->level = enhanceLevel;
		///> 修改装备强化属性
		EnhanceEquipMgr::Instance()->ChangeAttr(m_pRole, enhanceLevel, pItem);
		///> 强化套装检查
		EnhanceEquipMgr::Instance()->CheckEnhanceMaster(m_pRole, enhanceLevel);
	}
	m_isModify.Set();
}

void Bag::_BackFlowFirstInitEmblem()
{
	const BackFlowEmblemConf* conf = BackFlowTableMgr::Instance()->GetEmblemConf(GSConfig::Instance()->GetBackFlowLevel(), m_pRole->GetProfession());
	if (conf == NULL)
	{
		return;
	}
	for (auto i = conf->EmblemIDs.begin(); i != conf->EmblemIDs.end(); ++i)
	{
		ItemConf* itemConf = ItemConfig::Instance()->GetItemConf(*i);
		if (itemConf == NULL || itemConf->ItemType != EMBLEM)
		{
			LogWarn("BackFlowFirstInitEmblem, EmblemID [%u] not in itemlist or type not Emblem", *i);
			continue;
		}
		UINT64 uid = UIDHelper::CreateUID(UID_Item);
		XItem tmpItem(uid, *i, 1, EMBLEM);
		if (tmpItem.emblemInfo == NULL)
		{
			tmpItem.emblemInfo = new EmblemInfo;
		}
		tmpItem.emblemInfo->thirdSlot = 0;

		BagGiveItemTransition give(m_pRole);
		give.SetReason(ItemFlow_BackFlowInit, ItemFlow_BackFlowInit_Emblem);
		give.GiveItem(&tmpItem);
		give.NotifyClient();

		XItem* pItem = FindUniqueItemInBag(uid);
		if (pItem == NULL)
		{
			LogWarn("BackFlowFirstInitEmblem, Already add emblem [%u], but can't get Item in bag", *i);
			continue;
		}
		KKSG::ErrorCode code = WearEmblem(pItem, NULL);
		if (code != KKSG::ERR_SUCCESS)
		{
			LogWarn("BackFlowFirstInitEmblem, Wear Emblem [%u] failed, ErrorCode [%d]", *i, code);
			continue;
		}
		double percent = 1.0 * conf->SmeltRate / 100.0;
		for (auto i = 0; i < pItem->changeAttr.size(); ++i)
		{
			ItemChangeAttr& attr = pItem->changeAttr[i];
			const AttrEmblemSingle* single = CEmblemMgr::Instance()->GetAttrEmblemSingle(pItem->itemID, i, attr.AttrID);
			if (single == NULL)
			{
				continue;
			}
			attr.AttrValue = single->attrRange[0] + (UINT32)(percent * (single->attrRange[1] - single->attrRange[0]) + EPS);
		}
	}
	m_isModify.Set();
}

UINT32 Bag::_GetItemListMaxCapacity(UINT32 itemID)
{
	ItemConf *pConf = ItemConfig::Instance()->GetItemConf(itemID);
	if (pConf == NULL)
	{
		LogWarn("role:%llu, can't get itemlist, itemid:%u not configed", m_pRole->GetID(), itemID);
		return 0;
	}
	if (VIRTUAL_ITEM == pConf->ItemType || XItem::IsVirtualItem(itemID))
	{
		LogWarn("role:%llu, can't get virtual itemlist, itemid:%u", m_pRole->GetID(), itemID);
		return 0;
	}

	return _GetItemListMaxCapacityByType(pConf->ItemType);	
}

UINT32 Bag::_GetItemListMaxCapacityByType(UINT32 type)
{
	VIPTable::RowData* data = PayV2Mgr::Instance()->GetVipConf(m_pRole->Get<CPayV2Record>()->GetVipLevel());
	if (data == NULL)
	{
		LogWarn("VIPTable::RowData is NULL, viplevel:%d", m_pRole->Get<CPayV2Record>()->GetVipLevel());
		return 0;
	}
	switch (type)
	{
	case EQUIP:
		{
			return data->EquipMax + m_bagExpand[KKSG::EquipBag].first;
		}
	case EMBLEM:
		{
			return data->EmblemMax + m_bagExpand[KKSG::EmblemBag].first;
		}
	case ATLAS:
		{
			return 1000;
		}
	case ARTIFACT:
		{
			return data->ArtifactMax + m_bagExpand[KKSG::ArtifactBag].first;
		}
	case INSCRIPTION:
		{
			return data->InscriptionMax;
		}
	default:
		{
			return data->BagMax + m_bagExpand[KKSG::ItemBag].first;
		}
	}
}

void Bag::RearrangeBag()
{
	PtcG2C_ItemChangedNtf oItemChangePtc;
	oItemChangePtc.m_Data.set_isrearrange(true);

	std::unordered_map<UINT32, std::vector<XItem*>> mapItems[2];
	UINT32 nowTime = (UINT32)time(NULL);
	for (auto i = m_Bag.begin(); i != m_Bag.end(); ++i)
	{
		if ((*i)->cooldown <= nowTime)
		{
			(*i)->cooldown = 0;
			UINT32 overlap = ItemConfig::Instance()->GetOverlap((*i)->itemID);
			if (0 == overlap) overlap = (UINT32)(-1);
			if (overlap > (*i)->itemCount) //not overlap full
			{
				mapItems[(*i)->IsBind()][(*i)->itemID].push_back(*i);
			}
		}
	}
	std::unordered_set<UINT64> deleteItems;
	for (UINT32 i = 0; i < 2; ++i)
	{
		for (auto j = mapItems[i].begin(); j != mapItems[i].end(); ++j)
		{
			std::vector<XItem*>& vecItem = j->second;
			if (1 == vecItem.size())
			{
				continue;
			}
			UINT32 overlap = ItemConfig::Instance()->GetOverlap(j->first);
			if (0 == overlap) overlap = (UINT32)(-1);	

			Bag::SortCount(vecItem);
			UINT32 allcount = 0;
			for (UINT32 k = 0; k < vecItem.size(); ++k)
			{
				allcount += vecItem[k]->itemCount;
			}
			if ((UINT64)allcount > 1LL * vecItem.size() * overlap)
			{
				///> 出现这种情况就是此物品叠加超过当前的overlap,修改表导致，无法处理
				LogWarn("roleid:%llu, itemid:%u, some item overlap bigger than overlap in table now, can't rearrange", m_pRole->GetID(), j->first);
				continue;
			}
			for (int k = vecItem.size() - 1; k >= 0; --k)
			{
				if (0 == allcount)
				{
					oItemChangePtc.m_Data.add_removeitems(vecItem[k]->uid);
					deleteItems.insert(vecItem[k]->uid);
					continue;
				}
				if (allcount >= overlap)
				{
					vecItem[k]->itemCount = overlap;
					allcount -= overlap;
				}
				else
				{
					vecItem[k]->itemCount = allcount;
					allcount = 0;
				}
				oItemChangePtc.m_Data.add_changeitems(vecItem[k]->uid);
				oItemChangePtc.m_Data.add_changeitems(vecItem[k]->itemCount);	
			}
		}
	}
	//delete item
	for (auto i = m_Bag.begin(); i != m_Bag.end(); )
	{
		if (deleteItems.find((*i)->uid) != deleteItems.end())
		{
			auto cp = i;
			++i;
			_DeleteItem(&m_Bag, cp);
			continue;
		}
		++i;
	}
	m_pRole->Send(oItemChangePtc);

	m_isModify.Set();
}

void Bag::AddItemToEquipPosition(UINT32 itemID, int pos)
{
	if (pos < 0 || pos >= EquipCount)
	{
		LogWarn("AddItemToEquipPosition pos range error! %d / %d", pos, EquipCount);
		return;
	}

	ItemConf *pConf = ItemConfig::Instance()->GetItemConf(itemID);
	if (pConf == NULL)
	{
		LogWarn("can't create item %d, not configed", itemID);
		return ;
	}

	UINT64 uid = UIDHelper::CreateUID(UID_Item);

	XItem *pNewItem = new XItem(uid, itemID, 1, pConf->ItemType);
	pNewItem->uid = uid;
	m_Equip[pos] = pNewItem;
}

XItem* Bag::FindUniqueItem(UINT64 uid)
{
	XItem* pItem = FindUniqueItemOnBody(uid);
	if (pItem != NULL)
	{
		return pItem;
	}
	return FindUniqueItemInBag(uid);
}

XItem* Bag::FindUniqueItemOnBody(UINT64 uid, int* index)
{
	XItem* pItem = FindItemInArray(m_Equip, EquipCount, uid, index);
	if (pItem != NULL) return pItem;

	pItem = FindItemInArray(m_Emblem, EmblemCount, uid, index);
	if (pItem != NULL) return pItem;

	pItem = FindItemInArray(m_Artifact, ArtifactCount, uid, index);
	return pItem;
}

XItem* Bag::FindUniqueEquipOnBody(UINT64 uid, int* index)
{
	return FindItemInArray(m_Equip, EquipCount, uid, index);	
}

XItem* Bag::FindUniqueEmblemOnBody(UINT64 uid, int* index)
{
	return FindItemInArray(m_Emblem, EmblemCount, uid, index);
}

XItem* Bag::FindUniqueArtifactOnBody(UINT64 uid, int* index)
{
	return FindItemInArray(m_Artifact, ArtifactCount, uid, index);
}

XItem* Bag::GetItemOnBody(ItemType type, int slot)
{
	switch (type)
	{
	case EQUIP:
		{
			if (slot >= 0 && slot < EquipCount)
			{
				return m_Equip[slot];
			}
		}
	case EMBLEM:
		{
			if (slot >= 0 && slot < EmblemCount)
			{
				return m_Emblem[slot];
			}
		}
	case ARTIFACT:
		{
			if (slot >= 0 && slot < ArtifactCount)
			{
				return m_Artifact[slot];
			}
		}
	default:
		{
			break;
		}
	}
	return NULL;	
}

void Bag::FindItemsAll(UINT32 itemID, std::vector<XItem*>& items, ItemBind filter)
{
	if (filter == Bind_ALL || filter == Bind_Is)
	{
		FindItemsOnBody(itemID, items);
	}
	FindItemsInBag(itemID, items, filter);
}

XItem* Bag::FindUniqueItemInBag(UINT64 uid)
{
	auto iter = m_mapItems.find(uid);
	if (iter == m_mapItems.end())
	{
		return NULL;
	}
	return iter->second;
}

void Bag::FindItemsOnBody(UINT32 itemID, std::vector<XItem*>& items)
{
	for (int i = 0; i < EquipCount; ++i)
	{
		if (m_Equip[i] != NULL)
		{
			if (m_Equip[i]->itemID == itemID)
			{
				items.push_back(m_Equip[i]);
			}
		}
	}

	for (int i = 0; i < EmblemCount; ++i)
	{
		if (m_Emblem[i] != NULL)
		{
			if (m_Emblem[i]->itemID == itemID)
			{
				items.push_back(m_Emblem[i]);
			}
		}
	}

	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (m_Artifact[i] != NULL)
		{
			if (m_Artifact[i]->itemID == itemID)
			{
				items.push_back(m_Artifact[i]);
			}
		}
	}
}

void Bag::FindItemsInBag(UINT32 itemID, std::vector<XItem*>& items, ItemBind filter)
{
	std::list<XItem*>* itemList = _GetItemList(itemID);
	if (NULL == itemList)
	{
		return;
	}

	for (auto i = itemList->begin(); i != itemList->end(); ++i)
	{
		if (itemID == (*i)->itemID)
		{
			if (Bind_ALL == filter || (int)((*i)->isbind) == filter)
			{
				items.push_back(*i);
			}
		}
	}
}

UINT32 Bag::CountItemTotal(UINT32 itemID)
{
	if (XItem::IsVirtualItem(itemID))
	{
		return CountVirtualItem(itemID);
	}
	return CountItemOnBody(itemID) + CountItemInBag(itemID);
}

UINT32 Bag::CountItemInBag(UINT32 itemID)
{
	std::list<XItem*>* itemList = _GetItemList(itemID);
	if (NULL == itemList)
	{
		return 0;
	}

	UINT32 count = 0;
	for (auto i = itemList->begin(); i != itemList->end(); ++i)
	{
		if (itemID == (*i)->itemID)
		{
			count += (*i)->itemCount;
		}
	}

	return count;
}

UINT32 Bag::CountItemOnBody(UINT32 itemID)
{
	UINT32 count = 0;
	for (int i = 0; i < EquipCount; ++i)
	{
		if (m_Equip[i] != NULL)
		{
			if (m_Equip[i]->itemID == itemID)
			{
				count += m_Equip[i]->itemCount;
			}
		}
	}

	for (int i = 0; i < EmblemCount; ++i)
	{
		if (m_Emblem[i] != NULL)
		{
			if (m_Emblem[i]->itemID == itemID)
			{
				count += m_Emblem[i]->itemCount;
			}
		}
	}

	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (m_Artifact[i] != NULL)
		{
			if (m_Artifact[i]->itemID == itemID)
			{
				count += m_Artifact[i]->itemCount;
			}
		}
	}

	return count;
}

UINT32 Bag::CountEmblemOnBody()
{
	UINT32 count = 0;
	for (int i = 0; i < EmblemCount; ++i)
	{
		if (m_Emblem[i] != NULL)
		{
			count++;
		}
	}
	return count;
}

void Bag::SortBind(std::vector<XItem*>& items)
{
	std::sort(items.begin(), items.end(), XItem::CompareBind);
}

void Bag::SortCount(std::vector<XItem*>& items)
{
	std::sort(items.begin(), items.end(), XItem::CompareCount);
}

bool Bag::CanAddThisItem(UINT32 itemID)
{
	std::list<XItem*>* itemList = _GetItemList(itemID);
	if (itemList == NULL)
	{
		return false;
	}

	UINT32 maxNum = _GetItemListMaxCapacity(itemID);
	return itemList->size() < maxNum;	
}

bool Bag::CanAddThisTypeItem(UINT32 type)
{
	std::list<XItem*>& itemList = _GetItemListByType(type);

	UINT32 maxNum = _GetItemListMaxCapacityByType(type);
	return itemList.size() < maxNum;
}

struct ItemListLeft
{
	ItemListLeft()
	{
		equipLeft = 0;
		emblemLeft = 0;
		artifactLeft = 0;
		bagLeft = 0;
		inscriptionLeft = 0;
	}
	int equipLeft;
	int emblemLeft;
	int artifactLeft;
	int bagLeft;
	int inscriptionLeft;
};

static bool CheckCanAdd(UINT32 itemID, UINT32 itemCount, ItemListLeft& left)
{
	ItemConf* pConf = ItemConfig::Instance()->GetItemConf(itemID);
	if (pConf == NULL)
	{
		return true;
	}
	UINT32 overlap = pConf->Overlap;
	int num = 0;
	if (0 == overlap)
	{
		num = 1;
	}
	else
	{
		int count = (int)itemCount;
		if (count < 0)
		{
			LogError("add too many item, itemid:%u, count:%llu", itemID, itemCount);
			return false;
		}
		num = count / overlap;
		if (count % overlap != 0) num++;
	}
	switch (pConf->ItemType)
	{
	case VIRTUAL_ITEM:	///> 虚拟物品肯定能加的进
	case ATLAS:			///> 默认加的进
	case FASHION:
	case SPRITE:
		{
			return true;	
		}
	case EQUIP_PROTO:
	case EQUIP:
		{
			left.equipLeft -= num;
			return left.equipLeft >= 0;
		}
	case EMBLEM:
		{
			left.emblemLeft -= num;
			return left.emblemLeft >= 0;
		}
	case ARTIFACT:
		{
			left.artifactLeft -= num;
			return left.artifactLeft >= 0;
		}
	case INSCRIPTION:
		{
			left.inscriptionLeft -= num;
			return left.inscriptionLeft >= 0;
		}
	default:
		{
			left.bagLeft -= num;
			return left.bagLeft >= 0;
		}
	}
}

bool Bag::CanAddItem(const std::vector<ItemDesc>* itemDesc, const std::vector<XItem>* item)
{
	ItemListLeft left;
	UINT32 maxEquip = _GetItemListMaxCapacityByType(EQUIP);
	if (m_Equips.size() < maxEquip)
	{
		left.equipLeft = (int)(maxEquip - m_Equips.size());
	}
	UINT32 maxEmblem = _GetItemListMaxCapacityByType(EMBLEM);
	if (m_Emblems.size() < maxEmblem)
	{
		left.emblemLeft = (int)(maxEmblem - m_Emblems.size());
	}
	UINT32 maxArtifact = _GetItemListMaxCapacityByType(ARTIFACT);
	if (m_Artifacts.size() < maxArtifact)
	{
		left.artifactLeft = (int)(maxArtifact - m_Artifacts.size());
	}
	UINT32 maxInscription = _GetItemListMaxCapacityByType(INSCRIPTION);
	if (m_Inscription.size() < maxInscription)
	{
		left.inscriptionLeft = (int)(maxInscription - m_Inscription.size());
	}
	UINT32 maxBag = _GetItemListMaxCapacityByType(JADE);
	if (m_Bag.size() < maxBag)
	{
		left.bagLeft = (int)(maxBag - m_Bag.size());
	}

	LogDebug("equipleft:%d, emblemleft:%d, artifactleft:%d, inscriptionleft:%d, bagleft:%d", left.equipLeft, left.emblemLeft, left.artifactLeft, left.inscriptionLeft, left.bagLeft);

	if (itemDesc != NULL)
	{
		for (UINT32 i = 0; i < itemDesc->size(); ++i)
		{
			if (!CheckCanAdd((*itemDesc)[i].itemID, (*itemDesc)[i].itemCount, left))
			{
				return false;
			}
		}
	}

	if (item != NULL)
	{
		for (UINT32 i = 0; i < item->size(); ++i)
		{
			if (!CheckCanAdd((*item)[i].itemID, (*item)[i].itemCount, left))
			{
				return false;
			}
		}
	}
	return true;
}

void Bag::ChangeItemCount(XItem *pItem, int change, UINT32 cooldown, BagListener* listener)
{
	m_isModify.Set();

	if (cooldown && change > 0)
	{
		pItem->cooldown = (UINT32)(((UINT64)pItem->cooldown * pItem->itemCount + (UINT64)cooldown * change) / ((UINT64)pItem->itemCount + change));
		pItem->itemCount += change;
		CallBack(OnChangeItemAttr, pItem);
		return;
	}

	pItem->itemCount += change;
	CallBack(OnChangeItemCount, pItem);
}

void Bag::RemoveItemUnique(UINT64 uid, BagListener* listener)
{
	auto iter = m_mapItems.find(uid);
	if (iter == m_mapItems.end())
	{
		return;
	}
	XItem* pItem = iter->second;

	std::list<XItem*>* itemList = _GetItemList(pItem->itemID);
	if (itemList == NULL)
	{
		return;
	}
	auto listIter = std::find_if(itemList->begin(), itemList->end(), EqualItemUID(uid));
	if (listIter == itemList->end())
	{
		return;
	}

	CallBack(OnRemoveItem, *listIter);
	
	_DeleteItem(itemList, listIter);
	
	m_isModify.Set();
}

KKSG::ErrorCode Bag::WearEquip(XItem* pItem, int isSwap, BagListener* listener)
{
	if (FindUniqueItemInBag(pItem->uid) == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	auto i = std::find_if(m_Equips.begin(), m_Equips.end(), EqualItemUID(pItem->uid));
	if (i == m_Equips.end())
	{
		LogError("Unknown error, maybe this item [%llu] is not equip", pItem->uid);
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	ItemConf* pItemConf = ItemConfig::Instance()->GetItemConf(pItem->itemID);
	if (pItemConf == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	if (m_pRole->GetLevel() < pItemConf->ReqLevel)
	{
		return KKSG::ERR_ITEM_LEVELLIMIT;
	}
	if (pItem->type != EQUIP)
	{
		LogWarn("Wear Item type error: itemID = %d, itemType = %d", pItemConf->ItemID, pItemConf->ItemType);
		return KKSG::ERR_UNKNOWN; //外层应该判断
	}
	EquipConf* pEquipConf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (pEquipConf == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	int slot = pEquipConf->EquipPos;

	if (slot < 0 || slot >= EquipCount)
	{
		LogWarn("Equip slot [%d] error, itemid [%u]", slot, pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}

	XItem* tmpItem = NULL;
	if (m_Equip[slot] != NULL)
	{
		ItemConf* pItemConf2 = ItemConfig::Instance()->GetItemConf(m_Equip[slot]->itemID);
		if (pItemConf2 == NULL)
		{
			LogWarn("equip on body can't find in itemlist, itemid:%u", m_Equip[slot]->itemID);
			return KKSG::ERR_UNKNOWN;
		}
		EquipConf* pEquipConf2 = ItemConfig::Instance()->GetEquipConf(m_Equip[slot]->itemID);
		if (pEquipConf2 == NULL)
		{
			LogWarn("equip on body can't find in equiplist, itemid:%u", m_Equip[slot]->itemID);
			return KKSG::ERR_UNKNOWN;
		}

		if (m_Equip[slot]->jadeInfo != NULL && m_Equip[slot]->jadeInfo->vecJadeSingle.size() > 0
			&& (pItem->jadeInfo == NULL || pItem->jadeInfo->vecJadeSingle.size() == 0)) //身上装备有龙玉 && 新装备没有龙玉
			CJadeMgr::Instance()->SwapJadeOnEquip(m_pRole, m_Equip[slot], pItem, false);
		EnchantMgr::Instance()->DoTransfer(m_pRole, m_Equip[slot], pItem, false); //转移附魔

		UINT32 destLvl = pItem->enhanceInfo == NULL ? 0 : pItem->enhanceInfo->level;
		UINT32 originLvl = m_Equip[slot]->enhanceInfo == NULL ? 0 : m_Equip[slot]->enhanceInfo->level;
		if (destLvl < originLvl) //新装备强化等级小于身上装备
		{
			EnhanceEquipMgr::Instance()->DoTransfer(m_pRole, m_Equip[slot], pItem);
		}
		UINT32 destFuseLvl = 0, destFuseExp = 0;
		if (pItem->fuse != NULL)
		{
			destFuseLvl = pItem->fuse->fuseLevel;
			destFuseExp = pItem->fuse->fuseExpCount;
		}
		UINT32 originFuseLvl = 0, originFuseExp = 0;
		if (m_Equip[slot]->fuse != NULL)
		{
			originFuseLvl = m_Equip[slot]->fuse->fuseLevel;
			originFuseExp = m_Equip[slot]->fuse->fuseExpCount;
		}
		if (pEquipConf->EquipType == pEquipConf2->EquipType && pEquipConf->FuseCanBreakNum >= pEquipConf2->FuseCanBreakNum
			&& destFuseLvl <= pEquipConf2->FuseCanBreakNum && originFuseLvl <= pEquipConf->FuseCanBreakNum
			&& (originFuseLvl > destFuseLvl || (originFuseLvl == destFuseLvl && originFuseExp > destFuseExp)))
		{
			std::swap(pItem->fuse, m_Equip[slot]->fuse);
		}

		BagUpdateItemTransition oUpdateTransition(m_pRole);
		oUpdateTransition.AddChangedItem(m_Equip[slot]);
		oUpdateTransition.NotifyClient();

		tmpItem = m_Equip[slot];
		*i = m_Equip[slot];
		m_Equip[slot] = pItem;

		m_mapItems.erase(pItem->uid);
		m_mapItems.insert(std::make_pair((*i)->uid, *i));
	}
	else
	{
		m_Equip[slot] = pItem;
		m_Equips.erase(i);
		m_mapItems.erase(pItem->uid);
	}

	pItem->SetBind(true); //必须在notifyclient之前设置，因为swapitem会发整个物品给客户端

	CallBack(OnSwapItem, pItem, tmpItem, slot);

	m_pRole->OnWearEquip(slot);

	// 强化等级套装加成  统计
	UINT32 currentlevel = pItem->enhanceInfo == NULL ? 0 : pItem->enhanceInfo->level;
	if (currentlevel > m_enhanceMaster)
	{
		EnhanceEquipMgr::Instance()->CheckEnhanceMaster(m_pRole, currentlevel);
	}

	m_isModify.Set();

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Bag::WearEmblem(XItem* pItem, BagListener* listener)
{
	if (FindUniqueItemInBag(pItem->uid) == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}

	auto it = std::find_if(m_Emblems.begin(), m_Emblems.end(), EqualItemUID(pItem->uid));
	if (it == m_Emblems.end())
	{
		LogError("it's impossible, unknown error");
		assert(false);
		return KKSG::ERR_ITEM_NOTEXIST;
	}

	EmblemBasicConf* pEmblemConf = ItemConfig::Instance()->GetEmblemBasicConf(pItem->itemID);
	if (pEmblemConf == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	if (pEmblemConf->EmblemType > EmblemBoundary && CEmblemMgr::Instance()->GetSkillEmblemByID(pItem->itemID) == NULL)
	{
		LogWarn("skillEmblem:%d not in skillEmblem table", pItem->itemID);
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	if (m_pRole->GetLevel() < (int)pEmblemConf->Level) //看等级
	{
		return KKSG::ERR_ITEM_LEVELLIMIT;
	}
	if (pEmblemConf->ProfID != 0) //看职业
	{
		if (m_pRole->GetProfession() != pEmblemConf->ProfID)
		{
			if (m_pRole->GetProfession() % 100 != pEmblemConf->ProfID && m_pRole->GetProfession() % 10 != pEmblemConf->ProfID)
			{
				return KKSG::ERR_ITEM_WRONG_PROFESSION;
			}
		}
	}
	UINT32 emtype = pEmblemConf->EmblemType;
	if (emtype <= EmblemBoundary)
	{
		EmblemInfo* pEmblemInfo = pItem->emblemInfo;
		if (pEmblemInfo == NULL)
		{
			return KKSG::ERR_UNKNOWN;
		}
		if (1 == pEmblemInfo->thirdSlot) //未鉴定属性纹章
		{
			return KKSG::ERR_EMBLEM_NOIDENTIFY;
		}
	}

	XItem* pSwapEmblem = NULL;
	UINT32 slot = UINT32(-1);
	KKSG::EmblemSlotType slotType;
	// 属性纹章
	if (emtype <= EmblemBoundary)
	{
		slotType = KKSG::EmblemSlotType_Attri;
		UINT32 slotNum = CEmblemMgr::Instance()->GetAttributeEmblemSlotLevelLimit(m_pRole->GetLevel());
		UINT32 emptySlot = UINT32(-1);
		for (UINT32 i = 0; i < slotNum && i < AttributeEmblemCount; ++i)
		{
			XItem* emblem = m_Emblem[i];
			if (emblem == NULL)
			{
				// 记录第一个空闲孔
				if (emptySlot == UINT32(-1))
				{
					emptySlot = i;
				}
				continue;
			}
			if (emtype == ItemConfig::Instance()->GetEmblemBasicConf(emblem->itemID)->EmblemType)
			{
				slot = i;
				pSwapEmblem = emblem;
				break;
			}
		}
		// 没有可以替换的就放到空闲的孔上
		if (pSwapEmblem == NULL)
		{
			slot = emptySlot;
		}
	}
	// 技能纹章
	else
	{
		slotType = KKSG::EmblemSlotType_Skill;
		UINT32 slotNum = CEmblemMgr::Instance()->GetSkillEmblemSlotLevelLimit(m_pRole->GetLevel());
		UINT32 emptySlot = UINT32(-1);
		for (UINT32 i = AttributeEmblemCount; i < AttributeEmblemCount + slotNum && i < ExtraSkillEmblemStartIndex; ++i)
		{
			XItem* emblem = m_Emblem[i];
			if (emblem == NULL)
			{
				// 记录第一个空闲孔
				if (emptySlot == UINT32(-1))
				{
					emptySlot = i;
				}
				continue;
			}
			if (CEmblemMgr::Instance()->IsActOnSameSkill(emblem->itemID, pItem->itemID))
			{
				slot = i;
				pSwapEmblem = emblem;
				break;
			}
		}
		// 等级孔没找到孔的话, 继续找额外的孔
		if (slot == UINT32(-1))
		{
			slotType = KKSG::EmblemSlotType_ExtraSkill;
			for (UINT32 i = ExtraSkillEmblemStartIndex; i < ExtraSkillEmblemStartIndex + m_extraSkillEmblemSlot && i < EmblemCount; ++i)
			{
				XItem* emblem = m_Emblem[i];
				if (emblem == NULL)
				{
					if (emptySlot == UINT32(-1))
					{
						emptySlot = i;
					}
					continue;
				}
				if (CEmblemMgr::Instance()->IsActOnSameSkill(emblem->itemID, pItem->itemID))
				{
					slot = i;
					pSwapEmblem = emblem;
					break;
				}
			}
			if (pSwapEmblem == NULL)
			{
				slot = emptySlot;
			}
		}
	}
	if (slot == UINT32(-1))
	{
		return KKSG::ERR_EMBLEM_NOEMPTYSLOT;
	}
	slotType = CEmblemMgr::Instance()->GetSlotTypeByIndex(slot);
	//to do tlog
	std::vector<ItemDesc> vlist;
	XItem::PrintEmblemTLog(m_pRole, pItem, TX_Emblem_Wear, -1, vlist);
	XItem::PrintEmblemTLog(m_pRole, pSwapEmblem, TX_Emblem_TakeOff, -1, vlist);

	pItem->SetEmSlotType(slotType);

	CallBack(OnSwapItem, pItem, pSwapEmblem, slot);

	m_Emblem[slot] = pItem;
	m_mapItems.erase(pItem->uid);

	if (pSwapEmblem)
	{
		pSwapEmblem->SetEmSlotType(KKSG::EmblemSlotType_None);
		*it = pSwapEmblem;
		m_mapItems.insert(std::make_pair(pSwapEmblem->uid, pSwapEmblem));
	}
	else
	{
		m_Emblems.erase(it);
	}

	// 穿在身上的技能纹章
	if (emtype > EmblemBoundary && slot >= AttributeEmblemCount)
	{
		///> 上面判过空了
		const SkEmblem* pSkEmblem = CEmblemMgr::Instance()->GetSkillEmblemByID(pItem->itemID); 
		UINT32 wearSlot = slot - AttributeEmblemCount;
		if (pSwapEmblem)
		{
			delete m_WearedSkillEmblem[wearSlot];
		}
		m_WearedSkillEmblem[wearSlot] = new SkEmblem(*pSkEmblem);
	}

	pItem->SetBind(true);

	m_pRole->OnWearEmblem(slot);

	m_isModify.Set();
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Bag::WearArtifact(XItem* pItem, BagListener* listener)
{
	if (FindUniqueItemInBag(pItem->uid) == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	auto i = std::find_if(m_Artifacts.begin(), m_Artifacts.end(), EqualItemUID(pItem->uid));
	if (i == m_Artifacts.end())
	{
		LogError("Unknown error, maybe this item [%llu] is not equip", pItem->uid);
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	ItemConf* pItemConf = ItemConfig::Instance()->GetItemConf(pItem->itemID);
	if (pItemConf == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	if (m_pRole->GetLevel() < pItemConf->ReqLevel)
	{
		return KKSG::ERR_ITEM_LEVELLIMIT;
	}
	if (pItem->type != ARTIFACT)
	{
		LogWarn("Wear Artifact type error: itemID = %d, itemType = %d", pItemConf->ItemID, pItemConf->ItemType);
		return KKSG::ERR_UNKNOWN; //外层应该判断
	}

	const ArtifactListConf* pArtifactConf = ArtifactConfig::Instance()->GetArtifactListConf(pItem->itemID);
	if (pArtifactConf == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	UINT32 slot = pArtifactConf->ArtifactPos;

	if (slot >= ArtifactCount)
	{
		LogWarn("ArtifactPos slot [%u] error, itemid [%u]", slot, pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}

	XItem* tmpItem = NULL;
	if (m_Artifact[slot] != NULL)
	{
		tmpItem = m_Artifact[slot];
		*i = m_Artifact[slot];
		m_Artifact[slot] = pItem;

		m_mapItems.erase(pItem->uid);
		m_mapItems.insert(std::make_pair((*i)->uid, *i));
	}
	else
	{
		m_Artifact[slot] = pItem;
		m_Artifacts.erase(i);
		m_mapItems.erase(pItem->uid);
	}
	//to do log
	std::string strTrans = TTag::Instance()->GetTagString();
	XItem::PrintArtifactTLog(m_pRole, pItem, TX_Artifact_Wear, strTrans);
	XItem::PrintArtifactTLog(m_pRole, tmpItem, TX_Artifact_TakeOff, strTrans);

	pItem->SetBind(true); //必须在notifyclient之前设置，因为swapitem会发整个物品给客户端

	CallBack(OnSwapItem, pItem, tmpItem, slot);

	if (tmpItem != NULL)
	{
		EffectConfig::Instance()->UpdateEffectData(tmpItem->effects);
	}

	UpdateArtifactEffectData();

	m_pRole->OnWearArtifact(slot);

	m_isModify.Set();

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Bag::TakeoffItem(XItem* pItem, bool isFullSendMail, BagListener* listener)
{
	int slot;
	if (FindUniqueItemOnBody(pItem->uid, &slot) == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}	

	std::list<XItem*>& itemList = _GetItemListByType(pItem->type);

	UINT32 maxNum = _GetItemListMaxCapacity(pItem->itemID);

	switch (pItem->type)
	{
	case EQUIP:
		{
			if (m_Equip[slot] != pItem)
			{
				LogError("role [%llu], Strange error, m_Equip[slot] != pItem, change this itemid's type?", m_pRole->GetID());
				return KKSG::ERR_UNKNOWN;
			}
			if (itemList.size() >= maxNum)
			{
				if (isFullSendMail && listener != NULL)
				{
					m_Equip[slot] = NULL;
					CallBack(OnBagFullAndSendMail, pItem);
				}
				else
				{
					return KKSG::ERR_BAG_FULL_TAKEOFF_EQUIP;
				}
			}
			else
			{
				m_Equip[slot] = NULL;
				if (!_InsertToBag(pItem, false))
				{
					///> 让他脱不下来
					m_Equip[slot] = pItem;
					return KKSG::ERR_UNKNOWN;
				}
			}
			break;
		}
	case EMBLEM:
		{
			if (m_Emblem[slot] != pItem)
			{
				LogError("role [%llu], Strange error, m_Emblem[slot] != pItem, change this itemid's type?", m_pRole->GetID());
				return KKSG::ERR_UNKNOWN;
			}
			if (itemList.size() >= maxNum)
			{
				if (isFullSendMail && listener != NULL)
				{
					m_Emblem[slot] = NULL;
					CallBack(OnBagFullAndSendMail, pItem);
				}
				else
				{
					return KKSG::ERR_BAG_FULL_TAKEOFF_EMBLEM;
				}
			}
			else
			{
				m_Emblem[slot] = NULL;
				if (!_InsertToBag(pItem, false))
				{
					///> 让他脱不下来
					m_Emblem[slot] = pItem;
					return KKSG::ERR_UNKNOWN;
				}
			}
			std::vector<ItemDesc> vlist;
			XItem::PrintEmblemTLog(m_pRole, pItem, TX_Emblem_TakeOff, -1, vlist);
			break;
		}
	case ARTIFACT:
		{
			if (m_Artifact[slot] != pItem)
			{
				LogError("role [%llu], Strange error, m_Artifact[slot] != pItem, change this itemid's type?", m_pRole->GetID());
				return KKSG::ERR_UNKNOWN;
			}

			EffectConfig::Instance()->UpdateEffectData(pItem->effects);

			BagUpdateItemTransition update(m_pRole);
			update.AddChangedItem(pItem);
			update.NotifyClient();

			if (itemList.size() >= maxNum)
			{
				if (isFullSendMail && listener != NULL)
				{
					m_Artifact[slot] = NULL;
					CallBack(OnBagFullAndSendMail, pItem);
				}
				else
				{
					return KKSG::ERR_BAG_FULL_TAKEOFF_ARTIFACT;
				}
			}
			else
			{
				m_Artifact[slot] = NULL;
				if (!_InsertToBag(pItem, false))
				{
					///> 让他脱不下来
					m_Artifact[slot] = pItem;
					return KKSG::ERR_UNKNOWN;
				}
			}
			XItem::PrintArtifactTLog(m_pRole, pItem, TX_Artifact_TakeOff);
			break;
		}
	default:
		{
			LogError("role [%llu], type [%d] invalid", m_pRole->GetID(), pItem->type);
			return KKSG::ERR_UNKNOWN;
		}
	}

	CallBack(OnSwapItem, NULL, pItem, slot);
	
	switch (pItem->type)
	{
	case EQUIP:
		{
			m_pRole->OnWearEquip(slot);
			break;
		}
	case EMBLEM:
		{	
			pItem->SetEmSlotType(KKSG::EmblemSlotType_None);
			if (slot >= AttributeEmblemCount)
			{
				delete m_WearedSkillEmblem[slot - AttributeEmblemCount];
				m_WearedSkillEmblem[slot - AttributeEmblemCount] = NULL;
			}
			m_pRole->OnWearEmblem(slot);
			break;
		}
	case ARTIFACT:
		{
			UpdateArtifactEffectData();
			m_pRole->OnWearArtifact(slot);
			break;
		}
	default:
		{
			break;
		}
	}

	m_isModify.Set();
	
	return KKSG::ERR_SUCCESS;
}

const SkEmblem* Bag::GetWearedSkillEmblemBySlot(UINT32 slot)
{
	if (slot >= SkillEmblemCount)
	{
		return NULL;
	}
	return m_WearedSkillEmblem[slot];
}

void Bag::ReCalSkillEmblem()
{
	for (UINT32 i = AttributeEmblemCount; i < EmblemCount; ++i)
	{
		if (m_WearedSkillEmblem[i - AttributeEmblemCount] != NULL)
		{
			delete m_WearedSkillEmblem[i - AttributeEmblemCount];
			m_WearedSkillEmblem[i - AttributeEmblemCount] = NULL;
		}
		XItem* pItem = m_Emblem[i];
		if (pItem == NULL)
		{
			continue;
		}
		const SkEmblem* pSkEmblem = CEmblemMgr::Instance()->GetSkillEmblemByID(pItem->itemID);
		if (pSkEmblem == NULL)
		{
			continue;
		}
		m_WearedSkillEmblem[i - AttributeEmblemCount] = new SkEmblem(*pSkEmblem);
	}
}

void Bag::ChangeItemAttr(XItem* pItem, const ItemChangeAttr& stChangeAttr, BagListener* listener)
{
	if (pItem == NULL)
	{
		return;
	}
	if (FindUniqueItem(pItem->uid) != pItem)
	{
		LogError("strange error, item address find by uid != give item");
		return;
	}
	
	pItem->ChangeItemAttr(stChangeAttr);

	CallBack(OnChangeItemAttr, pItem);

	m_isModify.Set();
}

void Bag::ChangeItemEnhanceAttr(XItem* pItem, std::vector<ItemChangeAttr>& attrs, BagListener* listener)
{
	if (pItem == NULL)
	{
		return;
	}
	if (FindUniqueItem(pItem->uid) != pItem)
	{
		LogError("strange error, item address find by uid != give item");
		return;
	}

	pItem->ChangeEnhanceAttr(attrs);

	CallBack(OnChangeItemAttr, pItem);

	m_isModify.Set();
}

void Bag::ChangeItemJade(XItem* pItem, const JadeSingle& stJadeSingle, bool bIsDetach, BagListener* listener)
{
	if (NULL == pItem)
	{
		return;
	}
	if (FindUniqueItem(pItem->uid) != pItem)
	{
		LogError("strange error, item address find by uid != give item");
		return;
	}

	pItem->ChangeJade(stJadeSingle, bIsDetach);

	CallBack(OnChangeItemAttr, pItem);

	m_isModify.Set();
}

void Bag::MarkItemChanged(XItem *pItem, BagListener* listener)
{
	if (NULL == pItem)
	{
		return;
	}
	if (FindUniqueItem(pItem->uid) != pItem)
	{
		LogError("strange error, item address find by uid != give item");
		return;
	}

	CallBack(OnChangeItemAttr, pItem);

	m_isModify.Set();
}

void Bag::ApplyEquipEffect(CombatAttribute *combatAttr)
{
	EquipSuitDetector equipDetector(m_pRole->GetProfession());
	for (int i = 0; i < EquipCount; ++i)
	{
		if (m_Equip[i] != NULL)
		{
			m_Equip[i]->ApplyItemEffect(combatAttr, m_pRole);
			equipDetector.AddEquip(m_Equip[i]->itemID);
		}
	}
	equipDetector.ApplySuitEffect(combatAttr);

	EnhanceEquipMgr::Instance()->ApplySuitEffect(combatAttr, m_pRole);

	for (int i = 0; i < EmblemCount; ++i)
	{
		if (m_Emblem[i] != NULL)
		{
			m_Emblem[i]->ApplyItemEffect(combatAttr, m_pRole);
		}
	}

	ArtifactSuitDetector artifactDetector;
	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (m_Artifact[i] != NULL)
		{
			m_Artifact[i]->ApplyItemEffect(combatAttr, m_pRole);
			artifactDetector.AddArtifact(m_Artifact[i]->itemID);
		}
	}
	artifactDetector.ApplySuitEffect(combatAttr);
}

void Bag::GetDifferentModuleAttr(std::vector<CombatAttribute>& combatAttrs)
{
	combatAttrs.clear();
	combatAttrs.resize(7);

	EquipSuitDetector equipDetecter(m_pRole->GetProfession());
	for (int i = 0; i < EquipCount; ++i)
	{
		if (m_Equip[i] != NULL)
		{
			m_Equip[i]->GetEquipDifferentPartAttr(combatAttrs);
			equipDetecter.AddEquip(m_Equip[i]->itemID);
		}
	}
	equipDetecter.ApplySuitEffect(&combatAttrs[0]);

	for (int i = 0; i < EmblemCount; ++i)
	{
		if (m_Emblem[i] != NULL)
		{
			m_Emblem[i]->ApplyItemEffect(&combatAttrs[5], m_pRole);
		}
	}

	ArtifactSuitDetector artifactDetector;
	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (m_Artifact[i] != NULL)
		{
			m_Artifact[i]->ApplyItemEffect(&combatAttrs[6], m_pRole);
			artifactDetector.AddArtifact(m_Artifact[i]->itemID);
		}
	}
	artifactDetector.ApplySuitEffect(&combatAttrs[6]);
}

UINT64 Bag::CountVirtualItem(UINT32 itemID)
{
	if (!XItem::IsVirtualItem(itemID))
	{
		return 0;
	}
	return m_VirtualItem[itemID];
}

void Bag::DecVirtualItem(UINT32 itemID, UINT32 Count, BagListener* listener)
{
	if (!XItem::IsVirtualItem(itemID))
	{
		return;
	}
	if (m_VirtualItem[itemID] < Count)
	{	
		LogError("decrease errror, itemid:%u, count:%llu < dec count:%u", itemID, m_VirtualItem[itemID], Count);
		Count = m_VirtualItem[itemID];
	}
	m_VirtualItem[itemID] -= Count;
	
	CallBack(OnChangeVirtualItem, itemID, m_VirtualItem[itemID]);

	if (NULL != m_pRole)
	{
		m_pRole->OnChangeVirtualItem(itemID, -(INT64)Count);
	}

	m_isModify.Set(); 
}

void Bag::IncVirtualItemRaw(UINT32 itemID, UINT32 Count)
{
	if (!XItem::IsVirtualItem(itemID))
	{
		return;
	}
	m_VirtualItem[itemID] += Count;
	m_isModify.Set();
}

void Bag::IncVirtualItem(UINT32 itemID, UINT32 Count, BagListener* listener)
{
	if (!XItem::IsVirtualItem(itemID))
	{
		return;
	}
	m_VirtualItem[itemID] += Count;

	CallBack(OnChangeVirtualItem, itemID, m_VirtualItem[itemID]);

	if(NULL != m_pRole)
	{
		m_pRole->OnChangeVirtualItem(itemID, Count);
	}

	m_isModify.Set();
}

void Bag::ClearVirtualItem(UINT32 itemID, BagListener* listener)
{
	if (!XItem::IsVirtualItem(itemID))
	{
		return;
	}

	m_VirtualItem[itemID] = 0;

	CallBack(OnChangeVirtualItem, itemID, m_VirtualItem[itemID]);

	m_isModify.Set();
}

void Bag::SetVirtualItem(UINT32 itemID, UINT64 Count, bool isRollBack, BagListener* listener)
{
	if (!XItem::IsVirtualItem(itemID))
	{
		return;
	}

	m_VirtualItem[itemID] = Count;

	if(!isRollBack)
	{
		CallBack(OnChangeVirtualItem, itemID, m_VirtualItem[itemID]);
	}

	m_isModify.Set();
}

void Bag::ApplyVirtualItems(Role *pRole, bool isRealExp)
{
	if (m_VirtualItem[EXP] > 0)
	{
		int exp = (int)m_VirtualItem[EXP];
		m_VirtualItem[EXP] = 0;
		int oldLevel = pRole->GetLevel();

		if (!isRealExp)
		{
			pRole->AddExp(exp);
		}
		else
			pRole->AddRealExp(exp);

		if (oldLevel == pRole->GetLevel())
		{
			pRole->GetNetHandler()->SendExpAndLevelNtf();
		}
	}
	if (m_VirtualItem[VIP_POINT] > 0)
	{
		m_VirtualItem[VIP_POINT] = 0;
	}
	m_isModify.Set();
}

void Bag::ClearBag(Role* pRole, UINT32 dwType)
{
	std::vector<UINT64> vecItemUid;
	std::vector<UINT32> vecItemCount;
	std::list<XItem*>& itemList = _GetItemListByType(dwType);
	for (auto i = itemList.begin(); i != itemList.end(); ++i)
	{
		if (0 == dwType || (*i)->type == dwType)
		{
			vecItemUid.push_back((*i)->uid);
			vecItemCount.push_back((*i)->itemCount);
		}
	}
	BagTakeItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_GM, ItemFlow_GM_Clear);
	for (UINT32 j = 0; j < vecItemUid.size(); ++j)
	{
		stTransition.TakeUniqueItem(vecItemUid[j], vecItemCount[j]);
	}
	stTransition.NotifyClient();
}

void Bag::ForeachItem(ItemCB cb)
{
	for (auto i = m_mapItems.begin(); i != m_mapItems.end(); ++i)
		cb(i->second);
}

void Bag::ForeachEquip(ItemCB cb)
{
	for (auto i = m_Equips.begin(); i != m_Equips.end(); ++i)
		cb(*i);
}

void Bag::ForeachEquipOnBody(ItemCB cb)
{
	for (int i = 0; i < EquipCount; ++i)
	{
		cb(m_Equip[i]);
	}
}

void Bag::SetSummaryEquip(EquipBrief* brief, void* arg)
{
	if (brief == NULL)
	{
		return;
	}
	if (arg != NULL)
	{
		UINT32 pos = *(UINT32*)arg;
		if (pos >= EquipCount)
		{
			LogError("Update SummaryEquip, pos [%u] is invalid", pos);
			return;
		}
		if (m_Equip[pos] != NULL)
		{
			brief->SetEquip(pos, *m_Equip[pos]);
		}	
		else
		{
			brief->ClearEquip(pos);
		}
	}
	else
	{
		for (int i = 0; i < EquipCount; ++i)
		{
			if (m_Equip[i] != NULL)
			{
				brief->SetEquip(i, *m_Equip[i]);
			}
		}
	}
}

void Bag::SetSummaryEmblem(EmblemBrief* brief, void* arg)
{
	if (brief == NULL)
	{
		return;
	}
	if (arg != NULL)
	{
		UINT32 pos = *(UINT32*)arg;
		if (m_Emblem[pos] != NULL)
		{
			brief->SetEmblem(pos, *m_Emblem[pos]);
		}	
		else
		{
			brief->ClearEmblem(pos);
		}
	}
	else
	{
		for (int i = 0; i < EmblemCount; ++i)
		{
			if (m_Emblem[i] != NULL)
			{
				brief->SetEmblem(i, *m_Emblem[i]);
			}
		}
	}
}

void Bag::SetSummaryArtifact(ArtifactBrief* brief, void* arg)
{
	if (brief == NULL)
	{
		return;
	}
	if (arg != NULL)
	{
		UINT32 pos = *(UINT32*)arg;
		if (m_Artifact[pos] != NULL)
		{
			brief->SetArtifact(pos, *m_Artifact[pos]);
		}
		else
		{
			brief->ClearArtifact(pos);
		}
	}
	else
	{
		for (int i = 0; i < ArtifactCount; ++i)
		{
			if (m_Artifact[i] != NULL)
			{
				brief->SetArtifact(i, *m_Artifact[i]);
			}
		}
	}
}

void Bag::GetBodyEquip(std::vector<XItem*>& items)
{
	for (int i = 0; i < EquipCount; ++i)
	{
		if (m_Equip[i] != NULL)
		{
			items.push_back(m_Equip[i]);
		}
	}
}

void Bag::GetBodyEmblem(std::vector<XItem*>& items)
{
	for (int i = 0; i < EmblemCount; ++ i)
	{
		if (m_Emblem[i] != NULL)
		{
			items.push_back(m_Emblem[i]);
		}
	}
}

void Bag::GetBodyArtifact(std::vector<XItem*>& items)
{
	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (m_Artifact[i] != NULL)
		{
			items.push_back(m_Artifact[i]);
		}
	}
}

void Bag::GetBagItem(std::vector<XItem*>& items, ItemType type)
{
	if (ItemTypeAll == type)
	{
		for (auto i = m_mapItems.begin(); i != m_mapItems.end(); ++i)
		{
			items.push_back(i->second);
		}
		return;
	}
	std::list<XItem*>& itemList = _GetItemListByType(type);
	for (auto i = itemList.begin(); i != itemList.end(); ++i)
	{
		if ((*i)->type == type)
		{
			items.push_back(*i);
		}
	}
}

void Bag::GetOutLook(KKSG::OutLookEquip& outlookequip)
{
	for (int i = 0; i < EquipCount; ++i)	
	{
		XItem* item = m_Equip[i];
		if (NULL != item)
		{
			outlookequip.add_slot(i);
			outlookequip.add_itemid(item->itemID);
			outlookequip.add_enhancelevel(NULL == item->enhanceInfo ? 0 : item->enhanceInfo->level);
		}
	}
}

void Bag::NotifyEnhanceMaster()
{
	PtcG2C_NotifyEnhanceSuit msg;
	msg.m_Data.set_enhancesuit(m_enhanceMaster);
	m_pRole->Send(msg);
}

void Bag::SetEnhanceMaster(UINT32 enhanceMaster)
{
	if (enhanceMaster > m_enhanceMaster)
	{
		m_enhanceMaster = enhanceMaster;
		NotifyEnhanceMaster();

		EventMgr::Instance()->AddGuildBonusEvent(m_pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_STRENGTHEN, m_enhanceMaster);
		EventMgr::Instance()->AddEvent(m_pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_AllEquipStengthen, 0, m_enhanceMaster);
	}
}

void Bag::TransEquip(XItem* equip, UINT32 from, UINT32 to)
{
	if (equip == NULL)
	{
		return;
	}
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(equip->itemID);
	if (NULL == conf)
	{
		LogError("TransEquip conf is NULL, equip itemid [%u]", equip->itemID);
		return;
	}
	///> 职业信息
	if (from == conf->Profession)
	{
		UINT32 baseitemid = EquipProtoCalc::GetOriginItemId(equip->itemID, from);
		UINT32 tempitemid = EquipProtoCalc::ConvertToProfessionItemID(baseitemid, to); 
		ItemConf* tempconf = ItemConfig::Instance()->GetItemConf(tempitemid);
		if (NULL == tempconf)
		{
			LogError("TransEquip trans conf is NULL, trans equip itemid [%u]", tempitemid);
		}
		else
		{
			TItemFlowLog oLog1(m_pRole);
			oLog1.m_nItemID = equip->itemID;
			oLog1.m_nType = EQUIP;
			oLog1.m_nCount = (INT32)1;
			oLog1.m_nAfterCount = 0;
			oLog1.m_nReason = ItemFlow_ChangePro;
			oLog1.m_nSubReason = ItemFlow_ChangePro_ItemTake;
			oLog1.m_nMoneyCost = 0;
			oLog1.m_nMoneyType = 0;
			oLog1.m_nAddOrReduce = TX_REDUCE;
			oLog1.Do();

			UINT32 preItemID = equip->itemID;
			equip->itemID = tempitemid;

			TItemFlowLog oLog2(m_pRole);
			oLog2.m_nItemID = equip->itemID;
			oLog2.m_nType = EQUIP;
			oLog2.m_nCount = (INT32)1;
			oLog2.m_nAfterCount = 0;
			oLog2.m_nReason = ItemFlow_ChangePro;
			oLog2.m_nSubReason = ItemFlow_ChangePro_ItemGive;
			oLog2.m_nMoneyCost = 0;
			oLog2.m_nMoneyType = 0;
			oLog2.m_nAddOrReduce = TX_ADD;
			oLog2.Do();

			///> 随机属性
			if (NULL != equip->randAttr)
			{
				RandomAttrMgr::Instance()->RandomAttrTransfer(equip, preItemID);
			}
			///> 锻造属性
			if (NULL != equip->forge)
			{
				RandomAttrMgr::Instance()->ForgeEquipTransfer(m_pRole, equip, preItemID, from, to);
			}
		}
	}
	///> 其他属性
	if (0 == conf->Profession || from == conf->Profession)
	{
		///> 龙玉镶嵌
		if (NULL != equip->jadeInfo)
		{
			std::vector<JadeSingle>& jades = equip->jadeInfo->vecJadeSingle;
			for (auto i = jades.begin(); i != jades.end(); ++i)
			{
				JadeSingle& jade = *i;
				UINT32 destid = CJadeMgr::Instance()->GetTransId(jade.jadeid, from, to);
				if (0 != destid)
				{
					JadeConf* conf = ItemConfig::Instance()->GetJadeConf(destid);
					if (NULL != conf)
					{
						jade.jadeid = destid;
						std::vector<ItemChangeAttr> attrs;
						for (auto j = conf->JadeAttributes.begin(); j != conf->JadeAttributes.end(); ++j)
						{
							attrs.push_back(ItemChangeAttr(j->seq[0], j->seq[1]));
						}
						jade.attrs = attrs;
					}
				}
			}
		}
	}
}

void Bag::ChangeProfession(UINT32 from, UINT32 to)
{
	///> 身上装备
	for (int i = 0; i < EquipCount; ++i)
	{
		if (NULL != m_Equip[i])
		{
			TransEquip(m_Equip[i], from, to);
		}
	}
	///> 包裹装备
	for (auto i = m_Equips.begin(); i != m_Equips.end(); ++i)
	{
		TransEquip(*i, from, to);
	}

	///> 强化
	EnhanceEquipMgr::Instance()->OnRoleChangeProfession(m_pRole);

	///> 包裹龙玉
	std::map<UINT32, UINT32> jadetodel;
	std::map<UINT32, UINT32> jadetoadd;
	for (auto i = m_Bag.begin(); i != m_Bag.end(); ++i)
	{
		XItem* item = *i;
		if (JADE == item->type)
		{
			UINT32 destid = CJadeMgr::Instance()->GetTransId(item->itemID, from, to);
			if (0 != destid)
			{
				jadetodel[item->itemID] += item->itemCount;
				jadetoadd[destid] += item->itemCount;
			}
		}
	}

	bool isdel = true;
	BagTakeItemTransition taketrans(m_pRole);
	taketrans.SetReason(ItemFlow_ChangePro, ItemFlow_ChangePro_ItemTake);
	for (auto i = jadetodel.begin(); i != jadetodel.end(); ++i)
	{
		isdel |= taketrans.TakeItem(i->first, i->second);
	}
	taketrans.NotifyClient();

	if (isdel)
	{
		BagGiveItemTransition givetrans(m_pRole);
		givetrans.SetReason(ItemFlow_ChangePro, ItemFlow_ChangePro_ItemGive);
		for (auto i = jadetoadd.begin(); i != jadetoadd.end(); ++i)
		{
			givetrans.GiveItem(i->first, i->second);
		}
		givetrans.NotifyClient();
	}
	else
	{
		SSError<<"change pro error, roleid:"<<m_pRole->GetID()<<END;
	}

	///> 脱下技能纹章
	BagWearItemTransition weartrans(m_pRole);
	weartrans.SetSubReason(ItemFlow_ChangePro_ItemGive);
	for (UINT32 i = AttributeEmblemCount; i < EmblemCount; ++i)
	{
		XItem* item = m_Emblem[i];
		if (NULL != item)
		{
			weartrans.TakeoffItem(item, true);
		}
	}
	weartrans.NotifyClient();

	///> 卸下重复属性的龙玉
	for (int i = 0; i < EquipCount; ++i)
	{
		if (NULL != m_Equip[i])
		{
			std::vector<JadeSingle> jadesingles;
			m_Equip[i]->DetachSameJade(jadesingles);
			if (!jadesingles.empty())
			{
				DoDetachJade(m_Equip[i], jadesingles);
			}
		}
	}
	for (auto i = m_Equips.begin(); i != m_Equips.end(); ++i)
	{
		std::vector<JadeSingle> jadesingles;
		(*i)->DetachSameJade(jadesingles);
		if (!jadesingles.empty())
		{
			DoDetachJade(*i, jadesingles);
		}
	}
	m_isModify.Set();
}

void Bag::DoDetachJade(XItem* equip, const std::vector<JadeSingle>& jadesingles)
{
	for (auto i = jadesingles.begin(); i != jadesingles.end(); ++i)
	{
		BagChangeItemAttrTransition stChangeAttrTransiton(m_pRole);
		stChangeAttrTransiton.ChangeItemJade(equip, *i, true);
		stChangeAttrTransiton.NotifyClient();

		///> 构造新的龙玉给玩家
		BagGiveItemTransition stGiveTransition(m_pRole);
		stGiveTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_TakeOff);
		stGiveTransition.GiveItem(i->jadeid, 1);
		stGiveTransition.NotifyClient();
	}
}

KKSG::ErrorCode Bag::UpgradeEquip(UINT64 uid)
{
	int index = 0;
	XItem* pItem = FindUniqueEquipOnBody(uid, &index);
	if (pItem == NULL)
	{
		return KKSG::ERR_ITEM_NOTEXIST;
	}
	EquipConf* conf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (conf == NULL)
	{
		LogWarn("itemid:%u not in EquipList.txt", pItem->itemID);
		return KKSG::ERR_UNKNOWN;
	}
	EquipConf* newConf = ItemConfig::Instance()->GetEquipConf(conf->UpgadeTargetID);
	if (newConf == NULL || ItemConfig::Instance()->GetItemConf(conf->UpgadeTargetID) == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	if (conf->EquipPos != newConf->EquipPos)
	{
		LogWarn("EquipID:%u pos [%u] not equal to upgradeEquipID pos [%u]", pItem->itemID, conf->EquipPos, newConf->EquipPos);
		return KKSG::ERR_UNKNOWN;
	}

	BagTakeItemTransition take(m_pRole);
	take.SetReason(ItemFlow_Equip, ItemFlow_Equip_Upgrade);
	for (auto i = conf->UpgradeNeedMaterials.begin(); i != conf->UpgradeNeedMaterials.end(); ++i)
	{
		if (!take.TakeItem((*i)[0], (*i)[1]))
		{
			take.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
	}
	take.NotifyClient();

	UINT32 preItemID = pItem->itemID;
	pItem->itemID = conf->UpgadeTargetID;

	///> 随机属性
	if (NULL != pItem->randAttr)
	{
		RandomAttrMgr::Instance()->RandomAttrTransfer(pItem, preItemID);
	}
	///> 锻造属性
	if (NULL != pItem->forge)
	{
		RandomAttrMgr::Instance()->ForgeEquipTransfer(m_pRole, pItem, preItemID, 0, 0);
	}

	BagUpdateItemTransition trans(m_pRole);
	trans.AddChangedItem(pItem);
	trans.NotifyClient();

	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_UpgradeEquip);

	m_pRole->OnUpgradeEquip(index);

	return KKSG::ERR_SUCCESS;
}

void Bag::UpdateArtifactEffectData()
{
	std::vector<std::vector<KKSG::EffectData>*> vecAllEffectData;
	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (m_Artifact[i] != NULL)
		{
			vecAllEffectData.push_back(&m_Artifact[i]->effects);
		}
	}
	EffectConfig::Instance()->UpdateEffectData(vecAllEffectData);

	if (!m_pRole->GetFlag(RSF_ISWaitLogin))
	{
		BagUpdateItemTransition update(m_pRole);
		for (int i = 0; i < ArtifactCount; ++i)
		{
			if (m_Artifact[i] != NULL)
			{
				update.AddChangedItem(m_Artifact[i]);
			}
		}
		update.NotifyClient();
	}
}

void Bag::ArrangeEffectData()
{
	m_buff2EffectData.clear();
	m_skill2EffectData.clear();

	UpdateArtifactEffectData();

	for (int i = 0; i < ArtifactCount; ++i)
	{
		if (m_Artifact[i] == NULL)
		{
			continue;
		}

		for (auto j = m_Artifact[i]->effects.begin(); j != m_Artifact[i]->effects.end(); ++j)
		{
			const KKSG::EffectData& effectData = *j;
			if (!effectData.iswork())
			{
				continue;
			}
			for (int z = 0; z < effectData.multiparams_size(); ++z)
			{
				const KKSG::EffectMultiParams& multiParams = effectData.multiparams(z);
				const EffectConf* conf = NULL;
				std::unordered_map<UINT32, EffectDataParams>* dataMap = NULL;
				if (multiParams.idtype() == EffectIDType_Buff)
				{
					conf = EffectConfig::Instance()->GetBuffEffectConf(effectData.effectid(), multiParams.id());
					dataMap = &m_buff2EffectData;
				}
				else if (multiParams.idtype() == EffectIDType_Skill)
				{
					conf = EffectConfig::Instance()->GetSkillEffectConf(effectData.effectid(), multiParams.id());
					dataMap = &m_skill2EffectData;
				}
				if (conf == NULL)
				{
					continue;
				}
				EffectDataParams& dataParams = dataMap->operator[](multiParams.id());
				std::unordered_map<UINT32, EffectDataParams::TypeData> tempType2Data;

				for (UINT32 k = 0; k < conf->EffectParams.size(); ++k)
				{
					if (k >= multiParams.effectparams_size())
					{
						break;
					}
					auto iter = tempType2Data.find(conf->EffectParams[k][0]);
					if (iter == tempType2Data.end())
					{
						EffectDataParams::TypeData typeData;
						typeData.effectID = conf->EffectID;
						typeData.templatebuffID = conf->TemplateBuffID;
						tempType2Data.insert(std::make_pair(conf->EffectParams[k][0], typeData));
					}

					tempType2Data[conf->EffectParams[k][0]].params.push_back(multiParams.effectparams(k));
				}

				for (UINT32 k = 0; k < conf->ConstantParams.size(); ++k)
				{
					UINT32 type = convert<UINT32>(conf->ConstantParams[k][0]);
					auto iter = tempType2Data.find(type);
					if (iter == tempType2Data.end())
					{
						EffectDataParams::TypeData typeData;
						typeData.effectID = conf->EffectID;
						typeData.templatebuffID = conf->TemplateBuffID;
						tempType2Data.insert(std::make_pair(type, typeData));
					}

					tempType2Data[type].constantParams.push_back(conf->ConstantParams[k][1]);
				}
				for (auto k = tempType2Data.begin(); k != tempType2Data.end(); ++k)
				{
					dataParams.type2DataMap[k->first].push_back(k->second);
				}
			}
		}
	}
}

const EffectDataParams* Bag::GetBuffEffectData(UINT32 buffID)
{
	auto iter = m_buff2EffectData.find(buffID);
	if (iter == m_buff2EffectData.end())
	{
		return NULL;
	}
	return &iter->second;
}

const EffectDataParams* Bag::GetSkillEffectData(UINT32 skillID)
{
	auto iter = m_skill2EffectData.find(skillID);
	if (iter == m_skill2EffectData.end())
	{
		return NULL;
	}
	return &iter->second;
}

void Bag::AddExtraSkillEmblemSlot()
{
	if (m_extraSkillEmblemSlot >= GetGlobalConfig().ExtraSkillEmblemSlotLevelLimit.size())
	{
		return;
	}
	m_extraSkillEmblemSlot++;
	m_isModify.Set();
}

KKSG::ErrorCode Bag::AddBagExpand(KKSG::BagType type, UINT32 num)
{
	if (!KKSG::BagType_IsValid(type))
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (m_bagExpand.find(type) == m_bagExpand.end())
	{
		m_bagExpand[type] = std::make_pair(num, 1);
	}
	else
	{
		std::pair<UINT32, UINT32> data = m_bagExpand[type];
		m_bagExpand[type] = std::make_pair(data.first + num, data.second + 1);
	}
	LogInfo("role [%llu], BagExpand type [%d], num [%u], count [%u]", m_pRole->GetID(), type, m_bagExpand[type].first, m_bagExpand[type].second);
	m_isModify.Set();
	return KKSG::ERR_SUCCESS;
}

std::pair<UINT32, UINT32> Bag::GetBagExpandData(KKSG::BagType type)
{
	if (!KKSG::BagType_IsValid(type))
	{
		return std::make_pair(0, 0);
	}
	auto iter = m_bagExpand.find(type);
	if (iter == m_bagExpand.end())
	{
		return std::make_pair(0, 0);
	}
	return iter->second;
}

static void GetThisLevelEquipSuitID(Role* pRole, int& suitid3, int& suitid4)
{
	UINT32 level3 = 0, level4 = 0;
	EquipSuitTable& table = ItemConfig::Instance()->GetEquipSuitTable();
	int baseProf = ProfessionMgr::Instance()->GetBasicProfession(pRole->GetProfession());
	for (auto i = table.Table.begin(); i != table.Table.end(); ++i)
	{
		if (!(*i)->IsCreateShow || (*i)->Level > pRole->GetLevel()
			|| (*i)->ProfID != baseProf)
		{
			continue;
		}
		if ((*i)->SuitQuality == 3)
		{
			if ((*i)->Level > level3)
			{
				level3 = (*i)->Level;
				suitid3 = (*i)->SuitID;
			}
		}
		else if ((*i)->SuitQuality == 4)
		{
			if ((*i)->Level > level4)
			{
				level4 = (*i)->Level;
				suitid4 = (*i)->SuitID;
			}
		}
	}
	LogDebug("level:%u, suitid3:%u, suitid4:%u", pRole->GetLevel(), suitid3, suitid4);
}

static void GetFuseCompensation(UINT32 itemid, int baseProf, int suitid3, int suitid4, std::map<UINT32, UINT32>& giveItemMap)
{
	int suitid = ItemConfig::Instance()->FindEquipSuit(baseProf, itemid);
	if (suitid == 0)
	{
		return;
	}
	if (suitid != suitid3 && suitid != suitid4)
	{
		return;	
	}
	ItemComposeTableConf* conf = ItemConfig::Instance()->GetComposeConf(itemid);
	if (conf == NULL)
	{
		return;
	}
	for (auto i = conf->FuseCompensation.begin(); i != conf->FuseCompensation.end(); ++i)
	{
		if (ItemConfig::Instance()->GetItemConf((*i)[0]) != NULL && (*i)[1] > 0)
		{
			giveItemMap[(*i)[0]] += (*i)[1];
		}
	}
}

void Bag::_GiveFuseCompensation()
{
	int suitid3 = 0, suitid4 = 0;
	GetThisLevelEquipSuitID(m_pRole, suitid3, suitid4);

	int baseProf = ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession());
	std::set<UINT32> itemSet;
	std::map<UINT32, UINT32> giveItemMap;
	for (int i = 0; i < EquipCount; ++i)
	{
		if (m_Equip[i] != NULL)
		{
			if (itemSet.find(m_Equip[i]->itemID) == itemSet.end())
			{
				itemSet.insert(m_Equip[i]->itemID);
				GetFuseCompensation(m_Equip[i]->itemID, baseProf, suitid3, suitid4, giveItemMap);
			}
		}
	}
	for (auto i = m_Equips.begin(); i != m_Equips.end(); ++i)
	{
		if (itemSet.find((*i)->itemID) == itemSet.end())
		{
			itemSet.insert((*i)->itemID);
			GetFuseCompensation((*i)->itemID, baseProf, suitid3, suitid4, giveItemMap);
		}
	}
	if (!giveItemMap.empty())
	{
		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_FuseCompensation);
		if (conf != NULL)
		{
			MailSender sender;
			sender.SetContent(Mail_System, conf->m_title, conf->m_content);
			sender.SetReason(ItemFlow_Equip, ItemFlow_Equip_FuseCompensation);
			for (auto i = giveItemMap.begin(); i != giveItemMap.end(); ++i)
			{
				sender.AddItem(ItemDesc(i->first, i->second));
			}
			sender.Send(m_pRole->GetID());
		}
	}
}

static void GetForgeCompensation(XItem* pItem, int baseProf, int suitid3, int suitid4, std::map<UINT32, UINT32>& giveItemMap)
{
	int suitid = ItemConfig::Instance()->FindEquipSuit(baseProf, pItem->itemID);
	if (suitid == 0)
	{
		return;
	}
	if (suitid != suitid3 && suitid != suitid4)
	{
		return;
	}
	EquipConf* conf = ItemConfig::Instance()->GetEquipConf(pItem->itemID);
	if (conf == NULL)
	{
		return;
	}
	UINT32 count = pItem->forge == NULL ? 0 : pItem->forge->haveAttrs.size();
	if (count == 0)
	{
		return;
	}
	for (auto j = conf->ForgeCompensation.begin(); j != conf->ForgeCompensation.end(); ++j)
	{
		giveItemMap[(*j)[0]] += count * (*j)[1];
	}
}

void Bag::_GiveForgeCompensation()
{
	int suitid3 = 0, suitid4 = 0;
	GetThisLevelEquipSuitID(m_pRole, suitid3, suitid4);

	int baseProf = ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession());
	std::set<UINT32> itemSet;
	std::map<UINT32, UINT32> giveItemMap;
	for (int i = 0; i < EquipCount; ++i)
	{
		if (m_Equip[i] != NULL)
		{
			if (itemSet.find(m_Equip[i]->itemID) == itemSet.end())
			{
				itemSet.insert(m_Equip[i]->itemID);
				GetForgeCompensation(m_Equip[i], baseProf, suitid3, suitid4, giveItemMap);
			}
		}
	}
	for (auto i = m_Equips.begin(); i != m_Equips.end(); ++i)
	{
		if (itemSet.find((*i)->itemID) == itemSet.end())
		{
			itemSet.insert((*i)->itemID);
			GetForgeCompensation(*i, baseProf, suitid3, suitid4, giveItemMap);
		}
	}
	if (!giveItemMap.empty())
	{
		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_ForgeCompensation);
		if (conf != NULL)
		{
			MailSender sender;
			sender.SetContent(Mail_System, conf->m_title, conf->m_content);
			sender.SetReason(ItemFlow_Equip, ItemFlow_Equip_ForgeCompensation);
			for (auto i = giveItemMap.begin(); i != giveItemMap.end(); ++i)
			{
				sender.AddItem(ItemDesc(i->first, i->second));
			}
			sender.Send(m_pRole->GetID());
		}
	}
}