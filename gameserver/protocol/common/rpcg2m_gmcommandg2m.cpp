#include "pch.h"
#include "common/rpcg2m_gmcommandg2m.h"

// generate by ProtoGen at date: 2016/8/22 17:04:04

RPC_CLIENT_IMPLEMETION(RpcG2M_GMCommandG2M, GMCmdg2mArg, GMCmdg2mRes)

void RpcG2M_GMCommandG2M::OnReply(const GMCmdg2mArg &roArg, const GMCmdg2mRes &roRes, const CUserData &roUserData)
{
	UINT32 delayid = roArg.delayid();
	GMCmdg2mRes tmp;
	tmp.CopyFrom(roRes);
	CUserData userdata;
	userdata.m_pUserPtr = (void*)(&tmp);
	ReplyDelayRpc(delayid, userdata);	
}

void RpcG2M_GMCommandG2M::OnTimeout(const GMCmdg2mArg &roArg, const CUserData &roUserData)
{
}
