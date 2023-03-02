#include "pch.h"
#include "dragonguild/rpcg2m_getdragonguildinfog2m.h"

// generate by ProtoGen at date: 2017/9/6 11:19:23

RPC_CLIENT_IMPLEMETION(RpcG2M_GetDragonGuildInfoG2M, GetPartnerInfoG2MArg, GetPartnerInfoG2MRes)

void RpcG2M_GetDragonGuildInfoG2M::OnReply(const GetPartnerInfoG2MArg &roArg, const GetPartnerInfoG2MRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_pUserPtr = (void*)(&roRes);
	userData.m_dwUserData = KKSG::ERR_SUCCESS;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);	
}

void RpcG2M_GetDragonGuildInfoG2M::OnTimeout(const GetPartnerInfoG2MArg &roArg, const CUserData &roUserData)
{	
}
