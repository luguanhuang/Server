#include "pch.h"
#include "auction/rpcm2g_auctionallm2greq.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "auction/auctionrequest.h"

// generate by ProtoGen at date: 2016/8/27 17:37:25

RPC_CLIENT_IMPLEMETION(RpcM2G_AuctionAllM2GReq, AuctionReqM2GArg, AuctionReqM2GRes)

void RpcM2G_AuctionAllM2GReq::OnReply(const AuctionReqM2GArg &roArg, const AuctionReqM2GRes &roRes, const CUserData &roUserData)
{
	UINT32 delayid = roArg.delayid();

	AuctionReqM2GRes tmpRes;
	tmpRes.CopyFrom(roRes);

	CUserData pUserData;
	pUserData.m_pUserPtr = (void*)(&tmpRes);

	ReplyDelayRpc(delayid, pUserData);

}

void RpcM2G_AuctionAllM2GReq::OnTimeout(const AuctionReqM2GArg &roArg, const CUserData &roUserData)
{
}
