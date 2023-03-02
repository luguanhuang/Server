#include "pch.h"
#include "pay/ptcm2d_savepayibitem.h"
#include "db/mysqlmgr.h"
#include "db/task/payibitemtask.h"

// generate by ProtoGen at date: 2017/3/22 21:53:21

void PtcM2D_SavePayIbItem::Process(UINT32 dwConnID)
{
	CPayIbItemTask* poTask = new CPayIbItemTask();
	poTask->SetData(m_Data.op(), m_Data.data());
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(m_Data.data().fromroleid()));
}
