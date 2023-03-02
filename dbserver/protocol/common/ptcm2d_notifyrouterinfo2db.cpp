#include "pch.h"
#include "common/ptcm2d_notifyrouterinfo2db.h"
#include "network/rslink.h"

// generate by ProtoGen at date: 2017/3/3 16:55:41

void PtcM2D_NotifyRouterInfo2DB::Process(UINT32 dwConnID)
{
	RSLink::Instance()->SetRouterIpPort(m_Data.db().ip(), m_Data.db().port());
}
