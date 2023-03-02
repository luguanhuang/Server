#include "pch.h"
#include "league/rpcc2m_leaguebattlereq.h"
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
#include "other/matchhandler.h"

// generate by ProtoGen at date: 2017/1/6 11:31:11

RPC_SERVER_IMPLEMETION(RpcC2M_LeagueBattleReq, LeagueBattleReqArg, LeagueBattleReqRes)

void RpcC2M_LeagueBattleReq::OnCall(const LeagueBattleReqArg &roArg, LeagueBattleReqRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (!LeagueSeasonMgr::Instance()->IsInPointRaceMatchTime())
	{
		roRes.set_result(KKSG::ERR_LEAGUE_NOT_IN_MATCH_TIME);
		return;
	}
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamByRoleId(pRole->GetID());
	if (pLTeam == NULL)
	{
		roRes.set_result(KKSG::ERR_LEAGUE_HAS_NO_TEAM);
		return;
	}
	if (pRole->GetTeamID() == 0)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	Team* pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
	if (pTeam == NULL || pTeam->GetConf() == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (pTeam->GetConf()->Type != LEAGUETEAM_SCENE)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (pRole->GetID() != pTeam->GetLeader())
	{
		roRes.set_result(KKSG::ERR_TEAM_ONLY_LEADER_CAN_DO);
		return;
	}

	std::vector<UINT64> roleIDs;
	pTeam->GetRoleIDs(roleIDs);
	std::vector<CRole*> roles;

	for (auto it = roleIDs.begin(); it != roleIDs.end(); ++it)
	{
		if (!pLTeam->HasMember(*it))
		{
			roRes.set_result(KKSG::ERR_LEAGUE_HAS_MEMBER_NOT_TEAM);
			return;
		}
		CRole* pTemp = CRoleManager::Instance()->GetByRoleID(*it);
		if (pTemp)
		{
			roles.push_back(pTemp);
		}
	}

	int code = KKSG::ERR_SUCCESS;
	if (roArg.type() == KKSG::LBReqType_Match)
	{
		// 战队匹配中，直接把后来的队伍加入匹配中的队伍
		auto state = pLTeam->GetState();
		if (state == KKSG::LeagueTeamState_Match)
		{
			Team* pMatchTeam = TeamMgr::Instance()->FindTeam(pLTeam->GetMatchTeamId());
			if (pMatchTeam == NULL)
			{
				return;
			}
			// 如果是两个队伍匹配，则把后面的对面直接加入前面那个队伍
			if (pMatchTeam->GetID() != pTeam->GetID())
			{
				HMatchHandler handler;
				handler.CheckTeamStop(pMatchTeam, KKSG::KMT_LEAGUE);
				TeamForceOp op;
				op.LeaveTeam(roles);
				op.JoinTeam(pMatchTeam->GetID(), roles);
				handler.Data().leagueTeamId = pLTeam->GetId();
				handler.Start(pMatchTeam, KKSG::KMT_LEAGUE);
			}
			// 再通知一遍匹配中， 后进来的人也要正确的状态
			//LeagueBattleMatchMgr::Instance()->NotifyStartMatch(pMatchTeam);
		}
		// 战队战斗中， 直接拉进战场
		else if (state == KKSG::LeagueTeamState_Battle)
		{
			pLTeam->EnterBattleScene(roles);	
		}
		else if (state == KKSG::LeagueTeamState_Idle)
		{
			HMatchHandler handler;
			handler.Data().leagueTeamId = pLTeam->GetId();
			code = handler.Start(pTeam, KKSG::KMT_LEAGUE);
		}
	}
	else if (roArg.type() == KKSG::LBReqType_CancelMatch)
	{
		HMatchHandler handler;
		handler.Data().leagueTeamId = pLTeam->GetId();
		handler.Stop(pTeam, KKSG::KMT_LEAGUE);
	}

	roRes.set_result((KKSG::ErrorCode)code);
}

void RpcC2M_LeagueBattleReq::OnDelayReplyRpc(const LeagueBattleReqArg &roArg, LeagueBattleReqRes &roRes, const CUserData &roUserData)
{
}
