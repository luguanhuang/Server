#include "pch.h"
#include "leaguebattle/ptck2m_leaguecrossrewardntf.h"
#include "leagueteam/leagueteammgr.h"

// generate by ProtoGen at date: 2017/2/6 16:33:13

void PtcK2M_LeagueCrossRewardNtf::Process(UINT32 dwConnID)
{
	if (m_Data.has_rankreward())
	{
		LeagueTeamMgr::Instance()->SendRankReward(m_Data.rankreward().ltid(), m_Data.rankreward().rank(), true);
	}

	if (m_Data.has_elireward())
	{
		LeagueTeamMgr::Instance()->SendEliRoundReward(m_Data.elireward().round(), m_Data.elireward().ltid(),
												 	  m_Data.elireward().iswin(), true);
	}
}
