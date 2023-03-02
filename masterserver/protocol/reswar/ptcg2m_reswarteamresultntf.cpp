#include "pch.h"
#include "reswar/ptcg2m_reswarteamresultntf.h"
#include "reswar/reswarmgr.h"

// generate by ProtoGen at date: 2016/11/1 14:22:27

void PtcG2M_ResWarTeamResultNtf::Process(UINT32 dwConnID)
{
	UINT32 nGroup  = m_Data.group();
	ResWarMgr::Instance()->SetFightTeamState(nGroup);

}
