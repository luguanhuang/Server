#include "pch.h"
#include "common/ptcr2g_notifyserverconnect.h"
#include "servermgr/servermgr.h"

// generate by ProtoGen at date: 2016/12/27 11:00:41

void PtcR2G_NotifyServerConnect::Process(UINT32 dwConnID)
{
	LogInfo("server[%u] connect by router[%u]", m_Data.server().serverid(), dwConnID);
	ServerMgr::Instance()->AddServer(dwConnID, m_Data.server());
}
