#include "pch.h"
#include "common/rpcg2k_registergs2world.h"
#include "config.h"
#include "network/rslink.h"
#include "network/worldlink.h"

// generate by ProtoGen at date: 2016/10/27 10:22:45

RPC_CLIENT_IMPLEMETION(RpcG2K_RegisterGs2World, RegisterGs2WorldArg, RegisterGs2WorldRes)

void RpcG2K_RegisterGs2World::OnReply(const RegisterGs2WorldArg &roArg, const RegisterGs2WorldRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Game server line [%u] register to world success", roArg.line());
		if (roRes.line_list_size() != roRes.address_list_size())
		{
			LogError("line size not equal addr size");
			return;
		}
		for (INT32 i = 0; i < roRes.line_list_size(); ++i)
		{
			UINT32 line = roRes.line_list(i);
			RSLink::Instance()->AddToConnect(roRes.line_list(i), roRes.address_list(i).ip(), roRes.address_list(i).port());
		}
		if (roRes.is_master())
		{
			LogInfo("world id[%u] connId[%u]", roRes.serverid(), m_dwConnID);
			WorldLink::Instance()->SetMasterWorldConnId(m_dwConnID);
		}
	}
	else
	{
		LogError("Game server line [%u] register to world failed", roArg.line());
	}
}

void RpcG2K_RegisterGs2World::OnTimeout(const RegisterGs2WorldArg &roArg, const CUserData &roUserData)
{
	LogError("Game server line [%u] register to world timeout", roArg.line());
}
