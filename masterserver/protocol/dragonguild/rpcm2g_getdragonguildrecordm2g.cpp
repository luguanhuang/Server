#include "pch.h"
#include "dragonguild/rpcm2g_getdragonguildrecordm2g.h"

// generate by ProtoGen at date: 2017/9/6 10:33:09

RPC_CLIENT_IMPLEMETION(RpcM2G_GetDragonGuildRecordM2G, GetPartnerRecordM2GArg, GetPartnerRecordM2GRes)

void RpcM2G_GetDragonGuildRecordM2G::OnReply(const GetPartnerRecordM2GArg &roArg, const GetPartnerRecordM2GRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_SUCCESS;
	userData.m_pUserPtr = (void*)(&roRes);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcM2G_GetDragonGuildRecordM2G::OnTimeout(const GetPartnerRecordM2GArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
