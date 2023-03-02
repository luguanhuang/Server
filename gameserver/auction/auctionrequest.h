#ifndef __AUCTIONREQUEST_H__
#define __AUCTIONREQUEST_H__

#include "pb/project.pb.h"

namespace KKSG
{
	class AuctionReqM2GRes;
	class Item;
}

class Role;

class AuctionRequest
{
public:
	AuctionRequest(Role* pRole);

	int ReqOnSale(UINT64 itemuid, UINT32 itemid, UINT32 itemcount, UINT32 perprice, UINT32 mysalecount, bool isTreasure, KKSG::AuctionReqM2GRes& m2gRes);
	int ReqQuitSale(const KKSG::Item& itemdata);
	int ReqRefreshPay();
	int ReqAnewSale(UINT32 mysalecount);

	int ReqBuyItem(const KKSG::Item& itemdata, UINT32 perprice,bool& needcopy);

	UINT32 GetMaxSaleCount();

private:
	int SaleCost();
	bool CheckTradeCondition(UINT32 item_id);
	Role* m_role;
};

#endif
