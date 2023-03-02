#include "pch.h"
#include "common/ptcm2d_savepaylist.h"
#include "db/mysqlmgr.h"
#include "db/task/paylisttask.h"

// generate by ProtoGen at date: 2016/10/27 22:06:35

void PtcM2D_SavePayList::Process(UINT32 dwConnID)
{
	CPayListTask* poTask = new CPayListTask();
	poTask->m_oData.CopyFrom(m_Data);
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(poTask->m_oData.roleid()));
}
