#include "pch.h"
#include "leaguebattle/rpcg2m_leaguerankupdateg2m.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/leagueranklist.h"
#include "leagueteam/leagueteammgr.h"
#include "leagueteam/leagueteambattlerecord.h"
#include "timeutil.h"

// generate by ProtoGen at date: 2017/1/12 10:19:39

RPC_SERVER_IMPLEMETION(RpcG2M_LeagueRankUpdateG2M, LeagueRankUpdateG2MArg, LeagueRankUpdateG2MRes)

void RpcG2M_LeagueRankUpdateG2M::OnCall(const LeagueRankUpdateG2MArg &roArg, LeagueRankUpdateG2MRes &roRes)
{
	if (roArg.league_teamid_size() != roArg.point_size() 
		|| roArg.league_teamid_size() != roArg.total_num_size()
		|| roArg.league_teamid_size() != roArg.win_num_size())
	{
		LogError("error");
		return;
	}

	LeagueRankList* ranklist = CRankListMgr::Instance()->GetLeagueRankList();
	int n = roArg.league_teamid_size();

	UINT32 now = TimeUtil::GetTime();
	// 获取老的排名
	for (int i = 0; i < n; ++i)
	{
		UINT64 teamId = roArg.league_teamid(i);
		roRes.add_league_teamid(teamId);
		roRes.add_lastrank(ranklist->GetRank(teamId));
	}

	// 更新胜负场次和排名
	for (int i = 0; i < n; ++i)
	{
		UINT64 teamId = roArg.league_teamid(i);
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(teamId);
		if (pLTeam == NULL)
		{
			continue;
		}

		UINT32 score = roArg.point(i);
		bool isWin = (teamId == roArg.win_teamid()) ? true : false;
		pLTeam->SetScore(score);
		LeagueTeamBattleRecord& record = pLTeam->GetLeagueTeamBattleRecord();

		if (isWin)
		{
			record.AddWinNum();
		}
		else
		{
			record.AddLoseNum();
		}
		ranklist->OnChange(teamId, score, record.GetTotalWinRate(), now, record.GetMaxContinueWin(), record.GetTotalNum());
	}

	// 获取当前排名
	for (int i = 0; i < n; ++i)
	{
		UINT64 teamId = roArg.league_teamid(i);
		roRes.add_currank(ranklist->GetRank(teamId));
	}

	roRes.set_allrank(ranklist->GetSize());
}

void RpcG2M_LeagueRankUpdateG2M::OnDelayReplyRpc(const LeagueRankUpdateG2MArg &roArg, LeagueRankUpdateG2MRes &roRes, const CUserData &roUserData)
{
}
