#include "pch.h"
#include "qa/rpcg2m_createguildqaroom.h"

// generate by ProtoGen at date: 2016/10/8 16:07:14

RPC_CLIENT_IMPLEMETION(RpcG2M_CreateGuildQARoom, CreateGuildQARoomReq, CreateGuildQARoomRes)

void RpcG2M_CreateGuildQARoom::OnReply(const CreateGuildQARoomReq &roArg, const CreateGuildQARoomRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = roRes.result();
	data.m_pUserPtr = (void*)(&roRes);
	CRpc::ReplyDelayRpc(roArg.rpc_id(), data);
}

void RpcG2M_CreateGuildQARoom::OnTimeout(const CreateGuildQARoomReq &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.rpc_id(), data);
}
