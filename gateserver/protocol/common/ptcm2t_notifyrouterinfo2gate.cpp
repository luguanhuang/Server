#include "pch.h"
#include "common/ptcm2t_notifyrouterinfo2gate.h"
#include "network/rslink.h"

// generate by ProtoGen at date: 2017/3/3 16:55:03

void PtcM2T_NotifyRouterInfo2Gate::Process(UINT32 dwConnID)
{
	RSLink::Instance()->SetRouterIpPort(m_Data.gate().ip(), m_Data.gate().port());
}
