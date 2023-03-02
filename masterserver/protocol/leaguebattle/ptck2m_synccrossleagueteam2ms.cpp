#include "pch.h"
#include "leaguebattle/ptck2m_synccrossleagueteam2ms.h"
#include "leagueteam/crossleagueteammgr.h"

// generate by ProtoGen at date: 2017/2/2 15:09:13

void PtcK2M_SyncCrossLeagueTeam2MS::Process(UINT32 dwConnID)
{
	for (int i = 0; i < m_Data.teams_size(); ++i)
	{
		CrossLeagueTeamMgr::Instance()->UpdateTeam(m_Data.teams(i));
	}
}
