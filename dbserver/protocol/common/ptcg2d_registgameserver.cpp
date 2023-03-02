#include "pch.h"
#include "common/ptcg2d_registgameserver.h"
#include "gslink.h"

// generate by ProtoGen at date: 2014/9/17 14:42:25

void PtcG2D_RegistGameServer::Process(UINT32 dwConnID)
{
	GSLink::Instance()->AddServerInfo(dwConnID, m_Data.serverid(), m_Data.servername());
}
