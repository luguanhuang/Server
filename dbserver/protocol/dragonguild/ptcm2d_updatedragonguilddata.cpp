#include "pch.h"
#include "dragonguild/ptcm2d_updatedragonguilddata.h"
#include "db/task/dragonguilddeletetask.h"
#include "db/task/dragonguildinserttask.h"
#include "db/task/dragonguildupdatetask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/9/6 11:19:06

void PtcM2D_UpdateDragonGuildData::Process(UINT32 dwConnID)
{
	switch (m_Data.type())
	{
	case KKSG::DBOper_Insert:
		{
			DGuildInsertTask* task = new DGuildInsertTask;
			task->m_dguildSaveData = m_Data.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(m_Data.data().id()));
			break;
		}
	case KKSG::DBOper_Update:
		{
			DGuildUpdateTask* task = new DGuildUpdateTask;
			task->m_dguildSaveData = m_Data.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(m_Data.data().id()));
			break;
		}
	case KKSG::DBOper_Del:
		{
			DGuildDeleteTask* task = new DGuildDeleteTask;
			task->m_dguildSaveData = m_Data.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(m_Data.data().id()));
			break;
		}
	default:
		LogError("Invalid db type[%u]", m_Data.type());
	}
}
