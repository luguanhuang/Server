#include "pch.h"
#include "league/ptck2m_leaguematchworldresultntf.h"
#include "leaguebattle/leaguebattlematchmgr.h"

// generate by ProtoGen at date: 2017/2/3 15:18:49

void PtcK2M_LeagueMatchWorldResultNtf::Process(UINT32 dwConnID)
{
	// 匹配超时
	if (m_Data.type() == KKSG::MTWT_DEL)
	{
		LeagueBattleMatchMgr::Instance()->TimeOutFromWorld(m_Data.ltid(), m_Data.teamid());
	}
	// 匹配成功
	else if (m_Data.type() == KKSG::MTWT_MATCH_INWORLD)
	{
		LeagueBattleMatchMgr::Instance()->MatchSuccessFromWorld(m_Data.ltid(), m_Data.teamid(), m_Data.gsline(), m_Data.sceneid());
	}
	else
	{
		LogWarn("invalid match type[%u]", m_Data.type());
	}
}
