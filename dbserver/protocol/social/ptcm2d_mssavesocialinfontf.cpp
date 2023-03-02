#include "pch.h"
#include "social/ptcm2d_mssavesocialinfontf.h"
#include "db/mysqlmgr.h"
#include "db/task/socialoptask.h"

// generate by ProtoGen at date: 2016/8/17 17:51:57

void PtcM2D_MSSaveSocialInfoNtf::Process(UINT32 dwConnID)
{
	for (int i = 0; i < m_Data.socialinfo_size(); ++i)
	{
		CSocialOpTask* task = new CSocialOpTask();
		task->m_oData = m_Data.socialinfo(i);
		CMysqlMgr::Instance()->PushTask(task);
	}
}
