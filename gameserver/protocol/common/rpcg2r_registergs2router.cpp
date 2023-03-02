#include "pch.h"
#include "common/rpcg2r_registergs2router.h"
#include "network/rslink.h"
#include "servermgr/servermgr.h"

// generate by ProtoGen at date: 2016/10/27 10:20:42

RPC_CLIENT_IMPLEMETION(RpcG2R_RegisterGs2Router, RegisterGs2RouterArg, RegisterGs2RouterRes)

void RpcG2R_RegisterGs2Router::OnReply(const RegisterGs2RouterArg &roArg, const RegisterGs2RouterRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Register To Router Success");
		RSLink::Instance()->AddRouterServer(m_dwConnID, roRes.line());
		for (int i = 0; i < roRes.servers_size(); ++i)
		{
			ServerMgr::Instance()->AddServer(m_dwConnID, roRes.servers(i));
		}
	}
	else
	{
		LogError("Register To Router Failed!");
	}
}

void RpcG2R_RegisterGs2Router::OnTimeout(const RegisterGs2RouterArg &roArg, const CUserData &roUserData)
{
	LogError("Register To Router Timeout");
}
