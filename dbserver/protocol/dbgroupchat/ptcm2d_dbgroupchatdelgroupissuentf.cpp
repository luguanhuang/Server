#include "pch.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelgroupissuentf.h"
#include "db/mysqlmgr.h"
#include "db/task/dbgroupchatdelgroupissue.h"

// generate by ProtoGen at date: 2017/7/28 15:38:21

void PtcM2D_DBGroupChatDelGroupIssueNtf::Process(UINT32 dwConnID)
{
    CTaskGroupChatDelGroupIssue* poTask = new CTaskGroupChatDelGroupIssue();
    poTask->m_Index = m_Data.index();
    poTask->m_TodayBeginTime = m_Data.todaybegintime();
    poTask->m_IsClear = m_Data.isclear();
    CMysqlMgr::Instance()->PushTask(poTask);
}
