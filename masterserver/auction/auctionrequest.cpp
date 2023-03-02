#include "pch.h"
#include <cmath>
#include "auctionrequest.h"
#include "pb/project.pb.h"
#include "auctionmgr.h"
#include "role/role.h"
#include "auctioncart.h"
#include "traderecord.h"
#include "mail/mailmgr.h"
#include "define/itemdef.h"
#include "table/globalconfig.h"
#include "mail/mailconfig.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "table/ItemConfigMgr.h"
#include "table/auctionconfig.h"
#include "loghelper/tlogr.h"
#include "role/rolemanager.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"

AuctionRequest::AuctionRequest(CRole* pRole)
	:m_role(pRole)
{

}

int AuctionRequest::ReqOnSalePre(UINT32 itemid, UINT32 perprice, bool isTreasure)
{
	if(0 == itemid || 0 == perprice)
	{
		return KKSG::ERR_FAILED;
	}
	float priceRecommend = AuctionConfig::Instance()->GetAuctRecPrice(itemid);
	if(isTreasure)
	{
		priceRecommend = (priceRecommend * GetGlobalConfig().GetFloat("AuctTreasureTJPriceRate",2.0));
	}

	float highRate	= 1.0;
	float lowRate	= 1.0;
	auto pItemConf = CItemConfig::Instance()->GetItemConf(itemid);	
	if(NULL != pItemConf)
	{
		lowRate = pItemConf->AuctionRange.seq[0];
		highRate = pItemConf->AuctionRange.seq[1];
	}

	UINT32 pricehigh= (UINT32)std::ceil(priceRecommend * highRate);
	UINT32 pricelow = (UINT32)std::floor(priceRecommend * lowRate);
	if(perprice < pricelow || perprice > pricehigh )
	{
		LogError("price err perprice = %d low = %d high= %d",perprice,pricelow,pricehigh);
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

int AuctionRequest::ReqOnSaleAdd(UINT32 perprice, const KKSG::Item& itemdata)
{
	auto pSaleItem = AuctionMgr::Instance()->AddSaleItem(m_role->GetID(), AUCT_ADD_ONSALE, perprice, itemdata);

	if(pSaleItem)
	{
		LogOnSale(pSaleItem);
	}

	return KKSG::ERR_SUCCESS;
}

int AuctionRequest::ReqQuitSalePre(UINT64 auctuid, KKSG::Item& itemdata)
{
	AuctionItem* pSaleData = AuctionMgr::Instance()->GetSaleItem(auctuid);
	if(NULL == pSaleData)
	{
		return KKSG::ERR_AUCT_ITEMSALED;
	}
	if(pSaleData->GetRoleID() != m_role->GetID())
	{
		SSWarn << " auctuid = " << pSaleData->GetUID() << " sellerid = " << pSaleData->GetRoleID() << " err roleid = " << m_role->GetID() << END;
		return KKSG::ERR_FAILED;
	}
	if(pSaleData->IsLocked(GameTime::GetTime()))
	{
		SSInfo << " quit sale locked roleid = " << m_role->GetID() << " auctuid = " << pSaleData->GetUID() << END;
		return KKSG::ERR_AUCT_ITEM_LOCK;
	}

	itemdata.CopyFrom(pSaleData->GetItemData());	

	pSaleData->SetLock();

	return KKSG::ERR_SUCCESS;
}

void AuctionRequest::ReqQuitSaleAft(UINT64 auctuid, int ret)
{
	AuctionItem* pSaleData = AuctionMgr::Instance()->GetSaleItem(auctuid);
	if(NULL == pSaleData)
	{
		SSError << " auct = null auctud = " << auctuid << " ret = " << ret << END;
		return; 
	}
	pSaleData->ClearLock();
	if(KKSG::ERR_SUCCESS == ret)
	{
		AuctionMgr::Instance()->DelSaleItem(auctuid, AUCT_DEL_QUITSALE);
		pSaleData = NULL;
	}
}

int AuctionRequest::ReqAnewSale(UINT64 auctuid, UINT32 perprice)
{
	AuctionItem* pSaleData = AuctionMgr::Instance()->GetSaleItem(auctuid);
	if(NULL == pSaleData)
	{
		return KKSG::ERR_AUCT_ITEMSALED;
	}
	if(pSaleData->GetRoleID() != m_role->GetID())
	{
		SSWarn << " auctuid = " << pSaleData->GetUID() << " sellerid = " << pSaleData->GetRoleID() << " err roleid = " << m_role->GetID() << END;
		return KKSG::ERR_FAILED;
	}
	if(pSaleData->IsLocked(GameTime::GetTime()))
	{
		SSInfo << " anew sale locked roleid = " << m_role->GetID() << " auctuid = " << pSaleData->GetUID() << END;
		return KKSG::ERR_AUCT_ITEM_LOCK;
	}
	// 现在要求没有变化也可以上架
	/*if(pSaleData->GetPerPrice() == perprice)
	{
	return KKSG::ERR_AUCT_PRICE_NOTCHANGE;
	}*/

	// 检查价格是否在合理区间
	float priceRecommend = AuctionConfig::Instance()->GetAuctRecPrice(pSaleData->GetItemID());
	if (pSaleData->IsTreasuer())
	{
		priceRecommend = (priceRecommend * GetGlobalConfig().GetFloat("AuctTreasureTJPriceRate",2.0));
	}

	float highRate	= 1.0;
	float lowRate	= 1.0;
	auto pItemConf = CItemConfig::Instance()->GetItemConf(pSaleData->GetItemID());	
	if(NULL != pItemConf)
	{
		lowRate = pItemConf->AuctionRange.seq[0];
		highRate = pItemConf->AuctionRange.seq[1];
	}

	UINT32 pricehigh= (UINT32)std::ceil(priceRecommend * highRate);
	UINT32 pricelow = (UINT32)std::floor(priceRecommend * lowRate);
	if(perprice < pricelow || perprice > pricehigh )
	{
		LogError("price err perprice = %d low = %d high= %d",perprice,pricelow,pricehigh);
		return KKSG::ERR_FAILED;
	}

	//先加 再删
	AuctionMgr::Instance()->AddSaleItem(m_role->GetID(), AUCT_ADD_NEWSALE, perprice, pSaleData->GetItemData());

	AuctionMgr::Instance()->DelSaleItem(pSaleData->GetUID(), AUCT_DEL_NEWSALE);
	pSaleData = NULL;

	return KKSG::ERR_SUCCESS;
}


KKSG::ErrorCode AuctionRequest::ReqAnewSaleCheckItem(UINT64 auctuid)
{
	AuctionItem* pSaleData = AuctionMgr::Instance()->GetSaleItem(auctuid);
	if(NULL == pSaleData)
	{
		return KKSG::ERR_AUCT_ITEMSALED;
	}
	if(pSaleData->GetRoleID() != m_role->GetID())
	{
		SSWarn << " auctuid = " << pSaleData->GetUID() << " sellerid = " << pSaleData->GetRoleID() << " err roleid = " << m_role->GetID() << END;
		return KKSG::ERR_FAILED;
	}
	if(pSaleData->IsLocked(GameTime::GetTime()))
	{
		SSInfo << " quit sale locked roleid = " << m_role->GetID() << " auctuid = " << pSaleData->GetUID() << END;
		return KKSG::ERR_AUCT_ITEM_LOCK;
	}
	return KKSG::ERR_SUCCESS; 
}
int AuctionRequest::ReqMySale(KKSG::AuctionAllReqRes& roRes)
{
	RoleSaleItem* pMySale = AuctionMgr::Instance()->GetRoleSale(m_role->GetID());
	if(NULL == pMySale)
	{
		return KKSG::ERR_SUCCESS;//可以为空
	}

	pMySale->FillSaleItem(roRes);

	//test print
	//for(int i = 0; i < roRes.saledata_size(); ++i)
	//{
	//	const KKSG::AuctionSaleData& data = roRes.saledata(i);
	//	SSDebug << __FUNCTION__ << " auctuid = " << data.uid() << " duelefttime = " << data.duelefttime() << END;
	//}

	return KKSG::ERR_SUCCESS;
}

int AuctionRequest::ReqAllBriefData(KKSG::AuctionAllReqRes& roRes)
{
	ShoppingCart* myCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(NULL == myCart)
	{
		return KKSG::ERR_FAILED;
	}

	if(0 == myCart->GetAutoRefreshLeftTime())
	{
		myCart->Refresh(CART_AUTO);
	}

	myCart->FillAllBriefData(roRes);

	return KKSG::ERR_SUCCESS;
}

int AuctionRequest::ReqOverlapData(UINT32 itemid, KKSG::AuctionAllReqRes& roRes)
{
	ShoppingCart* myCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(NULL == myCart)
	{
		return KKSG::ERR_FAILED;
	}	

	myCart->FillOverlapData(itemid, roRes);

	return KKSG::ERR_SUCCESS;
}

UINT32 AuctionRequest::GetMySaleCount()
{
	return AuctionMgr::Instance()->GetRoleSaleCount(m_role->GetID());
}

UINT32 AuctionRequest::GetMyValidTimeSaleCount()
{
	return AuctionMgr::Instance()->GetRoleValidTimeSaleCount(m_role->GetID());
}

int AuctionRequest::GetTradePrice(UINT32 itemid, KKSG::AuctionAllReqRes& roRes)
{
	auto pRecord = AuctionMgr::Instance()->GetATradeRecord();
	if(NULL == pRecord)
	{
		return 0;
	}
	UINT32 avgprice = pRecord->GetItemAVGPrice(itemid);
	roRes.set_tradeprice(avgprice);

	return KKSG::ERR_SUCCESS;
}

void AuctionRequest::FillBaseData(KKSG::AuctionAllReqRes& roRes)
{
	auto pCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(pCart)
	{
		roRes.set_autorefreshlefttime(pCart->GetAutoRefreshLeftTime());
		roRes.set_freerefreshlefttime(pCart->GetFreeRefreshLeftTime());
		//SSDebug << __FUNCTION__ << " autolefttime = " << roRes.autorefreshlefttime() << " freelefttime = " << roRes.freerefreshlefttime() << END;
		//UINT32 usecount = pCart->GetFreeRefreshCount();
		//UINT32 leftcount = ( GetGlobalConfig().AuctFreeRefreshCount>=usecount)?( GetGlobalConfig().AuctFreeRefreshCount-usecount):0;
		//roRes.set_leftfreerefreshcount(leftcount);
		//SSDebug << __FUNCTION__ << " autorefreshlefttime = " << roRes.autorefreshlefttime() << " usefreerefreshcount = " << roRes.usefreerefreshcount() << END;
	}
}

int AuctionRequest::ReqBuyItemPre(UINT64 overlapid, UINT32 itemid, UINT32 itemcount, KKSG::Item& itemdata, UINT32& perprice, KKSG::AuctionAllReqRes& roRes)
{
	auto pCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(NULL == pCart)
	{
		return KKSG::ERR_FAILED;
	}

	auto pOverlap = pCart->GetOverlap(itemid, overlapid);
	if(NULL == pOverlap)
	{
		SSError << " get overlap failed roleid = " << m_role->GetID() << " itemid = " << itemid << " overlapid = " << overlapid << END;
		return KKSG::ERR_FAILED;
	}

	std::vector<AuctLockTemp> locktmp;
	if(!pOverlap->GetItemCountAuctID(locktmp, itemcount))
	{
		UINT32 leftcount = pOverlap->GetCount();
		roRes.set_itemleftcount(leftcount);	
		SSInfo << " count less roleid = " << m_role->GetID() << " itemid = " << itemid << " overlapid = " << overlapid << " leftcount = " << leftcount << END;
		return KKSG::ERR_AUCT_ITEM_LESS;
	}

	if(!pOverlap->FillItemData(itemdata, itemcount))
	{
		SSError << " fill itemdata failed roleid = " << m_role->GetID() << " itemid = " << itemid << " overlapid = " << overlapid << END;
		return KKSG::ERR_FAILED;
	}
	perprice = pOverlap->GetPreprice();

	if(locktmp.empty())
	{
		SSInfo << " locktmp empty roleid = " << m_role->GetID() << " itemid = " << itemid << " overlapid = " << overlapid << END;
		return KKSG::ERR_FAILED;
	}

	int ret = LockAuct(locktmp);
	if(KKSG::ERR_SUCCESS != ret)
	{
		SSInfo << " add lock fail roleid = " << m_role->GetID() << " itemid = " << itemid << " overlapid = " << overlapid << END;
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}

int AuctionRequest::ReqBuyItemRet(UINT64 overlapid, UINT32 itemid, UINT32 itemcount, std::vector<AuctLockTemp>& locktmp,bool needcopy)
{
	for(size_t i = 0; i < locktmp.size(); ++i)
	{
		UINT64 auctuid = locktmp[i].first;
		UINT32 rcount = locktmp[i].second;
		auto pSaleItem = AuctionMgr::Instance()->GetSaleItem(auctuid);
		if(pSaleItem)
		{
			if (needcopy)
			{
				ReqBuyCopyItem(pSaleItem,rcount);
				// 清理购物车
				auto pCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
				if(pCart)
				{
					auto pOverlap = pCart->GetOverlap(itemid, overlapid);
					if(pOverlap)
					{
						pOverlap->ReduceUnitCount(rcount);
					}
				}

				continue;
			}
			SendSaleMail(pSaleItem, rcount);
			LogOnBuy(pSaleItem, rcount, m_role->GetID());
			//add record
			auto pRecord = AuctionMgr::Instance()->GetATradeRecord();
			if(pRecord && !pSaleItem->IsTreasuer())//珍品不参与
			{
				pRecord->AddOneRecord(itemid, rcount, pSaleItem->GetPerPrice(), pSaleItem->GetUID(), m_role->GetAccount(), pSaleItem->GetRoleAccount());
			}

			UINT32 oldcount = pSaleItem->GetItemCount();
			if(oldcount > rcount)
			{
				AuctionMgr::Instance()->ReduceItemCount(pSaleItem, rcount);
			}
			else
			{
				AuctionMgr::Instance()->DelSaleItem(auctuid, AUCT_DEL_BUY);
				pSaleItem = NULL;
			}
		}
	}

	auto pCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(pCart)
	{
		auto pOverlap = pCart->GetOverlap(itemid, overlapid);
		if(pOverlap)
		{
			pOverlap->CheckDelEmptyUnit();
		}
	}

	return KKSG::ERR_SUCCESS;
}

void AuctionRequest::ReqBuyCopyItem(AuctionItem* pSaleItem, UINT32 salecount)
{	
	KKSG::Item itemdata;
	itemdata.set_itemid(pSaleItem->GetItemID());
	itemdata.set_itemcount(salecount);
	itemdata.set_uid(0);
	auto item_conf = CItemConfig::Instance()->GetItemConf(pSaleItem->GetItemID());	
	if(NULL == item_conf)
	{
		SSError << " find item failed itemid = " << pSaleItem->GetItemID() << END;
		return ;
	}
	itemdata.set_itemtype(item_conf->ItemType);

	time_t reqsaletime = GameTime::GetTime();
	UINT64 uid = AuctionMgr::Instance()->NewUID();
	AuctionItem* saleItem = new AuctionItem(uid, 0, AIS_ON_SALE, pSaleItem->GetPerPrice(), reqsaletime, reqsaletime, itemdata);	
	if(NULL == saleItem)
	{
		return ;
	}
	
	LogOnBuy(saleItem, salecount, m_role->GetID());

	auto pRecord = AuctionMgr::Instance()->GetATradeRecord();
	if(pRecord && !pSaleItem->IsTreasuer())//珍品不参与
	{
		pRecord->AddOneRecord(pSaleItem->GetItemID(), salecount, pSaleItem->GetPerPrice(), 0, m_role->GetAccount(), "0");
	}

	delete saleItem;
}
int AuctionRequest::LockAuct(std::vector<AuctLockTemp>& locktmp)
{
	SSDebug << __FUNCTION__ << " size = " << (int)locktmp.size() << END;
	auto pCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(NULL == pCart)
	{
		return KKSG::ERR_FAILED;
	}
	time_t now = GameTime::GetTime();
	if(pCart->HaveLockTemp(now))
	{
		SSWarn << " try relock roleid = " << m_role->GetID() << END;
		return KKSG::ERR_FAILED;
	}

	for(size_t i = 0; i < locktmp.size(); ++i)
	{
		auto pSaleItem = AuctionMgr::Instance()->GetSaleItem(locktmp[i].first);
		if(pSaleItem && pSaleItem->IsLocked(now))
		{
			SSInfo << " have locked uid = " << pSaleItem->GetUID() << END;
			return KKSG::ERR_AUCT_ITEM_LOCK;
		}
	}

	for(size_t i = 0; i < locktmp.size(); ++i)
	{
		auto pSaleItem = AuctionMgr::Instance()->GetSaleItem(locktmp[i].first);
		if(pSaleItem)
		{
			pSaleItem->SetLock();
		}
		else
		{
			SSError << " find sale item fail uid = " << locktmp[i].first << END;
		}
	}

	pCart->SetLockTemp(locktmp);
	return KKSG::ERR_SUCCESS;
}

void AuctionRequest::UnLockAuct(std::vector<AuctLockTemp>& locktmp)
{
	auto pCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(NULL == pCart)
	{
		SSError << " unlock auct fail roleid = " << m_role->GetID() << END;
		return ;
	}
	pCart->ClearLockTemp(locktmp);

	SSDebug << __FUNCTION__ << " size = " << (int)locktmp.size() << END;

	for(size_t i = 0; i < locktmp.size(); ++i)
	{
		auto pSaleItem = AuctionMgr::Instance()->GetSaleItem(locktmp[i].first);
		if(pSaleItem)
		{
			pSaleItem->ClearLock();
		}
		else
		{
			SSError << " find sale item fail uid = " << locktmp[i].first << END;
		}
	}
}

void AuctionRequest::SendSaleMail(AuctionItem* pSaleItem, UINT32 salecount)
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
}

int AuctionRequest::ReqFreeRefresh(KKSG::AuctionAllReqRes& roRes)
{
	ShoppingCart* myCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(NULL == myCart)
	{
		return KKSG::ERR_FAILED;
	}	
	//if(myCart->GetFreeRefreshCount() >= GetGlobalConfig().AuctFreeRefreshCount)
	//{
	//	return KKSG::ERR_FAILED;
	//}
	if(myCart->GetFreeRefreshLeftTime() > 30) //半分钟误差
	{
		return KKSG::ERR_FAILED;
	}

	myCart->Refresh(CART_FREE);
	//myCart->FillAllBriefData(roRes);

	return KKSG::ERR_SUCCESS;
}

int AuctionRequest::ReqAutoRefresh(KKSG::AuctionAllReqRes& roRes)
{
	auto myCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(NULL == myCart)
	{
		return KKSG::ERR_FAILED;
	}
	if(myCart->GetAutoRefreshLeftTime() > 60)//一分钟误差
	{
		return KKSG::ERR_AUCT_AUTOREFRESH_TIME;
	}

	myCart->Refresh(CART_AUTO);
	//myCart->FillAllBriefData(roRes);
	return KKSG::ERR_SUCCESS;
}

void AuctionRequest::LogOnSale(AuctionItem* pSaleItem)
{
	if(NULL == pSaleItem)
	{
		return ;
	}

	UINT64 roleID = pSaleItem->GetRoleID();
	//职业 等级 vip ---> from rolesummury
	//ip none
	UINT64 auctionUID = pSaleItem->GetUID();//(64位)
	//价格类型:龙币
	UINT32 perprice = pSaleItem->GetPerPrice();//单价
	UINT32 itemcount = pSaleItem->GetItemCount();
	UINT32 itemID = pSaleItem->GetItemID();
	UINT64 itemUID = pSaleItem->GetItemUID();//(64位)
	//道具等级和品质 和ItemID 直接对应 
	UINT32 onsaletime = GetGlobalConfig().AuctOnSaleTime;

	SSDebug << __FUNCTION__ << " auctUID = " << auctionUID << " itemID = " << itemID << END;

	TAuctionStartFlow oLog;
	oLog.m_lRoleUid = pSaleItem->GetRoleID();
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pSaleItem->GetRoleID());
	if (pSum)
	{
		oLog.m_iRoleJobID = pSum->GetProfession();
		oLog.m_iRoleLevel = pSum->GetLevel();
		oLog.m_iUseVIP = pSum->GetVipLevel();
		oLog.m_strOpenId = pSum->GetAccount();
	}
	oLog.m_strUserIP = m_role?m_role->GetIP():"0";
	oLog.m_lAuctionID = pSaleItem->GetUID();
	oLog.m_iAuctionPriceType = 2;
	oLog.m_iAuctionPrice1 = pSaleItem->GetPerPrice();
	oLog.m_iAuctionCount = pSaleItem->GetItemCount();
	oLog.m_iAuctionItemID = pSaleItem->GetItemID();
	oLog.m_lAuctionUUID = pSaleItem->GetItemUID();
	oLog.m_iAuctionTime = INT32(GetGlobalConfig().AuctOnSaleTime/3600);//小时;
	oLog.Do();
}

void AuctionRequest::LogOnBuy(AuctionItem* pSaleItem, UINT32 count, UINT64 buyerID)
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
	oLog.m_strBuyerUserIP = m_role?m_role->GetIP():"0";
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

int AuctionRequest::ReqPayRefresh(KKSG::AuctionAllReqRes& roRes)
{
	auto pCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(pCart)
	{
		pCart->Refresh(CART_PAY);
	}
	return KKSG::ERR_SUCCESS;
}

void AuctionRequest::ReqLoginHandle()
{
	auto myCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(myCart)
	{
		myCart->ResetFreeRefreshOnlineTime(myCart->GetFreeRefreshOnlineTime(), GameTime::GetTime());
	}
}

void AuctionRequest::ReqLogoutHandle()
{
	auto myCart = AuctionMgr::Instance()->GetShopingCart(m_role->GetID());
	if(myCart)
	{
		myCart->GetFreeRefreshOnlineUpdateTime();//更新用
	}
}
