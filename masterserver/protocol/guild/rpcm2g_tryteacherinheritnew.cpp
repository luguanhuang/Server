#include "pch.h"
#include "guild/rpcm2g_tryteacherinheritnew.h"

// generate by ProtoGen at date: 2017/6/14 18:38:10

RPC_CLIENT_IMPLEMETION(RpcM2G_TryTeacherInheritNew, TryTeacherInheritNewArg, TryTeacherInheritNewRes)

void RpcM2G_TryTeacherInheritNew::OnReply(const TryTeacherInheritNewArg &roArg, const TryTeacherInheritNewRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	CRpc::ReplyDelayRpc(roArg.rpc_id(), oUser);
}

void RpcM2G_TryTeacherInheritNew::OnTimeout(const TryTeacherInheritNewArg &roArg, const CUserData &roUserData)
{
}
