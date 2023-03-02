#include "pch.h"
#include "league/rpcc2m_joinleagueelebattle.h"
#include "role/rolemanager.h"
#include "leagueteam/leagueteammgr.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammatchmgr.h"
#include "leaguebattle/leaguebattlematchmgr.h"
#include "other/matchhandler.h"
#include "team/teammember.h"
#include "team/teamrequest.h"
#include "team/teamforceop.h"
#include "leaguebattle/leagueseasonmgr.h"

// generate by ProtoGen at date: 2017/1/14 15:20:24

RPC_SERVER_IMPLEMETION(RpcC2M_JoinLeagueEleBattle, JoinLeagueEleBattleArg, JoinLeagueEleBattleRes)

void RpcC2M_JoinLeagueEleBattle::OnCall(const JoinLeagueEleBattleArg &roArg, JoinLeagueEleBattleRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamByRoleId(pRole->GetID());
	if (pLTeam == NULL)
	{
		roRes.set_result(KKSG::ERR_LEAGUE_HAS_NO_TEAM);
		return;
	}
	KKSG::LeagueTeamState state = pLTeam->GetState();
	if (state != KKSG::LeagueTeamState_Battle)
	{
		roRes.set_result(KKSG::ERR_LEAGUE_TEAM_NOT_IN_BATTLE);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);

	std::vector<CRole*> roles;
	roles.push_back(pRole);
	pLTeam->EnterBattleScene(roles);
}

void RpcC2M_JoinLeagueEleBattle::OnDelayReplyRpc(const JoinLeagueEleBattleArg &roArg, JoinLeagueEleBattleRes &roRes, const CUserData &roUserData)
{
}
