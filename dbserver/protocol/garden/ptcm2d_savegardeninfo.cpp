#include "pch.h"
#include "garden/ptcm2d_savegardeninfo.h"
#include "db/task/garden_info_inserttask.h"
#include "db/task/garden_update_task.h"
#include "db/mysqlmgr.h"
// generate by ProtoGen at date: 2016/10/5 10:02:15

enum GardnDBOperateType
{
	dbCreate = 0,
	dbUpdate,	
	dbDelete,
};

void PtcM2D_SaveGardenInfo::Process(UINT32 dwConnID)
{
	if (m_Data.db_operatetype() == dbCreate)
	{	
		GardenInfoInsertTask* doTask = new GardenInfoInsertTask();
		doTask->garden_info_ = m_Data;
		CMysqlMgr::Instance()->PushTask(doTask);
		LogInfo("Create garden role_id  [%s] ", str(m_Data.role_id()));
	}
	else if (m_Data.db_operatetype() == dbUpdate)
	{
		GardenUpdateTask* doTask = new GardenUpdateTask();
		doTask->garden_info_ = m_Data;
		CMysqlMgr::Instance()->PushTask(doTask);
		LogInfo("Create garden role_id  [%s] ", str(m_Data.role_id()));
	}
}
