#include "pch.h"
#include "common/rpcm2k_registerms2world.h"
#include "cross/crosszonestatus.h"
#include "network/worldlink.h"
#include "network/rslink.h"
#include "other/servercrossdatamgr.h"

// generate by ProtoGen at date: 2016/10/27 10:26:30

RPC_CLIENT_IMPLEMETION(RpcM2K_RegisterMs2World, RegisterMs2WorldArg, RegisterMs2WorldRes)

void RpcM2K_RegisterMs2World::OnReply(const RegisterMs2WorldArg &roArg, const RegisterMs2WorldRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Register To World Success, serverId[%u] is_master[%u] gs_num[%u]", roRes.serverid(), roRes.is_master(), roRes.gs_num());
		if (roRes.is_master())
		{
			CrossZoneStatus::Instance()->SetWorldStatus(true);
			CrossZoneStatus::Instance()->SetCrossGsNum(roRes.gs_num());
			WorldLink::Instance()->SetMasterWorldConnId(m_dwConnID);
			RSLink::Instance()->QueryRouterFromWorld();
		}
		ServerCrossDataSyn::SynDataToWorld(m_dwConnID);
	}
	else
	{
		LogError("Register To World Failed!");
	}
}

void RpcM2K_RegisterMs2World::OnTimeout(const RegisterMs2WorldArg &roArg, const CUserData &roUserData)
{
	LogError("Register To World Timeout");
}
