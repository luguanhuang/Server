#include "pch.h"
#include "guild/ptcm2d_saveguilddata.h"
#include "db/task/guildinserttask.h"
#include "db/task/guildupdatetask.h"
#include "db/task/guilddeletetask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2016/8/26 10:56:00

enum GuildDBOPType
{
	GDBOP_UPDATE = 0,
	GDBOP_CREATE = 1,
	GDBOP_DELETE = 2,
};

void PtcM2D_SaveGuildData::Process(UINT32 dwConnID)
{
	if (m_Data.optype() == GDBOP_CREATE)  // 插入新数据
	{
		CGuildInsertTask* poTask = new CGuildInsertTask();
		poTask->m_guildAllData = m_Data;
		CMysqlMgr::Instance()->PushTask(poTask);
		LogInfo("Create Guild [%s] ", str(m_Data.gid()));
	}
	else if (m_Data.optype() == GDBOP_UPDATE) // 更新数据
	{
		CGuildUpdateTask* poTask = new CGuildUpdateTask();
		poTask->m_guildAllData = m_Data;
		CMysqlMgr::Instance()->PushTask(poTask);
		LogInfo("Update Guild [%s] ", str(m_Data.gid()));
	}
	else if (m_Data.optype() == GDBOP_DELETE)
	{
		CGuildDeleteTask* poTask = new CGuildDeleteTask();
		poTask->m_guildAllData = m_Data;
		CMysqlMgr::Instance()->PushTask(poTask);
		LogInfo("Delete Guild [%s] ", str(m_Data.gid()));
	}
}
