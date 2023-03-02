#include "pch.h"
#include "guild/rpcm2g_modifygsguildname.h"

// generate by ProtoGen at date: 2017/4/11 22:31:33

RPC_CLIENT_IMPLEMETION(RpcM2G_ModifyGsGuildName, ModifyFwArg, ModifyFwRes)

void RpcM2G_ModifyGsGuildName::OnReply(const ModifyFwArg &roArg, const ModifyFwRes &roRes, const CUserData &roUserData)
{
	UINT64 roleid = roArg.roleid();
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.error();
	oUser.m_pUserPtr = (void*)&roleid;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_ModifyGsGuildName::OnTimeout(const ModifyFwArg &roArg, const CUserData &roUserData)
{
}
