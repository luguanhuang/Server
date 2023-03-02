#ifndef __AUCTIONCONFIG_H__
#define __AUCTIONCONFIG_H__

#include "table/AuctionTradeLimitList.h"
#include "table/AuctionDiscountTable.h"
#include "table/AuctSupplement.h"
#include "table/AuctItemRecovery.h"

struct ItemRecoveryData
{
	UINT32 AuctCartItemAllCount;
	UINT32 ItemThreshold;
	UINT32 RecoveryCount;
	int WorldLevelLow;
	int WorldLevelHight;
	ItemRecoveryData()
	{
		AuctCartItemAllCount = 0;
		ItemThreshold = 0;
		RecoveryCount = 0;
		WorldLevelLow = 0;
		WorldLevelHight = 0;
	}
};

class AuctionConfig
{
	AuctionConfig();
	~AuctionConfig();
	DECLARE_SINGLETON(AuctionConfig)
public:
	bool Init();
	void Uninit();

	UINT32 GetTradeLimit();
	float GetAuctRecPrice(UINT32 itemid);

	void GetAuctSupplementByWorldLevel(const UINT32 world_level,std::unordered_map<UINT32,UINT32>& items);

	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	void GetAuctItemRecoveryData(UINT32 item_id,ItemRecoveryData& data,const UINT32 world_level);
private:
	AuctionTradeLimitList m_ATLTable;
	AuctionDiscountTable m_ADTTable;

	AuctSupplement auct_supplement_;
	AuctItemRecovery auct_item_recovery_;

	std::unordered_map<UINT32,ItemRecoveryData> item_revovery_data_;
};

#endif