#include "pch.h"
#include "guild/ptcg2m_gmsetguildbosstime.h"
#include "table/MultActivityMgr.h"
#include "guild/ptcm2g_changeguildbosstime.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/8/5 17:38:27

void PtcG2M_GmsetGuildBossTime::Process(UINT32 dwConnID)
{
	MultActivityMgr::Instance()->SetGuildBossTime(m_Data.starttime(), m_Data.endtime());
	PtcM2G_ChangeGuildBossTime msg;
	msg.m_Data.CopyFrom(m_Data);
	GSLink::Instance()->SendToAllLine(msg);
}
