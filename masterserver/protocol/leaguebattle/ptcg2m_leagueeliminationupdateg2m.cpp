#include "pch.h"
#include "leaguebattle/ptcg2m_leagueeliminationupdateg2m.h"
#include "leaguebattle/leagueeliminationmgr.h"

// generate by ProtoGen at date: 2017/1/14 14:05:14

void PtcG2M_LeagueEliminationUpdateG2M::Process(UINT32 dwConnID)
{
	// 观战信息产生
	if (m_Data.has_liveid())
	{
		LeagueEliminationMgr::Instance()->SetLiveId(m_Data.roomid(), m_Data.sceneid(), m_Data.liveid());
	}

	// 战斗结束
	if (m_Data.has_win_leagueid())
	{
		LeagueEliminationMgr::Instance()->OnBattleResult(m_Data.roomid(), m_Data.sceneid(), m_Data.win_leagueid());
	}
}
