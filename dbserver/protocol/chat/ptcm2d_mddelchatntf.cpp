#include "pch.h"
#include "chat/ptcm2d_mddelchatntf.h"
#include "db/mysqlmgr.h"
#include "db/task/chatdeltask.h"

// generate by ProtoGen at date: 2016/12/14 0:58:12

void PtcM2D_MDDelChatNtf::Process(UINT32 dwConnID)
{
	CChatDelTask* poTask = new CChatDelTask();
	poTask->m_dwChannel = m_Data.channel();
	poTask->m_dwStoreKey = m_Data.storekey();
	CMysqlMgr::Instance()->PushTask(poTask);
}
