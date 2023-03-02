#include "pch.h"
#include "leaguebattle/rpcm2k_leaguereporttop2world.h"
#include "leaguebattle/leagueseasonmgr.h"

// generate by ProtoGen at date: 2017/2/1 20:43:31

RPC_CLIENT_IMPLEMETION(RpcM2K_LeagueReportTop2World, LeagueReportTop2WorldArg, LeagueReportTop2WorldRes)

void RpcM2K_LeagueReportTop2World::OnReply(const LeagueReportTop2WorldArg &roArg, const LeagueReportTop2WorldRes &roRes, const CUserData &roUserData)
{
	LeagueSeasonMgr::Instance()->OnReportResult(roRes.result() == KKSG::ERR_SUCCESS);
}

void RpcM2K_LeagueReportTop2World::OnTimeout(const LeagueReportTop2WorldArg &roArg, const CUserData &roUserData)
{
	LeagueSeasonMgr::Instance()->OnReportResult(false);
}
