#include "pch.h"
#include "account/ptcm2d_savepunishdata.h"
#include "db/mysqlmgr.h"
#include "db/task/punishtask.h"

// generate by ProtoGen at date: 2016/9/1 21:46:47

void PtcM2D_SavePunishData::Process(UINT32 dwConnID)
{
	CPunishTask* poTask = new CPunishTask();
	poTask->m_oData.CopyFrom(m_Data);
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(poTask->m_oData.roleid()));
}
