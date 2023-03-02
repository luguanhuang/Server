#include "pch.h"
#include "auction/ptcm2d_guildauctupdatentf.h"
#include "db/task/guildaucttask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2016/11/2 20:10:01

void PtcM2D_GuildAuctUpdateNtf::Process(UINT32 dwConnID)
{
	CGuildAuctTask* pTask = new CGuildAuctTask(m_Data);	
	CMysqlMgr::Instance()->PushTask(pTask);
}
