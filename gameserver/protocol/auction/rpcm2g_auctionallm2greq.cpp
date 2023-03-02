#include "pch.h"
#include "auction/rpcm2g_auctionallm2greq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "auction/auctionrequest.h"

// generate by ProtoGen at date: 2016/8/27 17:37:25

RPC_SERVER_IMPLEMETION(RpcM2G_AuctionAllM2GReq, AuctionReqM2GArg, AuctionReqM2GRes)

void RpcM2G_AuctionAllM2GReq::OnCall(const AuctionReqM2GArg &roArg, AuctionReqM2GRes &roRes)
{
	const AuctionAllReqArg& c2mArg = roArg.c2marg();
	AuctionAllReqRes& c2mRes = *roRes.mutable_c2mres();

	Role* pRole = RoleManager::Instance()->FindBySession(roArg.sessionid());
	if(NULL == pRole)
	{
		c2mRes.set_errcode(ERR_FAILED);	
		return ;
	}
	ErrorCode errcode = ERR_SUCCESS;
	AuctionRequest req(pRole);
	switch(c2mArg.reqtype())
	{
	case ART_REQSALE:
		{
			errcode = (ErrorCode)req.ReqOnSale(c2mArg.itemuniqueid(), c2mArg.itemid(), c2mArg.itemcount(), c2mArg.perprice(), roArg.mysalecount(), c2mArg.istreasure(), roRes);
		}
		break;
	case ART_RESALE:
		{
			errcode = (ErrorCode)req.ReqAnewSale(roArg.mysalecount());
		}
		break;
	case ART_QUITSALE:
		{
			errcode = (ErrorCode)req.ReqQuitSale(roArg.itemdata());
		}
		break;
	case ART_REFRESH_PAY:
		{
			errcode = (ErrorCode)req.ReqRefreshPay();	
		}
		break;
	case ART_BUY:
		{
			bool need_copyitem = false;
			errcode = (ErrorCode)req.ReqBuyItem(roArg.itemdata(), roArg.perprice(),need_copyitem);	
			c2mRes.set_need_copyitem(need_copyitem);
		}
		break;
	default:
		break;
	}

	c2mRes.set_errcode(errcode);
}

void RpcM2G_AuctionAllM2GReq::OnDelayReplyRpc(const AuctionReqM2GArg &roArg, AuctionReqM2GRes &roRes, const CUserData &roUserData)
{
}
