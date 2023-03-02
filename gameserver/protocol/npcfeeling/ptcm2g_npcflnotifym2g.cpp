#include "pch.h"
#include "npcfeeling/ptcm2g_npcflnotifym2g.h"
#include "npcfeeling/npcfeelingmgr.h"

// generate by ProtoGen at date: 2017/9/12 10:33:44

void PtcM2G_NpcFlNotifyM2G::Process(UINT32 dwConnID)
{
	CNpcFeelingMgr::Instance()->SyncData(m_Data.globaldata());
}
