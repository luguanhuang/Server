#include "pch.h"
#include "guild/rpcm2g_createguildcost.h"

// generate by ProtoGen at date: 2016/8/26 17:30:59

RPC_CLIENT_IMPLEMETION(RpcM2G_CreateGuildCost, CreateGuildCostArg, CreateGuildCostRes)

void RpcM2G_CreateGuildCost::OnReply(const CreateGuildCostArg &roArg, const CreateGuildCostRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.errorcode();
	bool isCreate = roArg.iscreate(); 
	oUser.m_pUserPtr = (void*)&isCreate;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_CreateGuildCost::OnTimeout(const CreateGuildCostArg &roArg, const CUserData &roUserData)
{
}
