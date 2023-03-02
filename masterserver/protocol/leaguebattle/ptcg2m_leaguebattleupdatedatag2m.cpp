#include "pch.h"
#include "leaguebattle/ptcg2m_leaguebattleupdatedatag2m.h"
#include "leagueteam/leagueteammgr.h"

// generate by ProtoGen at date: 2017/1/12 10:11:54

void PtcG2M_LeagueBattleUpdateDataG2M::Process(UINT32 dwConnID)
{
	LeagueTeam* team = LeagueTeamMgr::Instance()->GetLeagueTeamById(m_Data.league_teamid());
	if (team == NULL)
	{
		LogWarn("can't find leagueteam[%llu]", m_Data.league_teamid());
		return;
	}

	LogDebug("Update league battle state[%llu]", m_Data.league_teamid());

	// 通知准备结束
	if (m_Data.has_is_ready_end() && m_Data.is_ready_end())
	{
		team->SetBattleReadyEnd(m_Data.sceneid());
	}

	// 通知战斗结束
	if (m_Data.has_is_battle_end() && m_Data.is_battle_end())
	{
		team->SetState(KKSG::LeagueTeamState_Idle);
	}

	// 对战记录
	if (m_Data.has_record())
	{
		team->GetLeagueTeamBattleRecord().AddBattleRecord(m_Data.record());
	}
}
