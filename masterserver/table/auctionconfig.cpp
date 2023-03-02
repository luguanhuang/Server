#include "pch.h"
#include "auctionconfig.h"
#include "levelseal/levelsealMgr.h"
#include "ItemConfigMgr.h"
#include "util/XRandom.h"
#include "table/globalconfig.h"


INSTANCE_SINGLETON(AuctionConfig)

	AuctionConfig::AuctionConfig()
{
	item_revovery_data_.clear();
}

AuctionConfig::~AuctionConfig()
{

}

bool AuctionConfig::Init()
{
	return LoadFile();
}

void AuctionConfig::Uninit()
{

}

void AuctionConfig::ClearFile()
{
	m_ATLTable.Clear();
	m_ADTTable.Clear();
	auct_supplement_.Clear();
	auct_item_recovery_.Clear();
	item_revovery_data_.clear();
}

bool AuctionConfig::CheckFile()
{
	AuctionTradeLimitList tempATLTable;
	if(!tempATLTable.LoadFile("table/AuctionTradeLimit.txt"))
	{
		SSError << "load AuctionTradeLimt.txt failed " << END;		
		return false;
	}
	AuctionDiscountTable tempADTTable;
	if(!tempADTTable.LoadFile("table/AuctionDiscount.txt"))
	{
		SSError << "load AuctionDiscount.txt failed " << END;		
		return false;
	}

	AuctSupplement tmpRewrad;
	if(!tmpRewrad.LoadFile("table/AuctSupplement.txt"))
	{
		SSWarn << " load AuctSupplement.txt failed " << END;
		return false;
	}

	AuctItemRecovery tmpRecovery;
	if(!tmpRecovery.LoadFile("table/AuctItemRecovery.txt"))
	{
		SSWarn << " load AuctItemRecovery.txt failed " << END;
		return false;
	}

	return true;
}

bool AuctionConfig::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	if(!m_ATLTable.LoadFile("table/AuctionTradeLimit.txt"))
	{
		SSError << "load AuctionTradeLimt.txt failed " << END;		
		return false;
	}

	if(!m_ADTTable.LoadFile("table/AuctionDiscount.txt"))
	{
		SSError << "load AuctionDiscount.txt failed " << END;		
		return false;
	}

	if(!auct_supplement_.LoadFile("table/AuctSupplement.txt"))
	{
		SSError << "load AuctSupplement.txt failed " << END;		
		return false;
	}

	if(!auct_item_recovery_.LoadFile("table/AuctItemRecovery.txt"))
	{
		SSWarn << " load AuctItemRecovery.txt failed " << END;
		return false;
	}

	for (auto it = auct_item_recovery_.Table.begin(); it != auct_item_recovery_.Table.end(); ++it)
	{
		ItemRecoveryData data;
		data.AuctCartItemAllCount = (*it)->AuctCartItemAllCount;
		data.RecoveryCount = (*it)->RecoveryCount;
		data.ItemThreshold = (*it)->ItemThreshold;
		data.WorldLevelLow = (*it)->WorldLevel.seq[0];
		data.WorldLevelHight = (*it)->WorldLevel.seq[1];
		item_revovery_data_.insert(std::make_pair((*it)->ItemID,data));
	}
	return true;
}

UINT32 AuctionConfig::GetTradeLimit()
{
	// 这个并非是封印类型，而是另外独立配置了等级
	UINT32 tmpLevel = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel;
	UINT32 tmpLimit = 0;
	for(size_t i = 0; i < m_ATLTable.Table.size(); ++i)
	{
		auto pConf = m_ATLTable.Table[i];
		tmpLimit = pConf->TradeLimit;
		if(pConf->SealLevel[0] <= tmpLevel && tmpLevel <= pConf->SealLevel[1])
		{
			break;
		}
	}
	return tmpLimit;
}

float AuctionConfig::GetAuctRecPrice(UINT32 itemid)
{
	auto pItemConf = CItemConfig::Instance()->GetItemConf(itemid);	
	if(NULL == pItemConf)
	{
		SSError << " find item failed itemid = " << itemid << END;
		return 1.0;
	}
	float tmpPrice = float(pItemConf->AuctPriceRecommend);
	UINT32 sealtype = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	for(size_t i = 0; i < m_ADTTable.Table.size(); ++i)
	{
		auto pConf = m_ADTTable.Table[i];
		if(pConf->Type == sealtype && pConf->Group == pItemConf->AuctionGroup)
		{
			tmpPrice = (tmpPrice*pConf->Discount);
			break;
		}
	}
	return tmpPrice;
}

void AuctionConfig::GetAuctSupplementByWorldLevel(
	const UINT32 world_level,
	std::unordered_map<UINT32,UINT32>& items)
{
	auto it		= auct_supplement_.Table.begin();
	auto it_end = auct_supplement_.Table.end();
	for (; it != it_end; ++it)
	{
		if (world_level == (*it)->WorldLevel)
		{			
			UINT32 little = (*it)->NumberRange.seq[0];
			UINT32 big =((*it)->NumberRange.seq[1]);
			UINT32 number = little;
			if (little < big)
			{
				number = XRandom::randInt(little,big);
			}
			if (number > GetGlobalConfig().AuctCartItemAllCount)
			{
				number = GetGlobalConfig().AuctCartItemAllCount;
			}

			items.insert(std::make_pair((*it)->ItemID,number));
		}
	}
}

void AuctionConfig::GetAuctItemRecoveryData(UINT32 item_id,ItemRecoveryData& data,const UINT32 world_level)
{
	auto it = item_revovery_data_.find(item_id);
	if (it != item_revovery_data_.end())
	{
		if ((it->second.WorldLevelLow == 0 &&it->second.WorldLevelHight == 0)  || (world_level >= it->second.WorldLevelLow  && world_level <= it->second.WorldLevelHight) )
		{
			data.AuctCartItemAllCount = it->second.AuctCartItemAllCount;
			data.RecoveryCount = it->second.RecoveryCount;
			data.ItemThreshold = it->second.ItemThreshold;
		}
	}
}
