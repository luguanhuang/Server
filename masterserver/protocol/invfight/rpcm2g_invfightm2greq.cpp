#include "pch.h"
#include "invfight/rpcm2g_invfightm2greq.h"

// generate by ProtoGen at date: 2016/12/15 16:49:02

RPC_CLIENT_IMPLEMETION(RpcM2G_InvFightM2GReq, InvFightM2GArg, InvFightM2GRes)

void RpcM2G_InvFightM2GReq::OnReply(const InvFightM2GArg &roArg, const InvFightM2GRes &roRes, const CUserData &roUserData)
{
	UINT32 delayid = roArg.delayid();

	InvFightM2GRes tmpRes;
	tmpRes.CopyFrom(roRes);

	CUserData pUserData;
	pUserData.m_pUserPtr = (void*)(&tmpRes);

	ReplyDelayRpc(delayid, pUserData);
}

void RpcM2G_InvFightM2GReq::OnTimeout(const InvFightM2GArg &roArg, const CUserData &roUserData)
{
}
