#include "pch.h"
#include "league/rpcc2m_leaveleagueteam.h"
#include "role/rolemanager.h"
#include "leagueteam/leagueteam.h"
#include "leagueteam/leagueteammgr.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2017/1/5 20:47:19

RPC_SERVER_IMPLEMETION(RpcC2M_LeaveLeagueTeam, LeaveLeagueTeamArg, LeaveLeagueTeamRes)

void RpcC2M_LeaveLeagueTeam::OnCall(const LeaveLeagueTeamArg &roArg, LeaveLeagueTeamRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	LeagueTeam* team = LeagueTeamMgr::Instance()->GetLeagueTeamByRoleId(pRole->GetID());
	if (team == NULL)
	{
		roRes.set_result(KKSG::ERR_LEAGUE_HAS_NO_TEAM);
		return;
	}
	roRes.set_result(team->LeaveMember(pRole->GetID()));
}

void RpcC2M_LeaveLeagueTeam::OnDelayReplyRpc(const LeaveLeagueTeamArg &roArg, LeaveLeagueTeamRes &roRes, const CUserData &roUserData)
{
}
