#include "pch.h"
#include "battlefield/ptcm2g_battlefiedlreadyscenegsntf.h"
#include "gamelogic/battlefieldgsmgr.h"

// generate by ProtoGen at date: 2017/9/6 13:31:30

void PtcM2G_BattleFiedlReadySceneGsNtf::Process(UINT32 dwConnID)
{
	BattleFieldGSMgr::Instance()-> ResetTime(m_Data.round(),m_Data.nextmatchtime(),m_Data.flag(),m_Data.end());
}
