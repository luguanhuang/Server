#include "pch.h"
#include "common/ptck2g_broadcastworld2gs.h"
#include "network/worldlink.h"

// generate by ProtoGen at date: 2017/2/18 21:16:35

void PtcK2G_BroadCastWorld2Gs::Process(UINT32 dwConnID)
{
	LogInfo("world id[%u] connId[%u]", m_Data.serverid(), dwConnID);
	WorldLink::Instance()->SetMasterWorldConnId(dwConnID);
}
