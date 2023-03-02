#ifndef __AUCTIONCART_H__
#define __AUCTIONCART_H__

#include "auctionbase.h"

class CRole;
class ATradeRecord;

class SCAuctUnit
{
public:
	SCAuctUnit(UINT64 uid);
	UINT64 GetAuctUid() const { return m_uid; }
	UINT32 GetCount();
	bool GetItemKKSG(KKSG::Item& itemdata);

	bool operator < (const SCAuctUnit& other) const
	{
		return (m_uid&0x00000000ffffffff) > (other.GetAuctUid()&0x00000000ffffffff);
	}

private:
	UINT64 m_uid;
};

//可叠加 单价一样
class SCAuctOverlap
{
public:
	SCAuctOverlap(UINT32 preprice, UINT64 overlapid);

	UINT64 GetOverlapID(){ return m_overlapID; }
	UINT32 GetPreprice(){ return m_preprice; }
	UINT32 GetCount();
	void AddAuctUnit(const SCAuctUnit& au);	
	UINT32 GetSize() { return m_auctUnit.size();}
	void FillOverlapData(KKSG::AuctOverlapData& data, UINT32 count);
	bool FillItemData(KKSG::Item& itemdata, UINT32 count);

	bool GetItemCountAuctID(std::vector<AuctLockTemp>& locktmp, UINT32 count);
	void CheckDelEmptyUnit();
	void ReduceUnitCount(UINT32 item_count);
	void Sort();

	void TestPrint();

private:

	UINT32 m_preprice;	
	UINT64 m_overlapID;
	std::vector<SCAuctUnit> m_auctUnit;//顺序和上架顺序相反
};

typedef std::pair<UINT32,AuctionItem*> SCPAIR;
typedef std::priority_queue<SCPAIR> SCPriQueue;//优先队列

class SCAuctItem
{
public:
	SCAuctItem(UINT32 itemid);

	UINT32 GetCount();
	void RefreshCount();

	void AddTempAuctItem(AuctionItem* pSaleItem);
	UINT32 AddRandAuctItem();
	void AddAuctItem(AuctionItem* pSaleItem);

	void FillBriefData(KKSG::AuctItemBrief& data);
	void FillOverlapData(KKSG::AuctionAllReqRes& roRes);

	SCAuctOverlap* GetOverlap(UINT64 overlapid);
	void CheckDelEmptyOverlap(UINT64 overlapid);

	void TestPrint();

private:
	void TreasurePercentSafe();
	void ClearTempData();

	void TempCountSafe(SCPriQueue& que, INT32& curcount, INT32 expcount);

	INT32 GetNormalExpCount();
	INT32 GetTreasureExpCount();

	SCPriQueue m_tempNormalItem;
	SCPriQueue m_tempTreasureItem;
	INT32 m_tempNormalItemCount;
	INT32 m_tempTreasureItemCount;

	std::vector<SCAuctOverlap> m_auctOverlap;
	UINT32 m_itemID;
	UINT32 m_itemCount;
};

//叶子类别
//class CSAuctType
//{
//public:
//	CSAuctType(int type);
//
//	int GetType(){ return m_type; }
//	void AddAuctItem(AuctionItem* pSaleItem);
//
//private:
//
//	int m_type;
//	std::unordered_map<UINT32,SCAuctItem> m_auctItem;
//};
//

class ShoppingCart
{
public:
	ShoppingCart(UINT64 roleid);
	~ShoppingCart();

	void Update(time_t now);

	UINT64 GetRoleID(){ return m_roleid; }
	UINT32 GetAutoRefreshLeftTime();

	void Refresh(AuctionRefreshType type);

	bool IsEmpty();

	SCAuctOverlap* GetOverlap(UINT32 itemid, UINT64 overlapid);
	void CheckDelEmptyOverlap(UINT32 itemid, UINT64 overlapid);

	void SetLockTemp(std::vector<AuctLockTemp>& locktmp);
	void ClearLockTemp(std::vector<AuctLockTemp>& locktmp);
	bool HaveLockTemp(time_t now);

	void FillAllBriefData(KKSG::AuctionAllReqRes& roRes);
	void FillOverlapData(UINT32 itemid, KKSG::AuctionAllReqRes& roRes);

	UINT32 GetFreeRefreshCount() { return m_freeRefreshCount; }//已经刷了多少次

	inline UINT32 GetFreeRefreshOnlineTime() { return m_freeRefreshOnlineTime; }
	UINT32 GetFreeRefreshOnlineUpdateTime();
	UINT32 GetFreeRefreshLeftTime();
	void ResetFreeRefreshOnlineTime(UINT32 onlineTime, time_t lastUpdateTime);

	UINT32 GetSaleSuccessCount() { return m_saleSuccessCount; }
	void AddSaleSuccessCount() { m_saleSuccessCount++; }

	void OnChangeSeeUnitCount(UINT32 nowSeeCount);

	void TestPrint();
	UINT32 GetConfigTime(const int type);
private:

	bool CanRecommondToMe(AuctionItem* pSaleItem, ATradeRecord* pRecord, CRole* pRole, UINT32 tradeLimit, UINT32 profession);
	void AddAuctItem(AuctionItem* pSaleItem);
	void Clear();

	void AddFreeRefreshCount(){ m_freeRefreshCount++; }
	void ResetFreeRefresh(){ m_freeRefreshCount = 0; }

	void ResetSaleSuccessCount() { m_saleSuccessCount = 0; }

	//std::unordered_map<int,CSAuctType> m_auctType;
	std::unordered_map<UINT32,SCAuctItem> m_auctItem;

	std::vector<AuctLockTemp> m_lockTmp;
	time_t m_lockEndTime;

	UINT64 m_roleid;

	UINT32 m_freeRefreshOnlineTime;
	time_t m_freeRefreshLastUpdateTime;

	time_t m_lastDayUpdateTime;
	UINT32 m_freeRefreshCount;
	time_t m_lastAutoRefreshTime;
	UINT32 m_saleSuccessCount;

	UINT32 m_lastSeeUnitCount;
};

#endif
