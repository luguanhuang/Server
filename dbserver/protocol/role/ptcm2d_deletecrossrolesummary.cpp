#include "pch.h"
#include "role/ptcm2d_deletecrossrolesummary.h"
#include "db/task/crossrolesummarydelete.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/2/6 20:45:07

void PtcM2D_DeleteCrossRoleSummary::Process(UINT32 dwConnID)
{
	CrossRoleSummaryDelete* pTask = new CrossRoleSummaryDelete;
	CMysqlMgr::Instance()->PushTask(pTask);
}
