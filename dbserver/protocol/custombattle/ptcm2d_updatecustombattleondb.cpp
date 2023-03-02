#include "pch.h"
#include "db/task/custombattletask.h"
#include "db/task/custombattledeletetask.h"
#include "db/mysqlmgr.h"
#include "custombattle/ptcm2d_updatecustombattleondb.h"

// generate by ProtoGen at date: 2017/5/6 9:44:32

void PtcM2D_UpdateCustomBattleOnDB::Process(UINT32 dwConnID)
{
	if (!m_Data.isdelete())
	{
		CustomBattleTask* task = new CustomBattleTask();
		task->m_uid = m_Data.data().uid();
		task->m_data = m_Data.data();
		CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(m_Data.data().uid()));

		SSInfo<<"update custombattle info, uid:"<<m_Data.data().uid()<<END;
	}
	else
	{
		CustomBattleDeleteTask* task = new CustomBattleDeleteTask();
		task->m_uid = m_Data.uid();
		CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(m_Data.data().uid()));

		SSInfo<<"delete custombattle , uid:"<<m_Data.uid()<<END;
	}
}
