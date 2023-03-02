#include "pch.h"
#include "mobabattle/ptcm2d_savemobarolerecord.h"
#include "db/task/mobasavetask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/6/17 19:15:36

void PtcM2D_SaveMobaRoleRecord::Process(UINT32 dwConnID)
{
	MobaSaveTask* poTask = new MobaSaveTask;
	poTask->m_qwRoleID = m_Data.roleid();
	poTask->m_oData = m_Data.record();
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(m_Data.roleid()));
}
