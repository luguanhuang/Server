#include "pch.h"
#include "firstpass/rpcg2m_mscommendfirstpass.h"

// generate by ProtoGen at date: 2016/8/24 17:31:58

RPC_CLIENT_IMPLEMETION(RpcG2M_MSCommendFirstPass, MSCommendFirstPassArg, MSCommendFirstPassRes)

void RpcG2M_MSCommendFirstPass::OnReply(const MSCommendFirstPassArg &roArg, const MSCommendFirstPassRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = roRes.error();
	userData.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcG2M_MSCommendFirstPass::OnTimeout(const MSCommendFirstPassArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), roUserData);
}
