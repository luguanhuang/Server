#include "pch.h"
#include "db/rpcm2d_skyteamdata2db.h"

// generate by ProtoGen at date: 2017/3/30 10:36:10

RPC_CLIENT_IMPLEMETION(RpcM2D_SkyTeamData2DB, SkyTeamData2DBArg, SkyTeamData2DBRes)

void RpcM2D_SkyTeamData2DB::OnReply(const SkyTeamData2DBArg &roArg, const SkyTeamData2DBRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() == KKSG::ERR_SUCCESS)
	{
		return ;
	}
	LogError("save to db failed, team[%llu] type[%u] code[%u]", roArg.data().id(), roArg.type(), roRes.result());
}

void RpcM2D_SkyTeamData2DB::OnTimeout(const SkyTeamData2DBArg &roArg, const CUserData &roUserData)
{
	LogWarn("save to db timeout, team[%llu] type[%u]", roArg.data().id(), roArg.type());
}
