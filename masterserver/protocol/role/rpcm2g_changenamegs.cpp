#include "pch.h"
#include "role/rpcm2g_changenamegs.h"

// generate by ProtoGen at date: 2016/10/10 17:40:30

RPC_CLIENT_IMPLEMETION(RpcM2G_ChangeNameGs, ChangeNameGsArg, ChangeNameGsRes)

void RpcM2G_ChangeNameGs::OnReply(const ChangeNameGsArg &roArg, const ChangeNameGsRes &roRes, const CUserData &roUserData)
{
	UINT64 roleid = roArg.roleid();
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.errorcode();
	oUser.m_pUserPtr = (void*)&roleid;
	CRpc::ReplyDelayRpc(roArg.delayrpcid(), oUser);
}

void RpcM2G_ChangeNameGs::OnTimeout(const ChangeNameGsArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)(KKSG::ERR_TIMEOUT);
	CRpc::ReplyDelayRpc(roArg.delayrpcid(), oUser);
}
