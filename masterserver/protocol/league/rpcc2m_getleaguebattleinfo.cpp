#include "pch.h"
#include "league/rpcc2m_getleaguebattleinfo.h"
#include "role/rolemanager.h"
#include "leagueteam/leagueteam.h"
#include "leagueteam/leagueteammgr.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "util/gametime.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/leagueranklist.h"
#include "ranklist/crossleagueranklist.h"

// generate by ProtoGen at date: 2017/1/4 15:11:50

RPC_SERVER_IMPLEMETION(RpcC2M_GetLeagueBattleInfo, GetLeagueBattleInfoArg, GetLeagueBattleInfoRes)

void RpcC2M_GetLeagueBattleInfo::OnCall(const GetLeagueBattleInfoArg &roArg, GetLeagueBattleInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	roRes.set_today_state(LeagueSeasonMgr::Instance()->GetTodayState());
	roRes.set_timestamp(GameTime::GetTime());
	roRes.set_matchlefttime(LeagueSeasonMgr::Instance()->GetMatchLeftTime());
	roRes.set_rankreward_lefttime(LeagueSeasonMgr::Instance()->GetRankRewardLeftTime());
	roRes.set_crossrankreward_lefttime(LeagueSeasonMgr::Instance()->GetCrossRankRewardLeftTime());
	roRes.set_eli_type(LeagueSeasonMgr::Instance()->GetEliType());
	roRes.set_is_cross(LeagueSeasonMgr::Instance()->IsInCrossState());
	roRes.set_rank(INVALID_RANK);
	LeagueTeam* team = LeagueTeamMgr::Instance()->GetLeagueTeamByRoleId(pRole->GetID());

	if (team)
	{
		roRes.set_league_teamid(team->GetId());
		roRes.set_league_teamname(team->GetName());
		std::vector<LeagueTeamMember>& members = team->GetAllMember();
		for (auto it = members.begin(); it != members.end(); ++it)
		{
			it->FillMemberDetail(*roRes.add_member());
		}

		LeagueTeamBattleRecord& record = team->GetLeagueTeamBattleRecord();
		// 非跨服阶段显示本服积分和记录
		if (!LeagueSeasonMgr::Instance()->IsInCrossState())
		{
			roRes.set_score(team->GetScore());
			roRes.set_battlenum(record.GetTotalNum());
			roRes.set_winrate(record.GetTotalWinRate());
			UINT32 rank = CRankListMgr::Instance()->GetLeagueRankList()->GetRank(team->GetId());
			if (rank != INVALID_RANK) rank--;
			roRes.set_rank(rank);
		}
		// 跨服阶段显示跨服积分和记录
		else
		{
			roRes.set_score(team->GetCrossScore());
			roRes.set_battlenum(record.GetCrossTotalNum());
			roRes.set_winrate(record.GetCrossWinRate());
			UINT32 rank = CRankListMgr::Instance()->GetCrossLeagueRankList()->GetRank(team->GetId());
			if (rank != INVALID_RANK) rank--;
			roRes.set_rank(rank);
		}
	}
}

void RpcC2M_GetLeagueBattleInfo::OnDelayReplyRpc(const GetLeagueBattleInfoArg &roArg, GetLeagueBattleInfoRes &roRes, const CUserData &roUserData)
{
}
