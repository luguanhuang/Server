#include "pch.h"
#include "marriage/rpcm2g_checkmarriagecouple.h"

// generate by ProtoGen at date: 2017/7/26 19:59:05

RPC_CLIENT_IMPLEMETION(RpcM2G_CheckMarriageCouple, CheckMarriageCoupleArg, CheckMarriageCoupleRes)

void RpcM2G_CheckMarriageCouple::OnReply(const CheckMarriageCoupleArg &roArg, const CheckMarriageCoupleRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = roRes.error();
	data.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.delayrpc(), data);
}

void RpcM2G_CheckMarriageCouple::OnTimeout(const CheckMarriageCoupleArg &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.delayrpc(), data);
}
