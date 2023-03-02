#include "pch.h"
#include "role/ptcm2d_delrolentf.h"
#include "role/rpcg2d_saveroledata.h"
#include "db/mysqlmgr.h"
#include "db/task/roledeltask.h"

// generate by ProtoGen at date: 2016/10/15 14:33:55

void PtcM2D_DelRoleNtf::Process(UINT32 dwConnID)
{
	CRoleDelTask* task = new CRoleDelTask;
	task->m_strAccount = m_Data.account();
	task->m_roleId = m_Data.role_id();
	task->m_slot = m_Data.slot();
	CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(m_Data.role_id()));
}
