#include "pch.h"
#include "league/rpcc2m_getleagueteaminfo.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "leagueteam/leagueteammgr.h"
#include "leagueteam/crossleagueteammgr.h"
#include "leaguebattle/leagueseasonmgr.h"

// generate by ProtoGen at date: 2017/1/4 15:25:09

RPC_SERVER_IMPLEMETION(RpcC2M_GetLeagueTeamInfo, GetLeagueTeamInfoArg, GetLeagueTeamInfoRes)

void RpcC2M_GetLeagueTeamInfo::OnCall(const GetLeagueTeamInfoArg &roArg, GetLeagueTeamInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	UINT64 leageTeamId = roArg.league_teamid();
	// 根据联赛阶段判断是本服战队还是跨服战队
	// 本服战队
	if (!LeagueSeasonMgr::Instance()->IsInCrossState())
	{
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(leageTeamId);
		if (pLTeam == NULL)
		{
			roRes.set_result(KKSG::ERR_LEAGUE_TEAM_NOT_EXIST);
			return;
		}
		roRes.set_result(KKSG::ERR_SUCCESS);
		pLTeam->FillLeagueTeamDetail(*roRes.mutable_team());
	}
	// 跨服战队
	else
	{
		CrossLeagueTeam* pLTeam = CrossLeagueTeamMgr::Instance()->GetTeam(leageTeamId);
		if (pLTeam == NULL)
		{
			roRes.set_result(KKSG::ERR_LEAGUE_TEAM_NOT_EXIST);
			return;
		}
		roRes.set_result(KKSG::ERR_SUCCESS);
		CrossLeagueTeamMgr::Instance()->FillLeagueTeamDetail(pLTeam, *roRes.mutable_team());
	}
}

void RpcC2M_GetLeagueTeamInfo::OnDelayReplyRpc(const GetLeagueTeamInfoArg &roArg, GetLeagueTeamInfoRes &roRes, const CUserData &roUserData)
{
}
