#include "pch.h"
#include "math.h"
#include "auctionrequest.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "table/globalconfig.h"
#include "gamelogic/payv2Mgr.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/rolesummarymgr.h"
#include "tsssdk/TssHandler.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/payconfig.h"
#include "gamelogic/platformshare_record.h"
#include "util/gametime.h"
#include "utility/uidhelper.h"
#include "gamelogic/pkrecord.h"

AuctionRequest::AuctionRequest(Role* pRole)
	:m_role(pRole)
{

}

int AuctionRequest::ReqOnSale(UINT64 itemuid, UINT32 itemid, UINT32 itemcount, UINT32 perprice, UINT32 mysalecount, bool isTreasure, KKSG::AuctionReqM2GRes& m2gRes)
{
	Bag* bag = m_role->Get<Bag>();
	if(NULL == bag)
	{
		return KKSG::ERR_AUCT_COMMONERR;
	}
	XItem* item = bag->FindUniqueItemInBag(itemuid);
	if(NULL == item)
	{
		return KKSG::ERR_AUCT_COMMONERR;
	}
	if(itemid != item->itemID)
	{
		return KKSG::ERR_AUCT_COMMONERR;
	}
	if(item->IsBind())
	{
		SSError << "onsale bind item" << END;
		return KKSG::ERR_AUCT_COMMONERR;
	}
	if(!item->IsCoolDown())
	{
		SSError << " item is cool down " << END;
		return KKSG::ERR_AUCT_COMMONERR;
	}
	if((UINT32)item->itemCount < itemcount)
	{
		SSError << "my item count = " << item->itemCount << " req count = " << itemcount << END;
		return KKSG::ERR_AUCT_COMMONERR;
	}

	if(item->IsTreasure() != isTreasure)//服务器和客户端判断珍品不统一
	{
		SSError << " isTreasure failed server = " << (int)(item->IsTreasure()) << " client = " << (int)(isTreasure) << END;
		return KKSG::ERR_AUCT_COMMONERR;
	}

	if(mysalecount >= GetMaxSaleCount())
	{
		SSError << " sale count max mysalecount = " << mysalecount << END;
		return KKSG::ERR_AUCT_ONSALEMAX;
	}

	int ret = SaleCost();
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	XItem::Item2KKSGItem(item , m2gRes.mutable_itemdata());
	m2gRes.mutable_itemdata()->set_itemcount(itemcount);

	BagTakeItemTransition take(m_role);
	take.SetReason(ItemFlow_Auction, ItemFlow_Auction_Take);
	take.TakeUniqueItem(itemuid, itemcount);// 没有扣除失败回滚
	take.NotifyClient();

	TAuctionFlow oLog(m_role);
	oLog.m_nItemID = itemid;
	oLog.m_nItemCount = itemcount;
	oLog.m_nPrice = itemcount * perprice;
	oLog.m_nOperType = TX_AUCTION_SELL;
	oLog.szItemName = ItemConfig::Instance()->GetItemName(itemid);
	oLog.Do();
	return KKSG::ERR_SUCCESS;

}

int AuctionRequest::ReqQuitSale(const KKSG::Item& itemdata)
{
	XItem item;
	XItem::KKSGItem2Item(&item , &itemdata);
	BagGiveItemTransition give(m_role);
	give.SetReason(ItemFlow_Auction, ItemFlow_Auction_GetBack);
	give.GiveItem(&item);
	give.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

int AuctionRequest::ReqBuyItem(const KKSG::Item& itemdata, UINT32 perprice,bool& needcopy)
{
	if(0 == itemdata.itemid() || 0 == itemdata.itemcount() || 0 == perprice)
	{
		SSError << " item info err itemid = " << itemdata.itemid() << " itemcount = " << itemdata.itemcount() << " perprice = " << perprice << END;
		return KKSG::ERR_FAILED;
	}

	ItemConf* pConf = ItemConfig::Instance()->GetItemConf(itemdata.itemid());
	if (NULL == pConf)
	{
		SSWarn<<"item id:"<<itemdata.itemid()<<" not find "<<END;
		return KKSG::ERR_FAILED;;
	}
	UINT32 price_recommend = pConf->AuctPriceRecommend;

	UINT32 allprice = perprice * itemdata.itemcount();

	UINT32 BagDragonCoinCount = m_role->Get<Bag>()->CountVirtualItem(DRAGON_COIN);
	if(BagDragonCoinCount < allprice)
	{
		return KKSG::ERR_AUCT_DRAGONCOINLESS;
	}

	UINT32 needDragonCoin = (BagDragonCoinCount>=allprice)?allprice:BagDragonCoinCount;

	//不做扣除失败处理
	BagTakeItemTransition take(m_role);
	take.SetReason(ItemFlow_Auction, ItemFlow_Auction_Take);
	if(needDragonCoin)
	{
		take.TakeItem(DRAGON_COIN, needDragonCoin);
	}
	take.NotifyClient();

	XItem item;
	XItem::KKSGItem2Item(&item , &itemdata);
	
	/*UINT32 cooldownt = GameTime::GetTime();
	UINT32 cooldownt_time = cooldownt + ( GetGlobalConfig().AuctNormalItemCoolDownDay * XCommon::GetOneDayTime());

	auto it = GetGlobalConfig().AuctItemCoolDownDay.begin();
	auto it_end = GetGlobalConfig().AuctItemCoolDownDay.end();
	for (; it != it_end; ++it)
	{
	if (price_recommend >= it->seq[0] && price_recommend <= it->seq[1])
	{
	cooldownt_time = cooldownt + (it->seq[2]* XCommon::GetOneDayTime());
	}
	}

	item.SetCoolDown(cooldownt_time);*/
	item.SetBind(true);
	BagGiveItemTransition give(m_role);
	give.SetReason(ItemFlow_Auction, ItemFlow_Auction_Give);
	// check item condion
	needcopy = CheckTradeCondition(item.itemID);
	if (needcopy)
	{
		item.uid =  UIDHelper::CreateUID(UID_Item);
	}
	give.GiveItem(&item);
	give.NotifyClient();

	m_role->OnAuction();

	TAuctionFlow oLog(m_role);
	oLog.m_nItemID = item.itemID;
	oLog.m_nItemCount = item.itemCount;
	oLog.m_nPrice = allprice;
	oLog.m_nOperType = TX_AUCTION_BUY;
	oLog.szItemName = ItemConfig::Instance()->GetItemName(item.itemID);
	oLog.Do();
	return KKSG::ERR_SUCCESS;
}

UINT32 AuctionRequest::GetMaxSaleCount()
{
	CPayV2Record* pr = m_role->Get<CPayV2Record>();
	if(pr)
	{
		UINT32 pay_add_count_ = 0;
		if(pr->GetExpireTimeByID(pay_member_3) > 0)
		{
			PayMemberTable::RowData* pRow = CPayConfig::Instance()->GetPayMemberByID(pay_member_3);
			if (NULL != pRow)
			{
				pay_add_count_ = pRow->AuctionCount;
			}			 
		}

		VIPTable::RowData* pRow = PayV2Mgr::Instance()->GetVipConf(pr->GetVipLevel());
		if(pRow)
		{
			return pRow->AuctionOnSaleMax + pay_add_count_;
		}
	}
	return 0;
}

int AuctionRequest::ReqRefreshPay()
{
	if(m_role->Get<Bag>()->CountVirtualItem(DRAGON_COIN) <  GetGlobalConfig().AuctRefreshPayCount)
	{
		return KKSG::ERR_AUCT_DRAGONCOINLESS;
	}

	BagTakeItemTransition take(m_role);
	take.SetReason(ItemFlow_Auction, ItemFlow_Auction_Take);
	take.TakeItem(DRAGON_COIN,  GetGlobalConfig().AuctRefreshPayCount);
	take.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

int AuctionRequest::ReqAnewSale(UINT32 mysalecount)
{
	int ret = SaleCost();
	if(KKSG::ERR_SUCCESS != ret)
	{
		SSError << " pay not enough roleid = " << m_role->GetID() << END;
	}
	// 重新上架
	if(mysalecount > GetMaxSaleCount())
	{
		SSError << " sale count max mysalecount = " << mysalecount << END;
		return KKSG::ERR_AUCT_ONSALEMAX;
	}

	return KKSG::ERR_SUCCESS;//一直返回成功
}

int AuctionRequest::SaleCost()
{
	INT32 onSaleCostGold =  GetGlobalConfig().GetInt("AuctOnSaleCostGold",20);
	if(m_role->Get<Bag>()->CountVirtualItem(GOLD) < onSaleCostGold)
	{
		return KKSG::ERR_AUCTGOLDLESS;
	}	

	BagTakeItemTransition take(m_role);
	take.SetReason(ItemFlow_Auction, ItemFlow_Auction_Take);
	take.TakeItem(GOLD, onSaleCostGold);
	take.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

bool AuctionRequest::CheckTradeCondition(UINT32 item_id)
{	
	// 创角大于7天
	// 近7天日均消耗
	// ---上周天梯分数
	// ---上周英雄战场场次
	if ( GameTime::GetTime() - m_role->GetCreateTime()  <= 7*XCommon::GetOneDayTime())
	{
		return false;
	}
	// 需要防刷的物品
	bool  auct_prevent = ItemConfig::Instance()->AuctItemPrevent(item_id);
	if (!auct_prevent)
	{
		return false;
	}

	UINT32 average_consume = m_role->Get<PlatformShareRecord>()->GetAverageConsume7Day();
	if (average_consume >=  GetGlobalConfig().AverageConsume7Day)
	{
		return false;
	}

	UINT32  PKLastWeekPoint = GetGlobalConfig().GetInt("PKLastWeekPoint" , 0);
	if (m_role->Get<CPkRecord>()->GetLastWeekPoint() >= PKLastWeekPoint)
	{
		return false;
	}

	return true;
}