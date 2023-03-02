#include "pch.h"
#include "league/rpcc2m_getleaguebattlerecord.h"
#include "leagueteam/leagueteammgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "leaguebattle/leagueseasonmgr.h"

// generate by ProtoGen at date: 2017/1/13 0:10:42

RPC_SERVER_IMPLEMETION(RpcC2M_GetLeagueBattleRecord, GetLeagueBattleRecordArg, GetLeagueBattleRecordRes)

void RpcC2M_GetLeagueBattleRecord::OnCall(const GetLeagueBattleRecordArg &roArg, GetLeagueBattleRecordRes &roRes)
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
	roRes.set_result(KKSG::ERR_SUCCESS);

	LeagueTeamBattleRecord& record = team->GetLeagueTeamBattleRecord();
	auto& allRecords = record.GetRecords();
	for (auto it = allRecords.begin(); it != allRecords.end(); ++it)
	{
		roRes.add_records()->CopyFrom(*it);
	}

	// 非跨服阶段显示本服积分和记录
	if (!LeagueSeasonMgr::Instance()->IsInCrossState())
	{
		roRes.set_winnum(record.GetTotalWinNum());
		roRes.set_losenum(record.GetTotalLoseNum());
		roRes.set_winrate(record.GetTotalWinRate());
		roRes.set_max_continuewin(record.GetMaxContinueWin());
		roRes.set_max_continuelose(record.GetMaxContinueLose());
	}
	// 跨服阶段显示跨服积分和记录
	else
	{
		roRes.set_winnum(record.GetCrossWinNum());
		roRes.set_losenum(record.GetCrossLoseNum());
		roRes.set_winrate(record.GetCrossWinRate());
		roRes.set_max_continuewin(record.GetCrossMaxContinueWin());
		roRes.set_max_continuelose(record.GetCrossMaxContinueLose());
	}
}

void RpcC2M_GetLeagueBattleRecord::OnDelayReplyRpc(const GetLeagueBattleRecordArg &roArg, GetLeagueBattleRecordRes &roRes, const CUserData &roUserData)
{
}
