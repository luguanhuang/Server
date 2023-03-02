#include "pch.h"
#include "guild/ptcm2g_changeguildbosstime.h"
#include "gamelogic/MultActivityMgr.h"

// generate by ProtoGen at date: 2016/8/8 15:31:01

void PtcM2G_ChangeGuildBossTime::Process(UINT32 dwConnID)
{
	MultActivityMgr::Instance()->SetGuildBossTime(m_Data.starttime(), m_Data.endtime());
}
