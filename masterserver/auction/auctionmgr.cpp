#include "pch.h"
#include "auctionmgr.h"
#include "auctioncart.h"
#include "traderecord.h"
#include "config.h"
#include "util/XRandom.h"
#include "table/globalconfig.h"
#include "auctiondbhandler.h"
#include "global/GlobalConfigDBBuild.h"
#include "table/auctionconfig.h"
#include "table/ItemConfigMgr.h"
#include <cmath>
#include "util/timespecificmgr.h"
#include "mail/mailmgr.h"
#include "mail/mailconfig.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "loghelper/tlogr.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "role/rolemanager.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"

INSTANCE_SINGLETON(AuctionMgr)

	AuctionMgr::AuctionMgr()
{

}

AuctionMgr::~AuctionMgr()
{
}

bool AuctionMgr::Init()
{
	m_SaleItemInex = 0;
	m_OverlapIndex = 0;

	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);	

	m_ATradeRecord = new ATradeRecord();	
	m_DBHandler = new AuctDbHandler();

	if(m_DBHandler)
	{
		if(!m_DBHandler->ReadSaleItemFromDB())
		{
			return false;
		}
		if(!m_DBHandler->ReadTradeRecordFromDB(m_ATradeRecord))
		{
			return false;
		}
	}

	start_time_	= GetGlobalConfig().AuctSupplementTime.seq[0];
	end_time_	= GetGlobalConfig().AuctSupplementTime.seq[1];
	auctSupplementFrequency_ = GetGlobalConfig().AuctSupplementFrequency / 5 ; 
	frequency_count_ = 0;

	m_useCartCount = 0;
	m_allSeeUnitCount = 0;

	recovery_item_handler_ = INVALID_HTIMER;
	last_update_time_ = 0;

	// 初始化系统回收购买
	InitRecoveryItemEvent();
	return true;
}

void AuctionMgr::Uninit()
{
	for(auto it = m_roleSaleItem.begin(); it != m_roleSaleItem.end(); ++it)
	{
		delete (it->second);
	}
	m_roleSaleItem.clear();

	for(auto it = m_allSaleItem.begin(); it != m_allSaleItem.end(); ++it)
	{
		delete (it->second);
	}
	m_allSaleItem.clear();

	for(auto it = m_shoppingCart.begin(); it != m_shoppingCart.end(); ++it)
	{
		delete(it->second);
	}
	m_shoppingCart.clear();

	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}

	if(m_ATradeRecord)
	{
		delete m_ATradeRecord;
		m_ATradeRecord = NULL;
	}

	if(m_DBHandler)
	{
		delete m_DBHandler;
		m_DBHandler = NULL;
	}
}

AuctionItem* AuctionMgr::AddSaleItem(UINT64 roleid, AuctAddType type, UINT32 perprice, const KKSG::Item& itemdata,bool supplement)
{
	time_t reqsaletime = GameTime::GetTime();
	time_t onsaletime = reqsaletime ;
	if (!supplement)
	{
		onsaletime += RandDelayTime();
	}

	UINT64 uid = NewUID();
	AuctionItem* pSaleItem = new AuctionItem(uid, roleid, AIS_DELAY_SALE, perprice, reqsaletime, onsaletime, itemdata);	
	if(NULL == pSaleItem)
	{
		return NULL;
	}

	SSInfo << " auctuid = " << uid << " type = " << type << " roleid = " << roleid <<" perprice = " << perprice<< END;

	Add(pSaleItem);

	if(m_DBHandler)
	{
		m_DBHandler->AddSaleItem(pSaleItem);
	}

	return pSaleItem;
}

void AuctionMgr::DelSaleItem(UINT64 uid, AuctDelType type)
{
	auto it = m_allSaleItem.find(uid);
	if(it == m_allSaleItem.end())
	{
		return ;
	}

	AuctionItem* pSaleItem = (it->second);
	if(NULL == pSaleItem)
	{
		return;
	}

	SSInfo << " uid = " << uid << " type = " << type << END;

	Del(pSaleItem);

	delete pSaleItem;
	pSaleItem = NULL;

	if(m_DBHandler)
	{
		m_DBHandler->DelSaleItem(uid);
	}
}

time_t AuctionMgr::RandDelayTime()
{
	return XRandom::randInt( GetGlobalConfig().AuctSaleDelayMin, GetGlobalConfig().AuctSaleDelayMax); 
}

void AuctionMgr::AddRoleSale(UINT64 roleid, AuctionItem* pSaleItem)
{
	auto it = m_roleSaleItem.find(roleid);
	if(it != m_roleSaleItem.end())
	{
		(it->second)->AddAuctionItem(pSaleItem);
	}
	else
	{
		RoleSaleItem* pRoleSale = new RoleSaleItem(roleid);	
		if(NULL == pRoleSale)
		{
			return ;
		}
		pRoleSale->AddAuctionItem(pSaleItem);
		m_roleSaleItem.insert(std::make_pair(pRoleSale->GetRoleID(), pRoleSale));
	}
}

void AuctionMgr::DelRoleSale(UINT64 roleid, AuctionItem* pSaleItem)
{
	auto it = m_roleSaleItem.find(roleid);
	if(it != m_roleSaleItem.end())
	{
		(it->second)->DelAuctionItem(pSaleItem);
		if(0 == (it->second)->GetCount())
		{
			delete (it->second);
			m_roleSaleItem.erase(it);
		}
	}
}

int AuctionMgr::GetRoleSaleCount(UINT64 roleid)
{
	auto it = m_roleSaleItem.find(roleid);
	if(it != m_roleSaleItem.end())
	{
		return (it->second)->GetCount();
	}
	return 0;
}

int AuctionMgr::GetRoleValidTimeSaleCount(UINT64 roleid)
{
	auto it = m_roleSaleItem.find(roleid);
	if(it != m_roleSaleItem.end())
	{
		return (it->second)->GetValidTimeCount();
	}
	return 0;
}
RoleSaleItem* AuctionMgr::GetRoleSale(UINT64 roleid)
{
	auto it = m_roleSaleItem.find(roleid);
	if(it != m_roleSaleItem.end())
	{
		return it->second;
	}
	return NULL;
}

void AuctionMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UpdateSaleItemState();
	UpdateShoppingCart();

	if(m_ATradeRecord)
	{
		m_ATradeRecord->Update();
	}
	if (frequency_count_++ >= auctSupplementFrequency_)
	{
		AuctSupplement();
		frequency_count_ = 0;
	}

	time_t now = GameTime::GetTime();
	if (!GameTime::IsInSameHour(now,last_update_time_))
	{
		InitRecoveryItemEvent();
	}
}

AuctionItem* AuctionMgr::GetSaleItem(UINT64 uid)
{
	auto it = m_allSaleItem.find(uid);
	if(it != m_allSaleItem.end())
	{
		return it->second;
	}
	return NULL;
}

ShoppingCart* AuctionMgr::GetShopingCart(UINT64 roleid)
{
	auto it = m_shoppingCart.find(roleid);
	if(it == m_shoppingCart.end())
	{
		ShoppingCart* pCart = new ShoppingCart(roleid);
		m_shoppingCart.insert(std::make_pair(roleid, pCart));
		it = m_shoppingCart.find(roleid);
	}

	if(it != m_shoppingCart.end())
	{
		return (it->second);
	}
	return NULL;
}

void AuctionMgr::UpdateSaleItemState()
{
	std::vector<UINT64> waitDel;
	time_t now = GameTime::GetTime();
	for(auto it = m_allSaleItem.begin(); it != m_allSaleItem.end(); ++it)
	{
		AuctionItem* pSaleItem = (it->second);
		if(NULL == pSaleItem)
		{
			continue;
		}
		if(pSaleItem->IsLocked(now))
		{
			continue;
		}
		if(pSaleItem->GetState() == AIS_DELAY_SALE && pSaleItem->GetOnSaleTime() <= now)
		{
			SetState(pSaleItem, AIS_ON_SALE);
		}
		else if(pSaleItem->GetState() == AIS_ON_SALE && 0 == pSaleItem->GetDueLeftTime(now))
		{
			pSaleItem->SetOutSaleTime(now);
			SetState(pSaleItem, AIS_OUT_SALE);
			DoTxLog(pSaleItem, 0);
		}
		else if(pSaleItem->GetState() == AIS_OUT_SALE && 0 == pSaleItem->GetRecoverLeftTime(now))
		{
			waitDel.push_back(pSaleItem->GetUID());
			DoTxLog(pSaleItem, 1);
		}
	}

	for(size_t i = 0; i < waitDel.size(); ++i)
	{
		DelSaleItem(waitDel[i], AUCT_DEL_TIMEOVER);
	}
}

UINT64 AuctionMgr::NewOverlapID()
{
	++m_OverlapIndex;
	return m_OverlapIndex;
}

void AuctionMgr::TestPrintAllSaleItem()
{
	for(auto it = m_allSaleItem.begin(); it != m_allSaleItem.end(); ++it)
	{
		(it->second)->TestPrint();
	}
}

void AuctionMgr::TestPrintMySaleItem(UINT64 roleid)
{
	auto it = m_roleSaleItem.find(roleid);
	if(it == m_roleSaleItem.end())
	{
		SSDebug << __FUNCTION__ << " have no my sale item " << END;
		return ;
	}
	(it->second)->TestPrint();
}

void AuctionMgr::TestPrintMyShoppingCart(UINT64 roleid)
{
	auto pCart = GetShopingCart(roleid);
	if(pCart)
	{
		pCart->TestPrint();
	}
}

void AuctionMgr::UpdateIDIndex(UINT64 uid)
{
	if(m_SaleItemInex < (uid&0x00000000ffffffff))
	{
		m_SaleItemInex = (uid&0x00000000ffffffff);
	}
	SSDebug << __FUNCTION__ << m_SaleItemInex << END;
}

UINT64 AuctionMgr::NewUID()
{
	UINT64 servergroupid = (UINT64)MSConfig::Instance()->GetServerID();
	//UINT64 now = (UINT64)(time(NULL));
	++m_SaleItemInex;

	UINT64 uid = m_SaleItemInex;
	//uid = (uid & 0x00000000ffffffff) | (now << 32);
	uid = (uid & 0x00000000ffffffff) | (servergroupid << 32);
	return uid;
}

void AuctionMgr::SetState(AuctionItem* pSaleItem, AuctionItemState state)
{
	if(NULL == pSaleItem)
	{
		return ;
	}
	pSaleItem->SetState(state);

	if(m_DBHandler)
	{
		m_DBHandler->UpdateSaleItem(pSaleItem);
	}
}

void AuctionMgr::ReduceItemCount(AuctionItem* pSaleItem, UINT32 rcount)
{
	if(NULL == pSaleItem)
	{
		return ;
	}
	pSaleItem->ReduceItemCount(rcount);

	if(m_DBHandler)
	{
		m_DBHandler->UpdateSaleItem(pSaleItem);
	}
}

void AuctionMgr::AddDBSaleItem(UINT64 uid, UINT64 roleid, AuctionItemState state, UINT32 perprice, time_t reqsaletime, time_t onsaletime, const std::string& itemstr, const std::string& roleaccount, UINT32 outsaletime)
{
	KKSG::Item itemdata;
	itemdata.ParseFromString(itemstr);

	AuctionItem* pSaleItem = new AuctionItem(uid, roleid, roleaccount, state, perprice, reqsaletime, onsaletime, outsaletime, itemdata);	
	if(NULL == pSaleItem)
	{
		return ;
	}

	Add(pSaleItem);

	UpdateIDIndex(uid);
}

void AuctionMgr::Add(AuctionItem* pSaleItem)
{
	if(NULL == pSaleItem)
	{
		return ;
	}

	m_allSaleItem.insert(std::make_pair(pSaleItem->GetUID(), pSaleItem));
	AddRoleSale(pSaleItem->GetRoleID(), pSaleItem);
}

void AuctionMgr::Del(AuctionItem* pSaleItem)
{
	if(NULL == pSaleItem)
	{
		return ;
	}

	DelRoleSale(pSaleItem->GetRoleID(), pSaleItem);
	m_allSaleItem.erase(pSaleItem->GetUID());
}

void AuctionMgr::UpdateShoppingCart()
{
	time_t now = GameTime::GetTime();
	for(auto it = m_shoppingCart.begin(); it != m_shoppingCart.end(); ++it)
	{
		auto pCart = it->second;
		if(pCart)
		{
			pCart->Update(now);
		}
	}
}



bool AuctionMgr::CheckAuctSupplementTime()
{
	UINT32 conf_time		= 0;			
	time_t now				= GameTime::GetTime();
	struct tm *oTime		= localtime(&now);
	UINT32 hour				= oTime->tm_hour;

	// 没跨天
	if (start_time_ < end_time_)
	{
		if (hour >= start_time_ && hour < end_time_)
		{
			return true;
		}
	}
	else // 跨天了
	{
		if (hour >= start_time_ || hour < end_time_)
		{
			return true;
		}
	}

	return false;
}

void  AuctionMgr::AuctSupplement()
{	
	if (!CheckAuctSupplementTime())
	{
		return ;
	}

	// 1.根据世界等级获取补充的物品
	std::unordered_map<UINT32,UINT32> items;
	UINT32 world_level = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel));
	AuctionConfig::Instance()->GetAuctSupplementByWorldLevel(world_level,items);

	LogDebug("world_level is %d ",world_level);

	time_t now = GameTime::GetTime();
	std::unordered_map<UINT32,UINT32> sale_items;
	std::unordered_set<UINT64> last_suplement_items;
	// 2.当前在售的物品
	for (auto it = m_allSaleItem.begin(); it != m_allSaleItem.end(); ++it)
	{		
		if (AIS_DELAY_SALE  == it->second->GetState() || AIS_ON_SALE == it->second->GetState() )
		{
			if (it->second->GetRoleID() > 0)
			{
				UINT32 count = it->second->GetItemCount();
				auto exist_it = sale_items.find(it->second->GetItemID());
				if ( exist_it != sale_items.end())
				{
					exist_it->second += count;
				}
				else
				{
					sale_items.insert(std::make_pair(it->second->GetItemID(),count));	
				}
			}
			else
			{	
				// 正在交易的物品不删
				if (!it->second->IsLocked(now))
				{
					last_suplement_items.insert(it->first);
					LogInfo("delete supplement item id  is %d ,count is %d",it->first,it->second);
				}
			}
		}
	}

	// 3.上轮补的货需要清理掉
	for (auto it = last_suplement_items.begin(); it != last_suplement_items.end(); ++it)
	{
		DelSaleItem(*it, AUCT_DEL_SUPPSALE);
	}

	// 4.需要系统补给的物品
	std::unordered_map<UINT32,UINT32> immediate_supplement_items;	
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		LogDebug("system supple : item id  is %d ,count is %d",it->first,it->second);

		auto sale_it = sale_items.find(it->first);
		if (sale_it != sale_items.end())
		{
			// 有正在卖的，检查补差额
			if ( it->second > sale_it->second )
			{
				UINT32 count =  it->second - sale_it->second;
				immediate_supplement_items.insert(std::make_pair(sale_it->first,count));				
			}
			continue;
		}

		// 直接上架
		immediate_supplement_items.insert(std::make_pair(it->first,it->second));		
	}

	// 5.先把直接上架的添加
	for (auto it = immediate_supplement_items.begin(); it != immediate_supplement_items.end(); ++it)
	{
		KKSG::Item itemdata;
		itemdata.set_itemid(it->first);
		itemdata.set_itemcount(it->second);
		itemdata.set_uid(0);
		auto item_conf = CItemConfig::Instance()->GetItemConf(it->first);	
		if(NULL == item_conf)
		{
			SSError << " find item failed itemid = " << it->first << END;
			continue;
		}
		itemdata.set_itemtype(item_conf->ItemType);
		SupplementAddItem(itemdata);

		LogInfo("immediate_supplement item id  is %d ,count is %d",it->first,it->second);
	}

	immediate_supplement_items.clear();
}

void AuctionMgr::SupplementAddItem(const KKSG::Item& item)
{
	float priceRecommend = AuctionConfig::Instance()->GetAuctRecPrice(item.itemid());
	float highRate	= 1.0;	
	auto pItemConf = CItemConfig::Instance()->GetItemConf(item.itemid());	
	if(NULL != pItemConf)
	{		
		highRate = pItemConf->AuctionRange.seq[1];
	}
	UINT32 pricehigh= (UINT32)std::ceil(priceRecommend * highRate);	
	AddSaleItem(0, AUCT_ADD_SUPPSALE, pricehigh, item,true);
}

int AuctionMgr::GetAllSaleCount()
{
	return (int)(m_allSaleItem.size());
}

int AuctionMgr::GetUseCartCount()
{
	return m_useCartCount;
}

int AuctionMgr::GetAllSeeUnitCount()
{
	return m_allSeeUnitCount;
}

void AuctionMgr::OnChangeSeeUnitCount(UINT32 lastSeeCount, UINT32 nowSeeCount)
{
	if(!lastSeeCount && nowSeeCount)
	{
		++m_useCartCount;
	}
	else if(lastSeeCount && !nowSeeCount)
	{
		--m_useCartCount;
	}

	int ch = int(nowSeeCount) - int(lastSeeCount);

	m_allSeeUnitCount += ch;
}


void AuctionMgr::InitRecoveryItemEvent()
{	
	if (!CheckAuctSupplementTime())
	{
		return;
	}

	time_t now		= GameTime::GetTime();
	last_update_time_ = (UINT32)now;

	struct tm *oTime = localtime(&now);
	UINT32 hour      = oTime->tm_hour;
	UINT32 min		= oTime->tm_min;
	if (min > 50)
	{
		return;
	}	

	UINT32 start_minuts  = XRandom::randInt(min+1,59);
	TimeSpecific start_revocry(hour,start_minuts,0);
	if (recovery_item_handler_ == INVALID_HTIMER)
	{
		recovery_item_handler_ = CTimeSpecificMgr::Instance()->RegisterEvent(start_revocry, std::bind(&AuctionMgr::SetRecoveryItemEvent, this));
	}
}

bool AuctionMgr::SetRecoveryItemEvent()
{
	std::unordered_map<UINT32,ItemSailItemInfo> sale_items;
	time_t now = GameTime::GetTime();
	UINT32 world_level = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel));
	// 2.当前在售的物品
	for (auto it = m_allSaleItem.begin(); it != m_allSaleItem.end(); ++it)
	{		
		if (AIS_ON_SALE == it->second->GetState()  && !it->second->IsLocked(now))
		{
			// 检测是否需要回收
			ItemRecoveryData data;
			AuctionConfig::Instance()->GetAuctItemRecoveryData(it->second->GetItemID(),data,world_level);
			if (it->second->GetRoleID() > 0 && data.RecoveryCount > 0 )
			{
				// 只有最低价的物品需要处理
				float priceRecommend = AuctionConfig::Instance()->GetAuctRecPrice(it->second->GetItemID());					
				float lowRate	= 1.0;
				auto pItemConf = CItemConfig::Instance()->GetItemConf(it->second->GetItemID());	
				if(NULL != pItemConf)
				{
					lowRate = pItemConf->AuctionRange.seq[0];					
				}			
				UINT32 pricelow = (UINT32)std::floor(priceRecommend * lowRate);
				if (it->second->GetPerPrice() > pricelow)
				{
					continue;
				}
				
				ItemSailItemInfo saile_Item_info;
				UINT32 count = it->second->GetItemCount();
				auto exist_it = sale_items.find(it->second->GetItemID());
				if ( exist_it != sale_items.end())
				{
					exist_it->second.ItemCount += count;
					exist_it->second.itemList.push_back(it->second);
				}
				else
				{
					saile_Item_info.ItemCount = count;
					saile_Item_info.itemList.push_back(it->second);
					sale_items.insert(std::make_pair(it->second->GetItemID(),saile_Item_info));	
				}
			}			
		}
	}

	for ( auto it = sale_items.begin(); it != sale_items.end(); ++it)
	{
		ItemRecoveryData data;
		AuctionConfig::Instance()->GetAuctItemRecoveryData(it->first,data,world_level);
		if ((it->second.ItemCount > data.ItemThreshold)  && (data.RecoveryCount > 0))
		{			
			UINT32 have_sell = 0;
			for (auto item_it = it->second.itemList.begin(); item_it != it->second.itemList.end(); ++item_it)
			{
				int saild_item_count = 0;
				UINT32 need_sell = data.RecoveryCount - have_sell;
				if ( need_sell<= 0)
				{
					break;
				}
				if ((*item_it)->GetItemCount() > need_sell)
				{
					saild_item_count = need_sell;
				}
				else
				{
					saild_item_count = (*item_it)->GetItemCount();
				}

				SendSaleMail(*item_it,saild_item_count);
				LogOnBuy(*item_it, saild_item_count, 0);
				BuyItemResult(*item_it,saild_item_count);
				have_sell += saild_item_count;
			}			
		}
	}

	if (recovery_item_handler_ != INVALID_HTIMER)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(recovery_item_handler_);
		recovery_item_handler_ = INVALID_HTIMER;
	}

	return true;
}



void AuctionMgr::SendSaleMail(AuctionItem* pSaleItem, UINT32 salecount)
{
	if(NULL == pSaleItem)
	{
		return ;
	}
	// 系统补的货，无需邮件发龙币
	if (0 == pSaleItem->GetRoleID())
	{
		return ;
	}

	UINT32 allprice = pSaleItem->GetPerPrice() * salecount;

	std::string mailtitle = "null";
	std::string mailcontent;
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_AuctionSale);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
	}
	std::string itemname;
	CRoleSummary* pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(pSaleItem->GetRoleID());
	if(pSummary)
	{
		itemname = CItemConfig::Instance()->GetItemProfessionName(pSaleItem->GetItemID(), pSummary->GetProfession());
	}

	UINT32  revenuePercent = GetGlobalConfig().AuctRevenueRate;

	UINT32 getPrice = allprice;
	if(revenuePercent < 100)
	{
		getPrice = allprice - (allprice * revenuePercent / 100);
		if(getPrice == allprice && getPrice > 1)
		{
			getPrice --;
		}
	}

	std::vector<std::string> vec;
	vec = Split(mailcontent, "x");
	if(6 == (int)vec.size())
	{
		std::stringstream ss;
		ss << vec[0] << salecount << vec[1] << itemname << vec[2] << allprice << vec[3] << revenuePercent << vec[4] << getPrice << vec[5];
		mailcontent = ss.str();
	}

	std::vector<ItemDesc> gain;
	ItemDesc tmp(DRAGON_COIN, getPrice, false); 
	gain.push_back(tmp);
	MailData maildata(mailtitle, mailcontent, gain, MAIL_EXPIRE_TIME);
	maildata.SetReason(ItemFlow_Auction_Give);
	CMailMgr::Instance()->SendMail(pSaleItem->GetRoleID(), maildata);


	//SendSaleMail(pSaleItem, rcount);
	//LogOnBuy(pSaleItem, salecount, 0);
	//add record

}


void AuctionMgr::LogOnBuy(AuctionItem* pSaleItem, UINT32 count, UINT64 buyerID)
{
	if(NULL == pSaleItem)
	{
		return ;
	}
	//个数使用 count 不要使用 pSaleItem 里面的个数
	//buyerID 买家ID
	SSDebug << __FUNCTION__ << " auctionUID = " << pSaleItem->GetUID() << " count = " << count << " buyerID = " << buyerID << END;

	TAuctionEndFlow oLog;
	oLog.m_lBuyerRoleUid = buyerID;
	CRoleSummary* pBuyerSum = CRoleSummaryMgr::Instance()->GetRoleSummary(buyerID);
	if (pBuyerSum)
	{
		oLog.m_iBuyerRoleJobID = pBuyerSum->GetProfession();
		oLog.m_iBuyerRoleLevel = pBuyerSum->GetLevel();
		oLog.m_iBuyerRoleBattlePoint = pBuyerSum->GetPowerPoint();
		oLog.m_strBuyerOpenID = pBuyerSum->GetAccount();
	}
	oLog.m_strBuyerUserIP = "0";
	oLog.m_lSellerRoleUid = pSaleItem->GetRoleID();
	CRoleSummary* pSellerSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pSaleItem->GetRoleID());
	if (pSellerSum)
	{
		oLog.m_iSellerRoleJobID = pSellerSum->GetProfession();
		oLog.m_iSellerRoleLevel = pSellerSum->GetLevel();
		oLog.m_strSellerOpenID = pSellerSum->GetAccount();
	}
	CRole* role = CRoleManager::Instance()->GetByRoleID(pSaleItem->GetRoleID());
	oLog.m_strSellerUserIP = role?role->GetIP():"0";
	oLog.m_iDealMainType = pSaleItem->GetItemData().itemtype();
	oLog.m_iDealPriceType = 2;
	oLog.m_iDealCount = count;
	oLog.m_iDealPrice = pSaleItem->GetPerPrice();
	oLog.m_iDealPriceTotal = count * pSaleItem->GetPerPrice();
	oLog.m_iDealItemID = pSaleItem->GetItemID();
	oLog.m_lDealUUID = pSaleItem->GetItemUID();
	oLog.m_lDealAuctionID = pSaleItem->GetUID();
	oLog.Do();

	EventMgr::Instance()->AddEventOnlyOnMS(buyerID, MENTOR_EVENT_OFFSET + KKSG::MentorTask_AuctBuy, 0, count);

	EventMgr::Instance()->AddEventOnlyOnMS(pSaleItem->GetRoleID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_AuctSale, 0, count);
}

void AuctionMgr::BuyItemResult(AuctionItem* pSaleItem, UINT32 salecount)
{
	auto pRecord = GetATradeRecord();
	if( !pSaleItem->IsTreasuer())//珍品不参与
	{
		m_ATradeRecord->AddOneRecord(pSaleItem->GetItemID(), salecount, pSaleItem->GetPerPrice(), pSaleItem->GetUID(), "0", pSaleItem->GetRoleAccount());
	}

	UINT32 oldcount = pSaleItem->GetItemCount();
	if(oldcount > salecount)
	{
		ReduceItemCount(pSaleItem, salecount);
	}
	else
	{
		DelSaleItem(pSaleItem->GetUID(), AUCT_DEL_BUY);
		pSaleItem = NULL;
	}
}

void AuctionMgr::DoTxLog(AuctionItem* pSaleItem, INT32 op)
{
	if (NULL == pSaleItem)
	{
		return;
	}
	TAuctionExpireFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.SetData(pSaleItem);
	oLog.m_Op = op;
	oLog.Do();
}
