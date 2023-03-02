#include "pch.h"
#include "auctioncart.h"
#include "auctionmgr.h"
#include "util/XRandom.h"
#include "table/globalconfig.h"
#include "table/ItemConfigMgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "traderecord.h"
#include "util/XCommon.h"
#include "table/auctionconfig.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "global/GlobalConfigDBBuild.h"

SCAuctUnit::SCAuctUnit(UINT64 uid)
	:m_uid(uid)
{

}

UINT32 SCAuctUnit::GetCount()
{
	AuctionItem* pSaleItem = AuctionMgr::Instance()->GetSaleItem(m_uid);	
	if(NULL == pSaleItem)//已经下架或者售出
	{
		return 0;
	}
	return pSaleItem->GetItemCount();
}

bool SCAuctUnit::GetItemKKSG(KKSG::Item& itemdata)
{
	AuctionItem* pSaleItem = AuctionMgr::Instance()->GetSaleItem(m_uid);	
	if(NULL == pSaleItem)
	{
		return false;
	}
	itemdata.CopyFrom(pSaleItem->GetItemData());
	return true;
}

SCAuctOverlap::SCAuctOverlap(UINT32 preprice, UINT64 overlapid)
	:m_preprice(preprice),m_overlapID(overlapid)
{

}

UINT32 SCAuctOverlap::GetCount()
{
	UINT32 count = 0;
	for(size_t i = 0; i < m_auctUnit.size(); ++i)
	{
		count += (m_auctUnit[i].GetCount());
	}
	return count;
}

void SCAuctOverlap::AddAuctUnit(const SCAuctUnit& au)
{
	m_auctUnit.push_back(au);
}

void SCAuctOverlap::FillOverlapData(KKSG::AuctOverlapData& data, UINT32 count)
{
	data.set_overlapid(m_overlapID);
	data.set_perprice(m_preprice);
	if(!FillItemData(*data.mutable_itemdata(), count))
	{
		SSError << " fill itemdata err overlap id = " << m_overlapID << END;
	}
}

void SCAuctOverlap::TestPrint()
{
	SSDebug << __FUNCTION__ << " overlapid = " << m_overlapID << " perprice = " << m_preprice << END;
	for(size_t i = 0; i < m_auctUnit.size(); ++i)
	{
		SSDebug << __FUNCTION__ << " auctid = " << m_auctUnit[i].GetAuctUid() << " count = " << m_auctUnit[i].GetCount() << END;
	}
}

bool SCAuctOverlap::FillItemData(KKSG::Item& itemdata, UINT32 count)
{
	for(size_t i = 0; i < m_auctUnit.size(); ++i)
	{
		if(m_auctUnit[i].GetItemKKSG(itemdata))
		{
			itemdata.set_itemcount(count);//设置个数
			return true;
		}
	}
	return false;
}

bool SCAuctOverlap::GetItemCountAuctID(std::vector<AuctLockTemp>& locktmp, UINT32 count)
{
	int needcount = (int)(count);
	int n = (int)m_auctUnit.size();
	if(0 == needcount || 0 == n)
	{
		return false;
	}
	std::map<UINT64,int> system_item;
	for(int i = n - 1; i >= 0; --i)
	{
		int tmp = (int)m_auctUnit[i].GetCount();
		if(0 == tmp)
		{
			continue;
		}

		AuctionItem* pSaleItem = AuctionMgr::Instance()->GetSaleItem(m_auctUnit[i].GetAuctUid());
		if (0 == pSaleItem->GetRoleID())
		{
			system_item.insert(std::make_pair(m_auctUnit[i].GetAuctUid(),tmp));
			continue;
		}
		AuctLockTemp lp(m_auctUnit[i].GetAuctUid(), (tmp < needcount ? tmp:needcount));
		locktmp.push_back(lp);

		needcount -= tmp;
		if(needcount <= 0)
		{
			return true;
		}
	}

	// 卖家的不够，用系统的补充
	for (auto it = system_item.begin(); it != system_item.end(); ++it)
	{
		AuctLockTemp lp(it->first, (it->second < needcount ? it->second : needcount));
		locktmp.push_back(lp);

		needcount -= it->second;
		if(needcount <= 0)
		{
			return true;
		}
	}

	return false;
}

void SCAuctOverlap::CheckDelEmptyUnit()
{
	while(!m_auctUnit.empty())
	{
		if(0 == m_auctUnit.back().GetCount())
		{
			m_auctUnit.pop_back();
		}
		else
		{
			break;
		}
	}
}

void SCAuctOverlap::ReduceUnitCount(UINT32 item_count)
{
	UINT32 reduce_count = 0;
	for (auto it = m_auctUnit.begin(); it != m_auctUnit.end();)
	{
		if (reduce_count >= item_count)
		{
			break;
		}
		reduce_count += it->GetCount();
		it = m_auctUnit.erase(it);	
	}
}

void SCAuctOverlap::Sort()
{
	std::sort(m_auctUnit.begin(), m_auctUnit.end());
}

SCAuctItem::SCAuctItem(UINT32 itemid)
	:m_itemID(itemid)
{
	m_itemCount = 0;

	ClearTempData();
}

UINT32 SCAuctItem::GetCount()
{
	//refresh every time for now
	RefreshCount();

	return m_itemCount;
}

void SCAuctItem::RefreshCount()
{
	m_itemCount = 0;
	for(size_t i = 0; i < m_auctOverlap.size(); ++i)
	{
		m_itemCount += m_auctOverlap[i].GetCount();
	}
}

void SCAuctItem::AddAuctItem(AuctionItem* pSaleItem)
{
	if(NULL == pSaleItem)
	{
		return ;
	}

	bool addnew = true;
	SCAuctUnit auctUnit(pSaleItem->GetUID());

	UINT32 perprice = pSaleItem->GetPerPrice();
	bool treasure = pSaleItem->IsTreasuer();

	bool canMerge = (CItemConfig::Instance()->GetOverlap(pSaleItem->GetItemID()) > 1);
	if(canMerge)
	{
		for(size_t i = 0; i < m_auctOverlap.size(); ++i)
		{
			if(m_auctOverlap[i].GetPreprice() == perprice)
			{
				m_auctOverlap[i].AddAuctUnit(auctUnit);

				addnew = false;	
				break;
			}
		}
	}

	if(addnew)
	{
		SCAuctOverlap auctOverlap(perprice, AuctionMgr::Instance()->NewOverlapID());
		auctOverlap.AddAuctUnit(auctUnit);
		m_auctOverlap.push_back(auctOverlap);
	}

	m_itemCount += pSaleItem->GetItemCount();
}

void SCAuctItem::FillBriefData(KKSG::AuctItemBrief& data)
{
	data.set_itemid(m_itemID);
	data.set_count(GetCount());
}

void SCAuctItem::FillOverlapData(KKSG::AuctionAllReqRes& roRes)
{
	m_itemCount = 0;//顺便更新数量

	for(size_t i = 0; i < m_auctOverlap.size(); ++i)
	{
		UINT32 count = m_auctOverlap[i].GetCount();
		if(0 == count)
		{
			continue;//考虑删除
		}
		m_itemCount += count;
		m_auctOverlap[i].FillOverlapData(*roRes.add_overlapdata(), count);
	}
	roRes.set_itemleftcount(m_itemCount);
}

void SCAuctItem::AddTempAuctItem(AuctionItem* pSaleItem)
{
	if(NULL == pSaleItem)
	{
		return ;
	}
	// 刷新物品的时候，补货的物品优先级要求最低，玩家出售的最高
	UINT32 randkey = XRandom::randInt(0, 1000001);
	//UINT32 randkey = XRandom::randInt(0, 990001);
	if (pSaleItem->GetRoleID() == 0 )
	{
		randkey += 1000000;
	}
	else
	{
		auto pItemConf = CItemConfig::Instance()->GetItemConf(pSaleItem->GetItemID());	
		if(NULL != pItemConf)
		{
			auto exist_it = GetGlobalConfig().AuctItemSpecialPush.find(pItemConf->ItemType);
			auto exist_it_end  = GetGlobalConfig().AuctItemSpecialPush.end();
			if (exist_it != exist_it_end)
			{
				//  价格高的，优先级低				
				randkey = pSaleItem->GetPerPrice();				
			}
		}
	}
	SCPAIR kp(randkey, pSaleItem);	

	if(pSaleItem->IsTreasuer())
	{
		m_tempTreasureItemCount += (INT32)pSaleItem->GetItemCount();
		m_tempTreasureItem.push(kp);
	}
	else
	{
		m_tempNormalItemCount += (INT32)pSaleItem->GetItemCount();
		m_tempNormalItem.push(kp);
	}

	INT32 NormalExpcount = GetNormalExpCount();	
	INT32 TreasureExpCount = GetTreasureExpCount();

	TempCountSafe(m_tempNormalItem, m_tempNormalItemCount, NormalExpcount);
	TempCountSafe(m_tempTreasureItem, m_tempTreasureItemCount, TreasureExpCount);
}

void SCAuctItem::ClearTempData()
{
	while(!m_tempNormalItem.empty())
	{
		m_tempNormalItem.pop();
	}
	while(!m_tempTreasureItem.empty())
	{
		m_tempTreasureItem.pop();
	}
	m_tempNormalItemCount = 0;
	m_tempTreasureItemCount = 0;
}

UINT32 SCAuctItem::AddRandAuctItem()
{
	TreasurePercentSafe();
	while(!m_tempNormalItem.empty())
	{
		AddAuctItem(m_tempNormalItem.top().second);
		m_tempNormalItem.pop();
	}
	while(!m_tempTreasureItem.empty())
	{
		AddAuctItem(m_tempTreasureItem.top().second);
		m_tempTreasureItem.pop();
	}
	ClearTempData();
	
	UINT32 auctunit_num = 0;
	for(size_t i = 0; i < m_auctOverlap.size(); ++i)
	{
		auctunit_num += m_auctOverlap[i].GetSize();
		m_auctOverlap[i].Sort();		
	}

	return  auctunit_num ;
}

INT32 SCAuctItem::GetNormalExpCount()
{
	UINT32 world_level = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel));
	ItemRecoveryData data;
	AuctionConfig::Instance()->GetAuctItemRecoveryData(m_itemID,data,world_level);
	if(0 == m_tempTreasureItemCount)
	{
		if (data.AuctCartItemAllCount > 0)
		{
			return data.AuctCartItemAllCount;
		}
		return  GetGlobalConfig().AuctCartItemAllCount;
	}
	else
	{
		if (data.AuctCartItemAllCount > 0 )
		{
			return data.AuctCartItemAllCount - (UINT32)(GetGlobalConfig().AuctCartItemTreasurePercent * data.AuctCartItemAllCount/100);
		}
	}
	return  GetGlobalConfig().AuctCartItemNormalCount;
}

INT32 SCAuctItem::GetTreasureExpCount()
{
	UINT32 world_level = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel));
	ItemRecoveryData data;
	AuctionConfig::Instance()->GetAuctItemRecoveryData(m_itemID,data,world_level);

	if (data.AuctCartItemAllCount > 0 )
	{		
		return  (INT32) (GetGlobalConfig().AuctCartItemTreasurePercent * data.AuctCartItemAllCount/100);
	}

	return  GetGlobalConfig().AuctCartItemTreasureCount;		
}

void SCAuctItem::TestPrint()
{
	SSDebug << __FUNCTION__ << " itemid = " << m_itemID << " itemcount = " << m_itemCount << END;
	for(size_t i = 0; i < m_auctOverlap.size(); ++i)
	{
		m_auctOverlap[i].TestPrint();	
	}
}

void SCAuctItem::TreasurePercentSafe()
{//确定珍品所占比例不失衡
	if(m_tempTreasureItem.size() < 1)
	{
		return ;
	}
	if(0 == m_tempNormalItemCount)
	{
		return ;
	}
	INT32 TreasurePercent = GetGlobalConfig().AuctCartItemTreasurePercent;
	INT32 NormalPercent = 100 - GetGlobalConfig().AuctCartItemTreasurePercent;
	if(0 == NormalPercent)
	{
		return ;
	}
	int expcount = m_tempNormalItemCount * TreasurePercent / NormalPercent;
	TempCountSafe(m_tempTreasureItem, m_tempTreasureItemCount, expcount);
}

void SCAuctItem::TempCountSafe(SCPriQueue& que, INT32& curcount, INT32 expcount)
{
	while(curcount > expcount && (int)que.size() > 1)
	{
		UINT32 temp_count = (que.top().second)->GetItemCount();
		if (curcount - temp_count < expcount)
		{
			break;
		}
		else
		{
			curcount -= (que.top().second)->GetItemCount();
			que.pop();
		}
	}
}

SCAuctOverlap* SCAuctItem::GetOverlap(UINT64 overlapid)
{
	for(size_t i = 0; i < m_auctOverlap.size(); ++i)
	{
		if(m_auctOverlap[i].GetOverlapID() == overlapid)
		{
			return &m_auctOverlap[i];
		}
	}
	return NULL;
}

void SCAuctItem::CheckDelEmptyOverlap(UINT64 overlapid)
{
	for(auto it = m_auctOverlap.begin(); it != m_auctOverlap.end(); ++it)
	{
		if((*it).GetOverlapID() == overlapid)
		{
			if(0 == (*it).GetCount())
			{
				m_auctOverlap.erase(it);
			}
			return ;
		}
	}
}

//CSAuctType::CSAuctType(int type)
//	:m_type(type)
//{
//
//}
//
//void CSAuctType::AddAuctItem(AuctionItem* pSaleItem)
//{
//	if(NULL == pSaleItem)
//	{
//		return ;
//	}
//	UINT32 itemid = pSaleItem->GetItemID();
//	auto it = m_auctItem.find(itemid);
//	if(it == m_auctItem.end())
//	{
//		SCAuctItem auctItem(itemid);
//		m_auctItem.insert(std::make_pair(itemid, auctItem));
//	}
//	it = m_auctItem.find(itemid);
//	if(it != m_auctItem.end())
//	{
//		(it->second).AddAuctItem(pSaleItem);
//	}
//}

ShoppingCart::ShoppingCart(UINT64 roleid)
	:m_roleid(roleid)
{
	m_lastAutoRefreshTime = time_t(0);
	m_lastDayUpdateTime = GameTime::GetTime();
	m_lastSeeUnitCount = 0;
	//ResetFreeRefresh();
	ResetFreeRefreshOnlineTime(GetGlobalConfig().AuctFreeRefreshTime, GameTime::GetTime());
	ResetSaleSuccessCount();
}

ShoppingCart::~ShoppingCart()
{
}

void ShoppingCart::AddAuctItem(AuctionItem* pSaleItem)
{
	if(NULL == pSaleItem)
	{
		return ;
	}
	UINT32 itemid = pSaleItem->GetItemID();
	auto it = m_auctItem.find(itemid);
	if(it == m_auctItem.end())
	{
		SCAuctItem auctItem(itemid);
		m_auctItem.insert(std::make_pair(itemid, auctItem));

		it = m_auctItem.find(itemid);
	}
	if(it != m_auctItem.end())
	{
		if(pSaleItem->GetRoleID() == m_roleid)//自己的
		{
			(it->second).AddAuctItem(pSaleItem);
		}
		else
		{
			(it->second).AddTempAuctItem(pSaleItem);
		}
	}
}

bool ShoppingCart::IsEmpty()
{
	return m_auctItem.empty();
}

void ShoppingCart::Clear()
{
	m_auctItem.clear();

	std::unordered_map<UINT32,SCAuctItem> oTemp;
	m_auctItem.swap(oTemp);

	OnChangeSeeUnitCount(0);
}

void ShoppingCart::Refresh(AuctionRefreshType type)
{
	SSDebug << __FUNCTION__ << " type = " << type << END;

	Clear();

	if(CART_FREE == type)
	{
		//AddFreeRefreshCount();
		ResetFreeRefreshOnlineTime(0, GameTime::GetTime());
	}
	else if(CART_AUTO == type)
	{
		m_lastAutoRefreshTime = GameTime::GetTime();
	}

	auto pRecord = AuctionMgr::Instance()->GetATradeRecord();
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_roleid);
	UINT32 TradeLimit = AuctionConfig::Instance()->GetTradeLimit();

	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleid);	
	if(NULL == pRoleSummary)
	{
		return;
	}

	UINT32 baseProfession = pRoleSummary->GetProfession() % 10;
	auto& AllSaleItem = AuctionMgr::Instance()->GetAllSaleItem();	
	for(auto it = AllSaleItem.begin(); it != AllSaleItem.end(); ++it)
	{
		if(CanRecommondToMe(it->second, pRecord, pRole, TradeLimit, baseProfession))
		{
			AddAuctItem(it->second);
		}
	}
	
	UINT32 auctUnitCount = 0;

	for(auto it = m_auctItem.begin(); it != m_auctItem.end(); ++it)
	{
		auctUnitCount += (it->second).AddRandAuctItem();
	}

	OnChangeSeeUnitCount(auctUnitCount);

}

void ShoppingCart::FillAllBriefData(KKSG::AuctionAllReqRes& roRes)
{
	for(auto it = m_auctItem.begin(); it != m_auctItem.end(); ++it)
	{
		(it->second).FillBriefData(*roRes.add_itembrief());
	}
}

UINT32 ShoppingCart::GetAutoRefreshLeftTime()
{
	time_t now = GameTime::GetTime();

	//UINT32 AutoRefreshConfigTime = GetGlobalConfig().AuctAutoRefreshTime;
	UINT32 AutoRefreshConfigTime = GetConfigTime(1);
	if(m_lastAutoRefreshTime + AutoRefreshConfigTime > now)
	{
		return UINT32(m_lastAutoRefreshTime + AutoRefreshConfigTime - now);
	}

	return 0;
}

void ShoppingCart::FillOverlapData(UINT32 itemid, KKSG::AuctionAllReqRes& roRes)
{
	auto it = m_auctItem.find(itemid);
	if(it != m_auctItem.end())
	{
		(it->second).FillOverlapData(roRes);
	}
}

bool ShoppingCart::CanRecommondToMe(AuctionItem* pSaleItem, ATradeRecord* pRecord, CRole* pRole, UINT32 tradeLimit, UINT32 profession)
{
	if(NULL == pSaleItem || NULL == pRecord || NULL == pRole)
	{
		return false;
	}
	if(pSaleItem->GetState() != AIS_ON_SALE)
	{
		return false;
	}

	if(pSaleItem->GetRoleID() == pRole->GetID())
	{
		return true;//可以看到自己的
	}

	if(pSaleItem->GetRoleAccount() == pRole->GetAccount())
	{
		return false;
	}

	if (pSaleItem->GetRoleID() > 0)
	{
		if(pRecord->IsTradeLimit(pRole->GetAccount(), pSaleItem->GetRoleAccount(), tradeLimit))//交易额限制
		{
			//SSDebug << __FUNCTION__ << " trade amount limit buyaccount = " << pRole->GetAccount() << " saleaccount = " << pSaleItem->GetRoleAccount() << END;
			return false;
		}
	}

	auto pItemConf = CItemConfig::Instance()->GetItemConf(pSaleItem->GetItemID());
	if(NULL == pItemConf)
	{
		return false;
	}

	if(pItemConf->Profession && pItemConf->Profession != profession)
	{
		return false;//职业不符
	}

	UINT32 tmp = pRecord->GetItemAVGPrice(pSaleItem->GetItemID());
	if(0 != tmp && !pSaleItem->IsTreasuer())//珍品不参与
	{
		if(pSaleItem->GetPerPrice() > tmp * GetGlobalConfig().AuctAVGPricePreHigh / 100)
		{
			return false;
		}
		if(pSaleItem->GetPerPrice() < tmp * GetGlobalConfig().AuctAVGPricePreLow / 100)
		{
			return false;
		}
	}

	return true;
}

void ShoppingCart::TestPrint()
{
	//SSDebug << __FUNCTION__ << " roleid = " << m_roleid << " lastfreerefreshtime = " << m_lastFreeRefreshTime << END;
	for(auto it = m_auctItem.begin(); it != m_auctItem.end(); ++it)
	{
		(it->second).TestPrint();
	}
}

SCAuctOverlap* ShoppingCart::GetOverlap(UINT32 itemid, UINT64 overlapid)
{
	auto it = m_auctItem.find(itemid);
	if(it != m_auctItem.end())
	{
		return (it->second).GetOverlap(overlapid);	
	}
	return NULL;
}

void ShoppingCart::CheckDelEmptyOverlap(UINT32 itemid, UINT64 overlapid)
{
	auto it = m_auctItem.find(itemid);
	if(it != m_auctItem.end())
	{
		(it->second).CheckDelEmptyOverlap(overlapid);	
	}
}

void ShoppingCart::SetLockTemp(std::vector<AuctLockTemp>& locktmp)
{
	m_lockTmp = locktmp;
	m_lockEndTime = GameTime::GetTime() + AuctLockContiTime;
}

void ShoppingCart::ClearLockTemp(std::vector<AuctLockTemp>& locktmp)
{
	locktmp = m_lockTmp;
	m_lockTmp.clear();
	m_lockEndTime = time_t(0);
}

bool ShoppingCart::HaveLockTemp(time_t now)
{
	if(!m_lockTmp.empty())
	{
		if(now < m_lockEndTime)
		{
			return true;
		}
		else
		{
			m_lockTmp.clear();	
		}
	}
	return false;
}

void ShoppingCart::Update(time_t now)
{
	if(!XCommon::IsDaySame(m_lastDayUpdateTime, now))
	{
		m_lastDayUpdateTime = now;
		ResetFreeRefresh();
		ResetSaleSuccessCount();
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_roleid);
	if(NULL == pRole && !IsEmpty() && 0 == GetAutoRefreshLeftTime())//不在线 且可自动刷新
	{
		Clear();//清除可见列表
	}
}

UINT32 ShoppingCart::GetFreeRefreshOnlineUpdateTime()
{
	time_t now = GameTime::GetTime();	
	if(m_freeRefreshLastUpdateTime < now)
	{
		m_freeRefreshOnlineTime += UINT32(now - m_freeRefreshLastUpdateTime);
		m_freeRefreshLastUpdateTime = now;
	}
	return m_freeRefreshOnlineTime;
}

UINT32 ShoppingCart::GetFreeRefreshLeftTime()
{
	UINT32 ConfTime =  GetConfigTime(0);	

	UINT32 onlineTime = GetFreeRefreshOnlineUpdateTime();
	if(ConfTime > onlineTime)
	{
		return ConfTime - onlineTime;
	}
	return 0;
}

void ShoppingCart::ResetFreeRefreshOnlineTime(UINT32 onlineTime, time_t lastUpdateTime)
{
	m_freeRefreshOnlineTime = onlineTime;
	m_freeRefreshLastUpdateTime = lastUpdateTime;
}


UINT32 ShoppingCart::GetConfigTime(const int type)
{
	UINT32 conf_time = 0;			
	time_t now = GameTime::GetTime();
	struct tm *oTime = localtime(&now);

	// 形如：0=8=10=15|8=0=30=60
	auto it = GetGlobalConfig().AuctRefreshTimeSegment.begin();
	auto it_end = GetGlobalConfig().AuctRefreshTimeSegment.end();
	for (; it != it_end; ++it)
	{
		if((UINT32)oTime->tm_hour >= it->seq[0])
		{
			// 1 : 系统刷新 0 ： 手动刷新
			if (0 == type)
			{
				conf_time = it->seq[2];
			}
			else if (1 == type)
			{
				conf_time = it->seq[3];
			}
		}
	}

	return conf_time;
}

void ShoppingCart::OnChangeSeeUnitCount(UINT32 nowSeeCount)
{
	AuctionMgr::Instance()->OnChangeSeeUnitCount(m_lastSeeUnitCount, nowSeeCount);

	m_lastSeeUnitCount = nowSeeCount;
}
