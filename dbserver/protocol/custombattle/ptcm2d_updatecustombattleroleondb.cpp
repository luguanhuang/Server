#include "pch.h"
#include "db/mysqlmgr.h"
#include "db/task/custombattleroletask.h"
#include "custombattle/ptcm2d_updatecustombattleroleondb.h"

// generate by ProtoGen at date: 2017/5/6 9:45:09

void PtcM2D_UpdateCustomBattleRoleOnDB::Process(UINT32 dwConnID)
{
	CustomBattleRoleTask* task = new CustomBattleRoleTask();
	task->m_roleid = m_Data.roleid();
	task->m_system = m_Data.system();
	task->m_custom = m_Data.custom();
	CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(m_Data.roleid()));

	SSInfo<<"update custombattlerole info, roleid:"<<m_Data.roleid()<<END;

}
