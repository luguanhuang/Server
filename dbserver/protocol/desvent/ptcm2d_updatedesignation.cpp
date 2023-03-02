#include "pch.h"
#include "db/mysqlmgr.h"
#include "desvent/ptcm2d_updatedesignation.h"
#include "db/task/designationtask.h"


// generate by ProtoGen at date: 2016/9/20 23:17:21

void PtcM2D_UpdateDesignation::Process(UINT32 dwConnID)
{
	DesignationTask* poTask = new DesignationTask();
	poTask->m_Data			= m_Data;
	poTask->m_qwRoleID		= m_Data.roleid();
	CMysqlMgr::Instance()->PushTask(poTask);
}
