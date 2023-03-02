#include "pch.h"
#include "dragonguild/rpcg2m_getdragonguildtaskchestg2m.h"

// generate by ProtoGen at date: 2017/9/11 14:37:17

RPC_CLIENT_IMPLEMETION(RpcG2M_GetDragonGuildTaskChestG2M, GetDragonGuildTaskChestG2MArg, GetDragonGuildTaskChestG2MRes)

void RpcG2M_GetDragonGuildTaskChestG2M::OnReply(const GetDragonGuildTaskChestG2MArg &roArg, const GetDragonGuildTaskChestG2MRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_pUserPtr = (void*)(&roRes);
	userData.m_dwUserData = KKSG::ERR_SUCCESS;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);	
}

void RpcG2M_GetDragonGuildTaskChestG2M::OnTimeout(const GetDragonGuildTaskChestG2MArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_pUserPtr = NULL;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);	
}
