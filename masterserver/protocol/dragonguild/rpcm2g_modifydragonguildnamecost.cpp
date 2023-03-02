#include "pch.h"
#include "dragonguild/rpcm2g_modifydragonguildnamecost.h"

// generate by ProtoGen at date: 2017/9/8 10:45:33

RPC_CLIENT_IMPLEMETION(RpcM2G_ModifyDragonGuildNameCost, ModifyDragonNameCostArg, ModifyDragonNameCostRes)

void RpcM2G_ModifyDragonGuildNameCost::OnReply(const ModifyDragonNameCostArg &roArg, const ModifyDragonNameCostRes &roRes, const CUserData &roUserData)
{
	UINT64 roleid = roArg.roleid();
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	oUser.m_pUserPtr = (void *)&roleid;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_ModifyDragonGuildNameCost::OnTimeout(const ModifyDragonNameCostArg &roArg, const CUserData &roUserData)
{
}
