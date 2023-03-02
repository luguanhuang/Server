#include "pch.h"
#include "skycraft/rpcc2m_skycraftmatchreq.h"
#include "role/rolemanager.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammatchmgr.h"
#include "other/matchhandler.h"
#include "team/teammember.h"
#include "team/teamrequest.h"
#include "team/teamforceop.h"
#include "skyteam/skyteam.h"
#include "skyteam/skyteammgr.h"

// generate by ProtoGen at date: 2017/4/1 11:56:04

RPC_SERVER_IMPLEMETION(RpcC2M_SkyCraftMatchReq, SkyCraftMatchReq, SkyCraftMatchRes)

void RpcC2M_SkyCraftMatchReq::OnCall(const SkyCraftMatchReq &roArg, SkyCraftMatchRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	/*
	if (!LeagueSeasonMgr::Instance()->IsInPointRaceMatchTime())
	{
		roRes.set_result(KKSG::ERR_LEAGUE_NOT_IN_MATCH_TIME);
		return;
	}
	*/
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamByRoleId(pRole->GetID());
	if (pSTeam == NULL)
	{
		roRes.set_result(KKSG::ERR_SKY_HAS_NO_TEAM);
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

	if (pTeam->GetConf()->Type != SKYCRAFT_SCENE) 
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
		if (!pSTeam->HasMember(*it))
		{
			roRes.set_result(KKSG::ERR_SKY_HAS_NOT_TEAM_MEMBER);
			return;
		}
		CRole* pTemp = CRoleManager::Instance()->GetByRoleID(*it);
		if (pTemp)
		{
			roles.push_back(pTemp);
		}
	}

	int code = KKSG::ERR_SUCCESS;
	if (roArg.type() == KKSG::SCMR_Match)
	{
		HMatchHandler handler;
		handler.Data().stId = pSTeam->GetId();
		code = handler.Start(pTeam, KKSG::KMT_SKYCRAFT);
	}
	else if (roArg.type() == KKSG::SCMR_CancelMatch)
	{
		HMatchHandler handler;
		handler.Data().stId = pSTeam->GetId();
		handler.Stop(pTeam, KKSG::KMT_SKYCRAFT);
	}
	roRes.set_result((KKSG::ErrorCode)code);
}

void RpcC2M_SkyCraftMatchReq::OnDelayReplyRpc(const SkyCraftMatchReq &roArg, SkyCraftMatchRes &roRes, const CUserData &roUserData)
{
}
