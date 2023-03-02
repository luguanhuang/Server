#include "pch.h"
#include "login/rpcn2g_notifyroleloginreconnect2gs.h"
#include "crpc.h"

// generate by ProtoGen at date: 2017/6/20 20:07:04

RPC_CLIENT_IMPLEMETION(RpcN2G_NotifyRoleLoginReconnect2Gs, NotifyRoleLoginReconnect2GsArg, NotifyRoleLoginReconnect2GsRes)

void RpcN2G_NotifyRoleLoginReconnect2Gs::OnReply(const NotifyRoleLoginReconnect2GsArg &roArg, const NotifyRoleLoginReconnect2GsRes &roRes, const CUserData &roUserData)
{
	CUserData userData(roRes.result(), NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcN2G_NotifyRoleLoginReconnect2Gs::OnTimeout(const NotifyRoleLoginReconnect2GsArg &roArg, const CUserData &roUserData)
{
	CUserData userData(KKSG::ERR_TIMEOUT, NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
