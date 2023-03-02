#include "pch.h"
#include "dragonguild/rpcm2g_getdragonguildrecieveinfo.h"

// generate by ProtoGen at date: 2017/9/12 21:36:52

RPC_CLIENT_IMPLEMETION(RpcM2G_GetDragonGuildRecieveInfo, GetDragonGuildRecieveInfoArg, GetDragonGuildRecieveInfoRes)

void RpcM2G_GetDragonGuildRecieveInfo::OnReply(const GetDragonGuildRecieveInfoArg &roArg, const GetDragonGuildRecieveInfoRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_SUCCESS;
	userData.m_pUserPtr = (void*)(&roRes);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcM2G_GetDragonGuildRecieveInfo::OnTimeout(const GetDragonGuildRecieveInfoArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
