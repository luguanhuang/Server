#ifndef __AUCTIONMGR_H__
#define __AUCTIONMGR_H__

#include "timer.h"

#include "auctionbase.h"

class ShoppingCart;
class ATradeRecord;
class AuctDbHandler;

enum AuctAddType
{
	AUCT_ADD_ONSALE,
	AUCT_ADD_NEWSALE,
	AUCT_ADD_SUPPSALE,
};

enum AuctDelType
{
	AUCT_DEL_QUITSALE,
	AUCT_DEL_NEWSALE,
	AUCT_DEL_SUPPSALE,
	AUCT_DEL_BUY,
	AUCT_DEL_TIMEOVER,
};

struct ItemSailItemInfo
{
	UINT32 ItemCount;
	std::list<AuctionItem*> itemList;
};
class AuctionMgr : public ITimer
{
	AuctionMgr();
	~AuctionMgr();
	DECLARE_SINGLETON(AuctionMgr)

public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void AddDBSaleItem(UINT64 uid, UINT64 roleid, AuctionItemState state, UINT32 perprice, time_t reqsaletime, time_t onsaletime, const std::string& itemstr, const std::string& roleaccount, UINT32 outsaletime);

	AuctionItem* AddSaleItem(UINT64 roleid, AuctAddType type, UINT32 perprice, const KKSG::Item& itemdata, bool supplement = false);
	void DelSaleItem(UINT64 uid, AuctDelType type);
	AuctionItem* GetSaleItem(UINT64 uid);

	void SetState(AuctionItem* pSaleItem, AuctionItemState state);
	void ReduceItemCount(AuctionItem* pSaleItem, UINT32 rcount);

	int GetRoleSaleCount(UINT64 roleid);

	int GetRoleValidTimeSaleCount(UINT64 roleid);
	RoleSaleItem* GetRoleSale(UINT64 roleid);

	ShoppingCart* GetShopingCart(UINT64 roleid);

	std::unordered_map<UINT64, AuctionItem*>& GetAllSaleItem(){ return m_allSaleItem; }

	ATradeRecord* GetATradeRecord(){ return m_ATradeRecord; }
	AuctDbHandler* GetDBHandler(){ return m_DBHandler; }

	UINT64 NewOverlapID();

	void TestPrintAllSaleItem();
	void TestPrintMySaleItem(UINT64 roleid);
	void TestPrintMyShoppingCart(UINT64 roleid);

	// 检查补货的时间
	bool CheckAuctSupplementTime();
	// 补货
	void AuctSupplement();
	void SupplementAddItem(const KKSG::Item & item);

	// profiler
	// 物品总量
	int GetAllSaleCount();
	int GetUseCartCount();
	int GetAllSeeUnitCount();
	void OnChangeSeeUnitCount(UINT32 lastSeeCount, UINT32 nowSeeCount);
	UINT64 NewUID();
private:
	void InitRecoveryItemEvent();
	bool SetRecoveryItemEvent();
	void SendSaleMail(AuctionItem* pSaleItem, UINT32 salecount);
	void LogOnBuy(AuctionItem* pSaleItem, UINT32 count, UINT64 buyerID);
	void BuyItemResult(AuctionItem* pSaleItem, UINT32 salecount);
private:
	void Add(AuctionItem* pSaleItem);
	void Del(AuctionItem* pSaleItem);
	void AddRoleSale(UINT64 roleid, AuctionItem* pSaleItem);
	void DelRoleSale(UINT64 roleid, AuctionItem* pSaleItem);

	void UpdateSaleItemState();
	void UpdateShoppingCart();

	void DoTxLog(AuctionItem* pSaleItem, INT32 op);

	
	void UpdateIDIndex(UINT64 uid);
	time_t RandDelayTime();

	std::unordered_map<UINT64, RoleSaleItem*> m_roleSaleItem;
	std::unordered_map<UINT64, AuctionItem*> m_allSaleItem; 

	std::unordered_map<UINT64, ShoppingCart*> m_shoppingCart;

	ATradeRecord* m_ATradeRecord;
	AuctDbHandler* m_DBHandler;

	UINT64 m_SaleItemInex;//重启服务器需要维护
	UINT64 m_OverlapIndex;

	HTIMER m_handler;
	
	UINT32 start_time_	;
	UINT32 end_time_	;
	UINT32 auctSupplementFrequency_;
	UINT32 frequency_count_;

	UINT32 m_useCartCount;
	UINT32 m_allSeeUnitCount;

	HTIMER recovery_item_handler_;
	UINT32 last_update_time_;
};

#endif