#ifndef __AUCTIONREQUEST_H__
#define __AUCTIONREQUEST_H__

#include "auctionbase.h"

namespace KKSG
{
	class Item;
	class AuctionAllReqArg;
	class AuctionAllReqRes;
}

class CRole;
class AuctionRequest
{
public:
	AuctionRequest(CRole* pRole);

	int ReqOnSalePre(UINT32 itemid, UINT32 perprice, bool isTreasure);
	int ReqOnSaleAdd(UINT32 perprice, const KKSG::Item& itemdata);

	int ReqQuitSalePre(UINT64 auctuid, KKSG::Item& itemdata);
	static void ReqQuitSaleAft(UINT64 auctuid, int ret);

	int ReqAnewSale(UINT64 auctuid, UINT32 perprice);
	KKSG::ErrorCode ReqAnewSaleCheckItem(UINT64 auctuid);

	int ReqMySale(KKSG::AuctionAllReqRes& roRes);

	int ReqAllBriefData(KKSG::AuctionAllReqRes& roRes);

	int ReqOverlapData(UINT32 itemid, KKSG::AuctionAllReqRes& roRes);

	int ReqFreeRefresh(KKSG::AuctionAllReqRes& roRes);

	int ReqPayRefresh(KKSG::AuctionAllReqRes& roRes);

	int ReqAutoRefresh(KKSG::AuctionAllReqRes& roRes);

	int ReqBuyItemPre(UINT64 overlapid, UINT32 itemid, UINT32 itemcount, KKSG::Item& itemdata, UINT32& perprice, KKSG::AuctionAllReqRes& roRes);
	int ReqBuyItemRet(UINT64 overlapid, UINT32 itemid, UINT32 itemcount, std::vector<AuctLockTemp>& locktmp,bool needcopy);
	
	void ReqLoginHandle(); 
	void ReqLogoutHandle();

	int LockAuct(std::vector<AuctLockTemp>& locktmp);
	void UnLockAuct(std::vector<AuctLockTemp>& locktmp);

	UINT32 GetMySaleCount();
	UINT32 GetMyValidTimeSaleCount();
	int GetTradePrice(UINT32 itemid, KKSG::AuctionAllReqRes& roRes);

	void FillBaseData(KKSG::AuctionAllReqRes& roRes);

private:

	void SendSaleMail(AuctionItem* pSaleItem, UINT32 salecount);

	void LogOnSale(AuctionItem* pSaleItem);
	void LogOnBuy(AuctionItem* pSaleItem, UINT32 count, UINT64 buyerID);
	void  ReqBuyCopyItem(AuctionItem* pSaleItem, UINT32 salecount);
	CRole* m_role;
};

#endif