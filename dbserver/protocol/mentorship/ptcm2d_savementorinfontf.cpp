#include "pch.h"
#include "mentorship/ptcm2d_savementorinfontf.h"
#include "db/task/mentorshipoptask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2016/12/17 21:02:15

void PtcM2D_SaveMentorInfoNtf::Process(UINT32 dwConnID)
{
		CMentorshipOpTask* task = new CMentorshipOpTask();
		task->m_oData = m_Data.info();
		task->m_roleID = m_Data.roleid();
		CMysqlMgr::Instance()->PushTask(task);
}
