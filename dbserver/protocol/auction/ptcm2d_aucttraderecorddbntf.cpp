#include "pch.h"
#include "auction/ptcm2d_aucttraderecorddbntf.h"
#include "db/task/aucttraderecord.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2016/9/16 22:24:04

void PtcM2D_AuctTradeRecordDBNtf::Process(UINT32 dwConnID)
{
	CTradeRecordTask* pTask = new CTradeRecordTask(m_Data);
	CMysqlMgr::Instance()->PushTask(pTask);
}
