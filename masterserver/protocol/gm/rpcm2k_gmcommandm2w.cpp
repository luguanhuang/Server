#include "pch.h"
#include "gm/rpcm2k_gmcommandm2w.h"

// generate by ProtoGen at date: 2017/2/4 21:15:05

RPC_CLIENT_IMPLEMETION(RpcM2K_GMCommandM2W, GMCmdg2mArg, GMCmdg2mRes)

void RpcM2K_GMCommandM2W::OnReply(const GMCmdg2mArg &roArg, const GMCmdg2mRes &roRes, const CUserData &roUserData)
{
	UINT32 delayid = roArg.delayid();
	GMCmdg2mRes tmp;
	tmp.CopyFrom(roRes);
	CUserData userdata;
	userdata.m_pUserPtr = (void*)(&tmp);
	ReplyDelayRpc(delayid, userdata);	
}

void RpcM2K_GMCommandM2W::OnTimeout(const GMCmdg2mArg &roArg, const CUserData &roUserData)
{
}
