#include "pch.h"
#include "dragonguild/rpcm2g_createdragonguildcost.h"

// generate by ProtoGen at date: 2017/9/5 11:50:46

RPC_CLIENT_IMPLEMETION(RpcM2G_CreateDragonGuildCost, CreateDragonGuildCostArg, CreateDragonGuildCostRes)

void RpcM2G_CreateDragonGuildCost::OnReply(const CreateDragonGuildCostArg &roArg, const CreateDragonGuildCostRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	bool iscreate = roArg.iscreate();
	oUser.m_pUserPtr = (void*)&iscreate;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_CreateDragonGuildCost::OnTimeout(const CreateDragonGuildCostArg &roArg, const CUserData &roUserData)
{
}
