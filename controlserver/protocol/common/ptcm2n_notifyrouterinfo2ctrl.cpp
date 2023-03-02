#include "pch.h"
#include "common/ptcm2n_notifyrouterinfo2ctrl.h"
#include "network/rslink.h"

// generate by ProtoGen at date: 2017/3/3 16:56:30

void PtcM2N_NotifyRouterInfo2Ctrl::Process(UINT32 dwConnID)
{
	RSLink::Instance()->SetRouterIpPort(m_Data.ctrl().ip(), m_Data.ctrl().port());
}
