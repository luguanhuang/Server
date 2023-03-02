#include "pch.h"
#include "gm/ptcm2d_saveplatnoticeptf.h"
#include "db/mysqlmgr.h"
#include "db/task/noticetask.h"

// generate by ProtoGen at date: 2016/11/19 22:28:12

void PtcM2D_SavePlatNoticePtf::Process(UINT32 dwConnID)
{
	CNoticeTask* poTask = new CNoticeTask();
	poTask->m_oData.CopyFrom(m_Data);
	CMysqlMgr::Instance()->PushTask(poTask, 0);
}
