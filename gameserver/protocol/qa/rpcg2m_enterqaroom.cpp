#include "pch.h"
#include "qa/rpcg2m_enterqaroom.h"

// generate by ProtoGen at date: 2016/10/8 14:05:24

RPC_CLIENT_IMPLEMETION(RpcG2M_EnterQARoom, EnterQARoomReq, EnterQARoomRes)

void RpcG2M_EnterQARoom::OnReply(const EnterQARoomReq &roArg, const EnterQARoomRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = roRes.result();
	data.m_pUserPtr = (void*)(&roRes);
	CRpc::ReplyDelayRpc(roArg.rpc_id(), data);
}

void RpcG2M_EnterQARoom::OnTimeout(const EnterQARoomReq &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.rpc_id(), data);
}
