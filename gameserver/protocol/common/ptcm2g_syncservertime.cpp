#include "pch.h"
#include "common/ptcm2g_syncservertime.h"
#include "util/gametime.h"
#include "timeutil.h"
#include "gamelogic/weekend4v4mgr.h"


// generate by ProtoGen at date: 2016/11/10 17:42:19

void PtcM2G_syncservertime::Process(UINT32 dwConnID)
{
	time_t t = m_Data.now();
	// 系统时间戳
	time_t sysTime = TimeUtil::GetTime();
	INT32 offset = t - sysTime; 
	GameTime::SetTimeOffset(offset);

    //初始化函数里面需要用到当前时间
    WeekEnd4v4Mgr::Instance()->setActivityID();

	SSWarn << "SetTime: " << TimeUtil::GetDateTimeStr(t) << END;
}
