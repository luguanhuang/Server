#ifndef __AUCTIONBASE_H__
#define __AUCTIONBASE_H__

#include "pb/project.pb.h"
#include "util/gametime.h"
enum AuctionItemState
{
	AIS_DELAY_SALE,
	AIS_ON_SALE,
	AIS_OUT_SALE,
};

enum AuctionRefreshType
{
	CART_FREE,
	CART_AUTO,
	CART_PAY,
};

const UINT32 AuctLockContiTime = 7;//7s

typedef std::pair<UINT64,UINT32> AuctLockTemp;

class AuctionItem
{
public:
	AuctionItem(UINT64 uid, UINT64 roleid, AuctionItemState state, UINT32 perprice, time_t reqsaletime, time_t onsaletime, const KKSG::Item& itemdata);
	AuctionItem(UINT64 uid, UINT64 roleid, const std::string& roleaccount, AuctionItemState state, UINT32 perprice, time_t reqsaletime, time_t onsaletime, UINT32 outsaletime, const KKSG::Item& itemdata);
	~AuctionItem();

	UINT64 GetUID(){ return m_uid; }
	UINT64 GetRoleID(){ return m_roleid; }
	UINT32 GetPerPrice(){ return m_perprice; }
	time_t GetReqSaleTime(){ return m_reqsaletime; }
	time_t GetOnSaleTime(){ return m_onsaletime; }
	std::string GetRoleAccount(){ return m_roleaccount; }

	UINT32 GetDueLeftTime(time_t now);

	AuctionItemState GetState(){ return m_state; }
	void SetState(AuctionItemState st) { m_state = st; }

	void SetOutSaleTime(time_t t){ m_outsaletime = t; }
	time_t GetOutSaleTime(){ return m_outsaletime; }
	UINT32 GetRecoverLeftTime(time_t now);

	UINT32 GetItemID(){ return m_item.itemid(); }
	UINT64 GetItemUID() { return m_item.uid(); }
	UINT32 GetItemCount(){ return m_item.itemcount(); }
	void ReduceItemCount(UINT32 rcount);
	void AddItemCount(UINT32 rcount);
	bool IsTreasuer();//珍品

	const KKSG::Item& GetItemData(){ return m_item; }

	void SetLock();
	void ClearLock();
	bool IsLocked(time_t now);

	void FillSaleItem(KKSG::AuctionSaleData& data);
	void FillDBData(KKSG::AuctDBItemDale& data);

	void TestPrint();

private:

	void SetRoleAccount(UINT64 roleid);

	UINT64 m_uid;
	UINT64 m_roleid;
	std::string m_roleaccount;
	AuctionItemState m_state;
	UINT32 m_perprice;
	time_t m_reqsaletime;
	time_t m_onsaletime;
	time_t m_outsaletime;
	KKSG::Item m_item;

	time_t m_lockEndTime;
};

class RoleSaleItem
{
public:
	RoleSaleItem(UINT64 roleid);

	UINT64 GetRoleID(){ return m_roleid; }

	void AddAuctionItem(AuctionItem* pSaleItem);
	void DelAuctionItem(AuctionItem* pSaleItem);

	// 超时了的不算
	int GetCount() {return (int) m_saleitems.size();}
	int GetValidTimeCount();

	void FillSaleItem(KKSG::AuctionAllReqRes& roRes);

	void TestPrint();

private:
	UINT64 m_roleid;
	std::unordered_set<AuctionItem*> m_saleitems;
};

#endif