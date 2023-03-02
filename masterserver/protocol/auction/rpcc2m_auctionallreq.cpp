#include "pch.h"
#include "auction/rpcc2m_auctionallreq.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "auction/rpcm2g_auctionallm2greq.h"
#include "network/gslink.h"
#include "auction/auctionrequest.h"
#include "network/dblink.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"

// generate by ProtoGen at date: 2016/8/27 17:37:01

RPC_SERVER_IMPLEMETION(RpcC2M_AuctionAllReq, AuctionAllReqArg, AuctionAllReqRes)

void RpcC2M_AuctionAllReq::OnCall(const AuctionAllReqArg &roArg, AuctionAllReqRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		return;
	}

	if(!DBLink::Instance()->IsConnected())//db err
	{
		SSWarn << " db connect err " << END;
		roRes.set_errcode(ERR_FAILED);
		return;
	}

	// 系统开放
	if (!pRole->IsSystemOpened(SYS_AUCTION) || OpenSystemMgr::Instance()->IsClosedSystem(SYS_AUCTION))
	{	
		roRes.set_errcode(KKSG::ERR_SYS_NOTOPEN);
		return ;
	}
	
	KKSG::Item itemdata;// use for quit sale/buy
	UINT32 perprice = 0;// use for buy

	bool gotogs = true;
	ErrorCode errcode = ERR_SUCCESS;
	AuctionRequest req(pRole);
	switch(roArg.reqtype())
	{
	case ART_REQSALE:
		{
			errcode = (ErrorCode)req.ReqOnSalePre(roArg.itemid(), roArg.perprice(), roArg.istreasure());
		}
		break;
	case ART_QUITSALE:
		{
			errcode = (ErrorCode)req.ReqQuitSalePre(roArg.auctuid(), itemdata);
		}
		break;
	case ART_RESALE:
		{
			errcode = req.ReqAnewSaleCheckItem(roArg.auctuid());
		}
		break;
	case ART_BUY:
		{
			errcode = (ErrorCode)req.ReqBuyItemPre(roArg.overlapid(), roArg.itemid(), roArg.itemcount(), itemdata, perprice, roRes);
		}
		break;
	case ART_MYSALE:
		{
			errcode = (ErrorCode)req.ReqMySale(roRes);
			gotogs = false;
		}
		break;
	case ART_ALLITEMBRIEF:
		{
			errcode = (ErrorCode)req.ReqAllBriefData(roRes);
			gotogs = false;
		}
		break;
	case ART_ITEMDATA:
		{
			errcode = (ErrorCode)req.ReqOverlapData(roArg.itemid(), roRes);
			gotogs = false;
		}
		break;
	case ART_TRADE_PRICE:
		{
			errcode = (ErrorCode)req.GetTradePrice(roArg.itemid(), roRes);
			gotogs = false;
		}
		break;
	case ART_REFRESH_FREE:
		{
			errcode = (ErrorCode)req.ReqFreeRefresh(roRes);
			gotogs = false;
		}
		break;
	case ART_REFRESH_AUTO:
		{
			errcode = (ErrorCode)req.ReqAutoRefresh(roRes);
			gotogs = false;
		}
		break;
	default:
		break;
	}

	if(ERR_SUCCESS != errcode || !gotogs)
	{
		req.FillBaseData(roRes);
		roRes.set_errcode(errcode);
	}
	else
	{
		RpcM2G_AuctionAllM2GReq *rpc = RpcM2G_AuctionAllM2GReq::CreateRpc();
		UINT32 delayid = DelayRpc();
		rpc->m_oArg.set_delayid(delayid);
		rpc->m_oArg.set_sessionid(m_sessionID);		
		if(ART_QUITSALE == roArg.reqtype())
		{
			rpc->m_oArg.mutable_itemdata()->CopyFrom(itemdata);
		}
		else if(ART_BUY == roArg.reqtype())
		{
			rpc->m_oArg.mutable_itemdata()->CopyFrom(itemdata);
			rpc->m_oArg.set_perprice(perprice);
		}
		if ( ART_RESALE == roArg.reqtype())
		{
			rpc->m_oArg.set_mysalecount(req.GetMyValidTimeSaleCount());
		}
		else
		{
			rpc->m_oArg.set_mysalecount(req.GetMySaleCount());
		}
		rpc->m_oArg.mutable_c2marg()->CopyFrom(m_oArg);
		GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
	}
}

void RpcC2M_AuctionAllReq::OnDelayReplyRpc(const AuctionAllReqArg &roArg, AuctionAllReqRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		roRes.set_errcode(ERR_FAILED);
		return ;
	}

	AuctionReqM2GRes* m2gRes = (AuctionReqM2GRes*)(roUserData.m_pUserPtr);
	roRes.CopyFrom(m2gRes->c2mres());

	if(roArg.reqtype() == ART_QUITSALE)//下架
	{
		AuctionRequest::ReqQuitSaleAft(roArg.auctuid(), roRes.errcode());
	}

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		return ;
	}
	
	AuctionRequest req(pRole);

	std::vector<AuctLockTemp> locktmp;
	if(roArg.reqtype() == ART_BUY)
	{
		req.UnLockAuct(locktmp);
	}

	if(ERR_SUCCESS != roRes.errcode())
	{
		req.FillBaseData(roRes);
		return;
	}

	ErrorCode errcode = ERR_SUCCESS;
	switch(roArg.reqtype())
	{
	case ART_RESALE:
		{
			errcode = (ErrorCode)req.ReqAnewSale(roArg.auctuid(), roArg.perprice());
		}
		break;
	case ART_REQSALE:
		{
			errcode = (ErrorCode)req.ReqOnSaleAdd(roArg.perprice(), m2gRes->itemdata());
		}
		break;
	case ART_REFRESH_PAY:
		{
			//errcode = (ErrorCode)req.ReqPayRefresh(roRes);
		}
		break;
	case ART_BUY:
		{
			errcode = (ErrorCode)req.ReqBuyItemRet(roArg.overlapid(), roArg.itemid(), roArg.itemcount(), locktmp,roRes.need_copyitem());
		}
		break;
	default:
		break;
	}

	req.FillBaseData(roRes);
	roRes.set_errcode(errcode);
}
