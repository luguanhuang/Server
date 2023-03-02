#include "pch.h"
#include "shoprecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "shop.h"
#include "unit/role.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

void CShopRecordOne::Clear(bool iscleardailycount)
{
	m_items.clear();
	//m_itemid2buycount.clear();
	if (iscleardailycount)
	{
		m_itemid2dailybuycount.clear();
	}
}

void CShopRecordOne::ToKKSG(KKSG::ShopRecordOne* one)
{
	one->set_ishint(ishint);
	one->set_updatetime(updatetime);
	one->set_refreshcount(refreshcount);
	one->set_refreshtime(refreshtime);
	one->set_refreshday(refreshday);
	for (auto i = m_items.begin(); i != m_items.end(); ++i)
	{
		one->add_slots(i->first);
		KKSG::Item* item = one->add_items();
		XItem::Item2KKSGItem(&i->second, item);
	}
	for (auto j = m_itemid2dailybuycount.begin(); j != m_itemid2dailybuycount.end(); ++j)
	{
		KKSG::ItemBrief* brief = one->add_dailybuycount();
		brief->set_itemid(j->first);
		brief->set_itemcount(j->second);
	}
	for (auto k = m_itemid2buycount.begin(); k != m_itemid2buycount.end(); ++k)
	{
		KKSG::ItemBrief* brief = one->add_buycount();
		brief->set_itemid(k->first);
		brief->set_itemcount(k->second);
	}
	for (auto l = m_itemid2weekbuycount.begin(); l != m_itemid2weekbuycount.end(); ++l)
	{
		KKSG::ItemBrief* brief = one->add_weekbuycount();
		brief->set_itemid(l->first);
		brief->set_itemcount(l->second);
	}
}

void CShopRecordOne::ToThisBrief(const KKSG::ShopRecordOne* one)
{
	ishint = one->ishint();
	updatetime = one->updatetime();
	refreshtime = one->refreshtime();
	refreshcount = one->refreshcount();
	refreshday = one->refreshday();
	for (int j = 0; j < one->dailybuycount_size(); ++j)
	{
		m_itemid2dailybuycount[one->dailybuycount(j).itemid()] = one->dailybuycount(j).itemcount();
	}
	for (int k = 0; k < one->buycount_size(); ++k)
	{
		m_itemid2buycount[one->buycount(k).itemid()] = one->buycount(k).itemcount();
	}
	for (int l = 0; l < one->weekbuycount_size(); ++l)
	{
		m_itemid2weekbuycount[one->weekbuycount(l).itemid()] = one->weekbuycount(l).itemcount();
	}
}

void CShopRecordOne::ToThis(const KKSG::ShopRecordOne* one)
{
	ishint = one->ishint();
	updatetime = one->updatetime();
	refreshtime = one->refreshtime();
	refreshcount = one->refreshcount();
	refreshday = one->refreshday();
	for (int i = 0; i < one->items_size(); ++i)
	{
		XItem& item = m_items[one->slots(i)];
		XItem::KKSGItem2Item(&item, &one->items(i));
	}
	for (int j = 0; j < one->dailybuycount_size(); ++j)
	{
		m_itemid2dailybuycount[one->dailybuycount(j).itemid()] = one->dailybuycount(j).itemcount();
	}
	for (int k = 0; k < one->buycount_size(); ++k)
	{
		m_itemid2buycount[one->buycount(k).itemid()] = one->buycount(k).itemcount();
	}
	for (int l = 0; l < one->weekbuycount_size(); ++l)
	{
		m_itemid2weekbuycount[one->weekbuycount(l).itemid()] = one->weekbuycount(l).itemcount();
	}
}

void CShopRecordOne::ToItems(const KKSG::ShopRecordOne* one)
{
	for (int i = 0; i < one->items_size(); ++i)
	{
		XItem& item = m_items[one->slots(i)];
		XItem::KKSGItem2Item(&item, &one->items(i));
	}
}

///////////////////////////////////////////////////////////////////////////
CShopRecord::CShopRecord(Role* role)
{
	m_updateday = 0;
	m_role = role;
}

bool CShopRecord::Load(const KKSG::RoleAllInfo* roleallinfo)
{
	if (roleallinfo->has_shoprecord())
	{
		const KKSG::ShopRecord* record = &roleallinfo->shoprecord();
		for (int i = 0; i < record->shops_size(); ++i)
		{
			const KKSG::ShopRecordOne* one = &record->shops(i);
			CShopRecordOne& cone = m_id2shop[one->type()];
			if (GetGlobalConfig().BriefShop)
			{
				cone.isbrief = true;
				cone.ToThisBrief(one);
			}
			else
			{
				cone.ToThis(one);
			}
		}
		m_updateday = record->dayupdate();
		m_weektime = record->weekupdate();

		if (!GetGlobalConfig().BriefShop)
		{
			Update();
		}
	}
	return true;
}

void CShopRecord::LoadItems(UINT32 type)
{
	if (NULL == m_role->GetRoleAllInfoPtr())
	{
		return;
	}
	KKSG::ShopRecord* record = m_role->GetRoleAllInfoPtr()->mutable_shoprecord();
	for (int i = 0; i < record->shops_size(); ++i)
	{
		if (record->shops(i).type() == type)
		{
			CShopRecordOne& cone = m_id2shop[type];
			cone.isbrief = false;
			cone.ToItems(&record->shops(i));
		}
	}
}

void CShopRecord::Save(KKSG::RoleAllInfo* roleallinfo, CDataFieldSet& roChanged)
{
	if (m_ismodify.TestAndReset())
	{
		KKSG::ShopRecord* record = roleallinfo->mutable_shoprecord();
		for (auto i = m_id2shop.begin(); i != m_id2shop.end(); ++i)
		{
			if (i->second.isbrief)
			{
				LoadItems(i->first);
			}
		}

		record->Clear();
		for (auto i = m_id2shop.begin(); i != m_id2shop.end(); ++i)
		{
			KKSG::ShopRecordOne* one = record->add_shops();
			i->second.ToKKSG(one);
			one->set_type(i->first);
		}
		record->set_dayupdate(m_updateday);
		record->set_weekupdate(m_weektime);
		roChanged.insert(record);
	}
}

void CShopRecord::Update()
{
	UINT32 nowday = XCommon::GetTodayUnique();
	if (nowday != m_updateday)
	{
		m_updateday = nowday;
		ResetDay();
		m_ismodify.Set();
	}
	time_t nowtime = GameTime::GetTime();
	if (!GameTime::IsInSameWeek(nowtime, m_weektime, true))
	{
		m_weektime = (UINT32)nowtime;
		ResetWeek();
		m_ismodify.Set();
	}

	TryRefresh();
}

void CShopRecord::ResetDay()
{
	for (auto i = m_id2shop.begin(); i != m_id2shop.end(); ++i)
	{
		i->second.refreshcount = 0;
		i->second.m_itemid2dailybuycount.clear();
	}
}

void CShopRecord::ResetWeek()
{
	for (auto i = m_id2shop.begin(); i != m_id2shop.end(); ++i)
	{
		i->second.m_itemid2weekbuycount.clear();
	}
}

std::set<UINT32> CShopRecord::TryRefresh()
{
	std::set<UINT32> shops_refreshed;
	const std::map<UINT32, ShopOneInfo>& shops = ShopMgr::Instance()->GetShops();
	for (auto i = shops.begin(); i != shops.end(); ++i)
	{
		if (ShopMgr::Instance()->IsOpen(m_role, i->first))
		{
			if (RefreshOne(i->first))
			{
				shops_refreshed.insert(i->first);
			}
		}
	}
	return shops_refreshed;
}

bool CShopRecord::RefreshOne(UINT32 type)
{
	if (ShopMgr::Instance()->IsNeedRefresh(type))
	{
		bool isnew = false;
		if (CheckRefresh(type, isnew))
		{
			Refresh(type, m_role->GetLevel(), m_role->GetProfession(), false, isnew, true);
			return true;
		}
	}
	else
	{
		if (IsEmpty(type))
		{
			Refresh(type, m_role->GetLevel(), m_role->GetProfession(), false, true, true);
			return true;
		}
	}
	return false;
}

void CShopRecord::OnFirstEnterScene()
{
	if (GetGlobalConfig().BriefShop)
	{
		Update();
	}
	for (auto i = m_id2shop.begin(); i != m_id2shop.end(); ++i)
	{
		if (i->second.ishint)
		{
			ShopMgr::Instance()->Hint(m_role, false, i->first);
		}
	}
}

CShopRecordOne* CShopRecord::GetShop(UINT32 type)
{
	auto i = m_id2shop.find(type);
	if (i == m_id2shop.end())
	{
		return NULL;
	}
	if (i->second.isbrief)
	{
		LoadItems(i->first);
	}
	return &i->second;
}

UINT32 CShopRecord::GetDailyBuyCount(UINT32 type, UINT32 uid)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		return 0;
	}
	auto i = shop->m_itemid2dailybuycount.find(uid);
	if (i == shop->m_itemid2dailybuycount.end())
	{
		return 0;
	}
	return i->second;
}

void CShopRecord::AddDailyBuyCount(UINT32 type, UINT32 uid, UINT32 count)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		CShopRecordOne one;
		one.m_itemid2dailybuycount[uid] = count;
		m_id2shop[type]	= one;
	}
	else
	{
		shop->m_itemid2dailybuycount[uid] += count;
	}
	m_ismodify.Set();
}

UINT32 CShopRecord::GetWeekBuyCount(UINT32 type, UINT32 uid)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		return 0;
	}
	auto i = shop->m_itemid2weekbuycount.find(uid);
	if (i == shop->m_itemid2weekbuycount.end())
	{
		return 0;
	}
	return i->second;
}

void CShopRecord::AddWeekBuyCount(UINT32 type,  UINT32 uid, UINT32 count)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		CShopRecordOne one;
		one.m_itemid2weekbuycount[uid] = count;
		m_id2shop[type]	= one;
	}
	else
	{
		shop->m_itemid2weekbuycount[uid] += count;
	}
	m_ismodify.Set();
}

UINT32 CShopRecord::GetBuyCount(UINT32 type, UINT32 uid)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		return 0;
	}
	auto i = shop->m_itemid2buycount.find(uid);
	if (i == shop->m_itemid2buycount.end())
	{
		return 0;
	}
	return i->second;
}

void CShopRecord::AddBuyCount(UINT32 type, UINT32 uid, UINT32 count)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		CShopRecordOne one;
		one.m_itemid2buycount[uid] = count;
		m_id2shop[type]	= one;
	}
	else
	{
		shop->m_itemid2buycount[uid] += count;
	}
	m_ismodify.Set();
}

bool CShopRecord::IsEmpty(UINT32 type)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop || shop->IsEmpty())
	{
		return true;
	}
	return false;
}

bool CShopRecord::CheckRefresh(UINT32 type, bool& isnew)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		CShopRecordOne one;
		m_id2shop[type] = one;
		shop = &m_id2shop[type];
		isnew = true;
	}
	UINT32 lefttime = 0;
	if (ShopMgr::Instance()->GetRefreshTime(type, shop->refreshday, shop->refreshtime, lefttime))
	{
		m_ismodify.Set();
		return true;
	}
	return false;
}

bool CShopRecord::Refresh(UINT32 type, UINT32 rolelevel, UINT32 roleprofession, bool isadd, bool isnew, bool iscleardailycount)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		CShopRecordOne one;
		if (isadd)
		{
			one.refreshcount++;
		}
		one.FillItems(ShopMgr::Instance()->RefreshShop(type, rolelevel, roleprofession));

		if (isnew)
		{
			if (type == Shop_Guild)
			{
				CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(m_role->GetID());
				if (NULL != guild)
				{
					one.ishint = true;
					ShopMgr::Instance()->Hint(m_role, false, type);
				}
			}
			else
			{
				one.ishint = true;
				ShopMgr::Instance()->Hint(m_role, false, type);
			}
		}

		m_id2shop[type] =  one;
		m_ismodify.Set();

		SSInfo<<"refresh new shop:"<<type<<" roleid"<<m_role->GetID()<<END;

		return !m_id2shop[type].IsEmpty();
	}
	else
	{
		shop->Clear(iscleardailycount);
		if (isadd)
		{
			shop->refreshcount++;
		}
		shop->FillItems(ShopMgr::Instance()->RefreshShop(type, rolelevel, roleprofession));

		if (isnew)
		{
			if (type == Shop_Guild)
			{
				CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(m_role->GetID());
				if (NULL != guild)
				{
					shop->ishint = true;
					ShopMgr::Instance()->Hint(m_role, false, type);
				}
			}
			else
			{

				shop->ishint = true;
				ShopMgr::Instance()->Hint(m_role, false, type);
			}
		}

		m_ismodify.Set();

		SSInfo<<"refresh old shop:"<<type<<" roleid"<<m_role->GetID()<<END;

		return !shop->IsEmpty();
	}
}

XItem* CShopRecord::GetItem(UINT32 type, UINT32 uid)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		return NULL;
	}
	std::map<UINT32, XItem>* items = shop->GetItems();
	for (auto i = items->begin(); i != items->end(); ++i)
	{
		if (i->second.uid == uid)
		{
			return &i->second;
		}
	}
	return NULL;
}

UINT32 CShopRecord::GetRefreshCount(UINT32 type)
{
	CShopRecordOne* shop = GetShop(type);
	if (NULL == shop)
	{
		return 0;
	}
	return shop->refreshcount;
}

void CShopRecord::OnLevelUp()
{
	std::set<UINT32> shops_refreshed;
	shops_refreshed = TryRefresh();

	for (auto i = GetGlobalConfig().LevelChangeShop.begin(); i != GetGlobalConfig().LevelChangeShop.end(); ++i)
	{
		if (shops_refreshed.find(*i) == shops_refreshed.end())
		{
			Refresh(*i, m_role->GetLevel(), m_role->GetProfession(), false, false, false);
		}
	}
	//for (auto i = m_id2shop.begin(); i != m_id2shop.end(); ++i)
	//{
	//	if (i->second.ishint)
	//	{
	//		continue;
	//	}
	//	CShopRecordOne& one = i->second;	
	//	for (auto j = one.m_items.begin(); j != one.m_items.end(); ++j)
	//	{
	//		const ShopItemBreif* breif = ShopMgr::Instance()->GetBreif(i->first, j->first);
	//		if (NULL != breif)
	//		{
	//			if (breif->condition.level == m_role->GetLevel())
	//			{
	//				one.ishint = true;
	//				ShopMgr::Instance()->Hint(m_role, false, i->first);
	//			}
	//		}
	//	}
	//}
}
