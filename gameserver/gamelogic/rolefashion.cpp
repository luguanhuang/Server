#include "pch.h"
#include "rolefashion.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "unit/combatattribute.h"
#include "LogInit.h"
#include "bag.h"
#include "unit/calcattribute.h"
#include "bagtransition.h"
#include "fashoin/ptcg2c_fashoinchangedntf.h"
#include "scene/ptcg2c_changefashionnotify.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "foreach.h"
#include "table/PowerPointMgr.h"
#include "utility/uidhelper.h"
#include "noticemgr.h"
#include "item.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "utility/tlogger.h"
#include "util/gametime.h"
#include "unit/roleoutlook.h"
#include "fashion/ptcg2c_displayadditem.h"
#include "fashion//ptcg2c_updatedisplayitems.h"
#include "table/OpenSystemMgr.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "buff/buffrecord.h"
#include "gamelogic/itemconfig.h"
#include "event/eventmgr.h"

#define INVALID_FASHION_TIME (UINT32)(-1)

void FashionInfo::ToThis(const KKSG::FashionData* data)
{
	uid = data->uid();
	itemID = data->itemid();
	timeleft = data->timeleft();
	timeend = data->timeend();
	for (int i = 0; i < data->attrs_size(); ++i)
	{
		m_attrs.push_back(ItemChangeAttr(data->attrs(i).id(), data->attrs(i).value()));
	}
}

void FashionInfo::ToKKSG(KKSG::FashionData* data)
{
	data->set_uid(uid);
	data->set_itemid(itemID);
	data->set_timeleft(timeleft);
	data->set_timeend(timeend);
	for (auto i = m_attrs.begin(); i != m_attrs.end(); ++i)
	{
		KKSG::AttributeInfo* info = data->add_attrs();
		info->set_id(i->AttrID);
		info->set_value(i->AttrValue);
	}
}

///////////////////////////////////////////////////////////////////////////////
RoleFashion::RoleFashion(Role *pRole)
{
	m_baglistener = NULL;
	m_pRole = pRole;
	memset(m_FashionOnBody, 0, sizeof(m_FashionOnBody));
	own_fashins_.clear();
	first_open_ = false;
	hair_color_id_ = 0;
	fashion_ibshop_buy_time_ = 0;
	fashion_compose_time_ = 0;

	special_effects_suitid_ = 0;
	special_effects_suitid_list_.clear();
	special_effects_suitid_list_.insert(0);

	compose_success_num_ = 0;
	compose_failed_num_;
	quality_compose_.clear();
}

RoleFashion::~RoleFashion()
{
	for (int i = 0; i < FashionCount; ++i)
	{
		if (NULL != m_FashionOnBody[i])
		{
			delete m_FashionOnBody[i];
			m_FashionOnBody[i] = NULL;
		}
	}
	for (auto j = m_Fashions.begin(); j != m_Fashions.end(); ++j)
	{
		if (NULL != j->second)
		{
			delete j->second;
			j->second = NULL;
		}
	}

	own_fashins_.clear();
}

void RoleFashion::FirstInit(UINT32 roleNum)
{
	m_isModifed.Set();
}

bool RoleFashion::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_fashionrecord())
	{
		const KKSG::FashionRecord& record = poRoleAllInfo->fashionrecord();
		for (int j = 0; j < record.bodyfashion_size(); ++j)
		{
			if (j < FashionCount)
			{
				if (0 == record.bodyfashion(j).uid())
				{
					m_FashionOnBody[j] = NULL;
				}
				else
				{
					FashionInfo* info = new FashionInfo();
					info->ToThis(&record.bodyfashion(j));
					m_FashionOnBody[j] = info;

					// 穿着的，可显示
					own_display_items_.insert(info->itemID);
				}
			}
		}
		for (int i = 0; i < record.bagfashion_size(); ++i)
		{
			FashionInfo* info = new FashionInfo();
			info->ToThis(&record.bagfashion(i));
			m_Fashions[info->uid] = info;
			// 背包有的，也需要排除不适合的装备
			if (CheckEquipNeedForProfession(info->itemID))
			{
				// 检查是否永久时装，如果不是，检查是否激活
				ItemConf *conf = ItemConfig::Instance()->GetItemConf(info->itemID);
				if (NULL == conf )
				{
					continue;
				}
				if (0 == conf->TimeLimit)
				{			
					own_display_items_.insert(info->itemID);
				}
				else
				{
					if (info->timeend != info->timeleft)
					{
						own_display_items_.insert(info->itemID);
					}
				}
			}
		}
		for (int k = 0; k < record.collected_size(); ++k)
		{
			m_collected.insert(record.collected(k));
		}
		// 设置的外显
		for (UINT32 i = 0; i < (UINT32)record.display_fashion_size(); ++i)
		{
			FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(record.display_fashion(i));
			if (pConf == NULL)
			{				
				continue;
			}
			display_fashion_.insert(std::make_pair(pConf->EquipPos,pConf->ItemID));
		}
		// 收藏的时装，第一次加载为空，后面会统一处理
		for (UINT32 i = 0; i < (UINT32)record.own_fashins_size(); ++i)
		{
			SuitDisplayStore stroe;
			stroe.suit_id = record.own_fashins(i).suit_id();
			for (UINT32 index = 0; index < (UINT32)record.own_fashins(i).items_size(); ++index)
			{
				UINT32 item_id = record.own_fashins(i).items(index);				
				stroe.items.insert(item_id);
				if (CheckEquipNeedForProfession(item_id))
				{
					own_display_items_.insert(item_id);
				}				
			}
			stroe.count = record.own_fashins(i).activate_count();
			own_fashins_.push_back(stroe);
		}

		//收藏的装备
		for (UINT32 i = 0; i < (UINT32)record.own_display_items_size(); ++i)
		{
			if (CheckEquipNeedForProfession(record.own_display_items(i)))
			{
				own_display_items_.insert(record.own_display_items(i));
			}
		}
		//可以穿戴的装备
		std::vector<XItem*> items;
		m_pRole->Get<Bag>()->GetBagItem(items,EQUIP);
		for (auto it = items.begin(); it != items.end(); ++it)
		{
			if (CheckEquipNeedForProfession((*it)->itemID))
			{
				UINT32 suit_id = FashionMgr::Instance()->GetSuitIdForCharm((*it)->itemID);
				if (0 == suit_id )
				{
					FashionConf *pConf = ItemConfig::Instance()->GetFashionConf((*it)->itemID);
					if (NULL != pConf)
					{
						own_display_items_.insert((*it)->itemID);							
					}
					continue;
				}	
				own_display_items_.insert((*it)->itemID);
			}
		}

		// 已经穿在身上的
		// 时装已经加载，再加载 装备
		for (int i = 0; i < EquipCount; ++i)	
		{
			XItem*  equip = m_pRole->Get<Bag>()->GetItemOnBody(EQUIP, i);			
			if (NULL != equip && equip->type == EQUIP)
			{
				UINT32 suit_id = FashionMgr::Instance()->GetSuitIdForCharm(equip->itemID);
				if (0 == suit_id )
				{
					FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(equip->itemID);
					if (NULL != pConf)
					{
						own_display_items_.insert(equip->itemID);							
					}
					continue;
				}	
				own_display_items_.insert(equip->itemID);				
			}
		}

		//  做一次防遗漏的处理，全部收藏物品看看是否需要激活
		auto item_it = own_display_items_.begin();
		for (; item_it != own_display_items_.end(); ++item_it)
		{
			AddFashionForCharm(*item_it);
		}

		UINT32 nowtime = (UINT32)GameTime::GetTime();
		UpdateAll(nowtime, false);
		first_open_ = record.conversion();
		if (!first_open_)
		{
			SetInitDisplayFashion();
			first_open_ = true;
			m_isModifed.Set();
		}
		// 头发信息
		hair_color_id_ = record.hair_color_id();
		for (UINT32 index = 0 ; index < (UINT32)record.hair_color_info_size(); ++index)
		{
			const KKSG::ActivateHairColor& color_info =  record.hair_color_info(index);
			for (UINT32 i = 0; i < (UINT32)color_info.hair_color_id_size(); ++i)
			{
				hair_color_list_[color_info.hair_id()].insert(color_info.hair_color_id(i));
			}
		}

		CheckHairColor();

		// fashionsynthersis_fail_info_
		fashionsynthersis_fail_info_.clear();
		for (UINT32 index = 0 ; index < (UINT32)record.fashionsynthersis_fail_info_size(); ++index)
		{
			auto& info = record.fashionsynthersis_fail_info(index);
			fashionsynthersis_fail_info_.insert(std::make_pair(info.itemid(),info.itemcount()));
		}

		fashion_ibshop_buy_time_ = record.fashionibshop_buy_time();
		fashion_compose_time_ = record.fashioncompose_time();
		special_effects_suitid_ = record.special_effects_id();
		for (UINT32 index = 0; index < (UINT32)record.special_effects_list_size(); ++index)
		{
			special_effects_suitid_list_.insert(record.special_effects_list(index));
		}

		compose_success_num_ = record.compose_success_num();
		compose_failed_num_ = record.compose_failed_num();
		for ( UINT32 index = 0 ; index < record.quality_num_list_size(); ++index)
		{
			const KKSG::MapIntItem& item =  record.quality_num_list(index); 
			quality_compose_.insert(std::make_pair((int)item.key(),item.value()));			
		}
	}

	return true;
}

void RoleFashion::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_isModifed.TestAndReset())
	{
		KKSG::FashionRecord* record = poRoleAllInfo->mutable_fashionrecord();
		record->Clear();
		for (auto i = 0; i < FashionCount; ++i)
		{
			KKSG::FashionData* data = record->add_bodyfashion();
			if (NULL != m_FashionOnBody[i])	
			{
				m_FashionOnBody[i]->ToKKSG(data);
			}
		}
		for (auto j = m_Fashions.begin(); j != m_Fashions.end(); ++j)
		{
			KKSG::FashionData* data = record->add_bagfashion();
			j->second->ToKKSG(data);
		}
		for (auto k = m_collected.begin(); k != m_collected.end(); ++k)
		{
			record->add_collected(*k);
		}

		// 设置的外显
		for (auto it = display_fashion_.begin(); it !=  display_fashion_.end(); ++it)
		{
			record->add_display_fashion(it->second);

		}

		// 收藏的外显
		record->clear_own_fashins();
		auto it_id = own_fashins_.begin();
		auto it_id_end = own_fashins_.end();
		for (; it_id != it_id_end; ++it_id)
		{
			KKSG::ActivateFashionCharm* fashion = record->add_own_fashins();
			fashion->set_suit_id(it_id->suit_id);
			fashion->set_activate_count(it_id->count);	
			auto item_it = it_id->items.begin();
			auto item_it_end = it_id->items.end();
			for (; item_it != item_it_end; ++item_it)
			{
				fashion->add_items(*item_it);
			}
		}

		for ( auto it = own_display_items_.begin(); it != own_display_items_.end(); ++it)
		{
			record->add_own_display_items(*it);
		}
		// 第一次转化后，就不需要修改了
		if (first_open_)
		{
			record->set_conversion(first_open_);
		}

		// 保存头发信息
		record->set_hair_color_id(hair_color_id_);
		for (auto hair_it = hair_color_list_.begin(); hair_it != hair_color_list_.end(); ++hair_it)
		{
			KKSG::ActivateHairColor* hair_color_info = record->add_hair_color_info();
			hair_color_info->set_hair_id(hair_it->first);		
			for (auto it = (hair_it->second).begin(); it != (hair_it->second).end(); ++it)
			{
				hair_color_info->add_hair_color_id(*it);
			}			
		}

		// 服装合成失败的次数信息
		for (auto fail_it = fashionsynthersis_fail_info_.begin(); fail_it != fashionsynthersis_fail_info_.end(); ++fail_it)
		{
			auto  fail_info = record->add_fashionsynthersis_fail_info();
			fail_info->set_itemid(fail_it->first);
			fail_info->set_itemcount(fail_it->second);
		}

		record->set_fashionibshop_buy_time(fashion_ibshop_buy_time_);
		record->set_fashioncompose_time(fashion_compose_time_);

		record->set_special_effects_id(special_effects_suitid_);
		for (auto sp_it = special_effects_suitid_list_.begin(); sp_it != special_effects_suitid_list_.end(); ++sp_it)
		{
			record->add_special_effects_list(*sp_it);
		}

		record->set_compose_success_num(compose_success_num_);
		record->set_compose_failed_num(compose_failed_num_);		
		for (auto it = quality_compose_.begin(); it != quality_compose_.end(); ++it)
		{
			auto* item =record->add_quality_num_list();
			item->set_key(it->first);
			item->set_value(it->second);
		}

		roChanged.insert(record);
	}
}

void RoleFashion::ApplyEffect(CombatAttribute *combatAttr)
{
	for (int i = 0; i < FashionCount; ++i)
	{
		if (NULL != m_FashionOnBody[i])
		{
			// 重新加载属性，支持配置热更
			FashionComposeTable::RowData* data = FashionMgr::Instance()->GetFashionComposeConf(m_FashionOnBody[i]->itemID, 0);
			if (NULL == data)
			{
				continue;;
			}
			m_FashionOnBody[i]->m_attrs.clear();
			for (auto it = data->Attributes.begin(); it != data->Attributes.end(); ++it)
			{
				m_FashionOnBody[i]->m_attrs.push_back(ItemChangeAttr(it->seq[0], it->seq[1]));
				m_isModifed.Set();
			}

			ApplyEffectAux(m_FashionOnBody[i]->m_attrs, combatAttr);
		}
	}

	FashionMgr::Instance()->SuitEffect(m_FashionOnBody, FashionCount, combatAttr);
	FashionMgr::Instance()->FashionActivateEffect(own_fashins_,combatAttr);
}

int RoleFashion::GetFashionNum()
{
	int count = 0;
	for (int i = 0; i < FashionCount; ++i)
	{
		if (NULL != m_FashionOnBody[i])
		{
			++count;
		}
	}
	return count + m_Fashions.size();
}

int RoleFashion::GetFashionNumByItemId(UINT32 itemid)
{
	int count = 0;
	for (auto i = m_Fashions.begin(); i != m_Fashions.end(); ++i)
	{
		if (itemid == i->second->itemID)
		{
			++count;
		}
	}
	return count;
}

bool RoleFashion::DelFashion(UINT64 uid, UINT32 reason, UINT32 subreason, const std::string trastag)
{
	auto i = m_Fashions.find(uid);
	if (i == m_Fashions.end())
	{
		return false;
	}
	UINT32 itemid = i->second->itemID;
	delete i->second;
	m_Fashions.erase(i);

	PtcG2C_FashoinChangedNtf oPtc;
	oPtc.m_Data.set_changetype(KKSG::DELBAG_FASHION);
	KKSG::FashionData* fashion = oPtc.m_Data.add_fashion();
	fashion->set_uid(uid);
	fashion->set_itemid(0);
	fashion->set_level(0);
	fashion->set_pos(0);
	fashion->set_timeleft(0);
	m_pRole->Send(oPtc);

	m_isModifed.Set();


	TItemFlowLog oLog1(m_pRole);
	oLog1.m_nItemID = itemid;
	oLog1.m_nType = FASHION;
	oLog1.m_nCount = (INT32)1;
	oLog1.m_nAfterCount = GetFashionNumByItemId(itemid);
	oLog1.m_nReason = reason;
	oLog1.m_nSubReason = subreason;
	oLog1.m_nAddOrReduce = TX_REDUCE;
	if (!trastag.empty())
	{
		oLog1.SetTransTag(trastag);
	}
	oLog1.Do();
	return true;
}

UINT64 RoleFashion::AddNewFashion(UINT32 itemid, bool isnotice)
{
	FashionComposeTable::RowData* data = FashionMgr::Instance()->GetFashionComposeConf(itemid, 0);
	if (NULL == data)
	{
		return 0;
	}
	FashionInfo* fd = new FashionInfo();
	fd->itemID = itemid;
	fd->level = 0;
	fd->uid = UIDHelper::CreateUID(UID_Item);
	for (auto i = data->Attributes.begin(); i != data->Attributes.end(); ++i)
	{
		fd->m_attrs.push_back(ItemChangeAttr(i->seq[0], i->seq[1]));
	}
	m_Fashions[fd->uid] = fd;

	AddCollected(itemid);

	// 永久时装需要保存
	AddOwnFashions(itemid);
	if (isnotice)
	{
		NotifyChangeSelf(KKSG::ADD_FASHION, m_pRole, fd, false);
	}

	m_isModifed.Set();

	m_pRole->OnAddFashion(itemid);

	if (NULL != m_baglistener)
	{
		m_baglistener->OnAddFashion(fd);
	}

	ItemConf* conf = ItemConfig::Instance()->GetItemConf(itemid);
	if (conf && conf->ItemQuality == QualityIV)
	{
		EventMgr::Instance()->AddGuildBonusEvent(m_pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_USETHREESUIT, itemid);
	}

	return fd->uid;
}

FashionInfo* RoleFashion::FindFashionByUId(UINT64 uid)
{
	auto i = m_Fashions.find(uid);
	if (i == m_Fashions.end())
	{
		return NULL;
	}
	return i->second;
}

FashionInfo* RoleFashion::GetBodyFashion(UINT64 uid)
{
	for (int i = 0; i < FashionCount; ++i)
	{
		if (NULL != m_FashionOnBody[i] && m_FashionOnBody[i]->uid == uid)
		{
			return m_FashionOnBody[i];
		}
	}
	return NULL;
}

KKSG::ErrorCode RoleFashion::Wear(UINT64 uid)
{
	auto i = m_Fashions.find(uid);
	if (i == m_Fashions.end())
	{
		return KKSG::ERR_FASHION_NOT_EXIST;
	}
	FashionInfo* fashion = i->second;
	FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(i->second->itemID);
	if (pConf == NULL || !ValidPos(pConf->EquipPos))
	{
		return KKSG::ERR_FASHION_NOT_EXIST;
	}
	m_Fashions.erase(i);
	// off
	FashionInfo* body = GetBodyFashionPos(pConf->EquipPos);
	if (NULL != body)
	{
		m_Fashions[body->uid] = body;
	}
	// add
	m_FashionOnBody[pConf->EquipPos] = fashion;
	fashion->pos = pConf->EquipPos;

	OpenTimeLimit(fashion);

	m_isModifed.Set();

	NotifyChangeSelf(KKSG::WEAR_FASHION, m_pRole, fashion, false);
	NotifyChangeOther(m_pRole, fashion, false);

	m_pRole->OnWearFashion();

	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Fashion);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode RoleFashion::Takeoff(UINT64 uid)
{
	FashionInfo* fashion = GetBodyFashion(uid);
	if (NULL == fashion)
	{
		return KKSG::ERR_FASHION_NOT_EXIST;
	}
	FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(fashion->itemID);
	if (NULL == pConf || !ValidPos(pConf->EquipPos))
	{
		return KKSG::ERR_FASHION_NOT_EXIST;
	}
	KKSG::ErrorCode error = Takeoff(fashion, pConf->EquipPos);

	if (error == KKSG::ERR_SUCCESS)
	{
		m_pRole->OnWearFashion();
		m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Fashion);
	}

	return error;
}

KKSG::ErrorCode RoleFashion::Takeoff(FashionInfo* fashion, UINT32 pos)
{
	fashion->pos = pos;
	m_FashionOnBody[pos] = NULL;
	m_Fashions[fashion->uid] = fashion;

	m_isModifed.Set();

	NotifyChangeSelf(KKSG::WEAR_FASHION, m_pRole, fashion, true);

	NotifyChangeOther(m_pRole, fashion, true);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode RoleFashion::Wear(std::vector<UINT64>& uids)
{
	std::vector<FashionInfo*> addfashions;
	for (auto i = uids.begin(); i != uids.end(); ++i)
	{
		UINT64 uid = *i;
		auto j = m_Fashions.find(uid);
		if (j == m_Fashions.end())
		{
			continue;
		}
		FashionInfo* fashion = j->second;
		FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(j->second->itemID);
		if (pConf == NULL || !ValidPos(pConf->EquipPos))
		{
			continue;
		}
		m_Fashions.erase(j);
		// off
		FashionInfo* body = GetBodyFashionPos(pConf->EquipPos);
		if (NULL != body)
		{
			m_Fashions[body->uid] = body;
		}
		// add
		m_FashionOnBody[pConf->EquipPos] = fashion;
		fashion->pos = pConf->EquipPos;
		OpenTimeLimit(fashion);

		addfashions.push_back(fashion);
	}

	m_isModifed.Set();

	NotifyChangeSelf(KKSG::WEAR_FASHION, m_pRole, addfashions, false);
	NotifyChangeOther(m_pRole, addfashions, false);

	m_pRole->OnWearFashion();
	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Fashion);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode RoleFashion::Takeoff(std::vector<UINT64>& uids)
{
	std::vector<FashionInfo*> offfashions;
	for (auto i = uids.begin(); i != uids.end(); ++i)
	{
		UINT64 uid = *i;
		// off
		FashionInfo* body = GetBodyFashion(uid);
		if (NULL != body)
		{
			FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(body->itemID);
			if (pConf == NULL || !ValidPos(pConf->EquipPos))
			{
				continue;
			}

			body->pos = pConf->EquipPos;
			m_FashionOnBody[pConf->EquipPos] = NULL;
			// add bag
			m_Fashions[body->uid] = body;
			offfashions.push_back(body);
		}
	}

	m_isModifed.Set();

	NotifyChangeSelf(KKSG::WEAR_FASHION, m_pRole, offfashions, true);
	NotifyChangeOther(m_pRole, offfashions, true);

	m_pRole->OnWearFashion();

	m_pRole->SetOffFashionStamp();
	//m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Fashion);

	return KKSG::ERR_SUCCESS;
}

void RoleFashion::NotifyChangeSelf(UINT32 type, Role* role, FashionInfo* fashion, bool isoff)
{
	std::vector<FashionInfo*> fashions;
	fashions.push_back(fashion);
	NotifyChangeSelf(type, role, fashions, isoff);
}

void RoleFashion::NotifyChangeSelf(UINT32 type, Role* role, std::vector<FashionInfo*>& fashions, bool isoff)
{
	PtcG2C_FashoinChangedNtf oPtc;
	oPtc.m_Data.set_changetype(KKSG::FashionNTFType(type));
	for (auto i = fashions.begin(); i != fashions.end(); ++i)
	{
		KKSG::FashionData* fashion = oPtc.m_Data.add_fashion();
		fashion->set_uid(isoff ? 0 : (*i)->uid);
		fashion->set_itemid(isoff ? 0 : (*i)->itemID);
		fashion->set_level(0);
		fashion->set_pos((*i)->pos);
		fashion->set_timeleft((*i)->timeleft);
	}
	m_pRole->Send(oPtc);
}

void RoleFashion::NotifyChangeOther(Role* role, FashionInfo* fashion, bool isoff)
{
	/*std::vector<FashionInfo*> fashions;
	fashions.push_back(fashion);
	NotifyChangeOther(role, fashions, isoff);*/
}

void RoleFashion::NotifyChangeOther(Role* role, std::vector<FashionInfo*>& fashions, bool isoff)
{
	/*if (m_pRole->GetCurrScene())
	{
	PtcG2C_ChangeFashionNotify oPtc;
	oPtc.m_Data.set_roleid(m_pRole->GetID());
	for (auto i = fashions.begin(); i != fashions.end(); ++i)
	{
	oPtc.m_Data.add_position((*i)->pos);
	oPtc.m_Data.add_newitemid(isoff ? 0 : (*i)->itemID);
	}
	m_pRole->GetCurrScene()->Broadcast(m_pRole, oPtc, m_pRole);
	}*/
}

int RoleFashion::TakeComposeCost(FashionComposeTable::RowData *Row)
{
	BagTakeItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_Fashion, ItemFlow_Fashion_Compose);

	if (Row->ComposeCost > 0)
	{
		if (!transition.TakeItem(GOLD, Row->ComposeCost))
		{
			transition.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
	}

	if (Row->SrcItem1[0] != 0/* && Row->SrcItem1[0] != itemid*/)
	{
		if (!transition.TakeItem(Row->SrcItem1[0], Row->SrcItem1[1]))
		{
			transition.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
	}

	if (Row->SrcItem2[0] != 0/* && Row->SrcItem2[0] != itemid*/)
	{
		if (!transition.TakeItem(Row->SrcItem2[0], Row->SrcItem2[1]))
		{
			transition.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
	}

	if (Row->SrcItem3[0] != 0/* && Row->SrcItem3[0] != itemid*/)
	{
		if (!transition.TakeItem(Row->SrcItem3[0], Row->SrcItem3[1]))
		{
			transition.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
	}

	transition.SetAction(COST_TAKECOMPOSECOST);
	transition.NotifyClient();
	return KKSG::ERR_SUCCESS;
}

void RoleFashion::OpenTimeLimit(FashionInfo* fashion)
{
	if (fashion->timeleft != INVALID_FASHION_TIME)
	{
		return;
	}
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(fashion->itemID);
	if (0 != conf->TimeLimit)
	{
		UINT32 nowtime = (UINT32)GameTime::GetTime();
		fashion->timeleft = conf->TimeLimit;
		fashion->timeend = conf->TimeLimit + nowtime;
		// 通知客户端		
		NotifyAddItem(conf->ItemID);
		own_display_items_.insert(conf->ItemID);
	}
}

void RoleFashion::UpdateAll(UINT32 nowtime, bool isnotify)
{
	std::map<UINT64, UINT32> bodys;
	std::vector<UINT64> bags;
	std::vector<UINT32> fashion_ids;
	for (int i = 0; i < FashionCount; ++i)	
	{
		if (NULL != m_FashionOnBody[i])
		{
			if (UpdateOne(m_FashionOnBody[i], nowtime))
			{
				FashionTimeout(m_FashionOnBody[i]);

				bodys[m_FashionOnBody[i]->uid] = i;

				TItemFlowLog oLog1(m_pRole);
				oLog1.m_nItemID = m_FashionOnBody[i]->itemID;
				oLog1.m_nType = FASHION;
				oLog1.m_nCount = (INT32)1;
				oLog1.m_nAfterCount = 0;
				oLog1.m_nReason = ItemFlow_Fashion;
				oLog1.m_nSubReason = ItemFlow_Fashion_TimeOut;
				oLog1.m_nMoneyCost = 0;
				oLog1.m_nMoneyType = 0;
				oLog1.m_nAddOrReduce = TX_REDUCE;
				oLog1.Do();

				UINT32 fashion_id =  m_FashionOnBody[i]->itemID;

				delete m_FashionOnBody[i];
				m_FashionOnBody[i] = NULL;

				// 身上失效的，需要检查
				fashion_ids.push_back(fashion_id);
			}
		}		
	}
	for (auto j = m_Fashions.begin(); j != m_Fashions.end();)
	{
		if (UpdateOne(j->second, nowtime))
		{
			FashionTimeout(j->second);

			TItemFlowLog oLog1(m_pRole);
			oLog1.m_nItemID = j->second->itemID;
			oLog1.m_nType = FASHION;
			oLog1.m_nCount = (INT32)1;
			oLog1.m_nAfterCount = 0;
			oLog1.m_nReason = ItemFlow_Fashion;
			oLog1.m_nSubReason = ItemFlow_Fashion_TimeOut;
			oLog1.m_nMoneyCost = 0;
			oLog1.m_nMoneyType = 0;
			oLog1.m_nAddOrReduce = TX_REDUCE;
			oLog1.Do();

			UINT32 fashion_id =  j->second->itemID;

			bags.push_back(j->first);
			delete j->second;
			j->second= NULL;
			m_Fashions.erase(j++);
			// 时装背包失效的，需要检查
			fashion_ids.push_back(fashion_id);
		}
		else
		{
			++j;
		}
	}
	if (!bodys.empty() && isnotify)
	{
		PtcG2C_FashoinChangedNtf oPtc;
		oPtc.m_Data.set_changetype(KKSG::DELBODY_FASHION);
		for (auto i = bodys.begin(); i != bodys.end(); ++i)
		{
			KKSG::FashionData* fashion = oPtc.m_Data.add_fashion();
			fashion->set_uid(i->first);
			fashion->set_itemid(0);
			fashion->set_level(0);
			fashion->set_pos(0);
			fashion->set_timeleft(0);
		}
		m_pRole->Send(oPtc);
		if (NULL != m_pRole->GetCurrScene())
		{
			/*PtcG2C_ChangeFashionNotify ntf;
			ntf.m_Data.set_roleid(m_pRole->GetID());
			for (auto i = bodys.begin(); i != bodys.end(); ++i)
			{
			ntf.m_Data.add_newitemid(0);
			ntf.m_Data.add_position(i->second);
			}
			m_pRole->GetCurrScene()->Broadcast(ntf);*/
			// recalculate in hall
			if (m_pRole->GetCurrScene()->GetSceneType() == KKSG::SCENE_HALL)
			{
				m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Fashion);
			}
		}
	}
	if (!bags.empty() && isnotify)
	{
		PtcG2C_FashoinChangedNtf oPtc;
		oPtc.m_Data.set_changetype(KKSG::DELBAG_FASHION);
		for (auto i = bags.begin(); i != bags.end(); ++i)
		{
			KKSG::FashionData* fashion = oPtc.m_Data.add_fashion();
			fashion->set_uid(*i);
			fashion->set_itemid(0);
			fashion->set_level(0);
			fashion->set_pos(0);
			fashion->set_timeleft(0);
		}
		m_pRole->Send(oPtc);
	}

	if (fashion_ids.size() > 0)
	{
		UpdateDisplayFashion(fashion_ids);
	}
}

bool RoleFashion::UpdateOne(FashionInfo* fashion, UINT32 nowtime)
{
	if (fashion->timeleft == INVALID_FASHION_TIME)
	{
		return false;
	}
	fashion->timeleft = fashion->timeend > nowtime ? (fashion->timeend - nowtime) : 0;
	m_isModifed.Set();
	return fashion->timeleft == 0;
}

void RoleFashion::FashionTimeout(FashionInfo* fashion)
{
	// notice
	Notice notice(FashionTimeoutNotice);
	XItem item(0, fashion->itemID, 1, FASHION);
	notice.Add(&item);
	notice.SendSelf(m_pRole);

	// mail
	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_FASHIONTIMEOUT);
	if (conf)
	{
		MailSender sender;
		const std::string itemname = ItemConfig::Instance()->GetItemName(fashion->itemID, ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession()));
		std::string mailcontent = MailConfig::Instance()->ReplaceString("c", itemname, conf->m_content);
		sender.SetContent(Mail_System, conf->m_title,  mailcontent);
		sender.Send(m_pRole->GetID());
	}
}

const FashionBrief RoleFashion::GetFashion()
{
	FashionBrief brief;
	for (int i = 0; i < FashionCount; ++i)
	{
		if (NULL == m_FashionOnBody[i])
		{
			brief.SetFashionPos(i, 0);	
			continue;
		}
		else
		{
			brief.SetFashionPos(i, m_FashionOnBody[i]->itemID);	
		}		
	}

	return brief;
}

const FashionBrief RoleFashion::GetWeddingFashion()
{
	FashionBrief brief;
	const ProfessionTable::RowData* pTable = XEntityInfoLibrary::Instance()->TemplateID2Data_Role(m_pRole->GetProfession());
	if (pTable)
	{
		for (auto it = pTable->WeddingFashion.begin(); it != pTable->WeddingFashion.end(); ++it)
		{
			FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(*it);
			if (pConf)
			{
				brief.SetFashionPos(pConf->EquipPos, *it);
			}
		}
	}
	return brief;
}

void RoleFashion::GetAllFashionItemID(std::set<UINT32>& fashion)
{
	fashion.clear();
	for(auto i = m_Fashions.begin(); i != m_Fashions.end(); i++)
	{
		if (i->second->timeend != i->second->timeleft)
		{
			fashion.insert(i->second->itemID);
		}
	}

	for(UINT32 i = 0; i < FashionCount; i++)
	{
		if(m_FashionOnBody[i] != NULL)
		{
			fashion.insert(m_FashionOnBody[i]->itemID);
		}
	}
}

void RoleFashion::GetAllFashionInfo(std::vector<FashionInfo*>& fashion)
{
	fashion.clear();
	for(auto i = m_Fashions.begin(); i != m_Fashions.end(); i++)
	{
		if (i->second)
		{
			fashion.push_back(i->second);
		}
	}

	for(UINT32 i = 0; i < FashionCount; i++)
	{
		if(m_FashionOnBody[i])
		{
			fashion.push_back(m_FashionOnBody[i]);
		}
	}
}


KKSG::ErrorCode RoleFashion::DisplayFashionWear(UINT32 item_id)
{
	if (!CheckFashionDisplaySystemOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	auto it = own_display_items_.find(item_id);
	if (it == own_display_items_.end())
	{
		return  KKSG::ERR_FASHION_NOT_EXIST;
	}
	// 是否存在这个有外显的物品
	UINT32 pos = 0;
	FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(item_id);
	if (pConf == NULL )
	{		
		//// 那可能是装备
		//EquipConf *peqpConf = ItemConfig::Instance()->GetEquipConf(item_id);
		//if (peqpConf == NULL || !ValidPos(peqpConf->EquipPos))
		//{
		//	return KKSG::ERR_FASHION_NOT_EXIST;
		//}	
		//pos = peqpConf->EquipPos;
		//// 装备的位置非空，需要脱掉，如果是套装，则全部脱掉
		//auto exist_equip_it = display_fashion_.find(pos);
		//if (exist_equip_it != display_fashion_.end())
		//{
		//	// 先脱掉装备
		//	UINT32 prof  = ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession());
		//	int suit_id = ItemConfig::Instance()->FindEquipSuit(prof, exist_equip_it->second);
		//	if (suit_id != 0)
		//	{
		//		std::map<UINT32,UINT32> items;
		//		GetSuitFashinonItemID(suit_id,items);
		//		for (auto it = items.begin(); it != items.end(); ++it)
		//		{
		//			display_fashion_.erase(it->first);
		//		}		
		//	}
		//	else
		//	{
		//		// 先脱
		//		display_fashion_.erase(pos);
		//	}
		//}
		return  KKSG::ERR_FASHION_NOT_EXIST;
	}
	else
	{
		pos = pConf->EquipPos;
		auto exist_equip_it = display_fashion_.find(pos);
		if (exist_equip_it != display_fashion_.end())
		{
			UINT32 suit_id = FashionMgr::Instance()->GetFashionSuitID(exist_equip_it->second);		
			if ( 0 != suit_id)
			{
				FashionSuitTable::RowData* suit_conf = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
				if (NULL != suit_conf)
				{
					if ( 1 == suit_conf->OverAll)
					{			
						//是套装，不能混搭，先把套装的位置清空
						std::map<UINT32,UINT32> items;
						GetSuitFashinonItemID(suit_id,items);
						for (auto it = items.begin(); it != items.end(); ++it)
						{
							display_fashion_.erase(it->first);
						}	
					}
					else
					{
						display_fashion_.erase(pos);
					}
				}
			}
			else
			{
				display_fashion_.erase(pos);
			}
		}

		UINT32 suit_id = FashionMgr::Instance()->GetFashionSuitID(item_id);		
		if ( 0 != suit_id)
		{
			FashionSuitTable::RowData* suit_conf = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
			if (NULL != suit_conf)
			{
				if ( 1 == suit_conf->OverAll)
				{			
					//是套装，不能混搭，先把套装的位置清空
					std::map<UINT32,UINT32> items;
					GetSuitFashinonItemID(suit_id,items);
					for (auto it = items.begin(); it != items.end(); ++it)
					{
						display_fashion_.erase(it->first);
					}	
				}
			}
		}		
	}

	// 再穿
	display_fashion_.insert(std::make_pair(pos,item_id));

	// 默认颜色
	UINT32 color_id = FashionMgr::Instance()->GetDefaultHairColor(item_id);
	if (color_id > 0)
	{
		hair_color_id_ = color_id;
		// 修改头发颜色列表
		auto hair_it = hair_color_list_.find(item_id);
		if (hair_it != hair_color_list_.end())
		{
			(hair_it->second).insert(color_id);
		}
		else
		{
			hair_color_list_[item_id].insert(color_id);
		}
	}

	m_isModifed.Set();

	NotifyDisplayFashion();	

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode RoleFashion::DisplaySuitFashionWear(UINT32 suit_id)
{
	if (!CheckFashionDisplaySystemOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	std::map<UINT32,UINT32> items;
	// 如果是整套才能穿的套装，需要判断下
	FashionSuitTable::RowData* conf = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
	if (NULL != conf)
	{
		if (conf->OverAll == 1)
		{			
			for (auto it = conf->FashionID.begin(); it != conf->FashionID.end(); ++it )
			{
				auto exist_it = own_display_items_.find(*it);
				if (exist_it == own_display_items_.end())
				{
					// 件数不够
					return  KKSG::ERR_FASHION_ITEM_NOT_ENOUGH;
				}
			}			
		}

		GetSuitFashinonItemID(suit_id,items);
	}
	else
	{
		// 那就是装备套装
		EquipSuitConf* eq_conf = ItemConfig::Instance()->GetEquipSuitConf(suit_id);
		if(eq_conf == NULL)
		{		
			return KKSG::ERR_ITEM_NOTEXIST;
		}

		auto it = eq_conf->EquipID.begin();
		auto it_end = eq_conf->EquipID.end();
		for (; it != it_end; ++it)
		{
			EquipConf *peqpConf = ItemConfig::Instance()->GetEquipConf(*it);
			if (peqpConf == NULL || !ValidPos(peqpConf->EquipPos))
			{
				continue;
			}	
			// 没有外显的也不穿
			FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(*it);
			if (NULL == pConf)
			{
				continue;
			}
			items.insert(std::make_pair(peqpConf->EquipPos,peqpConf->ItemID));
		}
	}

	// 穿戴的时候，如果要穿戴的位置有整套时装占着，则需要脱掉也有的整套
	for (auto need_wear_it = items.begin(); need_wear_it != items.end(); ++need_wear_it)
	{
		// 这一套可能不齐，但是有的套装允许这样子穿
		auto exist = own_display_items_.find(need_wear_it->second);
		if (exist == own_display_items_.end())
		{
			continue;
		}
		// 检查要脱的位置，如果空，直接穿，非空，则看看是否套装，是否需要整套脱
		auto wear_fashion_it = display_fashion_.find(need_wear_it->first);
		if (wear_fashion_it != display_fashion_.end())
		{
			UINT32 temp_suit_id = FashionMgr::Instance()->GetFashionSuitID(wear_fashion_it->second);		
			if ( 0 != temp_suit_id)
			{
				// 整套，检查是否需要整体处理
				FashionSuitTable::RowData* suit_conf = FashionMgr::Instance()->GetFashionSuitConf(temp_suit_id);
				if (NULL != suit_conf)
				{
					if (suit_conf->OverAll == 1)
					{
						// 本套全脱
						for (auto suit_item_it = suit_conf->FashionID.begin(); suit_item_it != suit_conf->FashionID.end(); ++suit_item_it )
						{
							FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(*suit_item_it);
							if (NULL != pConf )
							{
								display_fashion_.erase(pConf->EquipPos);
							}
						}
					}				
					else
					{
						//就算是套装，不需要整套穿，也可以直接脱
						display_fashion_.erase(need_wear_it->first);
					}
				}
				else
				{
					// 不是时装套装，直接脱
					display_fashion_.erase(need_wear_it->first);
				}
			} 
			else
			{
				// 不是整套的情况，直接脱
				display_fashion_.erase(need_wear_it->first);
			}
		}

		display_fashion_.insert(std::make_pair(need_wear_it->first,need_wear_it->second));
		m_isModifed.Set();
	}

	NotifyDisplayFashion();
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode RoleFashion::DisplayFashionTakeOff(UINT32 item_id)
{
	if (!CheckFashionDisplaySystemOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	auto exist_it = own_display_items_.find(item_id);
	if (exist_it == own_display_items_.end())
	{
		return  KKSG::ERR_FASHION_NOT_EXIST;
	}

	UINT32 pos = 0;
	FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(item_id);
	if (NULL == pConf)
	{
		// 那可能是装备
		EquipConf *peqpConf = ItemConfig::Instance()->GetEquipConf(item_id);
		if (peqpConf == NULL || !ValidPos(peqpConf->EquipPos))
		{
			return KKSG::ERR_FASHION_NOT_EXIST;
		}	
		pos = peqpConf->EquipPos;		
	}
	else
	{
		pos = pConf->EquipPos;
	}

	auto it = display_fashion_.find(pos);
	if (it != display_fashion_.end())
	{
		// 如果是头发，颜色清理掉
		UINT32 color_id = FashionMgr::Instance()->GetDefaultHairColor(item_id);
		if (color_id > 0)
		{
			hair_color_id_ = 0;
		}
		display_fashion_.erase(pos);	

		m_isModifed.Set();
	}

	NotifyDisplayFashion();

	return KKSG::ERR_SUCCESS;
}
KKSG::ErrorCode RoleFashion::DisplaySuitFashionTakeOff(UINT32 suit_id)
{
	if (!CheckFashionDisplaySystemOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	std::map<UINT32,UINT32> items;
	GetSuitFashinonItemID(suit_id,items);

	for (auto it = items.begin(); it != items.end(); ++it)
	{
		auto exist = own_display_items_.find(it->second);
		if (exist == own_display_items_.end())
		{
			continue;
		}
		auto wear_it = display_fashion_.find(it->first);
		if (wear_it != display_fashion_.end())
		{
			if (wear_it->second  ==  it->second)
			{
				display_fashion_.erase(it->first);
				m_isModifed.Set();
			}
		}
	}

	NotifyDisplayFashion();

	return KKSG::ERR_SUCCESS;
}


//	外显通知
void RoleFashion::NotifyDisplayFashion()
{	
	m_pRole->OnDisplayFashion();
	PtcG2C_UpdateDisplayItems ptc;
	for (auto it = display_fashion_.begin(); it != display_fashion_.end(); ++it)
	{
		ptc.m_Data.add_display_items(it->second);
	}
	ptc.m_Data.set_hair_color_id(hair_color_id_);
	ptc.m_Data.set_special_effects_id(special_effects_suitid_);
	m_pRole->Send(ptc);

	RoleOutLook outlook(m_pRole);
	outlook.Fill(SUMMARY_DISPLAYFASHION, m_pRole);
	outlook.BroadCast();	
}


void RoleFashion::AddOwnFashions(UINT32 item_id)
{
	ItemConf *conf = ItemConfig::Instance()->GetItemConf(item_id);
	if (NULL == conf )
	{
		return;
	}

	m_isModifed.Set();

	//永久时装
	if (0 == conf->TimeLimit)
	{
		// 永久的未必都是套装
		AddOwnFashionID(item_id);	
		UINT32 suit_id = FashionMgr::Instance()->GetSuitIdForCharm(item_id);
		if (0 == suit_id )
		{
			return ;
		}

		auto it = own_fashins_.begin();
		auto it_end = own_fashins_.end();
		for (; it != it_end; ++it)
		{
			if (it->suit_id == suit_id)
			{
				it->items.insert(item_id);
				return;
			}		
		}	

		bool need =  FashionMgr::Instance()->NeedActiveFashionSuit(suit_id);
		if (need)
		{
			SuitDisplayStore store;
			store.suit_id = suit_id;	
			store.items.insert(item_id);
			own_fashins_.push_back(store);
		}				
	}

	return;
}

void RoleFashion::AddFashionForCharm(UINT32 item_id)
{
	UINT32 suit_id = FashionMgr::Instance()->GetSuitIdForCharm(item_id);
	if (0 == suit_id )
	{
		return ;
	}	

	bool exist = false;
	for (auto it = own_fashins_.begin(); it != own_fashins_.end(); ++it)
	{
		if (it->suit_id == suit_id)
		{
			it->items.insert(item_id);
			exist = true;
			break;
		}
	}

	if (!exist)
	{
		SuitDisplayStore store;
		store.suit_id = suit_id;	
		store.items.insert(item_id);
		own_fashins_.push_back(store);
	}

	return ;
}

void RoleFashion::AddOwnFashionID(UINT32 item_id)
{
	auto it = own_display_items_.find(item_id);
	if (it != own_display_items_.end())
	{
		return;
	}
	own_display_items_.insert(item_id);
	m_isModifed.Set();	
	NotifyAddItem(item_id);

	// 如果是装备，且是套装，以便可以激活
	UINT32 prof  = ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession());
	int suit_id = ItemConfig::Instance()->FindEquipSuit(prof, item_id);
	if (suit_id != 0)
	{
		bool exist = false;
		auto exist_it = own_fashins_.begin();
		auto exist_it_end = own_fashins_.end();
		for (; exist_it != exist_it_end; ++exist_it)
		{
			if (exist_it->suit_id == suit_id)
			{
				// 存在就加进去
				exist_it->items.insert(item_id);
				exist = true;
			}
		}
		if (!exist)
		{
			SuitDisplayStore stroe;
			stroe.suit_id = suit_id;
			stroe.items.insert(item_id);
			own_fashins_.push_back(stroe);
		}
	}
}

bool RoleFashion::CheckEquipNeedForProfession(UINT32 item_id)
{
	// 1.物品是否存在
	// 2.职业是否匹配
	ItemConf* pConf = ItemConfig::Instance()->GetItemConf(item_id);
	if (pConf == NULL)
	{
		return false ;
	}

	if ( pConf->Profession > 0)
	{
		UINT32 prof  = ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession());
		if (pConf->Profession != prof)
		{
			return false;
		}
	}

	return true;
}
void RoleFashion::AddEquipItem(UINT32 item_id)
{
	if (CheckEquipNeedForProfession(item_id))
	{
		AddOwnFashionID(item_id);	
	}
}

UINT32  RoleFashion::ActiveFashionCharm(UINT32 suit_id,KKSG::ActivateFashionRes &roRes)
{
	/*UINT32 level = FashionMgr::Instance()->GetSuitActiveLevel(suit_id);
	if ((UINT32)m_pRole->GetLevel() <= level)
	{
	roRes.set_result(KKSG::ERR_ITEM_LEVELLIMIT);
	return 0;
	}*/
	if (!CheckFashionDisplaySystemOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	UINT32 exist_count = 0;
	auto it = own_fashins_.begin();
	auto it_end = own_fashins_.end();
	for (; it != it_end; ++it)
	{
		if (it->suit_id == suit_id)
		{			
			exist_count = it->items.size();
			UINT32 real_count =  FashionMgr::Instance()->GetActiveCount(suit_id,exist_count);

			it->count = real_count;
			m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Fashion);

			m_isModifed.Set();	

			roRes.set_result(KKSG::ERR_SUCCESS);
			DoTxLog(suit_id, real_count);
			return real_count;
		}		
	}	


	roRes.set_result(KKSG::ERR_FAILED);
	return 0;
}


void RoleFashion::UpdateDisplayFashion(std::vector<UINT32>& fashion_id)
{
	//1.是否穿了这衣服
	// 2. 背包和身上是否还有这个衣服
	std::map<UINT32,UINT32> retain_item;

	std::set<UINT32> fashion_item_id;
	GetAllFashionItemID(fashion_item_id);
	bool need_notify = false;
	for (auto it = fashion_id.begin(); it != fashion_id.end(); ++it)
	{
		bool need_delete = true;
		for (auto exist = display_fashion_.begin(); exist != display_fashion_.end(); ++exist)
		{
			// 如果身上还有，就不用删
			auto have_it = fashion_item_id.find(*it);
			if (have_it != fashion_item_id.end())
			{
				need_delete = false;
				retain_item.insert(std::make_pair(*it,exist->first));
				continue;
			}

			if (exist->second == *it)
			{
				display_fashion_.erase(exist->first);
				own_display_items_.erase(*it);
				// 如果是头发，还要清理颜色
				UINT32 hair_color_id = FashionMgr::Instance()->GetDefaultHairColor(*it);
				if (hair_color_id > 0)
				{
					hair_color_id_ = 0;
				}
				// 通知有减少
				NotifyDelItem(*it);
				CheckTakeOffFashion(*it);
				need_delete = false;
				need_notify = true;
				break;
			}
		}
		// 如果没穿在身上，
		if (need_delete)
		{
			// 且背包里没有另外一件，也要删除
			auto have_more_it = fashion_item_id.find(*it);
			if (have_more_it == fashion_item_id.end())
			{
				own_display_items_.erase(*it);
				NotifyDelItem(*it);
			}
		}
	}

	// 检查是否是套装，如果是套装，且整套的，脱一件，则全部脱
	for (auto it = retain_item.begin(); it != retain_item.end(); ++it)
	{
		UINT32 suit_id = FashionMgr::Instance()->GetFashionSuitID(it->first);		
		if ( 0 != suit_id)
		{
			FashionSuitTable::RowData* conf = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
			if (NULL != conf)
			{
				for (auto fashion_it = conf->FashionID.begin(); fashion_it != conf->FashionID.end(); ++fashion_it )
				{
					auto exist_it = own_display_items_.find(*fashion_it);
					if (exist_it == own_display_items_.end())
					{
						// 件数不够
						need_notify = true;
						display_fashion_.erase(it->second);
					}
				}
			}
		}
	}

	if (need_notify)
	{
		NotifyDisplayFashion();
	}
}

void RoleFashion::GetDisplayFashion(KKSG::OutLookDisplayFashion *display_fashion)
{
	// 设置的外显
	auto it = display_fashion_.begin();
	auto it_end = display_fashion_.end();
	for ( ;it != it_end; ++it)
	{
		display_fashion->add_display_fashions(it->second);
	}
	if (display_fashion->display_fashions_size() < 1)
	{
		display_fashion->add_display_fashions(0);
	}
	display_fashion->set_hair_color_id(hair_color_id_);
	display_fashion->set_special_effects_id(special_effects_suitid_);
}

void RoleFashion::GetWeddingDisplayFashion(KKSG::OutLookDisplayFashion* display_fashion)
{
	const ProfessionTable::RowData* pTable = XEntityInfoLibrary::Instance()->TemplateID2Data_Role(m_pRole->GetProfession());
	if (pTable)
	{
		for (auto it = pTable->WeddingFashion.begin(); it != pTable->WeddingFashion.end(); ++it)
		{
			FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(*it);
			if (pConf)
			{
				display_fashion->add_display_fashions(*it);
				if (pConf->EquipPos == FashionHeadgear)
				{
					display_fashion->set_hair_color_id(FashionMgr::Instance()->GetDefaultHairColor(*it));
				}
			}
		}
	}
}

void RoleFashion::SetInitDisplayFashion()
{	
	// 有没有穿东西
	/*if (display_fashion_.size() > 0)
	{
	return;
	}*/

	if (!CheckFashionDisplaySystemOpen())
	{
		return;
	}
	// 1.装备
	/*for (int i = 0; i < EquipCount; ++i)	
	{
	XItem*  equip = m_pRole->Get<Bag>()->GetItemOnBody(EQUIP, i);			
	if (NULL != equip && equip->type == EQUIP)
	{
	FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(equip->itemID);
	if (NULL == pConf)
	{
	continue;
	}
	display_fashion_.insert(std::make_pair(i,equip->itemID));
	}
	}*/

	// 2.时装
	for (auto i = 0; i < FashionCount; ++i)
	{
		if (NULL != m_FashionOnBody[i])
		{
			FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(m_FashionOnBody[i]->itemID);
			if (pConf == NULL || !ValidPos(pConf->EquipPos))
			{
				continue;
			}

			display_fashion_.insert(std::make_pair(pConf->EquipPos,pConf->ItemID));
		}
	}

	//	NotifyDisplayFashion();
}

void  RoleFashion::GetDispplayFashion(std::set<UINT32>& fashions)
{
	// 设置的外显
	/*for (auto i = 0; i < FashionCount; ++i)
	{
	auto it = display_fashion_.find(i);
	if ( it != display_fashion_.end())
	{
	fashions.insert(it->second);
	}
	else
	{
	fashions.insert(0);
	}
	}*/

	auto it = display_fashion_.begin();
	auto it_end = display_fashion_.end();
	for (; it != it_end; ++it)
	{
		fashions.insert(it->second);
	}
	if (fashions.size() < 1)
	{
		fashions.insert(0);
	}
}

UINT32  RoleFashion::GetDispplayHairColor()
{
	return  hair_color_id_;
}

void RoleFashion::NotifyAddItem(UINT32 item_id)
{
	PtcG2C_DisplayAddItem oPtc;
	oPtc.m_Data.set_add_item_id(item_id);
	m_pRole->Send(oPtc);
}

void RoleFashion::NotifyDelItem(UINT32 item_id)
{
	PtcG2C_DisplayAddItem oPtc;
	oPtc.m_Data.set_del_item_id(item_id);
	m_pRole->Send(oPtc);
}

void RoleFashion::GetSuitFashinonItemID(UINT32 suit_id,std::map<UINT32,UINT32>& items)
{	
	FashionSuitTable::RowData* conf = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
	if (NULL == conf)
	{
		// 那就是装备套装
		EquipSuitConf* eq_conf = ItemConfig::Instance()->GetEquipSuitConf(suit_id);
		if(eq_conf == NULL)
		{		
			return ;
		}

		auto it = eq_conf->EquipID.begin();
		auto it_end = eq_conf->EquipID.end();
		for (; it != it_end; ++it)
		{
			EquipConf *peqpConf = ItemConfig::Instance()->GetEquipConf(*it);
			if (peqpConf == NULL || !ValidPos(peqpConf->EquipPos))
			{
				continue;
			}	
			items.insert(std::make_pair(peqpConf->EquipPos,peqpConf->ItemID));
		}
	}
	else
	{
		auto it = conf->FashionID.begin();
		auto it_end = conf->FashionID.end();
		for (; it != it_end; ++it)
		{
			FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(*it);
			if (NULL == pConf || !ValidPos(pConf->EquipPos))
			{
				continue;
			}
			items.insert(std::make_pair(pConf->EquipPos,pConf->ItemID));
		}
	}

	return ;
}


KKSG::ErrorCode RoleFashion::ActivationFashion(UINT64 uid)
{
	auto i = m_Fashions.find(uid);
	if (i == m_Fashions.end())
	{
		return KKSG::ERR_FASHION_NOT_EXIST;
	}
	FashionInfo* fashion = i->second;
	FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(i->second->itemID);
	if (pConf == NULL)
	{
		return KKSG::ERR_FASHION_NOT_EXIST;
	}

	OpenTimeLimit(fashion);

	NotifyChangeSelf(KKSG::UPGRADE_FASHION, m_pRole, fashion, false);

	m_isModifed.Set();	

	return KKSG::ERR_SUCCESS;
}



void RoleFashion::CheckTakeOffFashion(UINT32 item_id)
{
	// 脱掉一件，如果是套装，没有其他能顶上来，其他位置也要脱掉
	UINT32 suit_id = FashionMgr::Instance()->GetFashionSuitID(item_id);		
	if ( 0 != suit_id)
	{
		FashionSuitTable::RowData* conf = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
		if (NULL != conf && 1 == conf->OverAll)
		{
			for (auto it = conf->FashionID.begin(); it != conf->FashionID.end(); ++it)
			{
				FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(*it);
				if (NULL != pConf )
				{
					display_fashion_.erase(pConf->EquipPos);
				}

				m_isModifed.Set();
			}
		}
	}
}

void RoleFashion::ChangeProfession(UINT32 from, UINT32 to)
{
	std::unordered_set<UINT32> tempitems;
	for (auto i = own_display_items_.begin(); i != own_display_items_.end(); ++i)
	{
		UINT32 destitemid = ItemConfig::Instance()->ChangeProfession(*i, from, to);
		tempitems.insert(destitemid);
	}
	own_display_items_ = tempitems;

	std::unordered_map<UINT32,UINT32> tempowns;
	for (auto j = display_fashion_.begin(); j != display_fashion_.end(); ++j)
	{
		UINT32 destitemid = ItemConfig::Instance()->ChangeProfession(j->second, from, to);
		tempowns[j->first] = destitemid;
	}
	display_fashion_ = tempowns;

	std::vector<SuitDisplayStore> charm_fashins;
	for (auto it = own_fashins_.begin(); it != own_fashins_.end(); ++it)
	{
		SuitDisplayStore store;	
		UINT32 suit_id = 0;
		store.count = it->count;
		for (auto exist_it = it->items.begin(); exist_it != it->items.end(); ++exist_it)
		{
			UINT32 destitemid = ItemConfig::Instance()->ChangeProfession(*exist_it, from, to);
			store.items.insert(destitemid);
			UINT32 temp_suit_id = FashionMgr::Instance()->GetSuitIdForCharm(destitemid);
			if (0 != temp_suit_id )
			{
				suit_id =  temp_suit_id;
			}
		}
		store.suit_id = suit_id;	
		charm_fashins.push_back(store);
	}
	own_fashins_.clear();
	own_fashins_ = charm_fashins;

	m_isModifed.Set();
}


void  RoleFashion::ActivationHairColor(UINT32 hair_id,UINT32 hair_color_id,KKSG::ActivateHairColorRes &roRes)
{
	std::unordered_set<UINT32> colors;
	FashionMgr::Instance()->GetColorByHair(hair_id,colors);

	auto need_it = colors.find(hair_color_id);
	if (need_it == colors.end())
	{
		// 错误的颜色
		roRes.set_result(KKSG::ERR_FASHION_ERROR_COLOR);
		return;
	}

	auto it = hair_color_list_.find(hair_id);
	if (it != hair_color_list_.end())
	{
		auto exist_it = it->second.begin();
		auto exist_it_end = it->second.end();
		for (; exist_it != exist_it_end; ++exist_it)
		{
			if (hair_color_id == *exist_it)
			{
				// 得使用了这个头发才能换色
				bool use_hair = false;
				for (auto display_it = display_fashion_.begin(); display_it != display_fashion_.end(); ++display_it )
				{
					if (hair_id == display_it->second)
					{
						use_hair = true;
					}
				}
				if (!use_hair)
				{
					// 没有头发缺换颜色，非法的操作
					roRes.set_result(KKSG::ERR_INVALID_REQUEST);
					return;;
				}

				// 已经解锁了这种颜色,直接换颜色				
				roRes.set_result(UseHairColor(hair_id,hair_color_id));
				roRes.set_hair_id(hair_id);

				auto color_it = hair_color_list_.find(hair_id);
				if (color_it != hair_color_list_.end())
				{
					for (auto exist_it = (color_it->second).begin(); exist_it != (color_it->second).end(); ++exist_it)
					{
						roRes.add_hair_colorid_list(*exist_it);
					}
					// 换颜色成功后通知
					NotifyDisplayFashion();
				}

				return;
			}
		}
	}
	else
	{
		// 还没有这个头发
		auto it = own_display_items_.find(hair_id);
		if (it == own_display_items_.end())
		{
			roRes.set_result(KKSG::ERR_FASHION_NO_HAIR);
			return;
		}
	}

	std::vector<ItemDesc> items;
	FashionMgr::Instance()->GetHairColorCost(hair_id,hair_color_id,items);
	if (items.size() > 0)
	{
		BagTakeItemTransition transition(m_pRole);
		transition.SetReason(ItemFlow_Fashion,ItemFlow_FlowerActivityDayRankAward);
		auto it_desc = items.begin();
		auto it_desc_end = items.end();
		for (; it_desc != it_desc_end; ++it_desc)
		{
			if (!transition.TakeItem(it_desc->itemID,it_desc->itemCount))
			{
				transition.RollBack();
				roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
				return;
			}

			SSInfo<<"itemid:"<<it_desc->itemID<<" count:"<<it_desc->itemCount<<END;
		}
		transition.NotifyClient();			
	}


	hair_color_list_[hair_id].insert(hair_color_id);
	roRes.set_result(KKSG::ERR_SUCCESS);
	roRes.set_hair_id(hair_id);

	auto color_it = hair_color_list_.find(hair_id);
	if (color_it != hair_color_list_.end())
	{
		for (auto exist_it = (color_it->second).begin(); exist_it != (color_it->second).end(); ++exist_it)
		{
			roRes.add_hair_colorid_list(*exist_it);
		}
	}

	UINT32 pos = 0;
	FashionConf *pConf = ItemConfig::Instance()->GetFashionConf(hair_id);
	if (pConf == NULL )
	{
		roRes.set_result(KKSG::ERR_FASHION_NOT_EXIST);
		return;
	}

	display_fashion_.erase(pConf->EquipPos);
	display_fashion_.insert(std::make_pair(pConf->EquipPos,pConf->ItemID));
	hair_color_id_ = hair_color_id;

	m_isModifed.Set();

	// 换颜色成功后通知
	NotifyDisplayFashion();

	DoHairTxLog(hair_id, hair_color_id);
	return ;
}


KKSG::ErrorCode RoleFashion::UseHairColor(UINT32 hari_id,UINT32 hair_color_id)
{
	auto it = hair_color_list_.find(hari_id);
	if (it == hair_color_list_.end())
	{
		return KKSG::ERR_FASHION_NO_HAIR;			 
	}

	auto exist_it = it->second.begin();
	auto exist_it_end = it->second.end();
	for (; exist_it != exist_it_end; ++exist_it)
	{
		if (hair_color_id == *exist_it)
		{
			hair_color_id_ = hair_color_id;
			m_isModifed.Set();
			return KKSG::ERR_SUCCESS;			
		}
	}

	return KKSG::ERR_FASHION_COLOR_LOOK;		
}

void RoleFashion::DoTxLog(UINT32 suitid, UINT32 count)
{
	TRoleChestFlow oLog(m_pRole);
	oLog.m_SuitId = suitid;
	oLog.m_RealCount = count;
	auto it = FashionMgr::Instance()->GetFashionCharmConf(suitid);
	if (it != NULL)
	{
		oLog.m_TotalCount = it->SuitParam.size();
	}
	oLog.Do();
}

void RoleFashion::DoHairTxLog(UINT32 hair_id,UINT32 hair_color_id)
{
	TRoleHairFlow oLog(m_pRole);
	oLog.m_HairId = hair_id;
	oLog.m_HairColorId = hair_color_id;
	oLog.Do();
}

bool RoleFashion::CheckFashionDisplaySystemOpen()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_FASHION_DISPLAY, m_pRole->GetID()))
	{
		return false;
	}

	if (!m_pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_FASHION_DISPLAY))
	{
		return false;
	}

	return true;
}

bool RoleFashion::HasFashion(UINT32 itemid)
{
	for (INT32 i = 0; i < FashionCount; i ++)
	{
		if (m_FashionOnBody[i])
		{
			if (itemid == m_FashionOnBody[i]->itemID)
			{
				return true;
			}
		}
	}

	for (auto it = m_Fashions.begin(); it != m_Fashions.end(); it ++)
	{
		if (it->second && it->second->itemID == itemid)
		{
			return true;
		}
	}
	return false;
}

bool RoleFashion::CheckSFashion()
{
	std::vector<FashionInfo*> fashion;
	GetAllFashionInfo(fashion);
	for (auto i = fashion.begin(); i != fashion.end(); i ++)
	{
		ItemConf* pConf = ItemConfig::Instance()->GetItemConf((*i)->itemID);
		if (pConf && pConf->ItemQuality == FashionSQuality)
		{
			return true;
		}
	}
	return false;
}

void RoleFashion::CheckASFashionSuit(bool &bHasA, bool &bHasS)
{
	std::vector<FashionInfo*> fashion;
	GetAllFashionInfo(fashion);
	for (auto i = fashion.begin(); i != fashion.end(); i ++)
	{
		if (bHasA && bHasS)
		{
			return ;
		}
		UINT32 suit_id = FashionMgr::Instance()->GetFashionSuitID((*i)->itemID);
		if (suit_id)
		{
			FashionSuitTable::RowData* pConf = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
			if (pConf && ((pConf->SuitQuality == 3 && !bHasA) || (pConf->SuitQuality == 4 && !bHasS)))
			{
				bool issuit = true;
				for (auto it = pConf->FashionID.begin(); it != pConf->FashionID.end(); it ++ )
				{
					if (!HasFashion((*it)))
					{
						issuit = false;//该套装没满整套;
						break;
					}
				}
				if (issuit)
				{
					if (pConf->SuitQuality==3)
					{
						bHasA = true;
					}
					else
					{
						bHasS = true;
					}
				}
			}
		}
	}
}

void RoleFashion::CheckHairColor()
{
	bool exist_hair = false;
	auto it = display_fashion_.begin();
	auto it_end = display_fashion_.end();
	for (; it != it_end; ++it)
	{
		// 默认颜色
		UINT32 color_id = FashionMgr::Instance()->GetDefaultHairColor(it->second);
		if (color_id > 0)
		{
			exist_hair = true;
			break;
		}
	}
	if (!exist_hair && (hair_color_id_ > 0))
	{		
		hair_color_id_ = 0;
		m_isModifed.Set();
	}
}

FashionInfo* RoleFashion::GetFashionItemByUID(UINT64 uid)
{
	auto it = m_Fashions.find(uid);
	if (it != m_Fashions.end())
	{
		return it->second;
	}

	return NULL;
}

void RoleFashion::DeleteFashionByUID(UINT64 uid)
{
	m_Fashions.erase(uid);
	PtcG2C_FashoinChangedNtf oPtc;
	oPtc.m_Data.set_changetype(KKSG::DELBAG_FASHION);

	KKSG::FashionData* fashion = oPtc.m_Data.add_fashion();
	fashion->set_uid(uid);
	fashion->set_itemid(0);
	fashion->set_level(0);
	fashion->set_pos(0);
	fashion->set_timeleft(0);

	m_pRole->Send(oPtc);

	m_isModifed.Set();
}

UINT32 RoleFashion::GetFashionSynthesisFaildCount(const UINT32 fashion_id)
{
	auto it = fashionsynthersis_fail_info_.find(fashion_id);
	if (it != fashionsynthersis_fail_info_.end())
	{
		return it->second;
	}
	return 0;
}


void RoleFashion::DeleteFashionByItemID(UINT32 item_id,UINT32 item_count)
{
	std::vector<UINT64> fashion_uids;
	fashion_uids.clear();
	UINT32 count = 0;
	for ( auto it = m_Fashions.begin(); it != m_Fashions.end(); ++it)
	{
		if (it->second->itemID == item_id)
		{
			fashion_uids.push_back(it->first);
			count += 1;
			if (count >= item_count)
			{
				break;
			}
		}		
	}

	for (auto it = fashion_uids.begin(); it != fashion_uids.end(); ++it)
	{
		DeleteFashionByUID(*it);
	}

	return ;
}


void RoleFashion::ClearFashionSynthesisByID(UINT32 fashion_id)
{
	auto it = fashionsynthersis_fail_info_.find(fashion_id);
	if (it != fashionsynthersis_fail_info_.end())
	{
		fashionsynthersis_fail_info_.erase(it);
		m_isModifed.Set();
	}
}
void RoleFashion::AddFashionSynthesisByID(UINT32 fashion_id)
{
	auto it = fashionsynthersis_fail_info_.find(fashion_id);
	if (it != fashionsynthersis_fail_info_.end())
	{
		it->second += 1;	
	}
	else
	{
		fashionsynthersis_fail_info_.insert(std::make_pair(fashion_id,1));
	}

	m_isModifed.Set();
}

UINT32 RoleFashion::GetFashionIBShopBuyTime()
{
	return fashion_ibshop_buy_time_;
}

void RoleFashion::SetFashionIBShopBuyTime(UINT32 buytime)
{
	fashion_ibshop_buy_time_ = buytime;
	m_isModifed.Set();
}


UINT32 RoleFashion::GetFashionComposeTime()
{
	return fashion_compose_time_;
}
void RoleFashion::SetFashionComposeTime(UINT32 composetime)
{
	fashion_compose_time_ = composetime;
	m_isModifed.Set();
}


void RoleFashion::ObtainSpecialEffectsID(UINT32 suit_it)
{	
	if (suit_it > 0)
	{
		auto it = special_effects_suitid_list_.find(suit_it);
		if (it == special_effects_suitid_list_.end())
		{		
			special_effects_suitid_list_.insert(suit_it);
			m_isModifed.Set();			
		}
	}
	return;
}

void RoleFashion::ChooseSpecialEffects(UINT32 suit_it,KKSG::ChooseSpecialEffectsRes& res)
{
	if (suit_it != 0)
	{
		UINT32 special_effects_id = FashionMgr::Instance()->GetSpecialEffectsIDBySuitID(suit_it);
		if (special_effects_id == 0 )
		{
			res.set_result(KKSG::ERR_FASHION_NOEXIST_EFFECTS);
			return;
		}
	}
	auto it = special_effects_suitid_list_.find(suit_it);
	if (it == special_effects_suitid_list_.end())
	{
		// 如果特效还没有激活，就激活下		
		FashionSuitTable::RowData* conf = FashionMgr::Instance()->GetFashionSuitConf(suit_it);
		if (NULL != conf)
		{
			for (auto it = conf->FashionID.begin(); it != conf->FashionID.end(); ++it )
			{
				auto exist_it = own_display_items_.find(*it);
				if (exist_it == own_display_items_.end())
				{
					// 件数不够
					res.set_result(KKSG::ERR_FASHION_NOEXIST_EFFECTS);
					return;
				}
			}
			ObtainSpecialEffectsID(suit_it);
		}		
	}

	it = special_effects_suitid_list_.find(suit_it);
	if (it == special_effects_suitid_list_.end())
	{
		res.set_result(KKSG::ERR_FASHION_NOEXIST_EFFECTS);
		return;
	}

	special_effects_suitid_ = suit_it;

	NotifyDisplayFashion();
	m_isModifed.Set();
}

UINT32 RoleFashion::GetDispplaySpecialEffectsID()
{
	return special_effects_suitid_;
}



void RoleFashion::TestAddDebugSpecialEffectsID(UINT32 special_effects_id)
{
	if (special_effects_id > 0)
	{
		special_effects_suitid_list_.insert(special_effects_id);
		m_isModifed.Set();
	}
	return;
}

void RoleFashion::AddComposeSuccessNum(UINT32 quality)
{
	++compose_success_num_;

	auto it = quality_compose_.find(quality);
	if (it != quality_compose_.end())
	{
		it->second += 1;
	}
	else
	{
		quality_compose_.insert(std::make_pair(quality,1));
	}

	m_isModifed.Set();
}

void RoleFashion::AddComposeFailedNum()
{
	++compose_failed_num_;
	m_isModifed.Set();
}


UINT32 RoleFashion::GetComposeNum()
{
	return compose_success_num_ + compose_failed_num_;
}
UINT32 RoleFashion::GetComposeSuccessNum()
{
	return compose_success_num_;
}
UINT32 RoleFashion::GetComposeFailedNum()
{
	return compose_failed_num_;
}
UINT32 RoleFashion::GetCompseSucessNumByQuality(UINT32 quality)
{
	auto it = quality_compose_.find(quality);
	if (it != quality_compose_.end())
	{
		return it->second ;
	}

	return 0;
}