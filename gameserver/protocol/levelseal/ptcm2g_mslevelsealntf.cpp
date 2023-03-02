#include "pch.h"
#include "levelseal/ptcm2g_mslevelsealntf.h"
#include "gamelogic/levelsealMgr.h"

// generate by ProtoGen at date: 2016/7/28 19:02:30

void PtcM2G_MsLevelSealNtf::Process(UINT32 dwConnID)
{
	CLevelSealMgr::Instance()->SetSealInfo(m_Data);
}
