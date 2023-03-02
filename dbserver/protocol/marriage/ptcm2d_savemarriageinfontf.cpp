#include "pch.h"
#include "marriage/ptcm2d_savemarriageinfontf.h"
#include "db/task/marriageoptask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/7/25 16:00:25

void PtcM2D_SaveMarriageInfoNtf::Process(UINT32 dwConnID)
{
	CMarriageOpTask* task = new CMarriageOpTask();
	task->m_oData = m_Data.info();
	task->m_roleID = m_Data.roleid();
	CMysqlMgr::Instance()->PushTask(task);
}
