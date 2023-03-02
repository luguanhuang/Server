#include "pch.h"
#include "skycity/ptcg2m_skycityteamresultntf.h"
#include "define/skycitydef.h"
#include "skycity/skycitymgr.h"
// generate by ProtoGen at date: 2016/10/8 17:04:39

void PtcG2M_SkyCityTeamResultNtf::Process(UINT32 dwConnID)
{
	UINT32 nGroupID = m_Data.group();
	UINT32 nWinTeamID = m_Data.winteamid();
	SkyCityMgr::Instance()->AddGameResult(nGroupID, nWinTeamID);
	for (UINT32 i = 0; i < m_Data.rolestatistics_size();i++)
	{
		KKSG::SCRoleStatistics *pStatistics = m_Data.mutable_rolestatistics(i);
		SkyCityMgr::Instance()->FillRoleSCRoleStatistics(nGroupID, pStatistics->roleid(),pStatistics->killcount(),pStatistics->deadcount(),pStatistics->totaldamage());
	}
}
