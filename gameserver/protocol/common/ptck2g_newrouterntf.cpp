#include "pch.h"
#include "common/ptck2g_newrouterntf.h"
#include "network/rslink.h"

// generate by ProtoGen at date: 2016/10/27 20:10:23

void PtcK2G_NewRouterNtf::Process(UINT32 dwConnID)
{
	RSLink::Instance()->AddToConnect(m_Data.line(), m_Data.address().ip(), m_Data.address().port());
}
