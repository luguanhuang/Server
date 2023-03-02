#include "pch.h"
#include "common/ptck2m_broadcastworld2ms.h"
#include "network/worldlink.h"
#include "cross/crosszonestatus.h"

// generate by ProtoGen at date: 2017/2/18 21:15:01

void PtcK2M_BroadCastWorld2Ms::Process(UINT32 dwConnID)
{
	LogInfo("master world id[%u] gs_num [%u] connId[%u]", m_Data.serverid(), m_Data.gs_num(), dwConnID);
	CrossZoneStatus::Instance()->SetWorldStatus(true);
	CrossZoneStatus::Instance()->SetCrossGsNum(m_Data.gs_num());
	WorldLink::Instance()->SetMasterWorldConnId(dwConnID);
}
