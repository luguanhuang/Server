#include "pch.h"
#include "platfriend/ptcm2d_saveplatfriendinfo.h"
#include "db/task/platfriendoptask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2016/10/31 19:43:39

void PtcM2D_SavePlatFriendInfo::Process(UINT32 dwConnID)
{
	for (int i = 0; i < m_Data.platfriendinfo_size(); ++i)
	{
		CPlatFriendOpTask* task = new CPlatFriendOpTask();
		task->m_oData = m_Data.platfriendinfo(i).info();
		task->m_openid = m_Data.platfriendinfo(i).openid();
		CMysqlMgr::Instance()->PushTask(task);
	}
}
