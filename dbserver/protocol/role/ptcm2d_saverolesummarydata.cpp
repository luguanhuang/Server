#include "pch.h"
#include "db/mysqlmgr.h"
#include "db/task/rolesummarytask.h"
#include "role/ptcm2d_saverolesummarydata.h"
#include "db/task/crossrolesummarytask.h"

// generate by ProtoGen at date: 2016/8/24 22:12:48

void PtcM2D_SaveRoleSummaryData::Process(UINT32 dwConnID)
{
	if (m_Data.is_cross_rolesummary() == false)
	{
		for (int i = 0; i < m_Data.data_size(); ++i)
		{
			RolesummaryTask* task = new RolesummaryTask();
			task->m_roleid = m_Data.data(i).roleid();
			task->m_info = m_Data.data(i);
			CMysqlMgr::Instance()->PushTask(task);

			SSInfo<<"update rolesummary info, roleid:"<<m_Data.data(i).roleid()<<END;
		}
	}
	else
	{
		for (int i = 0; i < m_Data.data_size(); ++i)
		{
			CrossRoleSummaryTask* task = new CrossRoleSummaryTask;
			task->m_roleid = m_Data.data(i).roleid();
			task->m_info = m_Data.data(i);
			CMysqlMgr::Instance()->PushTask(task);

			SSInfo<<"update cross rolesummary info, roleid:"<<m_Data.data(i).roleid()<<END;
		}
	}
}
