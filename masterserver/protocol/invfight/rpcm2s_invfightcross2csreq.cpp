#include "pch.h"
#include "invfight/rpcm2s_invfightcross2csreq.h"

// generate by ProtoGen at date: 2017/4/13 16:00:04

RPC_CLIENT_IMPLEMETION(RpcM2S_InvFightCross2CSReq, InvFightCross2CSReqArg, InvFightCross2CSReqRes)

void RpcM2S_InvFightCross2CSReq::OnReply(const InvFightCross2CSReqArg &roArg, const InvFightCross2CSReqRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = roRes.error();
	userData.m_pUserPtr = (void*)&roRes;

	CRpc::ReplyDelayRpc(roArg.delayid(), userData);
}

void RpcM2S_InvFightCross2CSReq::OnTimeout(const InvFightCross2CSReqArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	userData.m_pUserPtr = NULL;

	CRpc::ReplyDelayRpc(roArg.delayid(), userData);
}
