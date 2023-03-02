#include "pch.h"
#include "common/ptcn2d_onlinenumntf.h"
#include "db/mysqlmgr.h"
#include "db/task/onlinewritetask.h"

// generate by ProtoGen at date: 2016/11/3 10:24:17

void PtcN2D_OnlineNumNtf::Process(UINT32 dwConnID)
{
	COnlineWriteTask* poTask = new COnlineWriteTask();
	poTask->m_uTimeKey = m_Data.timekey();
	poTask->m_uServerID = m_Data.serverid();
	poTask->m_uAppID = m_Data.appid();
	poTask->m_uZoneID = m_Data.zoneid();
	poTask->m_uIosNum = m_Data.iosnum();
	poTask->m_uAndroidNum = m_Data.androidnum();
	CMysqlMgr::Instance()->PushOnlineNumTask(poTask);
}
