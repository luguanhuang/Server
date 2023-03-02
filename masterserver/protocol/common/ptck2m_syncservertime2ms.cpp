#include "pch.h"
#include "common/ptck2m_syncservertime2ms.h"
#include "util/gametime.h"
#include "timeutil.h"
#include "common/ptcm2g_syncservertime.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2017/2/5 16:53:22

void PtcK2M_SyncServerTime2MS::Process(UINT32 dwConnID)
{

	time_t t = m_Data.now();
	// 系统时间戳
	time_t sysTime = TimeUtil::GetTime();
	INT32 offset = t - sysTime; 
	GameTime::SetTimeOffset(offset);

	SSWarn << "SetTime: " << TimeUtil::GetDateTimeStr(t) << END;

	PtcM2G_syncservertime ptc;
	ptc.m_Data.set_now(t);
	GSLink::Instance()->SendToAllLine(ptc);
}
