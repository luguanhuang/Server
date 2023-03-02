#include "pch.h"
#include "auction/ptcm2d_auctsaleitemdbntf.h"
#include "mslink.h"
#include "db/task/auctsaleitemtask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2016/9/16 22:22:48

void PtcM2D_AuctSaleItemDBNtf::Process(UINT32 dwConnID)
{
	CSaleItemTask* pTask = new CSaleItemTask(m_Data);
	CMysqlMgr::Instance()->PushTask(pTask);
}
