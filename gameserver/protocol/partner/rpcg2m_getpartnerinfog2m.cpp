#include "pch.h"
#include "partner/rpcg2m_getpartnerinfog2m.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolepartner.h"

// generate by ProtoGen at date: 2016/12/10 22:00:34

RPC_CLIENT_IMPLEMETION(RpcG2M_GetPartnerInfoG2M, GetPartnerInfoG2MArg, GetPartnerInfoG2MRes)

void RpcG2M_GetPartnerInfoG2M::OnReply(const GetPartnerInfoG2MArg &roArg, const GetPartnerInfoG2MRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_pUserPtr = (void*)(&roRes);
	userData.m_dwUserData = KKSG::ERR_SUCCESS;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);	
}

void RpcG2M_GetPartnerInfoG2M::OnTimeout(const GetPartnerInfoG2MArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_pUserPtr = NULL;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);	
}
