#include "pch.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelgroupntf.h"
#include "db/mysqlmgr.h"
#include "db/task/dbgroupchatdelgroup.h"

// generate by ProtoGen at date: 2017/7/28 15:42:09

void PtcM2D_DBGroupChatDelGroupNtf::Process(UINT32 dwConnID)
{
    CTaskGroupChatDelGroup* poTask = new CTaskGroupChatDelGroup();
    poTask->m_Index = m_Data.groupid();
    poTask->m_TodayBeginTime = m_Data.todaybegintime();
    poTask->m_IsClear = m_Data.isclear();
    CMysqlMgr::Instance()->PushTask(poTask);
}
