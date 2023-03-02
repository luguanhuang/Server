#include "pch.h"
#include "pvp/rpcg2m_pvpallg2mreq.h"

// generate by ProtoGen at date: 2016/8/19 19:50:14

RPC_CLIENT_IMPLEMETION(RpcG2M_PvpAllG2MReq, PvpG2MArg, PvpG2MRes)

void RpcG2M_PvpAllG2MReq::OnReply(const PvpG2MArg &roArg, const PvpG2MRes &roRes, const CUserData &roUserData)
{
	//SSDebug << __FUNCTION__ << " delayid = " << roArg.delayid() << END;
	PvpRes tmp;
	tmp.CopyFrom(roRes.c2gres());
	CUserData data;
	data.m_pUserPtr = (void*)(&tmp);

	ReplyDelayRpc(roArg.delayid(), data);
}

void RpcG2M_PvpAllG2MReq::OnTimeout(const PvpG2MArg &roArg, const CUserData &roUserData)
{
	SSDebug << __FUNCTION__ << " delayid = " << roArg.delayid() << END;
}
