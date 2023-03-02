#include "pch.h"
#include "levelseal/levelsealMgr.h"
#include "levelseal/ptcg2m_gmlevelsealinfo.h"

// generate by ProtoGen at date: 2016/7/30 20:49:11

void PtcG2M_GmLevelSealInfo::Process(UINT32 dwConnID)
{
	if (m_Data.has_type())
	{
		CLevelSealMgr::Instance()->SetType(m_Data.type());
		return ;
	}
	if (m_Data.has_endtime())
	{
		CLevelSealMgr::Instance()->SetTime(m_Data.endtime());
	}
}
