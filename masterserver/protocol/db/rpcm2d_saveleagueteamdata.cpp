#include "pch.h"
#include "db/rpcm2d_saveleagueteamdata.h"

// generate by ProtoGen at date: 2016/12/30 10:40:22

RPC_CLIENT_IMPLEMETION(RpcM2D_SaveLeagueTeamData, SaveLeagueTeamDataArg, SaveLeagueTeamDataRes)

void RpcM2D_SaveLeagueTeamData::OnReply(const SaveLeagueTeamDataArg &roArg, const SaveLeagueTeamDataRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() == KKSG::ERR_SUCCESS)
	{
		return ;
	}
	LogError("save to db failed, team[%llu] type[%u] code[%u]", roArg.data().id(), roArg.type(), roRes.result());
}

void RpcM2D_SaveLeagueTeamData::OnTimeout(const SaveLeagueTeamDataArg &roArg, const CUserData &roUserData)
{
	LogWarn("save to db timeout, team[%llu] type[%u]", roArg.data().id(), roArg.type());
}
