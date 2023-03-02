#include "pch.h"
#include "partnershop.h"
#include "partner.h"
#include "util/gametime.h"
#include "tablemgr/partnertablemgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "partner/ptcm2c_updatepartnertoclient.h"
#include "table/globalconfig.h"
#include "table/ItemConfigMgr.h"
#include "define/itemdef.h"


PartnerShop::PartnerShop(Partner* partner)
:PartnerBase(partner)
,m_lastRefreshTime(0)
{

}

bool PartnerShop::LoadFromDB(const KKSG::PartnerSaveData& data)
{
	KKSG::PartnerShopData shop;
	if (!shop.ParseFromString(data.shop()))
	{
		LogError("parse shop error");
		return false;
	}

	m_lastRefreshTime = shop.refreshtime();
	m_item.clear();
	for (int i = 0; i < shop.item_size(); ++i)
	{
		PartnerShopItem item;
		item.FromKKSG(shop.item(i));
		m_item[item.id] = item;
	}

	m_record.clear();
	for (int i = 0; i < shop.record_size(); ++i)
	{
		PartnerShopRecord record;
		record.FromKKSG(shop.record(i));
		m_record.push_back(record);
	}

	return true;
}

void PartnerShop::SaveToDB(KKSG::PartnerSaveData& data)
{
	KKSG::PartnerShopData shop;

	shop.set_refreshtime(m_lastRefreshTime);
	shop.clear_item();
	for (auto it = m_item.begin(); it != m_item.end(); ++it)
	{
		KKSG::PartnerShopItemData& data = *shop.add_item();
		it->second.ToKKSG(data);
	}

	shop.clear_record();
	for (size_t i = 0; i < m_record.size(); ++i)
	{
		KKSG::PartnerShopRecordData& data = *shop.add_record();
		m_record[i].ToKKSG(data);
	}

	shop.SerializeToString(data.mutable_shop());
}

void PartnerShop::RefreshItem()
{
	m_item.clear();
	LogDebug("Refresh Partner Shop[%u]", m_owner->GetID());

	std::map<UINT64, PartnerMember>& members = m_owner->GetAllMember();
	std::set<UINT32> levels;
	for (auto it = members.begin(); it != members.end(); ++it)
	{
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(it->second.roleId);
		if (su == NULL)
		{
			continue;
		}
		UINT32 level = su->GetLevel();
		it->second.refreshShopLevel = level;

		// 一点小优化， 同等级的随机一次就行了
		if (levels.find(level) != levels.end())
		{
			continue;
		}
		levels.insert(level);

		std::vector<UINT32> ids;
		PartnerTableMgr::Instance()->RandomShop(level, ids);
		for (size_t i = 0; i < ids.size(); ++i)
		{
			UINT32 id = ids[i];
			if (m_item.find(id) != m_item.end())
			{
				continue;
			}
			PartnerShopItem item;		
			item.id = id;
			m_item.insert(std::make_pair(id, item));
			LogDebug("item id: [%u]", id);
		}
	}

	m_owner->SetDirty();
	/*
	m_lastRefreshTime = GameTime::GetTime();

	// 推送商店刷新
	PtcM2C_UpdatePartnerToClient ntf;
	ntf.m_Data.set_type(KKSG::PUType_Shop);
	ntf.m_Data.set_partid(m_owner->GetID());
	m_owner->BroadCast(ntf);
	SetDirty();
	*/
}

void PartnerShop::CheckRefresh()
{
	bool refresh = false;
	// 检查表格变化
	for (auto it = m_item.begin(); it != m_item.end(); ++it)
	{
		if (PartnerTableMgr::Instance()->GetPartnerShopItem(it->second.id) == NULL)
		{
			LogWarn("partner[%llu] shopid [%u] deleted, do shop refresh", m_owner->GetID(), it->second.id);
			refresh = true;
			break;
		}
	}

	if (refresh == true)
	{
		RefreshItem();
		return;
	}

	UINT32 now = GameTime::GetTime();
	UINT32 zero = GameTime::GetTodayBeginTime(false);

	auto times = PartnerTableMgr::Instance()->GetShopRefreshTime();
	for (auto it = times.begin(); it != times.end(); ++it)
	{
		UINT32 temp = *it;
		UINT32 hour =  temp / 100;
		UINT32 min = temp % 100;
		UINT32 totalSec = hour * 3600 + min * 60 + zero;
		if (totalSec > m_lastRefreshTime && totalSec < now)
		{
			RefreshItem();
			break;
		}
	}
}

void PartnerShop::OnDayPass()
{
	m_record.clear();
}

PartnerShopItem* PartnerShop::GetItem(UINT32 id)
{
	auto it = m_item.find(id);
	if (it != m_item.end())
	{
		return &it->second;
	}
	return NULL;
}

void PartnerShop::RemoveItem(UINT32 id)
{
	m_item.erase(id);
	SetDirty();
}

void PartnerShop::AddBuyRecord(UINT64 roleId, UINT32 itemId, UINT32 itemCount)
{
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su == NULL)
	{
		return;
	}
	itemId = CItemConfig::Instance()->ConvertItemId(itemId, su->GetProfession());

	PartnerShopRecord record;
	record.roleId = roleId;
	record.itemId = itemId;
	record.itemCount = itemCount;
	record.time = GameTime::GetTime();
	m_record.push_front(record);

	if (m_record.size() > GetGlobalConfig().PartnerShopRecordNum)
	{
		m_record.pop_back();
	}
	SetDirty();
}

void PartnerShop::FillShopItems(UINT64 roleId, KKSG::GetPartnerShopRes& res)
{
	PartnerMember* member = m_owner->GetMember(roleId);
	if (member == NULL)
	{
		return;
	}
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su == NULL)
	{
		return;
	}
	UINT32 level = member->refreshShopLevel;
	for (auto it = m_item.begin(); it != m_item.end(); ++it)
	{
		PartnerShopItem& item = it->second;
		ShopTable::RowData* pData = PartnerTableMgr::Instance()->GetPartnerShopItem(item.id);
		if (pData == NULL)
		{
			continue;
		}
		if (level < pData->LevelShow[0] || level > pData->LevelShow[1])
		{
			continue;
		}

		KKSG::PartnerShopItemClient* data = res.add_item();
		data->set_id(item.id);
		data->set_buy_count(item.buyCount);
		UINT32 itemId = CItemConfig::Instance()->ConvertItemId(pData->ItemId, su->GetProfession());
		data->set_itemid(itemId);
	}
}

void PartnerShop::FillShopRecords(KKSG::GetPartnerShopRecordRes& res)
{
	UINT32 now = GameTime::GetTime();
	for (size_t i = 0; i < m_record.size(); ++i)
	{
		PartnerShopRecord& record = m_record[i];
		KKSG::PartnerShopRecordItem* data = res.add_record();
		data->set_roleid(record.roleId);
		data->set_itemid(record.itemId);
		data->set_itemcount(record.itemCount);
		data->set_time(record.time);
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(record.roleId);
		if (su)
		{
			data->set_name(su->GetName());
			data->set_titleid(su->GetTitleID());
		}
	}
}

bool PartnerShop::HasRedPoint(UINT64 roleId)
{
	/*
	PartnerMember* mem = m_owner->GetMember(roleId);
	if (mem == NULL)
	{
		return false;
	}
	UINT32 lastTime = GetLastRefreshTime();
	if (mem->openShopTime < lastTime)
	{
		return true;
	}
	*/
	return false;
}